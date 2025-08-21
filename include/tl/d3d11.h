#pragma once
#include "thread.h"
#include "common.h"

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

#ifndef TL_D3D11_HRESULT_HANDLER
#define TL_D3D11_HRESULT_HANDLER(hr) (default_hresult_handler(hr, __FILE__, __LINE__, __FUNCTION__, #hr) || (invalid_code_path(), 0))
#endif

#define TL_COM_RELEASE(x) (((x) ? (x)->Release() : 0), (x) = 0)

#define GHR(hr) TL_D3D11_HRESULT_HANDLER(hr)

#pragma warning(push)
#pragma warning(disable: 4820) // struct padding

namespace tl { namespace d3d11 {

struct Shader {
	ID3D11VertexShader *vs = 0;
	ID3D11PixelShader *ps = 0;
};

struct ShaderResource {
	ID3D11ShaderResourceView *srv = 0;
};

struct UntypedBuffer {
	ID3D11Buffer *buffer = 0;
	D3D11_USAGE usage = {};
	u32 size = 0;
};

template <class T>
struct Buffer : UntypedBuffer {
};

struct UntypedStructuredBuffer : ShaderResource, UntypedBuffer {
};

template <class T>
struct StructuredBuffer : UntypedStructuredBuffer {};

struct UntypedConstantBuffer {
	ID3D11Buffer *buffer = 0;
	D3D11_USAGE usage = {};
	u32 size = 0;
};

template <class T>
struct ConstantBuffer : UntypedConstantBuffer {};

struct DepthStencilTexture {
	ID3D11DepthStencilView *dsv = 0;
	ID3D11Texture2D *tex = 0;
};

struct Texture2D : ShaderResource {
	ID3D11Texture2D *tex = 0;
	DXGI_FORMAT format = {};
	u32 width = 0;
	u32 height = 0;
};

struct Texture3D : ShaderResource {
	ID3D11Texture3D *tex = 0;
	DXGI_FORMAT format = {};
	u32 width = 0;
	u32 height = 0;
	u32 depth = 0;
};

struct Sampler {
	ID3D11SamplerState *sampler = 0;
};

struct RenderTarget {
	ID3D11RenderTargetView *rtv = 0;
	ID3D11Texture2D *tex = 0;
};

struct RenderTexture : ShaderResource, RenderTarget {
};

struct Rasterizer {
	ID3D11RasterizerState *raster = 0;
};

struct DepthStencil {
	ID3D11DepthStencilState *state = 0;
};

struct Blend {
	ID3D11BlendState *blend = 0;
};

// Same layout as D3D11_RASTERIZER_DESC, but with default values.
struct RasterizerDesc {
	D3D11_FILL_MODE FillMode = D3D11_FILL_SOLID;
	D3D11_CULL_MODE CullMode = D3D11_CULL_BACK;
	BOOL FrontCounterClockwise = false;
	INT DepthBias = 0;
	FLOAT DepthBiasClamp = 0;
	FLOAT SlopeScaledDepthBias = 0;
	BOOL DepthClipEnable = true;
	BOOL ScissorEnable = false;
	BOOL MultisampleEnable = false;
	BOOL AntialiasedLineEnable = false;
};

// Same layout as D3D11_DEPTH_STENCIL_DESC, but with default values.
struct DepthStencilDesc {
    BOOL DepthEnable = true;
    D3D11_DEPTH_WRITE_MASK DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	D3D11_COMPARISON_FUNC DepthFunc = D3D11_COMPARISON_LESS;
    BOOL StencilEnable = false;
    UINT8 StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    UINT8 StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	D3D11_DEPTH_STENCILOP_DESC FrontFace = {
		.StencilFailOp = D3D11_STENCIL_OP_KEEP,
		.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP,
		.StencilPassOp = D3D11_STENCIL_OP_KEEP,
		.StencilFunc = D3D11_COMPARISON_ALWAYS,
	};
    D3D11_DEPTH_STENCILOP_DESC BackFace = {
		.StencilFailOp = D3D11_STENCIL_OP_KEEP,
		.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP,
		.StencilPassOp = D3D11_STENCIL_OP_KEEP,
		.StencilFunc = D3D11_COMPARISON_ALWAYS,
	};
};

struct TL_API State {
	IDXGIAdapter *adapter;
	IDXGIAdapter1 *adapter1;
	IDXGIAdapter2 *adapter2;
	IDXGISwapChain *swap_chain;
	ID3D11Device  *device;
	ID3D11Device2 *device2;
	ID3D11Device3 *device3;
	ID3D11DeviceContext *immediate_context;
	ID3D11InfoQueue *info_queue;
	RenderTarget back_buffer;
	u32 sync_interval = 1;
	
	void init(IDXGISwapChain *swap_chain, ID3D11RenderTargetView *back_buffer, ID3D11Device *device, ID3D11DeviceContext *immediate_context);
	// 'sample_count' can be -1 for highest setting, -2 for second highest, etc
	void init(HWND window, u32 width, u32 height, DXGI_FORMAT back_buffer_format, u32 buffer_count, bool windowed, u32 sample_count, u32 device_flags = 0);
	void init_back_buffer();
	RenderTexture create_render_texture(u32 width, u32 height, u32 sample_count, DXGI_FORMAT format, u32 cpu_flags = 0);
	Texture2D create_texture_2d(u32 width, u32 height, DXGI_FORMAT format, void const *data, bool generate_mips = false);
	Texture3D create_texture_3d(u32 width, u32 height, u32 depth, DXGI_FORMAT format, void const *data, bool generate_mips = false);
	UntypedBuffer create_buffer(u32 count, u32 stride, void const *data, UINT bind_flags, UINT misc_flags);
	void *create_shader(HRESULT (ID3D11Device::*create_shader)(void *, SIZE_T, int, void **), Span<char> source, char const *name, char const *entry_point, char const *target, bool standard_include = true, D3D_SHADER_MACRO const *defines = 0, ID3DBlob **out_bytecode = 0);
	ID3D11VertexShader *create_vertex_shader(Span<char> source, char const *name, char const *entry_point, char const *target, bool standard_include = true, D3D_SHADER_MACRO const *defines = 0, ID3DBlob **out_bytecode = 0);
	ID3D11PixelShader *create_pixel_shader(Span<char> source, char const *name, char const *entry_point, char const *target, bool standard_include = true, D3D_SHADER_MACRO const *defines = 0, ID3DBlob **out_bytecode = 0);

	UntypedConstantBuffer create_constant_buffer(u32 size, D3D11_USAGE usage, void const *initial_data = 0);
	template <class T>
	inline ConstantBuffer<T> create_constant_buffer(D3D11_USAGE usage, T const *initial_data = 0) {
		ConstantBuffer<T> result;
		(UntypedConstantBuffer &)result = create_constant_buffer(sizeof(T), usage, initial_data);
		return result;
	}
	DepthStencilTexture create_depth_stencil_texture(u32 width, u32 height, DXGI_FORMAT format);
	Sampler create_sampler(D3D11_TEXTURE_ADDRESS_MODE address, D3D11_FILTER filter);
	Rasterizer create_rasterizer(RasterizerDesc desc);
	DepthStencil create_depth_stencil(DepthStencilDesc desc);
	Blend create_blend(D3D11_BLEND_OP op_color, D3D11_BLEND src_color, D3D11_BLEND dst_color, D3D11_BLEND_OP op_alpha, D3D11_BLEND src_alpha, D3D11_BLEND dst_alpha);
	Blend create_blend(D3D11_BLEND_OP op, D3D11_BLEND src, D3D11_BLEND dst);
	void update_texture_2d(Texture2D &texture, u32 min_x, u32 min_y, u32 max_x, u32 max_y, void const *data, u32 data_pitch = 0);
	void update_texture_3d(Texture3D &texture, u32 min_x, u32 min_y, u32 min_z, u32 max_x, u32 max_y, u32 max_z, void const *data, u32 data_pitch1 = 0, u32 data_pitch2 = 0);
	void update_buffer(UntypedBuffer &buffer, u32 count, u32 stride, void const *data, u32 first_element, UINT bind_flags, UINT misc_flags);
	template <class T>
	void update_buffer(Buffer<T> &buffer, Span<T> span, u32 first_element, UINT bind_flags, UINT misc_flags) {
		update_buffer(buffer, span.count, sizeof(T), span.data, first_element, bind_flags, misc_flags);
	}
	void update_structured_buffer(UntypedStructuredBuffer &buffer, u32 count, u32 stride, void const *data, u32 first_element = 0);
	template <class T>
	inline void update_structured_buffer(StructuredBuffer<T> &buffer, Span<T> span, u32 first_element = 0) {
		update_structured_buffer(buffer, span.count, sizeof(T), span.data, first_element);
	}
	void update_constant_buffer(UntypedConstantBuffer &buffer, u32 size, void const *data);
	template <class T>
	inline void update_constant_buffer(ConstantBuffer<T> &buffer, T const &data) {
		update_constant_buffer(buffer, sizeof(T), &data);
	}
	void clear_render_target(ID3D11RenderTargetView *render_target, f32 const rgba[4]);
	void clear_depth_stencil(ID3D11DepthStencilView *depth_stencil, f32 depth = 1, u8 stencil = 0);
	inline void clear_render_target(RenderTarget &rt, f32 const rgba[4]) { clear_render_target(rt.rtv, rgba); }
	inline void clear_depth_stencil(DepthStencilTexture &depth_stencil, f32 depth = 1, u8 stencil = 0) { clear_depth_stencil(depth_stencil.dsv, depth, stencil); }
	void draw(u32 vertex_count, u32 offset = 0);
	void present();
	void resize_back_buffer(u32 width, u32 height);
	void set_topology(D3D11_PRIMITIVE_TOPOLOGY topology);
	void set_vertex_shader(ID3D11VertexShader *shader);
	void set_pixel_shader(ID3D11PixelShader *shader);
	inline void set_shader(Shader shader) { 
		set_vertex_shader(shader.vs);
		set_pixel_shader(shader.ps);
	}
	inline void set_shader(ID3D11VertexShader *shader) { set_vertex_shader(shader); }
	inline void set_shader(ID3D11PixelShader  *shader) { set_pixel_shader(shader); }
	void set_shader_resource(ShaderResource const &resource, char stage, u32 slot);
	void set_sampler(Sampler const &sampler, char stage, u32 slot);
	void set_constant_buffer(UntypedConstantBuffer const &buffer, char stage, u32 slot);
	void set_viewport(f32 x, f32 y, f32 w, f32 h, f32 depth_min, f32 depth_max);
	inline void set_viewport(f32 x, f32 y, f32 w, f32 h) { set_viewport(x, y, w, h, 0, 1); }
	inline void set_viewport(f32 w, f32 h) { set_viewport(0, 0, w, h); }
	void set_render_target(ID3D11RenderTargetView *render_target, ID3D11DepthStencilView *depth_stencil);
	inline void set_render_target(RenderTarget &rt) { set_render_target(rt.rtv, 0); }
	inline void set_render_target(RenderTarget &rt, DepthStencilTexture &ds) { set_render_target(rt.rtv, ds.dsv); }
	void set_rasterizer(Rasterizer rasterizer = {});
	void set_depth_stencil(DepthStencil depth_stencil = {}, u8 stencil_ref = 0);
	void set_blend(Blend blend = {});
	u32 get_max_msaa_sample_count(DXGI_FORMAT format);
#ifdef TL_MATH_H
	inline void resize_back_buffer(v2u size) { resize_back_buffer(size.x, size.y); }
	inline void resize_back_buffer(v2s size) { resize_back_buffer((v2u)size); }
	inline void set_viewport(v2f position, v2f size) { set_viewport(position.x, position.y, size.x, size.y); }
	inline void set_viewport(v2s position, v2s size) { set_viewport((v2f)position, (v2f)size); }
	inline void set_viewport(v2u position, v2u size) { set_viewport((v2f)position, (v2f)size); }
	inline void set_viewport(v2f size) { set_viewport({}, size); }
	inline void set_viewport(v2s size) { set_viewport({}, (v2f)size); }
	inline void set_viewport(v2u size) { set_viewport({}, (v2f)size); }
#endif

	
	void print_messages();
	bool default_hresult_handler(HRESULT hr, char const *file, int line, char const *function, char const *expression);
};

TL_API void add_ref(Shader &v);
TL_API void add_ref(UntypedStructuredBuffer &v);
TL_API void add_ref(UntypedConstantBuffer &v);
TL_API void add_ref(RenderTarget &v);
TL_API void add_ref(DepthStencilTexture &v);
TL_API void add_ref(Texture2D &v);
TL_API void add_ref(Texture3D &v);
TL_API void add_ref(Sampler &v);
TL_API void add_ref(RenderTexture &v);
TL_API void add_ref(Rasterizer &v);
TL_API void add_ref(DepthStencil &v);
TL_API void add_ref(Blend &v);

TL_API void release(Shader &v);
TL_API void release(UntypedStructuredBuffer &v);
TL_API void release(UntypedConstantBuffer &v);
TL_API void release(RenderTarget &v);
TL_API void release(DepthStencilTexture &v);
TL_API void release(Texture2D &v);
TL_API void release(Texture3D &v);
TL_API void release(Sampler &v);
TL_API void release(RenderTexture &v);
TL_API void release(Rasterizer &v);
TL_API void release(DepthStencil &v);
TL_API void release(Blend &v);

inline bool valid(ShaderResource &v) { return v.srv != 0; }
inline bool valid(UntypedConstantBuffer &v) { return v.buffer != 0; }
inline bool valid(State &state) { return state.swap_chain != 0; }

TL_API u32 get_bits_per_pixel(DXGI_FORMAT format);

#ifdef TL_IMPL

void add_ref(Shader &v) {
	v.vs->AddRef();
	v.ps->AddRef();
}
void add_ref(UntypedStructuredBuffer &v) {
	v.buffer->AddRef();
	v.srv->AddRef();
}
void add_ref(UntypedConstantBuffer &v) {
	v.buffer->AddRef();
}
void add_ref(RenderTarget &v) {
	v.rtv->AddRef();
	v.tex->AddRef();
}
void add_ref(DepthStencilTexture &v) {
	v.dsv->AddRef();
	v.tex->AddRef();
}
void add_ref(Texture2D &v) {
	v.srv->AddRef();
	v.tex->AddRef();
}
void add_ref(Texture3D &v) {
	v.srv->AddRef();
	v.tex->AddRef();
}
void add_ref(Sampler &v) {
	v.sampler->AddRef();
}
void add_ref(RenderTexture &v) {
	v.rtv->AddRef();
	v.srv->AddRef();
	v.tex->AddRef();
}
void add_ref(Rasterizer &v) {
	v.raster->AddRef();
}
void add_ref(DepthStencil &v) {
	v.state->AddRef();
}
void add_ref(Blend &v) {
	v.blend->AddRef();
}

void release(Shader &v) {
	TL_COM_RELEASE(v.vs);
	TL_COM_RELEASE(v.ps);
}
void release(UntypedStructuredBuffer &v) {
	TL_COM_RELEASE(v.buffer);
	TL_COM_RELEASE(v.srv);
}
void release(UntypedConstantBuffer &v) {
	TL_COM_RELEASE(v.buffer);
}
void release(RenderTarget &v) {
	TL_COM_RELEASE(v.rtv);
	TL_COM_RELEASE(v.tex);
}
void release(DepthStencilTexture &v) {
	TL_COM_RELEASE(v.dsv);
	TL_COM_RELEASE(v.tex);
}
void release(Texture2D &v) {
	TL_COM_RELEASE(v.srv);
	TL_COM_RELEASE(v.tex);
}
void release(Texture3D &v) {
	TL_COM_RELEASE(v.srv);
	TL_COM_RELEASE(v.tex);
}
void release(Sampler &v) {
	TL_COM_RELEASE(v.sampler);
}
void release(RenderTexture &v) {
	TL_COM_RELEASE(v.rtv);
	TL_COM_RELEASE(v.srv);
	TL_COM_RELEASE(v.tex);
}
void release(Rasterizer &v) {
	TL_COM_RELEASE(v.raster);
}
void release(DepthStencil &v) {
	TL_COM_RELEASE(v.state);
}
void release(Blend &v) {
	TL_COM_RELEASE(v.blend);
}

inline u32 get_bits_per_pixel(DXGI_FORMAT format) {
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

        default:
            return 0;
#pragma warning(suppress: 4061)
    }
}

void State::init_back_buffer() {
	GHR(swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer.tex)));
	GHR(device->CreateRenderTargetView(back_buffer.tex, 0, &back_buffer.rtv));
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
		GHR(device->CreateTexture2D(&d, 0, &result.tex));
	}
	{
		D3D11_RENDER_TARGET_VIEW_DESC d = {};
		d.ViewDimension = sample_count == 1 ? D3D11_RTV_DIMENSION_TEXTURE2D : D3D11_RTV_DIMENSION_TEXTURE2DMS;
		GHR(device->CreateRenderTargetView(result.tex, &d, &result.rtv));
	}
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC d = {};
		d.ViewDimension = sample_count == 1 ? D3D11_SRV_DIMENSION_TEXTURE2D : D3D11_SRV_DIMENSION_TEXTURE2DMS;
		d.Texture2D.MipLevels = 1;
		GHR(device->CreateShaderResourceView(result.tex, &d, &result.srv));
	}
	return result;
}
Texture2D State::create_texture_2d(u32 width, u32 height, DXGI_FORMAT format, void const *data, bool generate_mips) {
	Texture2D result;
	result.format = format;
	result.width = width;
	result.height = height;

	u32 bpp = get_bits_per_pixel(format);
	assert(bpp % 8 == 0);
	u32 pitch = width * bpp / 8;

	{
		D3D11_TEXTURE2D_DESC d = {};
		d.Format = format;
		d.ArraySize = 1;
		d.BindFlags = D3D11_BIND_SHADER_RESOURCE | (generate_mips ? D3D11_BIND_RENDER_TARGET : 0u);
		d.Width = width;
		d.Height = height;
		d.MipLevels = generate_mips ? 0u : 1u;
		d.SampleDesc = {1, 0};
		d.MiscFlags = generate_mips ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0u;

		if (data) {
			D3D11_SUBRESOURCE_DATA initial_data {};
			initial_data.pSysMem = data;
			initial_data.SysMemPitch = pitch;
			GHR(device->CreateTexture2D(&d, generate_mips ? nullptr : &initial_data, &result.tex));
		}
		else {
			GHR(device->CreateTexture2D(&d, 0, &result.tex));
		}
	}
	D3D11_SHADER_RESOURCE_VIEW_DESC d = {};
	d.Format = format;
	d.Texture2D.MipLevels = generate_mips ? ~0u : 1u;
	d.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	GHR(device->CreateShaderResourceView(result.tex, &d, &result.srv));

	if (generate_mips) {
		immediate_context->UpdateSubresource(result.tex, 0, 0, data, pitch, 0);
		immediate_context->GenerateMips(result.srv);
	}

	return result;
}
Texture3D State::create_texture_3d(u32 width, u32 height, u32 depth, DXGI_FORMAT format, void const *data, bool generate_mips) {
	Texture3D result;
	result.format = format;
	result.width = width;
	result.height = height;
	result.depth = depth;

	u32 bpp = get_bits_per_pixel(format);
	assert(bpp % 8 == 0);
	u32 pitch = width * bpp / 8;
	u32 pitch2 = pitch * height;

	{
		D3D11_TEXTURE3D_DESC d = {};
		d.Format = format;
		d.BindFlags = D3D11_BIND_SHADER_RESOURCE | (generate_mips ? D3D11_BIND_RENDER_TARGET : 0u);
		d.Width = width;
		d.Height = height;
		d.Depth = depth;
		d.MipLevels = generate_mips ? 0u : 1u;
		d.MiscFlags = generate_mips ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0u;

		if (data) {
			D3D11_SUBRESOURCE_DATA initial_data {};
			initial_data.pSysMem = data;
			initial_data.SysMemPitch = pitch;
			initial_data.SysMemSlicePitch = pitch2;
			GHR(device->CreateTexture3D(&d, generate_mips ? nullptr : &initial_data, &result.tex));
		} else {
			GHR(device->CreateTexture3D(&d, 0, &result.tex));
		}
	}
	D3D11_SHADER_RESOURCE_VIEW_DESC d = {};
	d.Format = format;
	d.Texture3D.MipLevels = generate_mips ? ~0u : 1u;
	d.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;

	GHR(device->CreateShaderResourceView(result.tex, &d, &result.srv));

	if (generate_mips) {
		immediate_context->UpdateSubresource(result.tex, 0, 0, data, pitch, pitch2);
		immediate_context->GenerateMips(result.srv);
	}
	return result;
}
void *State::create_shader(HRESULT (ID3D11Device::*create_shader)(void *, SIZE_T, int, void **), Span<char> source, char const *name, char const *entry_point, char const *target, bool standard_include, D3D_SHADER_MACRO const *defines, ID3DBlob **out_bytecode) {
	ID3DBlob *bytecode = 0;
	ID3DBlob *messages = 0;
	HRESULT result = D3DCompile(source.data, source.count, name, defines, standard_include ? D3D_COMPILE_STANDARD_FILE_INCLUDE : 0, entry_point, target, 0, 0, &bytecode, &messages);

	if (messages) {
		auto severity = FAILED(result) ? LogSeverity::error : LogSeverity::info;
		current_logger.log(severity, Span((char *)messages->GetBufferPointer(), messages->GetBufferSize()));
		TL_COM_RELEASE(messages);
	}

	if (FAILED(result)) {
		return 0;
	}

	void *vertex_shader;
	GHR((device->*create_shader)(bytecode->GetBufferPointer(), bytecode->GetBufferSize(), 0, &vertex_shader));
	if (out_bytecode) {
		*out_bytecode = bytecode;
	} else {
		bytecode->Release();
	}
	if (messages)
		messages->Release();
	return vertex_shader;
}

ID3D11VertexShader *State::create_vertex_shader(Span<char> source, char const *name, char const *entry_point, char const *target, bool standard_include, D3D_SHADER_MACRO const *defines, ID3DBlob **bytecode) {
	return (ID3D11VertexShader *)create_shader(autocast &ID3D11Device::CreateVertexShader, source, name, entry_point, target, standard_include, defines, bytecode);
}
ID3D11PixelShader *State::create_pixel_shader(Span<char> source, char const *name, char const *entry_point, char const *target, bool standard_include, D3D_SHADER_MACRO const *defines, ID3DBlob **bytecode) {
	return (ID3D11PixelShader *)create_shader(autocast &ID3D11Device::CreatePixelShader, source, name, entry_point, target, standard_include, defines, bytecode);
}
UntypedConstantBuffer State::create_constant_buffer(u32 size, D3D11_USAGE usage, void const *initial_data) {
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = ceil(size, 16u);
	desc.CPUAccessFlags = usage == D3D11_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0u;
	desc.Usage = usage;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = initial_data;

	UntypedConstantBuffer result = {};
	GHR(device->CreateBuffer(&desc, initial_data ? &data : 0, &result.buffer));
	result.usage = usage;
	result.size = size;
	return result;
}
DepthStencilTexture State::create_depth_stencil_texture(u32 width, u32 height, DXGI_FORMAT format) {
	DepthStencilTexture result = {};
	{
		D3D11_TEXTURE2D_DESC desc = {};
		desc.ArraySize = 1;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.Format = format;
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.SampleDesc = {1, 0};
		GHR(device->CreateTexture2D(&desc, 0, &result.tex));
	}
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc = {};
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Format = format;
		GHR(device->CreateDepthStencilView(result.tex, &desc, &result.dsv));
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
	GHR(device->CreateSamplerState(&desc, &result.sampler));
	return result;
}

Rasterizer State::create_rasterizer(RasterizerDesc in_desc) {
	D3D11_RASTERIZER_DESC desc = {};
	desc.FillMode = in_desc.FillMode;
	desc.CullMode = in_desc.CullMode;
	desc.FrontCounterClockwise = in_desc.FrontCounterClockwise;
	desc.DepthBias = in_desc.DepthBias;
	desc.DepthBiasClamp = in_desc.DepthBiasClamp;
	desc.SlopeScaledDepthBias = in_desc.SlopeScaledDepthBias;
	desc.DepthClipEnable = in_desc.DepthClipEnable;
	desc.ScissorEnable = in_desc.ScissorEnable;
	desc.MultisampleEnable = in_desc.MultisampleEnable;
	desc.AntialiasedLineEnable = in_desc.AntialiasedLineEnable;

	Rasterizer result = {};
	GHR(device->CreateRasterizerState(&desc, &result.raster));
	return result;
}
DepthStencil State::create_depth_stencil(DepthStencilDesc in_desc) {
	D3D11_DEPTH_STENCIL_DESC desc = {
		.DepthEnable = in_desc.DepthEnable,
		.DepthWriteMask = in_desc.DepthWriteMask,
		.DepthFunc = in_desc.DepthFunc,
		.StencilEnable = in_desc.StencilEnable,
		.StencilReadMask = in_desc.StencilReadMask,
		.StencilWriteMask = in_desc.StencilWriteMask,
		.FrontFace = in_desc.FrontFace,
		.BackFace = in_desc.BackFace,
	};
	ID3D11DepthStencilState *state = 0;
	GHR(device->CreateDepthStencilState(&desc, &state));
	return {state};
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
	GHR(device->CreateBlendState(&desc, &result.blend));
	return result;
}
Blend State::create_blend(D3D11_BLEND_OP op, D3D11_BLEND src, D3D11_BLEND dst) {
	return create_blend(op, src, dst, D3D11_BLEND_OP_ADD, D3D11_BLEND_ONE, D3D11_BLEND_ZERO);
}
void State::update_texture_2d(Texture2D &texture, u32 min_x, u32 min_y, u32 max_x, u32 max_y, void const *data, u32 data_pitch) {
	if (data_pitch == 0) {
		data_pitch = get_bits_per_pixel(texture.format) * texture.width;
	}
	D3D11_BOX box = {
		.left = min_x,
		.top = min_y,
		.front = 0,
		.right = max_x,
		.bottom = max_y,
		.back = 1,
	};
	immediate_context->UpdateSubresource(texture.tex, 0, &box, data, data_pitch, 0);
}
void State::update_texture_3d(Texture3D &texture, u32 min_x, u32 min_y, u32 min_z, u32 max_x, u32 max_y, u32 max_z, void const *data, u32 data_pitch1, u32 data_pitch2) {
	if (data_pitch1 == 0) {
		data_pitch1 = get_bits_per_pixel(texture.format) * texture.width;
	}
	if (data_pitch2 == 0) {
		data_pitch2 = data_pitch1 * texture.height;
	}
	D3D11_BOX box = {
		.left = min_x,
		.top = min_y,
		.front = min_z,
		.right = max_x,
		.bottom = max_y,
		.back = max_z,
	};
	immediate_context->UpdateSubresource(texture.tex, 0, &box, data, data_pitch1, data_pitch2);
}
UntypedBuffer State::create_buffer(u32 count, u32 stride, void const *data, UINT bind_flags, UINT misc_flags) {
	UntypedBuffer buffer = {};

	if (count == 0)
		return {};

	buffer.size = count * stride;
			
	{
		D3D11_BUFFER_DESC d = {
			.ByteWidth = buffer.size,
			.Usage = buffer.usage,
			.BindFlags = bind_flags,
			.CPUAccessFlags = buffer.usage == D3D11_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0u,
			.MiscFlags = misc_flags,
			.StructureByteStride = stride,
		};
		GHR(device->CreateBuffer(&d, 0, &buffer.buffer));
	}

	return buffer;
}
void State::update_buffer(UntypedBuffer &buffer, u32 count, u32 stride, void const *data, u32 first_element, UINT bind_flags, UINT misc_flags) {
	if (buffer.usage == D3D11_USAGE_DYNAMIC)
		assert(first_element == 0, "Dynamic buffers can't be updated partially, only as a whole.");

	if (count == 0)
		return;

	u32 size = count * stride;
	u32 offset = first_element * stride;
	if (size + offset > buffer.size) {
		// Resize
			
		UINT min_required_count = first_element + count;
		UINT new_count = max(min_required_count * 3 / 2, 1u); // D3D11 does not allow zero sized buffers.
		UINT new_size = new_count * stride;

		ID3D11Buffer *new_buffer = {};
		{
			D3D11_BUFFER_DESC d = {
				.ByteWidth = new_size,
				.Usage = buffer.usage,
				.BindFlags = bind_flags,
				.CPUAccessFlags = buffer.usage == D3D11_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0u,
				.MiscFlags = misc_flags,
				.StructureByteStride = stride,
			};
			GHR(device->CreateBuffer(&d, 0, &new_buffer));
		}

		if (buffer.buffer) {
			// Copy old buffer to new one
			D3D11_BOX box = {
				.left = 0,
				.top = 0,
				.front = 0,
				.right = buffer.size,
				.bottom = 1,
				.back = 1,
			};
			immediate_context->CopySubresourceRegion(new_buffer, 0, 0, 0, 0, buffer.buffer, 0, &box);

			// Release old
			TL_COM_RELEASE(buffer.buffer);
		}

		buffer.buffer = new_buffer;
		buffer.size = new_size;
	}
	switch (buffer.usage) {
		case D3D11_USAGE_DYNAMIC: {
			D3D11_MAPPED_SUBRESOURCE mapped;
			GHR(immediate_context->Map(buffer.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped));
			memcpy(mapped.pData, data, size);
			immediate_context->Unmap(buffer.buffer, 0);
			break;
		}
		case D3D11_USAGE_DEFAULT: {
			D3D11_BOX box = {
				.left = offset,
				.top = 0,
				.front = 0,
				.right = offset + size,
				.bottom = 1,
				.back = 1,
			};
			immediate_context->UpdateSubresource(buffer.buffer, 0, &box, data, 0, 0);
			break;
		}
		default:
			invalid_code_path("buffer.usage");
			break;
	}
}
void State::update_structured_buffer(UntypedStructuredBuffer &buffer, u32 count, u32 stride, void const *data, u32 first_element) {
	if (buffer.usage == D3D11_USAGE_DYNAMIC)
		assert(first_element == 0, "Dynamic buffers can't be updated partially, only as a whole.");

	if (count == 0)
		return;

	u32 size = count * stride;
	u32 offset = first_element * stride;
	if (size + offset > buffer.size) {
		// Resize
			
		UINT min_required_count = first_element + count;
		UINT new_count = max(min_required_count * 3 / 2, 1u); // D3D11 does not allow zero sized buffers.
		UINT new_size = new_count * stride;

		ID3D11Buffer *new_buffer = {};
		{
			D3D11_BUFFER_DESC d = {
				.ByteWidth = new_size,
				.Usage = buffer.usage,
				.BindFlags = D3D11_BIND_SHADER_RESOURCE,
				.CPUAccessFlags = buffer.usage == D3D11_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0u,
				.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
				.StructureByteStride = stride,
			};
			GHR(device->CreateBuffer(&d, 0, &new_buffer));
		}

		ID3D11ShaderResourceView *new_srv = {};
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC d = {};
			d.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
			d.Buffer.FirstElement = 0;
			d.Buffer.NumElements = new_count;
			GHR(device->CreateShaderResourceView(new_buffer, &d, &new_srv));
		}

		if (buffer.buffer) {
			// Copy old buffer to new one
			D3D11_BOX box = {
				.left = 0,
				.top = 0,
				.front = 0,
				.right = buffer.size,
				.bottom = 1,
				.back = 1,
			};
			immediate_context->CopySubresourceRegion(new_buffer, 0, 0, 0, 0, buffer.buffer, 0, &box);

			// Release old
			TL_COM_RELEASE(buffer.buffer);
			TL_COM_RELEASE(buffer.srv);
		}

		buffer.buffer = new_buffer;
		buffer.srv = new_srv;
		buffer.size = new_size;
	}
	switch (buffer.usage) {
		case D3D11_USAGE_DYNAMIC: {
			D3D11_MAPPED_SUBRESOURCE mapped;
			GHR(immediate_context->Map(buffer.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped));
			memcpy(mapped.pData, data, size);
			immediate_context->Unmap(buffer.buffer, 0);
			break;
		}
		case D3D11_USAGE_DEFAULT: {
			D3D11_BOX box = {
				.left = offset,
				.top = 0,
				.front = 0,
				.right = offset + size,
				.bottom = 1,
				.back = 1,
			};
			immediate_context->UpdateSubresource(buffer.buffer, 0, &box, data, 0, 0);
			break;
		}
		default:
			invalid_code_path("buffer.usage");
			break;
	}
}
void State::update_constant_buffer(UntypedConstantBuffer &buffer, u32 size, void const *data) {
	if (buffer.usage == D3D11_USAGE_DYNAMIC) {
		D3D11_MAPPED_SUBRESOURCE mapped;
		GHR(immediate_context->Map(buffer.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped));
		memcpy(mapped.pData, data, size);
		immediate_context->Unmap(buffer.buffer, 0);
	} else if (buffer.usage == D3D11_USAGE_DEFAULT) {
		immediate_context->UpdateSubresource(buffer.buffer, 0, 0, data, 0, 0);
	} else {
		invalid_code_path("bad buffer.usage");
	}
}
void State::clear_render_target(ID3D11RenderTargetView *render_target, f32 const rgba[4]) {
	immediate_context->ClearRenderTargetView(render_target, rgba);
}
void State::clear_depth_stencil(ID3D11DepthStencilView *depth_stencil, f32 depth, u8 stencil) {
	immediate_context->ClearDepthStencilView(depth_stencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
}
void State::draw(u32 vertex_count, u32 offset) {
	immediate_context->Draw(vertex_count, offset);
}
void State::present() {
	swap_chain->Present(sync_interval, 0);
}
void State::resize_back_buffer(u32 width, u32 height) {
	release(back_buffer);
	swap_chain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
	init_back_buffer();
}
void State::set_topology(D3D11_PRIMITIVE_TOPOLOGY topology) { immediate_context->IASetPrimitiveTopology(topology); }
void State::set_vertex_shader(ID3D11VertexShader *shader) { immediate_context->VSSetShader(shader, 0, 0); }
void State::set_pixel_shader(ID3D11PixelShader  *shader) { immediate_context->PSSetShader(shader, 0, 0); }
void State::set_shader_resource(ShaderResource const &resource, char stage, u32 slot) {
	switch (stage) {
		case 'V': immediate_context->VSSetShaderResources(slot, 1, &resource.srv); break;
		case 'P': immediate_context->PSSetShaderResources(slot, 1, &resource.srv); break;
		default: invalid_code_path("bad stage"); break;
	}
}
void State::set_sampler(Sampler const &sampler, char stage, u32 slot) {
	switch (stage) {
		case 'V': immediate_context->VSSetSamplers(slot, 1, &sampler.sampler); break;
		case 'P': immediate_context->PSSetSamplers(slot, 1, &sampler.sampler); break;
		default: invalid_code_path("bad stage"); break;
	}
}
void State::set_constant_buffer(UntypedConstantBuffer const &buffer, char stage, u32 slot) {
	switch (stage) {
		case 'V': immediate_context->VSSetConstantBuffers(slot, 1, &buffer.buffer); break;
		case 'P': immediate_context->PSSetConstantBuffers(slot, 1, &buffer.buffer); break;
		default: invalid_code_path("bad stage"); break;
	}
}
void State::set_viewport(f32 x, f32 y, f32 w, f32 h, f32 depth_min, f32 depth_max) {
	D3D11_VIEWPORT v;
	v.TopLeftX = x;
	v.TopLeftY = y;
	v.Width = w;
	v.Height = h;
	v.MinDepth = depth_min;
	v.MaxDepth = depth_max;
	immediate_context->RSSetViewports(1, &v);
}
void State::set_render_target(ID3D11RenderTargetView *render_target, ID3D11DepthStencilView *depth_stencil) {
	immediate_context->OMSetRenderTargets(1, &render_target, depth_stencil);
}
void State::set_rasterizer(Rasterizer rasterizer) {
	immediate_context->RSSetState(rasterizer.raster);
}
void State::set_depth_stencil(DepthStencil depth_stencil, u8 stencil_ref) {
	immediate_context->OMSetDepthStencilState(depth_stencil.state, stencil_ref);
}
void State::set_blend(Blend blend) {
	float factor[4]{};
	immediate_context->OMSetBlendState(blend.blend, factor, ~0u);
}
u32 State::get_max_msaa_sample_count(DXGI_FORMAT format) {
	u32 quality_levels = 0;
	u32 sample_count = D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT;
	while (sample_count != 1) {
		GHR(device->CheckMultisampleQualityLevels(format, sample_count, &quality_levels));
		if (quality_levels)
			break;
		sample_count /= 2;
	}
	return sample_count;
}

void State::init(IDXGISwapChain *swap_chain, ID3D11RenderTargetView *back_buffer, ID3D11Device *device, ID3D11DeviceContext *immediate_context) {
	this->swap_chain = swap_chain;
	this->device = device;
	this->immediate_context = immediate_context;
	this->back_buffer.rtv = back_buffer;
}
// 'sample_count' can be -1 for highest setting, -2 for second highest, etc
void State::init(HWND window, u32 width, u32 height, DXGI_FORMAT back_buffer_format, u32 buffer_count, bool windowed, u32 sample_count, u32 device_flags) {
	IDXGIFactory1 *dxgi_factory;
	GHR(CreateDXGIFactory1(IID_PPV_ARGS(&dxgi_factory)));
	defer { TL_COM_RELEASE(dxgi_factory); };

	{
		IDXGIAdapter1 *adapter = 0;
		for (UINT adapter_index = 0; dxgi_factory->EnumAdapters1(adapter_index, &adapter) != DXGI_ERROR_NOT_FOUND; ++adapter_index) {
			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);
			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
				TL_COM_RELEASE(adapter);
				continue;
			}
			this->adapter = adapter;
			this->adapter1 = adapter;
			break;
		}
	}

	D3D_FEATURE_LEVEL max_feature = D3D_FEATURE_LEVEL_11_1;
	if (FAILED(D3D11CreateDevice(adapter, D3D_DRIVER_TYPE_UNKNOWN, 0, device_flags, &max_feature, 1, D3D11_SDK_VERSION, &device, 0, &immediate_context))) {
		GHR(D3D11CreateDevice(adapter, D3D_DRIVER_TYPE_UNKNOWN, 0, device_flags, 0, 0, D3D11_SDK_VERSION, &device, 0, &immediate_context));
	}

	u32 max_sample_count = get_max_msaa_sample_count(back_buffer_format);
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
	GHR(dxgi_factory->CreateSwapChain(device, &d, &swap_chain));

	if (FAILED(device->QueryInterface(&device2))) {
		device2 = 0;
	}
	if (FAILED(device->QueryInterface(&device3))) {
		device3 = 0;
	}
	if (device_flags & D3D11_CREATE_DEVICE_DEBUG) {
		if (FAILED(device->QueryInterface(&info_queue))) {
			info_queue = 0;
		}
	}

	init_back_buffer();
}

void State::print_messages() {
	if (!info_queue)
		return;
	UINT64 message_count = info_queue->GetNumStoredMessages();

	for(UINT64 i = 0; i < message_count; i++){
		SIZE_T message_size = 0;
		if (FAILED(info_queue->GetMessage(i, nullptr, &message_size)))
			continue;

		static u8 debug_message_buffer[4096];
		assert(message_size <= sizeof(debug_message_buffer));
		auto message = (D3D11_MESSAGE *)debug_message_buffer;

		if (FAILED(info_queue->GetMessage(i, message, &message_size)))
			continue;

		auto message_string = Span((char *)message->pDescription, message->DescriptionByteLength);

		auto severity = [&] {
			switch (message->Severity) {
				default:
				case D3D11_MESSAGE_SEVERITY_CORRUPTION: return LogSeverity::error;
				case D3D11_MESSAGE_SEVERITY_ERROR:      return LogSeverity::error;
				case D3D11_MESSAGE_SEVERITY_WARNING:    return LogSeverity::warning;
				case D3D11_MESSAGE_SEVERITY_INFO:       return LogSeverity::info;
				case D3D11_MESSAGE_SEVERITY_MESSAGE:    return LogSeverity::info;
			}
		}();
	}

	info_queue->ClearStoredMessages();
}

bool State::default_hresult_handler(HRESULT hr, char const *file, int line, char const *function, char const *expression) {
	print_messages();
	if (FAILED(hr)) {
		println("{}:{}:{}: {} - {}", file, line, function, expression, FormattedHRESULT{hr});
		return false;
	}
	return true;
}

#endif

}}

#undef GHR

#pragma warning(pop)
