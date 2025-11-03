#pragma once
#include "thread.h"
#include "common.h"
#include "math.h"

#pragma warning(push, 0)
#pragma push_macro("OS_WINDOWS")
#undef OS_WINDOWS
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d3d11.h>
#include <d3d11_3.h>
#include <d3dcompiler.h>
#pragma pop_macro("OS_WINDOWS")
#pragma warning(pop)

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxgi")

#ifndef TL_HRESULT_HANDLER
#define TL_HRESULT_HANDLER(hr) assert(SUCCEEDED(hr))
#endif

#define TL_COM_RELEASE(x) (((x) ? (x)->Release() : 0), (x) = 0)

#pragma warning(push)
#pragma warning(disable: 4820) // struct padding

namespace tl { namespace d3d11 {

struct Shader {
	ID3D11VertexShader *vs = 0;
	ID3D11PixelShader *ps = 0;
	
	inline void add_ref() { vs->AddRef(); ps->AddRef(); }
	inline void release() { TL_COM_RELEASE(vs); TL_COM_RELEASE(ps); }
	inline bool is_valid() { return vs; }
};

struct ShaderResource {
	ID3D11ShaderResourceView *srv = 0;
	
	inline void add_ref() { srv->AddRef(); }
	inline void release() { TL_COM_RELEASE(srv); }
	inline bool is_valid() { return srv; }
};

struct StructuredBuffer : ShaderResource {
	ID3D11Buffer *buffer = 0;
	D3D11_USAGE usage = {};
	u32 size = 0;
	
	inline void add_ref() { buffer->AddRef(); ShaderResource::add_ref(); }
	inline void release() { TL_COM_RELEASE(buffer); ShaderResource::release(); }
	inline bool is_valid() { return buffer; }
};

template <class T>
struct TypedStructuredBuffer : StructuredBuffer {};

struct ConstantBuffer {
	ID3D11Buffer *buffer = 0;
	D3D11_USAGE usage = {};
	u32 size = 0;
	
	inline void add_ref() { buffer->AddRef(); }
	inline void release() { TL_COM_RELEASE(buffer); }
	inline bool is_valid() { return buffer; }
};

template <class T>
struct TypedConstantBuffer : ConstantBuffer {};

// TODO: maybe inherit from Texture2D ?
struct DepthStencil {
	ID3D11DepthStencilView *dsv = 0;
	ID3D11Texture2D *tex = 0;
	
	inline void add_ref() { dsv->AddRef(); tex->AddRef(); }
	inline void release() { TL_COM_RELEASE(dsv); TL_COM_RELEASE(tex); }
	inline bool is_valid() { return dsv; }
};

struct Texture2D : ShaderResource {
	ID3D11Texture2D *tex = 0;
	u32 width = 0;
	u32 height = 0;
	
	inline void add_ref() { tex->AddRef(); }
	inline void release() { TL_COM_RELEASE(tex); }
	inline bool is_valid() { return tex; }
};

struct Sampler {
	ID3D11SamplerState *sampler = 0;
	
	inline void add_ref() { sampler->AddRef(); }
	inline void release() { TL_COM_RELEASE(sampler); }
	inline bool is_valid() { return sampler; }
};

struct RenderTarget {
	ID3D11RenderTargetView *rtv = 0;
	ID3D11Texture2D *tex = 0;
	
	inline void add_ref() { rtv->AddRef(); tex->AddRef(); }
	inline void release() { TL_COM_RELEASE(rtv); TL_COM_RELEASE(tex); }
	inline bool is_valid() { return rtv; }
};

struct RenderTexture : ShaderResource, RenderTarget {
};

using Rasterizer = ID3D11RasterizerState *;
using Blend = ID3D11BlendState *;

TL_API void default_shader_handler(HRESULT result, char const *messages);

enum class SyncInterval : UINT {
	immediate = 0,
	every_1   = 1,
	every_2   = 2,
	every_3   = 3,
	every_4   = 4,
};

#define TL_STRUCT_FLAGS_DEFINE_BIT(name) bool name : 1 = false;
#define TL_STRUCT_FLAGS_INVERT(name) a.name = !a.name;
#define TL_STRUCT_FLAGS_OR(name) a.name |= b.name;
#define TL_STRUCT_FLAGS_AND(name) a.name &= b.name;
#define TL_STRUCT_FLAGS_TO_BOOL(name) | name

#define TL_STRUCT_FLAGS(Name, ENUMERATE_BIT_NAMES)                     \
	struct Name {                                                      \
		ENUMERATE_BIT_NAMES(TL_STRUCT_FLAGS_DEFINE_BIT)                \
		friend Name operator~(Name a) {                                \
			ENUMERATE_BIT_NAMES(TL_STRUCT_FLAGS_INVERT)                \
			return a;                                                  \
		}                                                              \
                                                                       \
		friend Name operator|(Name a, Name b) {                        \
			ENUMERATE_BIT_NAMES(TL_STRUCT_FLAGS_OR)                    \
			return a;                                                  \
		}                                                              \
                                                                       \
		friend Name operator&(Name a, Name b) {                        \
			ENUMERATE_BIT_NAMES(TL_STRUCT_FLAGS_AND)                   \
			return a;                                                  \
		}                                                              \
                                                                       \
		operator bool() const {                                        \
			return false ENUMERATE_BIT_NAMES(TL_STRUCT_FLAGS_TO_BOOL); \
		}                                                              \
	}

#define TL_D3D11_ENUMERATE_SHADER_STAGE(x) \
	x(vs) \
	x(hs) \
	x(ds) \
	x(gs) \
	x(ps) \
	x(cs) \

TL_STRUCT_FLAGS(ShaderStage, TL_D3D11_ENUMERATE_SHADER_STAGE);

struct State;

struct Context {
	State *state;
	ID3D11DeviceContext *context;

	TL_API Texture2D create_texture_2d_with_mips(u32 width, u32 height, DXGI_FORMAT format, void const* data);
	
	TL_API void update(StructuredBuffer& buffer, u32 count, u32 stride, void const* data, u32 first_element_index = 0);
	template <class T>
	inline void update(TypedStructuredBuffer<T> &buffer, u32 count, T const *data, u32 first_element_index = 0) {
		update(buffer, count, sizeof(T), data, first_element_index);
	}
	TL_API void update(ConstantBuffer &buffer, u32 size, void const *data);
	template <class T>
	inline void update(TypedConstantBuffer<T> &buffer, T const *data) {
		update(buffer, sizeof(T), data);
	}
	
	TL_API void clear(ID3D11RenderTargetView* render_target, f32 const rgba[4]);
	TL_API void clear(ID3D11DepthStencilView *depth_stencil, f32 depth);
	inline void clear(RenderTarget &rt, f32 const rgba[4]) { clear(rt.rtv, rgba); }
	inline void clear(DepthStencil &depth_stencil, f32 depth) { clear(depth_stencil.dsv, depth); }
	TL_API void draw(u32 vertexCount, u32 offset = 0);
	
	TL_API void set_topology(D3D11_PRIMITIVE_TOPOLOGY topology);
	TL_API void set_vertex_shader(ID3D11VertexShader *shader);
	TL_API void set_pixel_shader(ID3D11PixelShader *shader);
	TL_API void set_shader_resource(ShaderResource const &resource, ShaderStage stage, u32 slot);
	TL_API void set_sampler(Sampler const &sampler, ShaderStage stage, u32 slot);
	TL_API void set_constant_buffer(ConstantBuffer const &buffer, ShaderStage stage, u32 slot);
	TL_API void set_viewport(f32 x, f32 y, f32 w, f32 h, f32 depthMin, f32 depthMax);
	inline void set_viewport(f32 x, f32 y, f32 w, f32 h) { set_viewport(x, y, w, h, 0, 1); }
	inline void set_viewport(f32 w, f32 h) { set_viewport(0, 0, w, h); }
	TL_API void set_render_target(ID3D11RenderTargetView *render_target, ID3D11DepthStencilView *depth_stencil);
	inline void set_render_target(RenderTarget &rt) { set_render_target(rt.rtv, 0); }
	inline void set_render_target(RenderTarget &rt, DepthStencil &ds) { set_render_target(rt.rtv, ds.dsv); }
	TL_API void set_rasterizer(Rasterizer rasterizer = {});
	TL_API void set_blend(Blend blend = {});
};

struct State {
	IDXGIAdapter *adapter;
	IDXGIAdapter1 *adapter1;
	IDXGIAdapter2 *adapter2;
	IDXGISwapChain *swap_chain;
	ID3D11Device  *device;
	ID3D11Device2 *device2;
	ID3D11Device3 *device3;
	RenderTarget back_buffer;
	SyncInterval sync_interval = SyncInterval::every_1;
	Context immediate_context;

	TL_API void init_back_buffer();
	TL_API StructuredBuffer create_structured_buffer(D3D11_USAGE usage, u32 count, u32 stride, void const* data = 0);
	template <class T>
	inline TypedStructuredBuffer<T> create_structured_buffer(D3D11_USAGE usage, u32 count, T const *data = 0) {
		TypedStructuredBuffer<T> result;
		(StructuredBuffer &)result = create_structured_buffer(usage, count, sizeof(T), data);
		return result;
	}
	TL_API RenderTexture create_render_texture(u32 width, u32 height, u32 sample_count, DXGI_FORMAT format, u32 cpu_flags = 0);
	TL_API Texture2D create_texture_2d(u32 width, u32 height, DXGI_FORMAT format, void const* data);
	template <class Handler = decltype(default_shader_handler)>
	ID3D11VertexShader *create_vertex_shader(char const *source, umm sourceSize, char const *name, char const *entry_point, char const *target, Handler &&handler = default_shader_handler) {
		ID3DBlob* byte_code = 0;
		ID3DBlob* messages = 0;
		HRESULT result = D3DCompile(source, sourceSize, name, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, entry_point, target, 0, 0, &byte_code, &messages);
		handler(result, messages ? (char *)messages->GetBufferPointer() : 0);
		ID3D11VertexShader *vertex_shader;
		TL_HRESULT_HANDLER(device->CreateVertexShader(byte_code->GetBufferPointer(), byte_code->GetBufferSize(), 0, &vertex_shader));
		byte_code->Release();
		if (messages)
			messages->Release();
		return vertex_shader;
	}
	template <class Handler = decltype(default_shader_handler)>
	ID3D11PixelShader *create_pixel_shader(char const *source, umm sourceSize, char const *name, char const *entry_point, char const *target, Handler &&handler = default_shader_handler) {
		ID3DBlob* byte_code = 0;
		ID3DBlob* messages = 0;
		HRESULT result = D3DCompile(source, sourceSize, name, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, entry_point, target, 0, 0, &byte_code, &messages);
		handler(result, messages ? (char *)messages->GetBufferPointer() : 0);
		ID3D11PixelShader *pixel_shader;
		TL_HRESULT_HANDLER(device->CreatePixelShader(byte_code->GetBufferPointer(), byte_code->GetBufferSize(), 0, &pixel_shader));
		byte_code->Release();
		if (messages)
			messages->Release();
		return pixel_shader;
	}
	TL_API ConstantBuffer create_constant_buffer(u32 size, D3D11_USAGE usage, void const *initial_data = 0);
	template <class T>
	inline TypedConstantBuffer<T> create_constant_buffer(D3D11_USAGE usage, T const *initial_data = 0) {
		TypedConstantBuffer<T> result;
		(ConstantBuffer &)result = create_constant_buffer(sizeof(T), usage, initial_data);
		return result;
	}
	TL_API DepthStencil create_depth_stencil(u32 width, u32 height, DXGI_FORMAT format);
	TL_API Sampler create_sampler(D3D11_TEXTURE_ADDRESS_MODE address, D3D11_FILTER filter);
	TL_API Rasterizer create_rasterizer(D3D11_FILL_MODE fill, D3D11_CULL_MODE cull, bool enable_ms = false, bool enable_ms_line = false);
	TL_API Blend create_blend(D3D11_BLEND_OP op_color, D3D11_BLEND src_color, D3D11_BLEND dst_color, D3D11_BLEND_OP op_alpha, D3D11_BLEND src_alpha, D3D11_BLEND dst_alpha);
	TL_API Blend create_blend(D3D11_BLEND_OP op, D3D11_BLEND src, D3D11_BLEND dst);
	TL_API void present();
	TL_API void resize_back_buffer(u32 width, u32 height);
	TL_API u32 get_max_msaa_sample_count(DXGI_FORMAT format);
	template <class S>
	inline void resize_back_buffer(v2<S> size) { resize_back_buffer((u32)size.x, (u32)size.y); }
	template <class S>
	inline void set_viewport(v2<S> position, v2<S> size) { set_viewport((f32)position.x, (f32)position.y, (f32)size.x, (f32)size.y); }
	template <class S>
	inline void set_viewport(v2<S> size) { set_viewport(0.0f, 0.0f, (f32)size.x, (f32)size.y); }

	inline bool is_valid() { return swap_chain != 0; }
};

TL_API Optional<u32> get_bits_per_pixel(DXGI_FORMAT format);

TL_API void init(State &state, IDXGISwapChain *swap_chain, ID3D11RenderTargetView *back_buffer, ID3D11Device *device, ID3D11DeviceContext *immediate_context);
// 'sample_count' can be -1 for highest setting, -2 for second highest, etc
TL_API void init(State &state, HWND window, u32 width, u32 height, DXGI_FORMAT back_buffer_format, u32 buffer_count, bool windowed, u32 sample_count, u32 device_flags = 0);

inline Span<utf8> to_string(DXGI_FORMAT format) {
	switch (format) {
        case DXGI_FORMAT_R32G32B32A32_TYPELESS:      return u8"R32G32B32A32_TYPELESS"s;
        case DXGI_FORMAT_R32G32B32A32_FLOAT:         return u8"R32G32B32A32_FLOAT"s;
        case DXGI_FORMAT_R32G32B32A32_UINT:          return u8"R32G32B32A32_UINT"s;
        case DXGI_FORMAT_R32G32B32A32_SINT:          return u8"R32G32B32A32_SINT"s;
        case DXGI_FORMAT_R32G32B32_TYPELESS:         return u8"R32G32B32_TYPELESS"s;
        case DXGI_FORMAT_R32G32B32_FLOAT:            return u8"R32G32B32_FLOAT"s;
        case DXGI_FORMAT_R32G32B32_UINT:             return u8"R32G32B32_UINT"s;
        case DXGI_FORMAT_R32G32B32_SINT:             return u8"R32G32B32_SINT"s;
        case DXGI_FORMAT_R16G16B16A16_TYPELESS:      return u8"R16G16B16A16_TYPELESS"s;
        case DXGI_FORMAT_R16G16B16A16_FLOAT:         return u8"R16G16B16A16_FLOAT"s;
        case DXGI_FORMAT_R16G16B16A16_UNORM:         return u8"R16G16B16A16_UNORM"s;
        case DXGI_FORMAT_R16G16B16A16_UINT:          return u8"R16G16B16A16_UINT"s;
        case DXGI_FORMAT_R16G16B16A16_SNORM:         return u8"R16G16B16A16_SNORM"s;
        case DXGI_FORMAT_R16G16B16A16_SINT:          return u8"R16G16B16A16_SINT"s;
        case DXGI_FORMAT_R32G32_TYPELESS:            return u8"R32G32_TYPELESS"s;
        case DXGI_FORMAT_R32G32_FLOAT:               return u8"R32G32_FLOAT"s;
        case DXGI_FORMAT_R32G32_UINT:                return u8"R32G32_UINT"s;
        case DXGI_FORMAT_R32G32_SINT:                return u8"R32G32_SINT"s;
        case DXGI_FORMAT_R32G8X24_TYPELESS:          return u8"R32G8X24_TYPELESS"s;
        case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:       return u8"D32_FLOAT_S8X24_UINT"s;
        case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:   return u8"R32_FLOAT_X8X24_TYPELESS"s;
        case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:    return u8"X32_TYPELESS_G8X24_UINT"s;
        case DXGI_FORMAT_Y416:                       return u8"Y416"s;
        case DXGI_FORMAT_Y210:                       return u8"Y210"s;
        case DXGI_FORMAT_Y216:                       return u8"Y216"s;
        case DXGI_FORMAT_R10G10B10A2_TYPELESS:       return u8"R10G10B10A2_TYPELESS"s;
        case DXGI_FORMAT_R10G10B10A2_UNORM:          return u8"R10G10B10A2_UNORM"s;
        case DXGI_FORMAT_R10G10B10A2_UINT:           return u8"R10G10B10A2_UINT"s;
        case DXGI_FORMAT_R11G11B10_FLOAT:            return u8"R11G11B10_FLOAT"s;
        case DXGI_FORMAT_R8G8B8A8_TYPELESS:          return u8"R8G8B8A8_TYPELESS"s;
        case DXGI_FORMAT_R8G8B8A8_UNORM:             return u8"R8G8B8A8_UNORM"s;
        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:        return u8"R8G8B8A8_UNORM_SRGB"s;
        case DXGI_FORMAT_R8G8B8A8_UINT:              return u8"R8G8B8A8_UINT"s;
        case DXGI_FORMAT_R8G8B8A8_SNORM:             return u8"R8G8B8A8_SNORM"s;
        case DXGI_FORMAT_R8G8B8A8_SINT:              return u8"R8G8B8A8_SINT"s;
        case DXGI_FORMAT_R16G16_TYPELESS:            return u8"R16G16_TYPELESS"s;
        case DXGI_FORMAT_R16G16_FLOAT:               return u8"R16G16_FLOAT"s;
        case DXGI_FORMAT_R16G16_UNORM:               return u8"R16G16_UNORM"s;
        case DXGI_FORMAT_R16G16_UINT:                return u8"R16G16_UINT"s;
        case DXGI_FORMAT_R16G16_SNORM:               return u8"R16G16_SNORM"s;
        case DXGI_FORMAT_R16G16_SINT:                return u8"R16G16_SINT"s;
        case DXGI_FORMAT_R32_TYPELESS:               return u8"R32_TYPELESS"s;
        case DXGI_FORMAT_D32_FLOAT:                  return u8"D32_FLOAT"s;
        case DXGI_FORMAT_R32_FLOAT:                  return u8"R32_FLOAT"s;
        case DXGI_FORMAT_R32_UINT:                   return u8"R32_UINT"s;
        case DXGI_FORMAT_R32_SINT:                   return u8"R32_SINT"s;
        case DXGI_FORMAT_R24G8_TYPELESS:             return u8"R24G8_TYPELESS"s;
        case DXGI_FORMAT_D24_UNORM_S8_UINT:          return u8"D24_UNORM_S8_UINT"s;
        case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:      return u8"R24_UNORM_X8_TYPELESS"s;
        case DXGI_FORMAT_X24_TYPELESS_G8_UINT:       return u8"X24_TYPELESS_G8_UINT"s;
        case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:         return u8"R9G9B9E5_SHAREDEXP"s;
        case DXGI_FORMAT_R8G8_B8G8_UNORM:            return u8"R8G8_B8G8_UNORM"s;
        case DXGI_FORMAT_G8R8_G8B8_UNORM:            return u8"G8R8_G8B8_UNORM"s;
        case DXGI_FORMAT_B8G8R8A8_UNORM:             return u8"B8G8R8A8_UNORM"s;
        case DXGI_FORMAT_B8G8R8X8_UNORM:             return u8"B8G8R8X8_UNORM"s;
        case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM: return u8"R10G10B10_XR_BIAS_A2_UNORM"s;
        case DXGI_FORMAT_B8G8R8A8_TYPELESS:          return u8"B8G8R8A8_TYPELESS"s;
        case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:        return u8"B8G8R8A8_UNORM_SRGB"s;
        case DXGI_FORMAT_B8G8R8X8_TYPELESS:          return u8"B8G8R8X8_TYPELESS"s;
        case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:        return u8"B8G8R8X8_UNORM_SRGB"s;
        case DXGI_FORMAT_AYUV:                       return u8"AYUV"s;
        case DXGI_FORMAT_Y410:                       return u8"Y410"s;
        case DXGI_FORMAT_YUY2:                       return u8"YUY2"s;
        case DXGI_FORMAT_P010:                       return u8"P010"s;
        case DXGI_FORMAT_P016:                       return u8"P016"s;
        case DXGI_FORMAT_R8G8_TYPELESS:              return u8"R8G8_TYPELESS"s;
        case DXGI_FORMAT_R8G8_UNORM:                 return u8"R8G8_UNORM"s;
        case DXGI_FORMAT_R8G8_UINT:                  return u8"R8G8_UINT"s;
        case DXGI_FORMAT_R8G8_SNORM:                 return u8"R8G8_SNORM"s;
        case DXGI_FORMAT_R8G8_SINT:                  return u8"R8G8_SINT"s;
        case DXGI_FORMAT_R16_TYPELESS:               return u8"R16_TYPELESS"s;
        case DXGI_FORMAT_R16_FLOAT:                  return u8"R16_FLOAT"s;
        case DXGI_FORMAT_D16_UNORM:                  return u8"D16_UNORM"s;
        case DXGI_FORMAT_R16_UNORM:                  return u8"R16_UNORM"s;
        case DXGI_FORMAT_R16_UINT:                   return u8"R16_UINT"s;
        case DXGI_FORMAT_R16_SNORM:                  return u8"R16_SNORM"s;
        case DXGI_FORMAT_R16_SINT:                   return u8"R16_SINT"s;
        case DXGI_FORMAT_B5G6R5_UNORM:               return u8"B5G6R5_UNORM"s;
        case DXGI_FORMAT_B5G5R5A1_UNORM:             return u8"B5G5R5A1_UNORM"s;
        case DXGI_FORMAT_A8P8:                       return u8"A8P8"s;
        case DXGI_FORMAT_B4G4R4A4_UNORM:             return u8"B4G4R4A4_UNORM"s;
        case DXGI_FORMAT_NV12:                       return u8"NV12"s;
        case DXGI_FORMAT_420_OPAQUE:                 return u8"420_OPAQUE"s;
        case DXGI_FORMAT_NV11:                       return u8"NV11"s;
        case DXGI_FORMAT_R8_TYPELESS:                return u8"R8_TYPELESS"s;
        case DXGI_FORMAT_R8_UNORM:                   return u8"R8_UNORM"s;
        case DXGI_FORMAT_R8_UINT:                    return u8"R8_UINT"s;
        case DXGI_FORMAT_R8_SNORM:                   return u8"R8_SNORM"s;
        case DXGI_FORMAT_R8_SINT:                    return u8"R8_SINT"s;
        case DXGI_FORMAT_A8_UNORM:                   return u8"A8_UNORM"s;
        case DXGI_FORMAT_AI44:                       return u8"AI44"s;
        case DXGI_FORMAT_IA44:                       return u8"IA44"s;
        case DXGI_FORMAT_P8:                         return u8"P8"s;
        case DXGI_FORMAT_R1_UNORM:                   return u8"R1_UNORM"s;
        case DXGI_FORMAT_BC1_TYPELESS:               return u8"BC1_TYPELESS"s;
        case DXGI_FORMAT_BC1_UNORM:                  return u8"BC1_UNORM"s;
        case DXGI_FORMAT_BC1_UNORM_SRGB:             return u8"BC1_UNORM_SRGB"s;
        case DXGI_FORMAT_BC4_TYPELESS:               return u8"BC4_TYPELESS"s;
        case DXGI_FORMAT_BC4_UNORM:                  return u8"BC4_UNORM"s;
        case DXGI_FORMAT_BC4_SNORM:                  return u8"BC4_SNORM"s;
        case DXGI_FORMAT_BC2_TYPELESS:               return u8"BC2_TYPELESS"s;
        case DXGI_FORMAT_BC2_UNORM:                  return u8"BC2_UNORM"s;
        case DXGI_FORMAT_BC2_UNORM_SRGB:             return u8"BC2_UNORM_SRGB"s;
        case DXGI_FORMAT_BC3_TYPELESS:               return u8"BC3_TYPELESS"s;
        case DXGI_FORMAT_BC3_UNORM:                  return u8"BC3_UNORM"s;
        case DXGI_FORMAT_BC3_UNORM_SRGB:             return u8"BC3_UNORM_SRGB"s;
        case DXGI_FORMAT_BC5_TYPELESS:               return u8"BC5_TYPELESS"s;
        case DXGI_FORMAT_BC5_UNORM:                  return u8"BC5_UNORM"s;
        case DXGI_FORMAT_BC5_SNORM:                  return u8"BC5_SNORM"s;
        case DXGI_FORMAT_BC6H_TYPELESS:              return u8"BC6H_TYPELESS"s;
        case DXGI_FORMAT_BC6H_UF16:                  return u8"BC6H_UF16"s;
        case DXGI_FORMAT_BC6H_SF16:                  return u8"BC6H_SF16"s;
        case DXGI_FORMAT_BC7_TYPELESS:               return u8"BC7_TYPELESS"s;
        case DXGI_FORMAT_BC7_UNORM:                  return u8"BC7_UNORM"s;
        case DXGI_FORMAT_BC7_UNORM_SRGB:             return u8"BC7_UNORM_SRGB"s;
    }
	return u8"(unknown DXGI_FORMAT)"s;
}

#ifdef TL_IMPL

inline void default_shader_handler(HRESULT result, char const *messages) {
#if BUILD_DEBUG
#if COMPILER_MSVC
	OutputDebugStringA(messages);
#endif
#endif
	assert(SUCCEEDED(result));
	(void)messages;
}

inline Optional<u32> get_bits_per_pixel(DXGI_FORMAT format) {
	switch (format) {
        case DXGI_FORMAT_R32G32B32A32_TYPELESS:
        case DXGI_FORMAT_R32G32B32A32_FLOAT:
        case DXGI_FORMAT_R32G32B32A32_UINT:
        case DXGI_FORMAT_R32G32B32A32_SINT:
            return 128;

        case DXGI_FORMAT_R32G32B32_TYPELESS:
        case DXGI_FORMAT_R32G32B32_FLOAT:
        case DXGI_FORMAT_R32G32B32_UINT:
        case DXGI_FORMAT_R32G32B32_SINT:
            return 96;

        case DXGI_FORMAT_R16G16B16A16_TYPELESS:
        case DXGI_FORMAT_R16G16B16A16_FLOAT:
        case DXGI_FORMAT_R16G16B16A16_UNORM:
        case DXGI_FORMAT_R16G16B16A16_UINT:
        case DXGI_FORMAT_R16G16B16A16_SNORM:
        case DXGI_FORMAT_R16G16B16A16_SINT:
        case DXGI_FORMAT_R32G32_TYPELESS:
        case DXGI_FORMAT_R32G32_FLOAT:
        case DXGI_FORMAT_R32G32_UINT:
        case DXGI_FORMAT_R32G32_SINT:
        case DXGI_FORMAT_R32G8X24_TYPELESS:
        case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
        case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
        case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
        case DXGI_FORMAT_Y416:
        case DXGI_FORMAT_Y210:
        case DXGI_FORMAT_Y216:
            return 64;

        case DXGI_FORMAT_R10G10B10A2_TYPELESS:
        case DXGI_FORMAT_R10G10B10A2_UNORM:
        case DXGI_FORMAT_R10G10B10A2_UINT:
        case DXGI_FORMAT_R11G11B10_FLOAT:
        case DXGI_FORMAT_R8G8B8A8_TYPELESS:
        case DXGI_FORMAT_R8G8B8A8_UNORM:
        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
        case DXGI_FORMAT_R8G8B8A8_UINT:
        case DXGI_FORMAT_R8G8B8A8_SNORM:
        case DXGI_FORMAT_R8G8B8A8_SINT:
        case DXGI_FORMAT_R16G16_TYPELESS:
        case DXGI_FORMAT_R16G16_FLOAT:
        case DXGI_FORMAT_R16G16_UNORM:
        case DXGI_FORMAT_R16G16_UINT:
        case DXGI_FORMAT_R16G16_SNORM:
        case DXGI_FORMAT_R16G16_SINT:
        case DXGI_FORMAT_R32_TYPELESS:
        case DXGI_FORMAT_D32_FLOAT:
        case DXGI_FORMAT_R32_FLOAT:
        case DXGI_FORMAT_R32_UINT:
        case DXGI_FORMAT_R32_SINT:
        case DXGI_FORMAT_R24G8_TYPELESS:
        case DXGI_FORMAT_D24_UNORM_S8_UINT:
        case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
        case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
        case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
        case DXGI_FORMAT_R8G8_B8G8_UNORM:
        case DXGI_FORMAT_G8R8_G8B8_UNORM:
        case DXGI_FORMAT_B8G8R8A8_UNORM:
        case DXGI_FORMAT_B8G8R8X8_UNORM:
        case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
        case DXGI_FORMAT_B8G8R8A8_TYPELESS:
        case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
        case DXGI_FORMAT_B8G8R8X8_TYPELESS:
        case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
        case DXGI_FORMAT_AYUV:
        case DXGI_FORMAT_Y410:
        case DXGI_FORMAT_YUY2:
            return 32;

        case DXGI_FORMAT_P010:
        case DXGI_FORMAT_P016:
            return 24;

        case DXGI_FORMAT_R8G8_TYPELESS:
        case DXGI_FORMAT_R8G8_UNORM:
        case DXGI_FORMAT_R8G8_UINT:
        case DXGI_FORMAT_R8G8_SNORM:
        case DXGI_FORMAT_R8G8_SINT:
        case DXGI_FORMAT_R16_TYPELESS:
        case DXGI_FORMAT_R16_FLOAT:
        case DXGI_FORMAT_D16_UNORM:
        case DXGI_FORMAT_R16_UNORM:
        case DXGI_FORMAT_R16_UINT:
        case DXGI_FORMAT_R16_SNORM:
        case DXGI_FORMAT_R16_SINT:
        case DXGI_FORMAT_B5G6R5_UNORM:
        case DXGI_FORMAT_B5G5R5A1_UNORM:
        case DXGI_FORMAT_A8P8:
        case DXGI_FORMAT_B4G4R4A4_UNORM:
            return 16;

        case DXGI_FORMAT_NV12:
        case DXGI_FORMAT_420_OPAQUE:
        case DXGI_FORMAT_NV11:
            return 12;

        case DXGI_FORMAT_R8_TYPELESS:
        case DXGI_FORMAT_R8_UNORM:
        case DXGI_FORMAT_R8_UINT:
        case DXGI_FORMAT_R8_SNORM:
        case DXGI_FORMAT_R8_SINT:
        case DXGI_FORMAT_A8_UNORM:
        case DXGI_FORMAT_AI44:
        case DXGI_FORMAT_IA44:
        case DXGI_FORMAT_P8:
            return 8;

        case DXGI_FORMAT_R1_UNORM:
            return 1;

        case DXGI_FORMAT_BC1_TYPELESS:
        case DXGI_FORMAT_BC1_UNORM:
        case DXGI_FORMAT_BC1_UNORM_SRGB:
        case DXGI_FORMAT_BC4_TYPELESS:
        case DXGI_FORMAT_BC4_UNORM:
        case DXGI_FORMAT_BC4_SNORM:
            return 4;

        case DXGI_FORMAT_BC2_TYPELESS:
        case DXGI_FORMAT_BC2_UNORM:
        case DXGI_FORMAT_BC2_UNORM_SRGB:
        case DXGI_FORMAT_BC3_TYPELESS:
        case DXGI_FORMAT_BC3_UNORM:
        case DXGI_FORMAT_BC3_UNORM_SRGB:
        case DXGI_FORMAT_BC5_TYPELESS:
        case DXGI_FORMAT_BC5_UNORM:
        case DXGI_FORMAT_BC5_SNORM:
        case DXGI_FORMAT_BC6H_TYPELESS:
        case DXGI_FORMAT_BC6H_UF16:
        case DXGI_FORMAT_BC6H_SF16:
        case DXGI_FORMAT_BC7_TYPELESS:
        case DXGI_FORMAT_BC7_UNORM:
        case DXGI_FORMAT_BC7_UNORM_SRGB:
            return 8;
    }
	return {};
}

void State::init_back_buffer() {
	TL_HRESULT_HANDLER(swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer.tex)));
	TL_HRESULT_HANDLER(device->CreateRenderTargetView(back_buffer.tex, 0, &back_buffer.rtv));
}
StructuredBuffer State::create_structured_buffer(D3D11_USAGE usage, u32 count, u32 stride, void const* data) {
	StructuredBuffer result;
	result.usage = usage;
	result.size = count * stride;
	{
		D3D11_BUFFER_DESC d = {};
		d.ByteWidth = result.size;
		d.Usage = usage;
		d.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		if (usage == D3D11_USAGE_DYNAMIC)
			d.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		d.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		d.StructureByteStride = stride;

		if (data) {
			D3D11_SUBRESOURCE_DATA initial_data {};
			initial_data.pSysMem = data;
			TL_HRESULT_HANDLER(device->CreateBuffer(&d, &initial_data, &result.buffer));
		}
		else {
			TL_HRESULT_HANDLER(device->CreateBuffer(&d, 0, &result.buffer));
		}
	}
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC d = {};
		d.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		d.Buffer.FirstElement = 0;
		d.Buffer.NumElements = count;
		TL_HRESULT_HANDLER(device->CreateShaderResourceView(result.buffer, &d, &result.srv));
	}
	return result;
}
RenderTexture State::create_render_texture(u32 width, u32 height, u32 sample_count, DXGI_FORMAT format, u32 cpu_flags) {
	RenderTexture result;
	{
		D3D11_TEXTURE2D_DESC d = {};
		d.Format = format;
		d.ArraySize = 1;
		d.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		d.Width = width;
		d.Height = height;
		d.MipLevels = 1;
		d.SampleDesc = {sample_count, 0};
		d.CPUAccessFlags = cpu_flags;
		TL_HRESULT_HANDLER(device->CreateTexture2D(&d, 0, &result.tex));
	}
	{
		D3D11_RENDER_TARGET_VIEW_DESC d = {};
		d.ViewDimension = sample_count == 1 ? D3D11_RTV_DIMENSION_TEXTURE2D : D3D11_RTV_DIMENSION_TEXTURE2DMS;
		TL_HRESULT_HANDLER(device->CreateRenderTargetView(result.tex, &d, &result.rtv));
	}
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC d = {};
		d.ViewDimension = sample_count == 1 ? D3D11_SRV_DIMENSION_TEXTURE2D : D3D11_SRV_DIMENSION_TEXTURE2DMS;
		d.Texture2D.MipLevels = 1;
		TL_HRESULT_HANDLER(device->CreateShaderResourceView(result.tex, &d, &result.srv));
	}
	return result;
}
Texture2D State::create_texture_2d(u32 width, u32 height, DXGI_FORMAT format, void const* data) {
	Texture2D result;

	u32 bpp = get_bits_per_pixel(format).value_or((invalid_code_path("Bits per pixel for format {} is undefined", to_string(format)), 0));
	assert(bpp % 8 == 0);
	u32 pitch = width * bpp / 8;

	{
		D3D11_TEXTURE2D_DESC d = {};
		d.Format = format;
		d.ArraySize = 1;
		d.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		d.Width = width;
		d.Height = height;
		d.MipLevels = 1u;
		d.SampleDesc = {1, 0};
		d.MiscFlags = 0u;

		if (data) {
			D3D11_SUBRESOURCE_DATA initial_data {};
			initial_data.pSysMem = data;
			initial_data.SysMemPitch = pitch;
			TL_HRESULT_HANDLER(device->CreateTexture2D(&d, &initial_data, &result.tex));
		}
		else {
			TL_HRESULT_HANDLER(device->CreateTexture2D(&d, 0, &result.tex));
		}
	}
	D3D11_SHADER_RESOURCE_VIEW_DESC d = {};
	d.Format = format;
	d.Texture2D.MipLevels = 1u;
	d.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	TL_HRESULT_HANDLER(device->CreateShaderResourceView(result.tex, &d, &result.srv));

	return result;
}
Texture2D Context::create_texture_2d_with_mips(u32 width, u32 height, DXGI_FORMAT format, void const* data) {
	Texture2D result;

	u32 bpp = get_bits_per_pixel(format).value_or((invalid_code_path("Bits per pixel for format {} is undefined", to_string(format)), 0));
	assert(bpp % 8 == 0);
	u32 pitch = width * bpp / 8;

	{
		D3D11_TEXTURE2D_DESC d = {};
		d.Format = format;
		d.ArraySize = 1;
		d.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		d.Width = width;
		d.Height = height;
		d.MipLevels = 0u;
		d.SampleDesc = {1, 0};
		d.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		TL_HRESULT_HANDLER(state->device->CreateTexture2D(&d, 0, &result.tex));
	}
	D3D11_SHADER_RESOURCE_VIEW_DESC d = {};
	d.Format = format;
	d.Texture2D.MipLevels = ~0u;
	d.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	TL_HRESULT_HANDLER(state->device->CreateShaderResourceView(result.tex, &d, &result.srv));

	context->UpdateSubresource(result.tex, 0, 0, data, pitch, 0);
	context->GenerateMips(result.srv);

	return result;
}
ConstantBuffer State::create_constant_buffer(u32 size, D3D11_USAGE usage, void const *initial_data) {
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = ceil(size, 16u);
	desc.CPUAccessFlags = usage == D3D11_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0u;
	desc.Usage = usage;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = initial_data;

	ConstantBuffer result = {};
	TL_HRESULT_HANDLER(device->CreateBuffer(&desc, initial_data ? &data : 0, &result.buffer));
	result.usage = usage;
	result.size = size;
	return result;
}
DepthStencil State::create_depth_stencil(u32 width, u32 height, DXGI_FORMAT format) {
	DepthStencil result = {};
	{
		D3D11_TEXTURE2D_DESC desc = {};
		desc.ArraySize = 1;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.Format = format;
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.SampleDesc = {1, 0};
		TL_HRESULT_HANDLER(device->CreateTexture2D(&desc, 0, &result.tex));
	}
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc = {};
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Format = format;
		TL_HRESULT_HANDLER(device->CreateDepthStencilView(result.tex, &desc, &result.dsv));
	}
	return result;
}
Sampler State::create_sampler(D3D11_TEXTURE_ADDRESS_MODE address, D3D11_FILTER filter) {
	D3D11_SAMPLER_DESC desc = {};
	desc.AddressU = address;
	desc.AddressV = address;
	desc.AddressW = address;
	desc.Filter = filter;
	desc.MaxAnisotropy = D3D11_MAX_MAXANISOTROPY;
	desc.MaxLOD = FLT_MAX;

	Sampler result = {};
	TL_HRESULT_HANDLER(device->CreateSamplerState(&desc, &result.sampler));
	return result;
}
Rasterizer State::create_rasterizer(D3D11_FILL_MODE fill, D3D11_CULL_MODE cull, bool enable_ms, bool enable_ms_line) {
	D3D11_RASTERIZER_DESC desc = {};
	desc.CullMode = cull;
	desc.FillMode = fill;
	desc.MultisampleEnable = enable_ms;
	desc.AntialiasedLineEnable = enable_ms_line;
	desc.DepthClipEnable = true;

	Rasterizer result = {};
	TL_HRESULT_HANDLER(device->CreateRasterizerState(&desc, &result));
	return result;
}
Blend State::create_blend(D3D11_BLEND_OP op_color, D3D11_BLEND src_color, D3D11_BLEND dst_color, D3D11_BLEND_OP op_alpha, D3D11_BLEND src_alpha, D3D11_BLEND dst_alpha) {
	D3D11_BLEND_DESC desc = {};
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].BlendOp = op_color;
	desc.RenderTarget[0].SrcBlend = src_color;
	desc.RenderTarget[0].DestBlend = dst_color;
	desc.RenderTarget[0].BlendOpAlpha = op_alpha;
	desc.RenderTarget[0].SrcBlendAlpha = src_alpha;
	desc.RenderTarget[0].DestBlendAlpha = dst_alpha;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	Blend result = {};
	TL_HRESULT_HANDLER(device->CreateBlendState(&desc, &result));
	return result;
}
Blend State::create_blend(D3D11_BLEND_OP op, D3D11_BLEND src, D3D11_BLEND dst) {
	return create_blend(op, src, dst, D3D11_BLEND_OP_ADD, D3D11_BLEND_ONE, D3D11_BLEND_ZERO);
}
void Context::update(StructuredBuffer& buffer, u32 count, u32 stride, void const* data, u32 first_element_index) {
	u32 size = count * stride;
	u32 offset = first_element_index * stride;
	assert(size + offset <= buffer.size);
	if (buffer.usage == D3D11_USAGE_DYNAMIC) {
		D3D11_MAPPED_SUBRESOURCE mapped;
		TL_HRESULT_HANDLER(context->Map(buffer.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped));
		memcpy(mapped.pData, data, size);
		context->Unmap(buffer.buffer, 0);
	}
	else if (buffer.usage == D3D11_USAGE_DEFAULT) {
		D3D11_BOX box = {};
		box.left = offset;
		box.right = box.left + size;
		box.back = 1;
		box.bottom = 1;
		context->UpdateSubresource(buffer.buffer, 0, &box, data, 0, 0);
	}
	else {
		invalid_code_path("bad buffer.usage");
	}
}
void Context::update(ConstantBuffer &buffer, u32 size, void const *data) {
	if (buffer.usage == D3D11_USAGE_DYNAMIC) {
		D3D11_MAPPED_SUBRESOURCE mapped;
		TL_HRESULT_HANDLER(context->Map(buffer.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped));
		memcpy(mapped.pData, data, size);
		context->Unmap(buffer.buffer, 0);
	} else if (buffer.usage == D3D11_USAGE_DEFAULT) {
		context->UpdateSubresource(buffer.buffer, 0, 0, data, 0, 0);
	} else {
		invalid_code_path("bad buffer.usage");
	}
}
void Context::clear(ID3D11RenderTargetView* render_target, f32 const rgba[4]) {
	context->ClearRenderTargetView(render_target, rgba);
}
void Context::clear(ID3D11DepthStencilView *depth_stencil, f32 depth) {
	context->ClearDepthStencilView(depth_stencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, 0);
}
void Context::draw(u32 vertexCount, u32 offset) {
	context->Draw(vertexCount, offset);
}
void State::present() {
	swap_chain->Present((UINT)sync_interval, 0);
}
void State::resize_back_buffer(u32 width, u32 height) {
	back_buffer.release();
	swap_chain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
	init_back_buffer();
}
void Context::set_topology(D3D11_PRIMITIVE_TOPOLOGY topology) { context->IASetPrimitiveTopology(topology); }
void Context::set_vertex_shader(ID3D11VertexShader *shader) { context->VSSetShader(shader, 0, 0); }
void Context::set_pixel_shader(ID3D11PixelShader  *shader) { context->PSSetShader(shader, 0, 0); }
void Context::set_shader_resource(ShaderResource const &resource, ShaderStage stage, u32 slot) {
	if (stage.vs) { context->VSSetShaderResources(slot, 1, &resource.srv); }
	if (stage.ps) { context->PSSetShaderResources(slot, 1, &resource.srv); }
}
void Context::set_sampler(Sampler const &sampler, ShaderStage stage, u32 slot) {
	if (stage.vs) { context->VSSetSamplers(slot, 1, &sampler.sampler); }
	if (stage.ps) { context->PSSetSamplers(slot, 1, &sampler.sampler); }
}
void Context::set_constant_buffer(ConstantBuffer const &buffer, ShaderStage stage, u32 slot) {
	if (stage.vs) { context->VSSetConstantBuffers(slot, 1, &buffer.buffer); }
	if (stage.ps) { context->PSSetConstantBuffers(slot, 1, &buffer.buffer); }
}
void Context::set_viewport(f32 x, f32 y, f32 w, f32 h, f32 depthMin, f32 depthMax) {
	D3D11_VIEWPORT v;
	v.TopLeftX = x;
	v.TopLeftY = y;
	v.Width = w;
	v.Height = h;
	v.MinDepth = depthMin;
	v.MaxDepth = depthMax;
	context->RSSetViewports(1, &v);
}
void Context::set_render_target(ID3D11RenderTargetView *render_target, ID3D11DepthStencilView *depth_stencil) {
	context->OMSetRenderTargets(1, &render_target, depth_stencil);
}
void Context::set_rasterizer(Rasterizer rasterizer) {
	context->RSSetState(rasterizer);
}
void Context::set_blend(Blend blend) {
	float factor[4]{};
	context->OMSetBlendState(blend, factor, ~0u);
}
u32 State::get_max_msaa_sample_count(DXGI_FORMAT format) {
	u32 quality_levels = 0;
	u32 sample_count = D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT;
	for (;;) {
		TL_HRESULT_HANDLER(device->CheckMultisampleQualityLevels(format, sample_count, &quality_levels));
		if (quality_levels != 0)
			break;
		sample_count /= 2;
		if (sample_count == 1)
			break;
	}
	return sample_count;
}

void init(State &state, IDXGISwapChain *swap_chain, ID3D11RenderTargetView *back_buffer, ID3D11Device *device, ID3D11DeviceContext *immediate_context) {
	state.swap_chain = swap_chain;
	state.device = device;
	state.immediate_context.state = &state;
	state.immediate_context.context = immediate_context;
	state.back_buffer.rtv = back_buffer;
}
// 'sample_count' can be -1 for highest setting, -2 for second highest, etc
void init(State &state, HWND window, u32 width, u32 height, DXGI_FORMAT back_buffer_format, u32 buffer_count, bool windowed, u32 sample_count, u32 device_flags) {
	IDXGIFactory1 *dxgi_factory;
	TL_HRESULT_HANDLER(CreateDXGIFactory1(IID_PPV_ARGS(&dxgi_factory)));
	defer { TL_COM_RELEASE(dxgi_factory); };

	IDXGIAdapter1 *adapter = 0;
	for (UINT adapter_index = 0; dxgi_factory->EnumAdapters1(adapter_index, &adapter) != DXGI_ERROR_NOT_FOUND; ++adapter_index) {
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);
		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
			TL_COM_RELEASE(adapter);
			continue;
		}
		state.adapter = adapter;
		state.adapter1 = adapter;
		break;
	}
	
	state.immediate_context.state = &state;

	D3D_FEATURE_LEVEL max_feature = D3D_FEATURE_LEVEL_11_1;
	if (FAILED(D3D11CreateDevice(state.adapter, D3D_DRIVER_TYPE_UNKNOWN, 0, device_flags, &max_feature, 1, D3D11_SDK_VERSION, &state.device, 0, &state.immediate_context.context))) {
		TL_HRESULT_HANDLER(D3D11CreateDevice(state.adapter, D3D_DRIVER_TYPE_UNKNOWN, 0, device_flags, 0, 0, D3D11_SDK_VERSION, &state.device, 0, &state.immediate_context.context));
	}

	u32 max_sample_count = state.get_max_msaa_sample_count(back_buffer_format);
	if ((s32)sample_count < 0) sample_count = max_sample_count / (u32)-(s32)sample_count;

	DXGI_SWAP_CHAIN_DESC d = {};
	d.BufferDesc.Width = width;
	d.BufferDesc.Height = height;
	d.BufferDesc.Format = back_buffer_format;
	d.BufferCount = buffer_count;
	d.Windowed = windowed;
	d.SampleDesc.Count = sample_count;
	d.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	d.OutputWindow = window;
	TL_HRESULT_HANDLER(dxgi_factory->CreateSwapChain(state.device, &d, &state.swap_chain));

	if (FAILED(state.device->QueryInterface(&state.device2))) {
		state.device2 = 0;
	}
	if (FAILED(state.device->QueryInterface(&state.device3))) {
		state.device3 = 0;
	}

	state.init_back_buffer();
}

#endif

}}
#pragma warning(pop)
