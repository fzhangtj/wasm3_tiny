
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
	BGFX_CALL_TRACE("vertex_layout_begin");
	m3ApiReturn(m3ApiPtrToOffset(ret))
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
	BGFX_CALL_TRACE("vertex_layout_add");
	m3ApiReturn(m3ApiPtrToOffset(ret))
	}
	
	m3ApiRawFunction(m3_bgfx_vertex_layout_end)
	{
		m3ApiGetArgMem   (bgfx_vertex_layout_t*       , _bgfx_this)
	bgfx_vertex_layout_end(_bgfx_this);
	BGFX_CALL_TRACE("vertex_layout_end");
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_get_renderer_name)
	{
	m3ApiReturnType(ptr32)
		m3ApiGetArg   (bgfx_renderer_type_t       , _bgfx__type)
	const char* ret = bgfx_get_renderer_name(_bgfx__type);
	BGFX_CALL_TRACE("get_renderer_name");
	m3ApiReturn(m3ApiPtrToOffset(ret))
	}
	
	m3ApiRawFunction(m3_bgfx_reset)
	{
		m3ApiGetArg   (uint32_t       , _bgfx__width)
		m3ApiGetArg   (uint32_t       , _bgfx__height)
		m3ApiGetArg   (uint32_t       , _bgfx__flags)
		m3ApiGetArg   (bgfx_texture_format_t       , _bgfx__format)
	bgfx_reset(_bgfx__width, _bgfx__height, _bgfx__flags, _bgfx__format);
	BGFX_CALL_TRACE("reset");
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_frame)
	{
	m3ApiReturnType(int32_t)
		m3ApiGetArg   (bool       , _bgfx__capture)
	uint32_t ret = bgfx_frame(_bgfx__capture);
	BGFX_CALL_TRACE("frame");
	m3ApiReturn(ret)
	}
	
	m3ApiRawFunction(m3_bgfx_get_renderer_type)
	{
	m3ApiReturnType(int32_t)
	bgfx_renderer_type_t ret = bgfx_get_renderer_type();
	BGFX_CALL_TRACE("get_renderer_type");
	m3ApiReturn(ret)
	}
	
	m3ApiRawFunction(m3_bgfx_set_debug)
	{
		m3ApiGetArg   (uint32_t       , _bgfx__debug)
	bgfx_set_debug(_bgfx__debug);
	BGFX_CALL_TRACE("set_debug");
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_dbg_text_clear)
	{
		m3ApiGetArg   (uint8_t       , _bgfx__attr)
		m3ApiGetArg   (bool       , _bgfx__small)
	bgfx_dbg_text_clear(_bgfx__attr, _bgfx__small);
	BGFX_CALL_TRACE("dbg_text_clear");
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_create_index_buffer)
	{
	m3ApiReturnType(ptr32)
		m3BgfxApiMemoryArg(_bgfx__mem)
		m3ApiGetArg   (uint16_t       , _bgfx__flags)
	bgfx_index_buffer_handle_t ret = bgfx_create_index_buffer(_bgfx__mem, _bgfx__flags);
	BGFX_CALL_TRACE("create_index_buffer");
	m3ApiReturn(bgfx_handle_to_idx(ret))
	}
	
	m3ApiRawFunction(m3_bgfx_create_vertex_layout)
	{
	m3ApiReturnType(ptr32)
		m3ApiGetArgMem   (const bgfx_vertex_layout_t *       , _bgfx__layout)
	bgfx_vertex_layout_handle_t ret = bgfx_create_vertex_layout(_bgfx__layout);
	BGFX_CALL_TRACE("create_vertex_layout");
	m3ApiReturn(bgfx_handle_to_idx(ret))
	}
	
	m3ApiRawFunction(m3_bgfx_create_vertex_buffer)
	{
	m3ApiReturnType(ptr32)
		m3BgfxApiMemoryArg(_bgfx__mem)
		m3ApiGetArgMem   (const bgfx_vertex_layout_t *       , _bgfx__layout)
		m3ApiGetArg   (uint16_t       , _bgfx__flags)
	bgfx_vertex_buffer_handle_t ret = bgfx_create_vertex_buffer(_bgfx__mem, _bgfx__layout, _bgfx__flags);
	BGFX_CALL_TRACE("create_vertex_buffer");
	m3ApiReturn(bgfx_handle_to_idx(ret))
	}
	
	m3ApiRawFunction(m3_bgfx_get_avail_transient_index_buffer)
	{
	m3ApiReturnType(int32_t)
		m3ApiGetArg   (uint32_t       , _bgfx__num)
	uint32_t ret = bgfx_get_avail_transient_index_buffer(_bgfx__num);
	BGFX_CALL_TRACE("get_avail_transient_index_buffer");
	m3ApiReturn(ret)
	}
	
	m3ApiRawFunction(m3_bgfx_get_avail_transient_vertex_buffer)
	{
	m3ApiReturnType(int32_t)
		m3ApiGetArg   (uint32_t       , _bgfx__num)
		m3ApiGetArgMem   (const bgfx_vertex_layout_t *       , _bgfx__layout)
	uint32_t ret = bgfx_get_avail_transient_vertex_buffer(_bgfx__num, _bgfx__layout);
	BGFX_CALL_TRACE("get_avail_transient_vertex_buffer");
	m3ApiReturn(ret)
	}
	
	m3ApiRawFunction(m3_bgfx_alloc_transient_index_buffer)
	{
		m3BgfxApiTransientIndexBufferArg(_bgfx__tib)
		m3ApiGetArg   (uint32_t       , _bgfx__num)
	bgfx_alloc_transient_index_buffer(_bgfx__tib, _bgfx__num);
	transient_index_buffer_t_to_wasm(_wasm__bgfx__tib, _bgfx__tib, _mem);
	BGFX_CALL_TRACE("alloc_transient_index_buffer");
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_alloc_transient_vertex_buffer)
	{
		m3BgfxApiTransientVertexBufferArg(_bgfx__tvb)
		m3ApiGetArg   (uint32_t       , _bgfx__num)
		m3ApiGetArgMem   (const bgfx_vertex_layout_t *       , _bgfx__layout)
	bgfx_alloc_transient_vertex_buffer(_bgfx__tvb, _bgfx__num, _bgfx__layout);
	transient_vertex_buffer_t_to_wasm(_wasm__bgfx__tvb, _bgfx__tvb, _mem);
	BGFX_CALL_TRACE("alloc_transient_vertex_buffer");
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_alloc_transient_buffers)
	{
	m3ApiReturnType(int32_t)
		m3BgfxApiTransientVertexBufferArg(_bgfx__tvb)
		m3ApiGetArgMem   (const bgfx_vertex_layout_t *       , _bgfx__layout)
		m3ApiGetArg   (uint32_t       , _bgfx__numVertices)
		m3BgfxApiTransientIndexBufferArg(_bgfx__tib)
		m3ApiGetArg   (uint32_t       , _bgfx__numIndices)
	bool ret = bgfx_alloc_transient_buffers(_bgfx__tvb, _bgfx__layout, _bgfx__numVertices, _bgfx__tib, _bgfx__numIndices);
	transient_index_buffer_t_to_wasm(_wasm__bgfx__tib, _bgfx__tib, _mem);
	transient_vertex_buffer_t_to_wasm(_wasm__bgfx__tvb, _bgfx__tvb, _mem);
	BGFX_CALL_TRACE("alloc_transient_buffers");
	m3ApiReturn(ret)
	}
	
	m3ApiRawFunction(m3_bgfx_create_shader)
	{
	m3ApiReturnType(ptr32)
		m3BgfxApiMemoryArg(_bgfx__mem)
	bgfx_shader_handle_t ret = bgfx_create_shader(_bgfx__mem);
	BGFX_CALL_TRACE("create_shader");
	m3ApiReturn(bgfx_handle_to_idx(ret))
	}
	
	m3ApiRawFunction(m3_bgfx_set_shader_name)
	{
		m3ApiGetArg   (bgfx_shader_handle_t       , _bgfx__handle)
		m3ApiGetArgMem   (const char*       , _bgfx__name)
		m3ApiGetArg   (int32_t       , _bgfx__len)
	bgfx_set_shader_name(_bgfx__handle, _bgfx__name, _bgfx__len);
	BGFX_CALL_TRACE("set_shader_name");
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_create_program)
	{
	m3ApiReturnType(ptr32)
		m3ApiGetArg   (bgfx_shader_handle_t       , _bgfx__vsh)
		m3ApiGetArg   (bgfx_shader_handle_t       , _bgfx__fsh)
		m3ApiGetArg   (bool       , _bgfx__destroyShaders)
	bgfx_program_handle_t ret = bgfx_create_program(_bgfx__vsh, _bgfx__fsh, _bgfx__destroyShaders);
	BGFX_CALL_TRACE("create_program");
	m3ApiReturn(bgfx_handle_to_idx(ret))
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
		m3BgfxApiMemoryArg(_bgfx__mem)
	bgfx_texture_handle_t ret = bgfx_create_texture_2d(_bgfx__width, _bgfx__height, _bgfx__hasMips, _bgfx__numLayers, _bgfx__format, _bgfx__flags, _bgfx__mem);
	BGFX_CALL_TRACE("create_texture_2d");
	m3ApiReturn(bgfx_handle_to_idx(ret))
	}
	
	m3ApiRawFunction(m3_bgfx_destroy_texture)
	{
		m3ApiGetArg   (bgfx_texture_handle_t       , _bgfx__handle)
	bgfx_destroy_texture(_bgfx__handle);
	BGFX_CALL_TRACE("destroy_texture");
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_create_frame_buffer_from_attachment)
	{
	m3ApiReturnType(ptr32)
		m3ApiGetArg   (uint8_t       , _bgfx__num)
		m3ApiGetArgMem   (const bgfx_attachment_t*       , _bgfx__attachment)
		m3ApiGetArg   (bool       , _bgfx__destroyTexture)
	bgfx_frame_buffer_handle_t ret = bgfx_create_frame_buffer_from_attachment(_bgfx__num, _bgfx__attachment, _bgfx__destroyTexture);
	BGFX_CALL_TRACE("create_frame_buffer_from_attachment");
	m3ApiReturn(bgfx_handle_to_idx(ret))
	}
	
	m3ApiRawFunction(m3_bgfx_create_uniform)
	{
	m3ApiReturnType(ptr32)
		m3ApiGetArgMem   (const char*       , _bgfx__name)
		m3ApiGetArg   (bgfx_uniform_type_t       , _bgfx__type)
		m3ApiGetArg   (uint16_t       , _bgfx__num)
	bgfx_uniform_handle_t ret = bgfx_create_uniform(_bgfx__name, _bgfx__type, _bgfx__num);
	BGFX_CALL_TRACE("create_uniform");
	m3ApiReturn(bgfx_handle_to_idx(ret))
	}
	
	m3ApiRawFunction(m3_bgfx_set_view_rect)
	{
		m3ApiGetArg   (bgfx_view_id_t       , _bgfx__id)
		m3ApiGetArg   (uint16_t       , _bgfx__x)
		m3ApiGetArg   (uint16_t       , _bgfx__y)
		m3ApiGetArg   (uint16_t       , _bgfx__width)
		m3ApiGetArg   (uint16_t       , _bgfx__height)
	bgfx_set_view_rect(_bgfx__id, _bgfx__x, _bgfx__y, _bgfx__width, _bgfx__height);
	BGFX_CALL_TRACE("set_view_rect");
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
	BGFX_CALL_TRACE("set_view_scissor");
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
	BGFX_CALL_TRACE("set_view_clear");
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_set_view_mode)
	{
		m3ApiGetArg   (bgfx_view_id_t       , _bgfx__id)
		m3ApiGetArg   (bgfx_view_mode_t       , _bgfx__mode)
	bgfx_set_view_mode(_bgfx__id, _bgfx__mode);
	BGFX_CALL_TRACE("set_view_mode");
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_set_view_frame_buffer)
	{
		m3ApiGetArg   (bgfx_view_id_t       , _bgfx__id)
		m3ApiGetArg   (bgfx_frame_buffer_handle_t       , _bgfx__handle)
	bgfx_set_view_frame_buffer(_bgfx__id, _bgfx__handle);
	BGFX_CALL_TRACE("set_view_frame_buffer");
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_set_view_transform)
	{
		m3ApiGetArg   (bgfx_view_id_t       , _bgfx__id)
		m3ApiGetArgMem   (const void*       , _bgfx__view)
		m3ApiGetArgMem   (const void*       , _bgfx__proj)
	bgfx_set_view_transform(_bgfx__id, _bgfx__view, _bgfx__proj);
	BGFX_CALL_TRACE("set_view_transform");
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_encoder_begin)
	{
	m3ApiReturnType(ptr32)
		m3ApiGetArg   (bool       , _bgfx__forThread)
	bgfx_encoder_t* ret = bgfx_encoder_begin(_bgfx__forThread);
	BGFX_CALL_TRACE("encoder_begin");
	m3ApiReturn(m3ApiPtrToOffset(ret))
	}
	
	m3ApiRawFunction(m3_bgfx_encoder_end)
	{
		m3ApiGetArgMem   (bgfx_encoder_t*       , _bgfx__encoder)
	bgfx_encoder_end(_bgfx__encoder);
	BGFX_CALL_TRACE("encoder_end");
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_encoder_set_state)
	{
		m3ApiGetArgMem   (bgfx_encoder_t*       , _bgfx_this)
		m3ApiGetArg   (uint64_t       , _bgfx__state)
		m3ApiGetArg   (uint32_t       , _bgfx__rgba)
	bgfx_encoder_set_state(_bgfx_this, _bgfx__state, _bgfx__rgba);
	BGFX_CALL_TRACE("encoder_set_state");
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_encoder_set_transform)
	{
	m3ApiReturnType(int32_t)
		m3ApiGetArgMem   (bgfx_encoder_t*       , _bgfx_this)
		m3ApiGetArgMem   (const void*       , _bgfx__mtx)
		m3ApiGetArg   (uint16_t       , _bgfx__num)
	uint32_t ret = bgfx_encoder_set_transform(_bgfx_this, _bgfx__mtx, _bgfx__num);
	BGFX_CALL_TRACE("encoder_set_transform");
	m3ApiReturn(ret)
	}
	
	m3ApiRawFunction(m3_bgfx_encoder_set_uniform)
	{
		m3ApiGetArgMem   (bgfx_encoder_t*       , _bgfx_this)
		m3ApiGetArg   (bgfx_uniform_handle_t       , _bgfx__handle)
		m3ApiGetArgMem   (const void*       , _bgfx__value)
		m3ApiGetArg   (uint16_t       , _bgfx__num)
	bgfx_encoder_set_uniform(_bgfx_this, _bgfx__handle, _bgfx__value, _bgfx__num);
	BGFX_CALL_TRACE("encoder_set_uniform");
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_encoder_set_index_buffer)
	{
		m3ApiGetArgMem   (bgfx_encoder_t*       , _bgfx_this)
		m3ApiGetArg   (bgfx_index_buffer_handle_t       , _bgfx__handle)
		m3ApiGetArg   (uint32_t       , _bgfx__firstIndex)
		m3ApiGetArg   (uint32_t       , _bgfx__numIndices)
	bgfx_encoder_set_index_buffer(_bgfx_this, _bgfx__handle, _bgfx__firstIndex, _bgfx__numIndices);
	BGFX_CALL_TRACE("encoder_set_index_buffer");
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_encoder_set_transient_index_buffer)
	{
		m3ApiGetArgMem   (bgfx_encoder_t*       , _bgfx_this)
		m3BgfxApiTransientIndexBufferArg(_bgfx__tib)
		m3ApiGetArg   (uint32_t       , _bgfx__firstIndex)
		m3ApiGetArg   (uint32_t       , _bgfx__numIndices)
	bgfx_encoder_set_transient_index_buffer(_bgfx_this, _bgfx__tib, _bgfx__firstIndex, _bgfx__numIndices);
	BGFX_CALL_TRACE("encoder_set_transient_index_buffer");
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
	BGFX_CALL_TRACE("encoder_set_vertex_buffer");
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_encoder_set_transient_vertex_buffer)
	{
		m3ApiGetArgMem   (bgfx_encoder_t*       , _bgfx_this)
		m3ApiGetArg   (uint8_t       , _bgfx__stream)
		m3BgfxApiTransientVertexBufferArg(_bgfx__tvb)
		m3ApiGetArg   (uint32_t       , _bgfx__startVertex)
		m3ApiGetArg   (uint32_t       , _bgfx__numVertices)
		m3ApiGetArg   (bgfx_vertex_layout_handle_t       , _bgfx__layoutHandle)
	bgfx_encoder_set_transient_vertex_buffer(_bgfx_this, _bgfx__stream, _bgfx__tvb, _bgfx__startVertex, _bgfx__numVertices, _bgfx__layoutHandle);
	BGFX_CALL_TRACE("encoder_set_transient_vertex_buffer");
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
	BGFX_CALL_TRACE("encoder_set_texture");
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
	BGFX_CALL_TRACE("encoder_submit");
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_request_screen_shot)
	{
		m3ApiGetArg   (bgfx_frame_buffer_handle_t       , _bgfx__handle)
		m3ApiGetArgMem   (const char*       , _bgfx__filePath)
	bgfx_request_screen_shot(_bgfx__handle, _bgfx__filePath);
	BGFX_CALL_TRACE("request_screen_shot");
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_set_platform_data)
	{
		m3BgfxApiGetPlatformDataArg(_bgfx__data)
	bgfx_set_platform_data(_bgfx__data);
	BGFX_CALL_TRACE("set_platform_data");
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_set_state)
	{
		m3ApiGetArg   (uint64_t       , _bgfx__state)
		m3ApiGetArg   (uint32_t       , _bgfx__rgba)
	bgfx_set_state(_bgfx__state, _bgfx__rgba);
	BGFX_CALL_TRACE("set_state");
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_set_transform)
	{
	m3ApiReturnType(int32_t)
		m3ApiGetArgMem   (const void*       , _bgfx__mtx)
		m3ApiGetArg   (uint16_t       , _bgfx__num)
	uint32_t ret = bgfx_set_transform(_bgfx__mtx, _bgfx__num);
	BGFX_CALL_TRACE("set_transform");
	m3ApiReturn(ret)
	}
	
	m3ApiRawFunction(m3_bgfx_set_uniform)
	{
		m3ApiGetArg   (bgfx_uniform_handle_t       , _bgfx__handle)
		m3ApiGetArgMem   (const void*       , _bgfx__value)
		m3ApiGetArg   (uint16_t       , _bgfx__num)
	bgfx_set_uniform(_bgfx__handle, _bgfx__value, _bgfx__num);
	BGFX_CALL_TRACE("set_uniform");
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_set_index_buffer)
	{
		m3ApiGetArg   (bgfx_index_buffer_handle_t       , _bgfx__handle)
		m3ApiGetArg   (uint32_t       , _bgfx__firstIndex)
		m3ApiGetArg   (uint32_t       , _bgfx__numIndices)
	bgfx_set_index_buffer(_bgfx__handle, _bgfx__firstIndex, _bgfx__numIndices);
	BGFX_CALL_TRACE("set_index_buffer");
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_set_transient_index_buffer)
	{
		m3BgfxApiTransientIndexBufferArg(_bgfx__tib)
		m3ApiGetArg   (uint32_t       , _bgfx__firstIndex)
		m3ApiGetArg   (uint32_t       , _bgfx__numIndices)
	bgfx_set_transient_index_buffer(_bgfx__tib, _bgfx__firstIndex, _bgfx__numIndices);
	BGFX_CALL_TRACE("set_transient_index_buffer");
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_set_vertex_buffer)
	{
		m3ApiGetArg   (uint8_t       , _bgfx__stream)
		m3ApiGetArg   (bgfx_vertex_buffer_handle_t       , _bgfx__handle)
		m3ApiGetArg   (uint32_t       , _bgfx__startVertex)
		m3ApiGetArg   (uint32_t       , _bgfx__numVertices)
	bgfx_set_vertex_buffer(_bgfx__stream, _bgfx__handle, _bgfx__startVertex, _bgfx__numVertices);
	BGFX_CALL_TRACE("set_vertex_buffer");
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_set_transient_vertex_buffer)
	{
		m3ApiGetArg   (uint8_t       , _bgfx__stream)
		m3BgfxApiTransientVertexBufferArg(_bgfx__tvb)
		m3ApiGetArg   (uint32_t       , _bgfx__startVertex)
		m3ApiGetArg   (uint32_t       , _bgfx__numVertices)
	bgfx_set_transient_vertex_buffer(_bgfx__stream, _bgfx__tvb, _bgfx__startVertex, _bgfx__numVertices);
	BGFX_CALL_TRACE("set_transient_vertex_buffer");
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_set_texture)
	{
		m3ApiGetArg   (uint8_t       , _bgfx__stage)
		m3ApiGetArg   (bgfx_uniform_handle_t       , _bgfx__sampler)
		m3ApiGetArg   (bgfx_texture_handle_t       , _bgfx__handle)
		m3ApiGetArg   (uint32_t       , _bgfx__flags)
	bgfx_set_texture(_bgfx__stage, _bgfx__sampler, _bgfx__handle, _bgfx__flags);
	BGFX_CALL_TRACE("set_texture");
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_touch)
	{
		m3ApiGetArg   (bgfx_view_id_t       , _bgfx__id)
	bgfx_touch(_bgfx__id);
	BGFX_CALL_TRACE("touch");
	return m3Err_none;
	}
	
	m3ApiRawFunction(m3_bgfx_submit)
	{
		m3ApiGetArg   (bgfx_view_id_t       , _bgfx__id)
		m3ApiGetArg   (bgfx_program_handle_t       , _bgfx__program)
		m3ApiGetArg   (uint32_t       , _bgfx__depth)
		m3ApiGetArg   (bool       , _bgfx__preserveState)
	bgfx_submit(_bgfx__id, _bgfx__program, _bgfx__depth, _bgfx__preserveState);
	BGFX_CALL_TRACE("submit");
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

M3Result  m3_LinkBGFX_Gen  (IM3Module module, void* handle)
{
    M3Result result = m3Err_none;
    whandle = handle;

_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_vertex_layout_begin", "*(*i)", &m3_bgfx_vertex_layout_begin)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_vertex_layout_add", "*(*iiiii)", &m3_bgfx_vertex_layout_add)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_vertex_layout_end", "v(*)", &m3_bgfx_vertex_layout_end)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_get_renderer_name", "*(i)", &m3_bgfx_get_renderer_name)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_reset", "v(iiii)", &m3_bgfx_reset)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_frame", "i(i)", &m3_bgfx_frame)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_get_renderer_type", "i()", &m3_bgfx_get_renderer_type)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_set_debug", "v(i)", &m3_bgfx_set_debug)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_dbg_text_clear", "v(ii)", &m3_bgfx_dbg_text_clear)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_create_index_buffer", "*(*i)", &m3_bgfx_create_index_buffer)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_create_vertex_layout", "*(*)", &m3_bgfx_create_vertex_layout)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_create_vertex_buffer", "*(**i)", &m3_bgfx_create_vertex_buffer)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_get_avail_transient_index_buffer", "i(i)", &m3_bgfx_get_avail_transient_index_buffer)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_get_avail_transient_vertex_buffer", "i(i*)", &m3_bgfx_get_avail_transient_vertex_buffer)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_alloc_transient_index_buffer", "v(*i)", &m3_bgfx_alloc_transient_index_buffer)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_alloc_transient_vertex_buffer", "v(*i*)", &m3_bgfx_alloc_transient_vertex_buffer)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_alloc_transient_buffers", "i(**i*i)", &m3_bgfx_alloc_transient_buffers)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_create_shader", "*(*)", &m3_bgfx_create_shader)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_set_shader_name", "v(**i)", &m3_bgfx_set_shader_name)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_create_program", "*(**i)", &m3_bgfx_create_program)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_create_texture_2d", "*(iiiiiI*)", &m3_bgfx_create_texture_2d)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_destroy_texture", "v(*)", &m3_bgfx_destroy_texture)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_create_frame_buffer_from_attachment", "*(i*i)", &m3_bgfx_create_frame_buffer_from_attachment)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_create_uniform", "*(*ii)", &m3_bgfx_create_uniform)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_set_view_rect", "v(iiiii)", &m3_bgfx_set_view_rect)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_set_view_scissor", "v(iiiii)", &m3_bgfx_set_view_scissor)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_set_view_clear", "v(iiifi)", &m3_bgfx_set_view_clear)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_set_view_mode", "v(ii)", &m3_bgfx_set_view_mode)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_set_view_frame_buffer", "v(i*)", &m3_bgfx_set_view_frame_buffer)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_set_view_transform", "v(i**)", &m3_bgfx_set_view_transform)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_encoder_begin", "*(i)", &m3_bgfx_encoder_begin)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_encoder_end", "v(*)", &m3_bgfx_encoder_end)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_encoder_set_state", "v(*Ii)", &m3_bgfx_encoder_set_state)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_encoder_set_transform", "i(**i)", &m3_bgfx_encoder_set_transform)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_encoder_set_uniform", "v(***i)", &m3_bgfx_encoder_set_uniform)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_encoder_set_index_buffer", "v(**ii)", &m3_bgfx_encoder_set_index_buffer)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_encoder_set_transient_index_buffer", "v(**ii)", &m3_bgfx_encoder_set_transient_index_buffer)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_encoder_set_vertex_buffer", "v(*i*ii*)", &m3_bgfx_encoder_set_vertex_buffer)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_encoder_set_transient_vertex_buffer", "v(*i*ii*)", &m3_bgfx_encoder_set_transient_vertex_buffer)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_encoder_set_texture", "v(*i**i)", &m3_bgfx_encoder_set_texture)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_encoder_submit", "v(*i*ii)", &m3_bgfx_encoder_submit)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_request_screen_shot", "v(**)", &m3_bgfx_request_screen_shot)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_set_platform_data", "v(*)", &m3_bgfx_set_platform_data)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_set_state", "v(Ii)", &m3_bgfx_set_state)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_set_transform", "i(*i)", &m3_bgfx_set_transform)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_set_uniform", "v(**i)", &m3_bgfx_set_uniform)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_set_index_buffer", "v(*ii)", &m3_bgfx_set_index_buffer)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_set_transient_index_buffer", "v(*ii)", &m3_bgfx_set_transient_index_buffer)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_set_vertex_buffer", "v(i*ii)", &m3_bgfx_set_vertex_buffer)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_set_transient_vertex_buffer", "v(i*ii)", &m3_bgfx_set_transient_vertex_buffer)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_set_texture", "v(i**i)", &m3_bgfx_set_texture)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_touch", "v(i)", &m3_bgfx_touch)));
	
	_   (SuppressLookupFailure (m3_LinkRawFunction (module, "env", "bgfx_submit", "v(i*ii)", &m3_bgfx_submit)));
	

_catch:
    return result;
}
