#include <stdlib.h>
#include <stdint.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include "Base64.h"
#include "ThreadPool.h"
#include "Image2DHelpers.h"
#include "utils.h"
#include<map>

//#include <Unity/Runtime.h>

//using namespace ut;
using namespace ThreadPool;

// keep this in sync with C#
class ImageSTB {
public:
    ImageSTB() {
        w = 0;
        h = 0;
        pixels = 0;
        hasAlpha = false;
    }

    ImageSTB(int _w, int _h) {
        w = _w;
        h = _h;
        pixels = new uint32_t[w*h];
        hasAlpha = false;
    }

    ~ImageSTB() {
        Free();
    }

    void Free() {
        delete [] pixels;
        pixels = 0;
    }

    ImageSTB(ImageSTB&& other) {
        pixels = other.pixels; 
        w = other.w;
        h = other.h;
        hasAlpha = other.hasAlpha;
        other.pixels = 0;
    }

    ImageSTB& operator=(ImageSTB&& other) {
        if ( this == &other ) return *this;
        delete[] pixels; 
        pixels = other.pixels; 
        w = other.w;
        h = other.h;
        hasAlpha = other.hasAlpha;
        other.pixels = 0;
        return *this;
    }

    void Set(uint32_t *_pixels, int _w, int _h, bool _hasAlpha) {
        delete[] pixels;
        pixels = _pixels; 
        w = _w;
        h = _h;
        hasAlpha = _hasAlpha;
    }

    int w, h;
    bool hasAlpha;
    uint32_t *pixels;
};

static std::vector<ImageSTB*> allImages(1); // by handle, reserve handle 0
static ImageSTB loadingImage;

#if defined(UNITY_ANDROID)
extern "C" void* loadAsset(const char *path, int *size);
#endif

static bool
LoadImageFromFile(const char* fn, size_t fnlen, ImageSTB& colorImg)
{
    int bpp = 0;
    int w = 0, h = 0;
    uint32_t* pixels = 0;
    // first try if it is a data uri
    std::string mediatype;
    std::vector<uint8_t> dataurimem;
    if (DecodeDataURIBase64(dataurimem, mediatype, fn, fnlen)) // try loading as data uri (ignore media type)
        pixels = (uint32_t*)stbi_load_from_memory(dataurimem.data(), (int)dataurimem.size(), &w, &h, &bpp, 4);
#if defined(UNITY_ANDROID)
    int size;
    void *data = loadAsset(fn, &size);
    pixels = (uint32_t*)stbi_load_from_memory((uint8_t*)data, size, &w, &h, &bpp, 4);
    free(data);
#endif
    int size;
    void *data = loadRemoteAsset(fn, &size);
    if (!pixels) // try loading as file
        pixels = (uint32_t*)stbi_load_from_memory((uint8_t*)data, size, &w, &h, &bpp, 4);
    if (!pixels)
        return false;
    colorImg.Set(pixels, w, h, bpp==4);
    free(data);
    return true;
}

static bool
LoadSTBImageOnly(ImageSTB& colorImg, const char *imageFile, const char *maskFile)
{
    bool hasColorFile = imageFile && imageFile[0];
    bool hasMaskFile = maskFile && maskFile[0];

    if (!hasMaskFile && !hasColorFile)
        return false;

    if (hasColorFile && strcmp(imageFile,"::white1x1")==0 ) { // special case 1x1 image
        colorImg.Set(new uint32_t(1), 1, 1, false);
        colorImg.pixels[0] = ~0;
        return true;
    }

    // color from file first
    ImageSTB maskImg;
    if (hasColorFile) {
        if (!LoadImageFromFile(imageFile, strlen(imageFile), colorImg))
            return false;
    }
    // mask from file
    if (hasMaskFile) {
        if (!LoadImageFromFile(maskFile, strlen(maskFile), maskImg))
            return false;
        if (hasColorFile && (colorImg.w != maskImg.w || colorImg.h != maskImg.h))
            return false;
    }

    if (hasMaskFile && hasColorFile) { // merge mask into color if we have both
        // copy alpha from maskImg, and premultiply alpha to match browser
        uint32_t* cbits = colorImg.pixels;
        uint32_t* mbits = maskImg.pixels;
        uint32_t npix = colorImg.w * colorImg.h;
        colorImg.hasAlpha = false;
        for (uint32_t i = 0; i < npix; i++) {
            uint32_t c = cbits[i] & 0x00ffffff;
            uint32_t m = (mbits[i] << 24) & 0xff000000;
            c |= m;
            if (m != 0xff000000) {
                colorImg.hasAlpha = true;
                c = Image2DHelpers::PremultiplyAlpha(c);
            }
            cbits[i] = c;
        }
    } else if (hasColorFile) { // color only
        if (colorImg.hasAlpha)
            colorImg.hasAlpha = Image2DHelpers::PremultiplyAlphaAndCheckCopy(colorImg.pixels, colorImg.pixels, colorImg.w, colorImg.h);
    } else if (hasMaskFile) { // mask only: copy mask to colorImage to all channels
        uint32_t* mbits = maskImg.pixels;
        uint32_t npix = maskImg.w*maskImg.h;
        maskImg.hasAlpha = true;
        // take R channel to all
        for (uint32_t i = 0; i < npix; i++) {
            uint32_t c = mbits[i] & 0xff;
            mbits[i] = c | (c << 8) | (c << 16) | (c << 24);
        }
        colorImg = std::move(maskImg);
    }
    return true;
}

static void
initImage2DMask(const ImageSTB& colorImg, uint8_t* dest)
{
    const uint32_t* src = colorImg.pixels;
    int size = colorImg.w * colorImg.h;
    for (int i = 0; i < size; ++i)
        dest[i] = (uint8_t)(src[i]>>24);
}

struct STBIToMemory
{
    static void FWriteStatic(void* context, void* data, int size) { ((STBIToMemory*)context)->FWrite(data, size); }
    void FWrite(void* data, int size)
    {
        size_t olds = mem.size();
        mem.resize(olds + size);
        memcpy(mem.data() + olds, data, size);
    }
    std::vector<uint8_t> mem;
};

class AsyncGLFWImageLoader : public ThreadPool::Job {
public:
    // state needed for Do()
    ImageSTB colorImg;
    std::string imageFile;
    std::string maskFile;
    int imageIdx;

    virtual bool Do()
    {
        progress = 0;
// simulate being slow
#if 0
        for (int i=0; i<20; i++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            progress = i;
            if ( abort )
                return false;
        }
#endif
        // actual work
        return LoadSTBImageOnly(colorImg, imageFile.c_str(), maskFile.c_str());
    }
};

std::map<int, int32_t> idxToLoadId;

#define DOTS_EXPORT(X) X
// zero player API
DOTS_EXPORT(void)
freeimage_stb(int imageHandle)
{
    if (imageHandle<0 || imageHandle>=(int)allImages.size())
        return;
    delete allImages[imageHandle];
    allImages[imageHandle] = 0;
}

extern "C" int32_t startload_stb(const char *imageFile, const char *maskFile)
{
    std::unique_ptr<AsyncGLFWImageLoader> loader(new AsyncGLFWImageLoader);
    int found = -1;
    for (int i=1; i<(int)allImages.size(); i++ ) {
        if (!allImages[i]) {
            found = i;
            break;
        }
    }
    
    int imageIdx;
    
    if (found==-1) {
        allImages.push_back(&loadingImage);
        imageIdx = (int)allImages.size()-1;
    } else {
        allImages[found] = &loadingImage;
        imageIdx = found;
    }
    printf("startload_stb %s %d \n", imageFile, imageIdx);
    loader->imageIdx = imageIdx;
    loader->imageFile = imageFile;
    loader->maskFile = maskFile;
    int32_t loadId = (int32_t)(Pool::GetInstance()->Enqueue(std::move(loader)));
    if (idxToLoadId.find(imageIdx) != idxToLoadId.end()) {
        printf("error:image idx %d with loadid %d already exists\n", imageIdx, idxToLoadId[imageIdx]);
        return 0;
    }
    idxToLoadId[imageIdx] = loadId;
    return imageIdx;
}

//DOTS_EXPORT(void)
//abortload_stb(int32_t loadId)
//{
//    Pool::GetInstance()->Abort(loadId);
//}

extern "C"  int checkload_stb(int32_t idx)
{
     printf("checkload_stb %d \n", idx);
    if (idxToLoadId.find(idx) == idxToLoadId.end()) {
        return 2;
    }
    int loadId = idxToLoadId[idx];
    std::unique_ptr<ThreadPool::Job> resultTemp = Pool::GetInstance()->CheckAndRemove(loadId);
    if (!resultTemp)
        return 0; // still loading
    
    idxToLoadId.erase(idxToLoadId.find(idx));
    if (!resultTemp->GetReturnValue()) {
        resultTemp.reset(0);
        return 2; // failed
    }
    
    AsyncGLFWImageLoader* resultGLFW = (AsyncGLFWImageLoader*)resultTemp.get();
    allImages[resultGLFW->imageIdx] = new ImageSTB(std::move(resultGLFW->colorImg));
    return 1; // ok
}

extern "C" void finishLoading_stb(int idx, int* w, int* h, int* hasAlpha) {
    ImageSTB* image = allImages[idx];
    *w = image->w;
    *h = image->h;
    *hasAlpha = image->hasAlpha ? 1 : 0;
}

extern "C" void freeImage_stb(int idx) {
    if (idx<0 || idx>=(int)allImages.size())
           return;
       delete allImages[idx];
       allImages[idx] = 0;
}

extern "C" int imageToMemory_stb(int idx, int w, int h, uint8_t* destPtr) {
    if (idx<0 || idx>=(int)allImages.size())
        return -1;
    ImageSTB* img = allImages[idx];
    memcpy(destPtr, img->pixels, 4*w*h);
    return 0;
}
