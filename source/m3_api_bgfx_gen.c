
#include<bgfx/c99/bgfx.h>
#include "m3_api_bgfx.h"

#include "m3_api_defs.h"
#include "m3_env.h"
#include "m3_exception.h"

static void* whandle;



m3ApiRawFunction(m3_bgfx_vertex_layout_begin)
	{
	m3ApiReturnType(ptr32)
		m3ApiGetArgMem   (bgfx_vertex_layout_t*       , _bgfx_this)
		m3ApiGetArg   (bgfx_renderer_type_t       , _bgfx__rendererType)
	bgfx_vertex_layout_t* ret = bgfx_vertex_layout_begin(_bgfx_this, _bgfx__rendererType);
	m3ApiReturn(ret)
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_vertex_layout_add)
	{
	m3ApiReturnType(ptr32)
		m3ApiGetArgMem   (bgfx_vertex_layout_t*       , _bgfx_this)
		m3ApiGetArg   (bgfx_attrib_t       , _bgfx__attrib)
		m3ApiGetArg   (uint8_t       , _bgfx__num)
		m3ApiGetArg   (bgfx_attrib_type_t       , _bgfx__type)
		m3ApiGetArg   (bool       , _bgfx__normalized)
		m3ApiGetArg   (bool       , _bgfx__asInt)
	bgfx_vertex_layout_t* ret = bgfx_vertex_layout_add(_bgfx_this, _bgfx__attrib, _bgfx__num, _bgfx__type, _bgfx__normalized, _bgfx__asInt);
	m3ApiReturn(ret)
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_vertex_layout_end)
	{
		m3ApiGetArgMem   (bgfx_vertex_layout_t*       , _bgfx_this)
	bgfx_vertex_layout_end(_bgfx_this);
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_init)
	{
	m3ApiReturnType(int32_t)
		m3BgfxApiGetInitArg(_bgfx__init)
	bool ret = bgfx_init(_bgfx__init);
	m3ApiReturn(ret)
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_reset)
	{
		m3ApiGetArg   (uint32_t       , _bgfx__width)
		m3ApiGetArg   (uint32_t       , _bgfx__height)
		m3ApiGetArg   (uint32_t       , _bgfx__flags)
		m3ApiGetArg   (bgfx_texture_format_t       , _bgfx__format)
	bgfx_reset(_bgfx__width, _bgfx__height, _bgfx__flags, _bgfx__format);
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_frame)
	{
	m3ApiReturnType(int32_t)
		m3ApiGetArg   (bool       , _bgfx__capture)
	uint32_t ret = bgfx_frame(_bgfx__capture);
	m3ApiReturn(ret)
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_get_renderer_type)
	{
	m3ApiReturnType(int32_t)
	bgfx_renderer_type_t ret = bgfx_get_renderer_type();
	m3ApiReturn(ret)
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_get_caps)
	{
	m3ApiReturnType(ptr32)
	const bgfx_caps_t* ret = bgfx_get_caps();
	m3ApiReturn(ret)
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_copy)
	{
	m3ApiReturnType(ptr32)
		m3ApiGetArgMem   (const void*       , _bgfx__data)
		m3ApiGetArg   (uint32_t       , _bgfx__size)
	const bgfx_memory_t* ret = bgfx_copy(_bgfx__data, _bgfx__size);
	m3ApiReturn(ret)
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_set_debug)
	{
		m3ApiGetArg   (uint32_t       , _bgfx__debug)
	bgfx_set_debug(_bgfx__debug);
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_dbg_text_clear)
	{
		m3ApiGetArg   (uint8_t       , _bgfx__attr)
		m3ApiGetArg   (bool       , _bgfx__small)
	bgfx_dbg_text_clear(_bgfx__attr, _bgfx__small);
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_create_index_buffer)
	{
	m3ApiReturnType(ptr32)
		m3ApiGetArgMem   (const bgfx_memory_t*       , _bgfx__mem)
		m3ApiGetArg   (uint16_t       , _bgfx__flags)
	bgfx_index_buffer_handle_t ret = bgfx_create_index_buffer(_bgfx__mem, _bgfx__flags);
	m3ApiReturn(bgfx_handle_to_idx(ret))
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_create_vertex_layout)
	{
	m3ApiReturnType(ptr32)
		m3ApiGetArgMem   (const bgfx_vertex_layout_t *       , _bgfx__layout)
	bgfx_vertex_layout_handle_t ret = bgfx_create_vertex_layout(_bgfx__layout);
	m3ApiReturn(bgfx_handle_to_idx(ret))
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_create_vertex_buffer)
	{
	m3ApiReturnType(ptr32)
		m3ApiGetArgMem   (const bgfx_memory_t*       , _bgfx__mem)
		m3ApiGetArgMem   (const bgfx_vertex_layout_t *       , _bgfx__layout)
		m3ApiGetArg   (uint16_t       , _bgfx__flags)
	bgfx_vertex_buffer_handle_t ret = bgfx_create_vertex_buffer(_bgfx__mem, _bgfx__layout, _bgfx__flags);
	m3ApiReturn(bgfx_handle_to_idx(ret))
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_alloc_transient_buffers)
	{
	m3ApiReturnType(int32_t)
		m3ApiGetArgMem   (bgfx_transient_vertex_buffer_t*       , _bgfx__tvb)
		m3ApiGetArgMem   (const bgfx_vertex_layout_t *       , _bgfx__layout)
		m3ApiGetArg   (uint32_t       , _bgfx__numVertices)
		m3ApiGetArgMem   (bgfx_transient_index_buffer_t*       , _bgfx__tib)
		m3ApiGetArg   (uint32_t       , _bgfx__numIndices)
	bool ret = bgfx_alloc_transient_buffers(_bgfx__tvb, _bgfx__layout, _bgfx__numVertices, _bgfx__tib, _bgfx__numIndices);
	m3ApiReturn(ret)
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_create_shader)
	{
	m3ApiReturnType(ptr32)
		m3ApiGetArgMem   (const bgfx_memory_t*       , _bgfx__mem)
	bgfx_shader_handle_t ret = bgfx_create_shader(_bgfx__mem);
	m3ApiReturn(bgfx_handle_to_idx(ret))
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_set_shader_name)
	{
		m3ApiGetArg   (bgfx_shader_handle_t       , _bgfx__handle)
		m3ApiGetArgMem   (const char*       , _bgfx__name)
		m3ApiGetArg   (int32_t       , _bgfx__len)
	bgfx_set_shader_name(_bgfx__handle, _bgfx__name, _bgfx__len);
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_create_program)
	{
	m3ApiReturnType(ptr32)
		m3ApiGetArg   (bgfx_shader_handle_t       , _bgfx__vsh)
		m3ApiGetArg   (bgfx_shader_handle_t       , _bgfx__fsh)
		m3ApiGetArg   (bool       , _bgfx__destroyShaders)
	bgfx_program_handle_t ret = bgfx_create_program(_bgfx__vsh, _bgfx__fsh, _bgfx__destroyShaders);
	m3ApiReturn(bgfx_handle_to_idx(ret))
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_create_texture_2d)
	{
	m3ApiReturnType(ptr32)
		m3ApiGetArg   (uint16_t       , _bgfx__width)
		m3ApiGetArg   (uint16_t       , _bgfx__height)
		m3ApiGetArg   (bool       , _bgfx__hasMips)
		m3ApiGetArg   (uint16_t       , _bgfx__numLayers)
		m3ApiGetArg   (bgfx_texture_format_t       , _bgfx__format)
		m3ApiGetArg   (uint64_t       , _bgfx__flags)
		m3ApiGetArgMem   (const bgfx_memory_t*       , _bgfx__mem)
	bgfx_texture_handle_t ret = bgfx_create_texture_2d(_bgfx__width, _bgfx__height, _bgfx__hasMips, _bgfx__numLayers, _bgfx__format, _bgfx__flags, _bgfx__mem);
	m3ApiReturn(bgfx_handle_to_idx(ret))
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_destroy_texture)
	{
		m3ApiGetArg   (bgfx_texture_handle_t       , _bgfx__handle)
	bgfx_destroy_texture(_bgfx__handle);
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_create_frame_buffer_from_attachment)
	{
	m3ApiReturnType(ptr32)
		m3ApiGetArg   (uint8_t       , _bgfx__num)
		m3ApiGetArgMem   (const bgfx_attachment_t*       , _bgfx__attachment)
		m3ApiGetArg   (bool       , _bgfx__destroyTexture)
	bgfx_frame_buffer_handle_t ret = bgfx_create_frame_buffer_from_attachment(_bgfx__num, _bgfx__attachment, _bgfx__destroyTexture);
	m3ApiReturn(bgfx_handle_to_idx(ret))
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_create_uniform)
	{
	m3ApiReturnType(ptr32)
		m3ApiGetArgMem   (const char*       , _bgfx__name)
		m3ApiGetArg   (bgfx_uniform_type_t       , _bgfx__type)
		m3ApiGetArg   (uint16_t       , _bgfx__num)
	bgfx_uniform_handle_t ret = bgfx_create_uniform(_bgfx__name, _bgfx__type, _bgfx__num);
	m3ApiReturn(bgfx_handle_to_idx(ret))
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_set_view_rect)
	{
		m3ApiGetArg   (bgfx_view_id_t       , _bgfx__id)
		m3ApiGetArg   (uint16_t       , _bgfx__x)
		m3ApiGetArg   (uint16_t       , _bgfx__y)
		m3ApiGetArg   (uint16_t       , _bgfx__width)
		m3ApiGetArg   (uint16_t       , _bgfx__height)
	bgfx_set_view_rect(_bgfx__id, _bgfx__x, _bgfx__y, _bgfx__width, _bgfx__height);
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_set_view_scissor)
	{
		m3ApiGetArg   (bgfx_view_id_t       , _bgfx__id)
		m3ApiGetArg   (uint16_t       , _bgfx__x)
		m3ApiGetArg   (uint16_t       , _bgfx__y)
		m3ApiGetArg   (uint16_t       , _bgfx__width)
		m3ApiGetArg   (uint16_t       , _bgfx__height)
	bgfx_set_view_scissor(_bgfx__id, _bgfx__x, _bgfx__y, _bgfx__width, _bgfx__height);
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_set_view_clear)
	{
		m3ApiGetArg   (bgfx_view_id_t       , _bgfx__id)
		m3ApiGetArg   (uint16_t       , _bgfx__flags)
		m3ApiGetArg   (uint32_t       , _bgfx__rgba)
		m3ApiGetArg   (float       , _bgfx__depth)
		m3ApiGetArg   (uint8_t       , _bgfx__stencil)
	bgfx_set_view_clear(_bgfx__id, _bgfx__flags, _bgfx__rgba, _bgfx__depth, _bgfx__stencil);
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_set_view_mode)
	{
		m3ApiGetArg   (bgfx_view_id_t       , _bgfx__id)
		m3ApiGetArg   (bgfx_view_mode_t       , _bgfx__mode)
	bgfx_set_view_mode(_bgfx__id, _bgfx__mode);
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_set_view_frame_buffer)
	{
		m3ApiGetArg   (bgfx_view_id_t       , _bgfx__id)
		m3ApiGetArg   (bgfx_frame_buffer_handle_t       , _bgfx__handle)
	bgfx_set_view_frame_buffer(_bgfx__id, _bgfx__handle);
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_set_view_transform)
	{
		m3ApiGetArg   (bgfx_view_id_t       , _bgfx__id)
		m3ApiGetArgMem   (const void*       , _bgfx__view)
		m3ApiGetArgMem   (const void*       , _bgfx__proj)
	bgfx_set_view_transform(_bgfx__id, _bgfx__view, _bgfx__proj);
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_encoder_begin)
	{
	m3ApiReturnType(ptr32)
		m3ApiGetArg   (bool       , _bgfx__forThread)
	bgfx_encoder_t* ret = bgfx_encoder_begin(_bgfx__forThread);
	m3ApiReturn(ret)
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_encoder_end)
	{
		m3ApiGetArgMem   (bgfx_encoder_t*       , _bgfx__encoder)
	bgfx_encoder_end(_bgfx__encoder);
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_encoder_set_state)
	{
		m3ApiGetArgMem   (bgfx_encoder_t*       , _bgfx_this)
		m3ApiGetArg   (uint64_t       , _bgfx__state)
		m3ApiGetArg   (uint32_t       , _bgfx__rgba)
	bgfx_encoder_set_state(_bgfx_this, _bgfx__state, _bgfx__rgba);
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_encoder_set_transform)
	{
	m3ApiReturnType(int32_t)
		m3ApiGetArgMem   (bgfx_encoder_t*       , _bgfx_this)
		m3ApiGetArgMem   (const void*       , _bgfx__mtx)
		m3ApiGetArg   (uint16_t       , _bgfx__num)
	uint32_t ret = bgfx_encoder_set_transform(_bgfx_this, _bgfx__mtx, _bgfx__num);
	m3ApiReturn(ret)
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_encoder_set_uniform)
	{
		m3ApiGetArgMem   (bgfx_encoder_t*       , _bgfx_this)
		m3ApiGetArg   (bgfx_uniform_handle_t       , _bgfx__handle)
		m3ApiGetArgMem   (const void*       , _bgfx__value)
		m3ApiGetArg   (uint16_t       , _bgfx__num)
	bgfx_encoder_set_uniform(_bgfx_this, _bgfx__handle, _bgfx__value, _bgfx__num);
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_encoder_set_index_buffer)
	{
		m3ApiGetArgMem   (bgfx_encoder_t*       , _bgfx_this)
		m3ApiGetArg   (bgfx_index_buffer_handle_t       , _bgfx__handle)
		m3ApiGetArg   (uint32_t       , _bgfx__firstIndex)
		m3ApiGetArg   (uint32_t       , _bgfx__numIndices)
	bgfx_encoder_set_index_buffer(_bgfx_this, _bgfx__handle, _bgfx__firstIndex, _bgfx__numIndices);
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_encoder_set_vertex_buffer)
	{
		m3ApiGetArgMem   (bgfx_encoder_t*       , _bgfx_this)
		m3ApiGetArg   (uint8_t       , _bgfx__stream)
		m3ApiGetArg   (bgfx_vertex_buffer_handle_t       , _bgfx__handle)
		m3ApiGetArg   (uint32_t       , _bgfx__startVertex)
		m3ApiGetArg   (uint32_t       , _bgfx__numVertices)
		m3ApiGetArg   (bgfx_vertex_layout_handle_t       , _bgfx__layoutHandle)
	bgfx_encoder_set_vertex_buffer(_bgfx_this, _bgfx__stream, _bgfx__handle, _bgfx__startVertex, _bgfx__numVertices, _bgfx__layoutHandle);
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_encoder_set_texture)
	{
		m3ApiGetArgMem   (bgfx_encoder_t*       , _bgfx_this)
		m3ApiGetArg   (uint8_t       , _bgfx__stage)
		m3ApiGetArg   (bgfx_uniform_handle_t       , _bgfx__sampler)
		m3ApiGetArg   (bgfx_texture_handle_t       , _bgfx__handle)
		m3ApiGetArg   (uint32_t       , _bgfx__flags)
	bgfx_encoder_set_texture(_bgfx_this, _bgfx__stage, _bgfx__sampler, _bgfx__handle, _bgfx__flags);
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_encoder_submit)
	{
		m3ApiGetArgMem   (bgfx_encoder_t*       , _bgfx_this)
		m3ApiGetArg   (bgfx_view_id_t       , _bgfx__id)
		m3ApiGetArg   (bgfx_program_handle_t       , _bgfx__program)
		m3ApiGetArg   (uint32_t       , _bgfx__depth)
		m3ApiGetArg   (bool       , _bgfx__preserveState)
	bgfx_encoder_submit(_bgfx_this, _bgfx__id, _bgfx__program, _bgfx__depth, _bgfx__preserveState);
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_request_screen_shot)
	{
		m3ApiGetArg   (bgfx_frame_buffer_handle_t       , _bgfx__handle)
		m3ApiGetArgMem   (const char*       , _bgfx__filePath)
	bgfx_request_screen_shot(_bgfx__handle, _bgfx__filePath);
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_set_state)
	{
		m3ApiGetArg   (uint64_t       , _bgfx__state)
		m3ApiGetArg   (uint32_t       , _bgfx__rgba)
	bgfx_set_state(_bgfx__state, _bgfx__rgba);
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_set_transform)
	{
	m3ApiReturnType(int32_t)
		m3ApiGetArgMem   (const void*       , _bgfx__mtx)
		m3ApiGetArg   (uint16_t       , _bgfx__num)
	uint32_t ret = bgfx_set_transform(_bgfx__mtx, _bgfx__num);
	m3ApiReturn(ret)
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_set_uniform)
	{
		m3ApiGetArg   (bgfx_uniform_handle_t       , _bgfx__handle)
		m3ApiGetArgMem   (const void*       , _bgfx__value)
		m3ApiGetArg   (uint16_t       , _bgfx__num)
	bgfx_set_uniform(_bgfx__handle, _bgfx__value, _bgfx__num);
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_set_index_buffer)
	{
		m3ApiGetArg   (bgfx_index_buffer_handle_t       , _bgfx__handle)
		m3ApiGetArg   (uint32_t       , _bgfx__firstIndex)
		m3ApiGetArg   (uint32_t       , _bgfx__numIndices)
	bgfx_set_index_buffer(_bgfx__handle, _bgfx__firstIndex, _bgfx__numIndices);
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_set_vertex_buffer)
	{
		m3ApiGetArg   (uint8_t       , _bgfx__stream)
		m3ApiGetArg   (bgfx_vertex_buffer_handle_t       , _bgfx__handle)
		m3ApiGetArg   (uint32_t       , _bgfx__startVertex)
		m3ApiGetArg   (uint32_t       , _bgfx__numVertices)
	bgfx_set_vertex_buffer(_bgfx__stream, _bgfx__handle, _bgfx__startVertex, _bgfx__numVertices);
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_set_texture)
	{
		m3ApiGetArg   (uint8_t       , _bgfx__stage)
		m3ApiGetArg   (bgfx_uniform_handle_t       , _bgfx__sampler)
		m3ApiGetArg   (bgfx_texture_handle_t       , _bgfx__handle)
		m3ApiGetArg   (uint32_t       , _bgfx__flags)
	bgfx_set_texture(_bgfx__stage, _bgfx__sampler, _bgfx__handle, _bgfx__flags);
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_touch)
	{
		m3ApiGetArg   (bgfx_view_id_t       , _bgfx__id)
	bgfx_touch(_bgfx__id);
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_submit)
	{
		m3ApiGetArg   (bgfx_view_id_t       , _bgfx__id)
		m3ApiGetArg   (bgfx_program_handle_t       , _bgfx__program)
		m3ApiGetArg   (uint32_t       , _bgfx__depth)
		m3ApiGetArg   (bool       , _bgfx__preserveState)
	bgfx_submit(_bgfx__id, _bgfx__program, _bgfx__depth, _bgfx__preserveState);
	return m3Err_none;
	}
	

static
M3Result SuppressLookupFailure(M3Result i_result)
{
    if (i_result == m3Err_functionLookupFailed)
        return m3Err_none;
    else
        return i_result;
}

M3Result  m3_LinkBGFX  (IM3Module module, void* handle)
{
    M3Result result = m3Err_none;
    whandle = handle;

   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_vertex_layout_begin", "*(*i)", &m3_bgfx_vertex_layout_begin)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_vertex_layout_add", "*(*iiiii)", &m3_bgfx_vertex_layout_add)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_vertex_layout_end", "v(*)", &m3_bgfx_vertex_layout_end)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_init", "i(*)", &m3_bgfx_init)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_reset", "v(iiii)", &m3_bgfx_reset)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_frame", "i(i)", &m3_bgfx_frame)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_get_renderer_type", "i()", &m3_bgfx_get_renderer_type)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_get_caps", "*()", &m3_bgfx_get_caps)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_copy", "*(*i)", &m3_bgfx_copy)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_set_debug", "v(i)", &m3_bgfx_set_debug)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_dbg_text_clear", "v(ii)", &m3_bgfx_dbg_text_clear)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_create_index_buffer", "*(*i)", &m3_bgfx_create_index_buffer)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_create_vertex_layout", "*(*)", &m3_bgfx_create_vertex_layout)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_create_vertex_buffer", "*(**i)", &m3_bgfx_create_vertex_buffer)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_alloc_transient_buffers", "i(**i*i)", &m3_bgfx_alloc_transient_buffers)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_create_shader", "*(*)", &m3_bgfx_create_shader)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_set_shader_name", "v(**i)", &m3_bgfx_set_shader_name)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_create_program", "*(**i)", &m3_bgfx_create_program)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_create_texture_2d", "*(iiiiiI*)", &m3_bgfx_create_texture_2d)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_destroy_texture", "v(*)", &m3_bgfx_destroy_texture)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_create_frame_buffer_from_attachment", "*(i*i)", &m3_bgfx_create_frame_buffer_from_attachment)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_create_uniform", "*(*ii)", &m3_bgfx_create_uniform)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_set_view_rect", "v(iiiii)", &m3_bgfx_set_view_rect)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_set_view_scissor", "v(iiiii)", &m3_bgfx_set_view_scissor)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_set_view_clear", "v(iiifi)", &m3_bgfx_set_view_clear)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_set_view_mode", "v(ii)", &m3_bgfx_set_view_mode)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_set_view_frame_buffer", "v(i*)", &m3_bgfx_set_view_frame_buffer)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_set_view_transform", "v(i**)", &m3_bgfx_set_view_transform)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_encoder_begin", "*(i)", &m3_bgfx_encoder_begin)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_encoder_end", "v(*)", &m3_bgfx_encoder_end)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_encoder_set_state", "v(*Ii)", &m3_bgfx_encoder_set_state)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_encoder_set_transform", "i(**i)", &m3_bgfx_encoder_set_transform)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_encoder_set_uniform", "v(***i)", &m3_bgfx_encoder_set_uniform)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_encoder_set_index_buffer", "v(**ii)", &m3_bgfx_encoder_set_index_buffer)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_encoder_set_vertex_buffer", "v(*i*ii*)", &m3_bgfx_encoder_set_vertex_buffer)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_encoder_set_texture", "v(*i**i)", &m3_bgfx_encoder_set_texture)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_encoder_submit", "v(*i*ii)", &m3_bgfx_encoder_submit)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_request_screen_shot", "v(**)", &m3_bgfx_request_screen_shot)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_set_state", "v(Ii)", &m3_bgfx_set_state)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_set_transform", "i(*i)", &m3_bgfx_set_transform)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_set_uniform", "v(**i)", &m3_bgfx_set_uniform)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_set_index_buffer", "v(*ii)", &m3_bgfx_set_index_buffer)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_set_vertex_buffer", "v(i*ii)", &m3_bgfx_set_vertex_buffer)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_set_texture", "v(i**i)", &m3_bgfx_set_texture)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_touch", "v(i)", &m3_bgfx_touch)));
	
	   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_submit", "v(i*ii)", &m3_bgfx_submit)));
	

_catch:
    return result;
}
