#pragma once
#include "thread.h"
#include "common.h"
#ifdef TL_D3D11_USE_MATH
#include "math.h"
#endif

#pragma warning(push, 0)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d3d11.h>
#include <d3d11_3.h>
#include <d3dcompiler.h>
#pragma warning(pop)

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxgi")

#ifndef TL_HRESULT_HANDLER
#define TL_HRESULT_HANDLER(hr) ASSERT(SUCCEEDED(hr))
#endif

#define TL_COM_RELEASE(x) (((x) ? (x)->Release() : 0), (x) = 0)

#pragma warning(push)
#pragma warning(disable: 4820) // struct padding

namespace TL { namespace D3D11 {

struct Shader {
	ID3D11VertexShader *vs = 0;
	ID3D11PixelShader *ps = 0;
};

struct ShaderResource {
	ID3D11ShaderResourceView *srv = 0;
};

struct StructuredBuffer : ShaderResource {
	ID3D11Buffer *buffer = 0;
	D3D11_USAGE usage = {};
	u32 size = 0;
};

template <class T>
struct TypedStructuredBuffer : StructuredBuffer {};

struct ConstantBuffer {
	ID3D11Buffer *buffer = 0;
	D3D11_USAGE usage = {};
	u32 size = 0;
};

template <class T>
struct TypedConstantBuffer : ConstantBuffer {};

struct DepthStencil {
	ID3D11DepthStencilView *dsv = 0;
	ID3D11Texture2D *tex = 0;
};

struct Texture : ShaderResource {
	ID3D11Texture2D *tex = 0;
	u32 width = 0;
	u32 height = 0;
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

struct Blend {
	ID3D11BlendState *blend = 0;
};

TL_API void defaultShaderHandler(HRESULT result, char const *messages);

struct State {
	IDXGIAdapter *adapter;
	IDXGIAdapter1 *adapter1;
	IDXGIAdapter2 *adapter2;
	IDXGISwapChain *swapChain;
	ID3D11Device  *device;
	ID3D11Device2 *device2;
	ID3D11Device3 *device3;
	ID3D11DeviceContext *immediateContext;
	RenderTarget backBuffer;
	RecursiveMutex immediateContextMutex;
	u32 syncInterval = 1;
	
	template <class Fn>
	inline void useContext(Fn &&fn) {
		SCOPED_LOCK(immediateContextMutex);
		fn();
	}
	TL_API void initBackBuffer();
	TL_API StructuredBuffer createStructuredBuffer(D3D11_USAGE usage, u32 count, u32 stride, void const* data = 0);
	template <class T>
	inline TypedStructuredBuffer<T> createStructuredBuffer(D3D11_USAGE usage, u32 count, T const *data = 0) {
		TypedStructuredBuffer<T> result;
		(StructuredBuffer &)result = createStructuredBuffer(usage, count, sizeof(T), data);
		return result;
	}
	TL_API RenderTexture createRenderTexture(u32 width, u32 height, u32 sampleCount, DXGI_FORMAT format, u32 cpuFlags = 0);
	TL_API Texture createTexture(u32 width, u32 height, DXGI_FORMAT format, void const* data, bool generateMips = false);
	template <class Handler = decltype(defaultShaderHandler)>
	ID3D11VertexShader *createVertexShader(char const *source, umm sourceSize, char const *name, char const *entryPoint, char const *target, Handler &&handler = defaultShaderHandler) {
		ID3DBlob* byteCode = 0;
		ID3DBlob* messages = 0;
		HRESULT result = D3DCompile(source, sourceSize, name, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, target, 0, 0, &byteCode, &messages);
		handler(result, messages ? (char *)messages->GetBufferPointer() : 0);
		ID3D11VertexShader *vertexShader;
		TL_HRESULT_HANDLER(device->CreateVertexShader(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), 0, &vertexShader));
		byteCode->Release();
		if (messages)
			messages->Release();
		return vertexShader;
	}
	template <class Handler = decltype(defaultShaderHandler)>
	ID3D11PixelShader *createPixelShader(char const *source, umm sourceSize, char const *name, char const *entryPoint, char const *target, Handler &&handler = defaultShaderHandler) {
		ID3DBlob* byteCode = 0;
		ID3DBlob* messages = 0;
		HRESULT result = D3DCompile(source, sourceSize, name, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, target, 0, 0, &byteCode, &messages);
		handler(result, messages ? (char *)messages->GetBufferPointer() : 0);
		ID3D11PixelShader *pixelShader;
		TL_HRESULT_HANDLER(device->CreatePixelShader(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), 0, &pixelShader));
		byteCode->Release();
		if (messages)
			messages->Release();
		return pixelShader;
	}
	TL_API ConstantBuffer createConstantBuffer(u32 size, D3D11_USAGE usage, void const *initialData = 0);
	template <class T>
	inline TypedConstantBuffer<T> createConstantBuffer(D3D11_USAGE usage, T const *initialData = 0) {
		TypedConstantBuffer<T> result;
		(ConstantBuffer &)result = createConstantBuffer(sizeof(T), usage, initialData);
		return result;
	}
	TL_API DepthStencil createDepthStencil(u32 width, u32 height, DXGI_FORMAT format);
	TL_API Sampler createSampler(D3D11_TEXTURE_ADDRESS_MODE address, D3D11_FILTER filter);
	TL_API Rasterizer createRasterizer(D3D11_FILL_MODE fill, D3D11_CULL_MODE cull, bool enableMs = false, bool enableMsLine = false);
	TL_API Blend createBlend(D3D11_BLEND_OP opColor, D3D11_BLEND srcColor, D3D11_BLEND dstColor, D3D11_BLEND_OP opAlpha, D3D11_BLEND srcAlpha, D3D11_BLEND dstAlpha);
	TL_API Blend createBlend(D3D11_BLEND_OP op, D3D11_BLEND src, D3D11_BLEND dst);
	TL_API void updateStructuredBuffer(StructuredBuffer& buffer, u32 count, u32 stride, void const* data, u32 firstElement = 0);
	template <class T>
	inline void updateStructuredBuffer(TypedStructuredBuffer<T> &buffer, u32 count, T const *data, u32 firstElement = 0) {
		updateStructuredBuffer(buffer, count, sizeof(T), data, firstElement);
	}
	TL_API void updateConstantBuffer(ConstantBuffer &buffer, u32 size, void const *data);
	template <class T>
	inline void updateConstantBuffer(TypedConstantBuffer<T> &buffer, T const *data) {
		updateConstantBuffer(buffer, sizeof(T), data);
	}
	TL_API void clearRenderTarget(ID3D11RenderTargetView* renderTarget, f32 const rgba[4]);
	TL_API void clearDepthStencil(ID3D11DepthStencilView *depthStencil, f32 depth);
	inline void clearRenderTarget(RenderTarget &rt, f32 const rgba[4]) { clearRenderTarget(rt.rtv, rgba); }
	inline void clearDepthStencil(DepthStencil &depthStencil, f32 depth) { clearDepthStencil(depthStencil.dsv, depth); }
	TL_API void draw(u32 vertexCount, u32 offset = 0);
	TL_API void present();
	TL_API void resizeBackBuffer(u32 width, u32 height);
	TL_API void setTopology(D3D11_PRIMITIVE_TOPOLOGY topology);
	TL_API void setVertexShader(ID3D11VertexShader *shader);
	TL_API void setPixelShader(ID3D11PixelShader *shader);
	inline void setShader(ID3D11VertexShader *shader) { setVertexShader(shader); }
	inline void setShader(ID3D11PixelShader  *shader) { setPixelShader(shader); }
	TL_API void setShaderResource(ShaderResource const &resource, char stage, u32 slot);
	TL_API void setSampler(Sampler const &sampler, char stage, u32 slot);
	TL_API void setConstantBuffer(ConstantBuffer const &buffer, char stage, u32 slot);
	TL_API void setViewport(f32 x, f32 y, f32 w, f32 h, f32 depthMin, f32 depthMax);
	inline void setViewport(f32 x, f32 y, f32 w, f32 h) { setViewport(x, y, w, h, 0, 1); }
	inline void setViewport(f32 w, f32 h) { setViewport(0, 0, w, h); }
	TL_API void setRenderTarget(ID3D11RenderTargetView *renderTarget, ID3D11DepthStencilView *depthStencil);
	inline void setRenderTarget(RenderTarget &rt) { setRenderTarget(rt.rtv, 0); }
	inline void setRenderTarget(RenderTarget &rt, DepthStencil &ds) { setRenderTarget(rt.rtv, ds.dsv); }
	TL_API void setRasterizer(Rasterizer rasterizer = {});
	TL_API void setBlend(Blend blend = {});
	TL_API u32 getMaxMsaaSampleCount(DXGI_FORMAT format);
#ifdef TL_D3D11_USE_MATH
	inline void resizeBackBuffer(v2u size) { resizeBackBuffer(size.x, size.y); }
	inline void resizeBackBuffer(v2s size) { resizeBackBuffer((v2u)size); }
	inline void setViewport(v2f position, v2f size) { setViewport(position.x, position.y, size.x, size.y); }
	inline void setViewport(v2s position, v2s size) { setViewport((v2f)position, (v2f)size); }
	inline void setViewport(v2u position, v2u size) { setViewport((v2f)position, (v2f)size); }
	inline void setViewport(v2f size) { setViewport({}, size); }
	inline void setViewport(v2s size) { setViewport({}, (v2f)size); }
	inline void setViewport(v2u size) { setViewport({}, (v2f)size); }
#endif
};

TL_API void addRef(StructuredBuffer &v);
TL_API void addRef(ConstantBuffer &v);
TL_API void addRef(RenderTarget &v);
TL_API void addRef(DepthStencil &v);
TL_API void addRef(Texture &v);
TL_API void addRef(Sampler &v);
TL_API void addRef(RenderTexture &v);
TL_API void addRef(Rasterizer &v);
TL_API void addRef(Blend &v);

TL_API void release(StructuredBuffer &v);
TL_API void release(ConstantBuffer &v);
TL_API void release(RenderTarget &v);
TL_API void release(DepthStencil &v);
TL_API void release(Texture &v);
TL_API void release(Sampler &v);
TL_API void release(RenderTexture &v);
TL_API void release(Rasterizer &v);
TL_API void release(Blend &v);

inline bool valid(ShaderResource &v) { return v.srv != 0; }
inline bool valid(ConstantBuffer &v) { return v.buffer != 0; }
inline bool valid(State &state) { return state.swapChain != 0; }

TL_API u32 getBitsPerPixel(DXGI_FORMAT format);

TL_API void initState(State &state, IDXGISwapChain *swapChain, ID3D11RenderTargetView *backBuffer, ID3D11Device *device, ID3D11DeviceContext *immediateContext);
// 'sampleCount' can be -1 for highest setting, -2 for second highest, etc
TL_API void initState(State &state, HWND window, u32 width, u32 height, DXGI_FORMAT backBufferFormat, u32 bufferCount, bool windowed, u32 sampleCount, u32 deviceFlags = 0);

#ifdef TL_IMPL

void addRef(StructuredBuffer &v) {
	v.buffer->AddRef();
	v.srv->AddRef();
}
void addRef(ConstantBuffer &v) {
	v.buffer->AddRef();
}
void addRef(RenderTarget &v) {
	v.rtv->AddRef();
	v.tex->AddRef();
}
void addRef(DepthStencil &v) {
	v.dsv->AddRef();
	v.tex->AddRef();
}
void addRef(Texture &v) {
	v.srv->AddRef();
	v.tex->AddRef();
}
void addRef(Sampler &v) {
	v.sampler->AddRef();
}
void addRef(RenderTexture &v) {
	v.rtv->AddRef();
	v.srv->AddRef();
	v.tex->AddRef();
}
void addRef(Rasterizer &v) {
	v.raster->AddRef();
}
void addRef(Blend &v) {
	v.blend->AddRef();
}

void release(StructuredBuffer &v) {
	TL_COM_RELEASE(v.buffer);
	TL_COM_RELEASE(v.srv);
}
void release(ConstantBuffer &v) {
	TL_COM_RELEASE(v.buffer);
}
void release(RenderTarget &v) {
	TL_COM_RELEASE(v.rtv);
	TL_COM_RELEASE(v.tex);
}
void release(DepthStencil &v) {
	TL_COM_RELEASE(v.dsv);
	TL_COM_RELEASE(v.tex);
}
void release(Texture &v) {
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
void release(Blend &v) {
	TL_COM_RELEASE(v.blend);
}

inline void defaultShaderHandler(HRESULT result, char const *messages) {
#if BUILD_DEBUG
#if COMPILER_MSVC
	OutputDebugStringA(messages);
#endif
#endif
	ASSERT(SUCCEEDED(result));
	(void)messages;
}

inline u32 getBitsPerPixel(DXGI_FORMAT format) {
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

void State::initBackBuffer() {
	TL_HRESULT_HANDLER(swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer.tex)));
	TL_HRESULT_HANDLER(device->CreateRenderTargetView(backBuffer.tex, 0, &backBuffer.rtv));
}
StructuredBuffer State::createStructuredBuffer(D3D11_USAGE usage, u32 count, u32 stride, void const* data) {
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
			D3D11_SUBRESOURCE_DATA initialData {};
			initialData.pSysMem = data;
			TL_HRESULT_HANDLER(device->CreateBuffer(&d, &initialData, &result.buffer));
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
RenderTexture State::createRenderTexture(u32 width, u32 height, u32 sampleCount, DXGI_FORMAT format, u32 cpuFlags) {
	RenderTexture result;
	{
		D3D11_TEXTURE2D_DESC d = {};
		d.Format = format;
		d.ArraySize = 1;
		d.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		d.Width = width;
		d.Height = height;
		d.MipLevels = 1;
		d.SampleDesc = {sampleCount, 0};
		d.CPUAccessFlags = cpuFlags;
		TL_HRESULT_HANDLER(device->CreateTexture2D(&d, 0, &result.tex));
	}
	{
		D3D11_RENDER_TARGET_VIEW_DESC d = {};
		d.ViewDimension = sampleCount == 1 ? D3D11_RTV_DIMENSION_TEXTURE2D : D3D11_RTV_DIMENSION_TEXTURE2DMS;
		TL_HRESULT_HANDLER(device->CreateRenderTargetView(result.tex, &d, &result.rtv));
	}
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC d = {};
		d.ViewDimension = sampleCount == 1 ? D3D11_SRV_DIMENSION_TEXTURE2D : D3D11_SRV_DIMENSION_TEXTURE2DMS;
		d.Texture2D.MipLevels = 1;
		TL_HRESULT_HANDLER(device->CreateShaderResourceView(result.tex, &d, &result.srv));
	}
	return result;
}
Texture State::createTexture(u32 width, u32 height, DXGI_FORMAT format, void const* data, bool generateMips) {
	Texture result;

	u32 bpp = getBitsPerPixel(format);
	ASSERT(bpp % 8 == 0);
	u32 pitch = width * bpp / 8;

	{
		D3D11_TEXTURE2D_DESC d = {};
		d.Format = format;
		d.ArraySize = 1;
		d.BindFlags = D3D11_BIND_SHADER_RESOURCE | (generateMips ? D3D11_BIND_RENDER_TARGET : 0u);
		d.Width = width;
		d.Height = height;
		d.MipLevels = generateMips ? 0u : 1u;
		d.SampleDesc = {1, 0};
		d.MiscFlags = generateMips ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0u;

		if (data) {
			D3D11_SUBRESOURCE_DATA initialData {};
			initialData.pSysMem = data;
			initialData.SysMemPitch = pitch;
			TL_HRESULT_HANDLER(device->CreateTexture2D(&d, generateMips ? nullptr : &initialData, &result.tex));
		}
		else {
			TL_HRESULT_HANDLER(device->CreateTexture2D(&d, 0, &result.tex));
		}
	}
	D3D11_SHADER_RESOURCE_VIEW_DESC d = {};
	d.Format = format;
	d.Texture2D.MipLevels = generateMips ? ~0u : 1u;
	d.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	TL_HRESULT_HANDLER(device->CreateShaderResourceView(result.tex, &d, &result.srv));

	if (generateMips) {
		useContext([&] {
			immediateContext->UpdateSubresource(result.tex, 0, 0, data, pitch, 0);
			immediateContext->GenerateMips(result.srv);
		});
	}
	return result;
}
ConstantBuffer State::createConstantBuffer(u32 size, D3D11_USAGE usage, void const *initialData) {
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = ceil(size, 16u);
	desc.CPUAccessFlags = usage == D3D11_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0u;
	desc.Usage = usage;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = initialData;

	ConstantBuffer result = {};
	TL_HRESULT_HANDLER(device->CreateBuffer(&desc, initialData ? &data : 0, &result.buffer));
	result.usage = usage;
	result.size = size;
	return result;
}
DepthStencil State::createDepthStencil(u32 width, u32 height, DXGI_FORMAT format) {
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
Sampler State::createSampler(D3D11_TEXTURE_ADDRESS_MODE address, D3D11_FILTER filter) {
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
Rasterizer State::createRasterizer(D3D11_FILL_MODE fill, D3D11_CULL_MODE cull, bool enableMs, bool enableMsLine) {
	D3D11_RASTERIZER_DESC desc = {};
	desc.CullMode = cull;
	desc.FillMode = fill;
	desc.MultisampleEnable = enableMs;
	desc.AntialiasedLineEnable = enableMsLine;
	desc.DepthClipEnable = true;

	Rasterizer result = {};
	TL_HRESULT_HANDLER(device->CreateRasterizerState(&desc, &result.raster));
	return result;
}
Blend State::createBlend(D3D11_BLEND_OP opColor, D3D11_BLEND srcColor, D3D11_BLEND dstColor, D3D11_BLEND_OP opAlpha, D3D11_BLEND srcAlpha, D3D11_BLEND dstAlpha) {
	D3D11_BLEND_DESC desc = {};
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].BlendOp = opColor;
	desc.RenderTarget[0].SrcBlend = srcColor;
	desc.RenderTarget[0].DestBlend = dstColor;
	desc.RenderTarget[0].BlendOpAlpha = opAlpha;
	desc.RenderTarget[0].SrcBlendAlpha = srcAlpha;
	desc.RenderTarget[0].DestBlendAlpha = dstAlpha;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	Blend result = {};
	TL_HRESULT_HANDLER(device->CreateBlendState(&desc, &result.blend));
	return result;
}
Blend State::createBlend(D3D11_BLEND_OP op, D3D11_BLEND src, D3D11_BLEND dst) {
	return createBlend(op, src, dst, D3D11_BLEND_OP_ADD, D3D11_BLEND_ONE, D3D11_BLEND_ZERO);
}
void State::updateStructuredBuffer(StructuredBuffer& buffer, u32 count, u32 stride, void const* data, u32 firstElement) {
	u32 size = count * stride;
	u32 offset = firstElement * stride;
	ASSERT(size + offset <= buffer.size);
	if (buffer.usage == D3D11_USAGE_DYNAMIC) {
		D3D11_MAPPED_SUBRESOURCE mapped;
		useContext([&] { TL_HRESULT_HANDLER(immediateContext->Map(buffer.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)); });
		memcpy(mapped.pData, data, size);
		useContext([&] { immediateContext->Unmap(buffer.buffer, 0); });
	}
	else if (buffer.usage == D3D11_USAGE_DEFAULT) {
		D3D11_BOX box = {};
		box.left = offset;
		box.right = box.left + size;
		box.back = 1;
		box.bottom = 1;
		useContext([&] { immediateContext->UpdateSubresource(buffer.buffer, 0, &box, data, 0, 0); });
	}
	else {
		INVALID_CODE_PATH("bad buffer.usage");
	}
}
void State::updateConstantBuffer(ConstantBuffer &buffer, u32 size, void const *data) {
	if (buffer.usage == D3D11_USAGE_DYNAMIC) {
		D3D11_MAPPED_SUBRESOURCE mapped;
		useContext([&] { TL_HRESULT_HANDLER(immediateContext->Map(buffer.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)); });
		memcpy(mapped.pData, data, size);
		useContext([&] { immediateContext->Unmap(buffer.buffer, 0); });
	} else if (buffer.usage == D3D11_USAGE_DEFAULT) {
		useContext([&] { immediateContext->UpdateSubresource(buffer.buffer, 0, 0, data, 0, 0); });
	} else {
		INVALID_CODE_PATH("bad buffer.usage");
	}
}
void State::clearRenderTarget(ID3D11RenderTargetView* renderTarget, f32 const rgba[4]) {
	useContext([&] { immediateContext->ClearRenderTargetView(renderTarget, rgba); });
}
void State::clearDepthStencil(ID3D11DepthStencilView *depthStencil, f32 depth) {
	useContext([&] {
		immediateContext->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, 0);	
	});
}
void State::draw(u32 vertexCount, u32 offset) { 
	useContext([&] { immediateContext->Draw(vertexCount, offset); });
}
void State::present() {
	swapChain->Present(syncInterval, 0);
}
void State::resizeBackBuffer(u32 width, u32 height) {
	release(backBuffer);
	swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
	initBackBuffer();
}
void State::setTopology(D3D11_PRIMITIVE_TOPOLOGY topology) { useContext([&] { immediateContext->IASetPrimitiveTopology(topology); }); }
void State::setVertexShader(ID3D11VertexShader *shader) { useContext([&] { immediateContext->VSSetShader(shader, 0, 0); }); }
void State::setPixelShader(ID3D11PixelShader  *shader) { useContext([&] { immediateContext->PSSetShader(shader, 0, 0); }); }
void State::setShaderResource(ShaderResource const &resource, char stage, u32 slot) {
	useContext([&] { 
		switch (stage) {
			case 'V': immediateContext->VSSetShaderResources(slot, 1, &resource.srv); break;
			case 'P': immediateContext->PSSetShaderResources(slot, 1, &resource.srv); break;
			default: INVALID_CODE_PATH("bad stage"); break;
		}
	});
}
void State::setSampler(Sampler const &sampler, char stage, u32 slot) {
	useContext([&] { 
		switch (stage) {
			case 'V': immediateContext->VSSetSamplers(slot, 1, &sampler.sampler); break;
			case 'P': immediateContext->PSSetSamplers(slot, 1, &sampler.sampler); break;
			default: INVALID_CODE_PATH("bad stage"); break;
		}
	});
}
void State::setConstantBuffer(ConstantBuffer const &buffer, char stage, u32 slot) {
	useContext([&] { 
		switch (stage) {
			case 'V': immediateContext->VSSetConstantBuffers(slot, 1, &buffer.buffer); break;
			case 'P': immediateContext->PSSetConstantBuffers(slot, 1, &buffer.buffer); break;
			default: INVALID_CODE_PATH("bad stage"); break;
		}
	});
}
void State::setViewport(f32 x, f32 y, f32 w, f32 h, f32 depthMin, f32 depthMax) {
	D3D11_VIEWPORT v;
	v.TopLeftX = x;
	v.TopLeftY = y;
	v.Width = w;
	v.Height = h;
	v.MinDepth = depthMin;
	v.MaxDepth = depthMax;
	useContext([&] { immediateContext->RSSetViewports(1, &v); });
}
void State::setRenderTarget(ID3D11RenderTargetView *renderTarget, ID3D11DepthStencilView *depthStencil) { 
	useContext([&] { 
		immediateContext->OMSetRenderTargets(1, &renderTarget, depthStencil);
	});
}
void State::setRasterizer(Rasterizer rasterizer) { 
	useContext([&] { 
		immediateContext->RSSetState(rasterizer.raster);
	});
}
void State::setBlend(Blend blend) { 
	useContext([&] { 
		float factor[4]{};
		immediateContext->OMSetBlendState(blend.blend, factor, ~0u);
	});
}
u32 State::getMaxMsaaSampleCount(DXGI_FORMAT format) {
	u32 qualityLevels = 0;
	u32 sampleCount = D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT;
	for (;;) {
		TL_HRESULT_HANDLER(device->CheckMultisampleQualityLevels(format, sampleCount, &qualityLevels));
		if (qualityLevels != 0)
			break;
		sampleCount /= 2;
		if (sampleCount == 1)
			break;
	}
	return sampleCount;
}

void initState(State &state, IDXGISwapChain *swapChain, ID3D11RenderTargetView *backBuffer, ID3D11Device *device, ID3D11DeviceContext *immediateContext) {
	state.swapChain = swapChain;
	state.device = device;
	state.immediateContext = immediateContext;
	state.backBuffer.rtv = backBuffer;
}
// 'sampleCount' can be -1 for highest setting, -2 for second highest, etc
void initState(State &state, HWND window, u32 width, u32 height, DXGI_FORMAT backBufferFormat, u32 bufferCount, bool windowed, u32 sampleCount, u32 deviceFlags) {
	IDXGIFactory1 *dxgiFactory;
	TL_HRESULT_HANDLER(CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)));
	DEFER { TL_COM_RELEASE(dxgiFactory); };

	IDXGIAdapter1 *adapter = 0;
	for (UINT adapterIndex = 0; dxgiFactory->EnumAdapters1(adapterIndex, &adapter) != DXGI_ERROR_NOT_FOUND; ++adapterIndex) {
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

	D3D_FEATURE_LEVEL maxFeature = D3D_FEATURE_LEVEL_11_1;
	if (FAILED(D3D11CreateDevice(state.adapter, D3D_DRIVER_TYPE_UNKNOWN, 0, deviceFlags, &maxFeature, 1, D3D11_SDK_VERSION, &state.device, 0, &state.immediateContext))) {
		TL_HRESULT_HANDLER(D3D11CreateDevice(state.adapter, D3D_DRIVER_TYPE_UNKNOWN, 0, deviceFlags, 0, 0, D3D11_SDK_VERSION, &state.device, 0, &state.immediateContext));
	}

	u32 maxSampleCount = state.getMaxMsaaSampleCount(backBufferFormat);
	if ((s32)sampleCount < 0) sampleCount = maxSampleCount / (u32)-(s32)sampleCount;

	DXGI_SWAP_CHAIN_DESC d = {};
	d.BufferDesc.Width = width;
	d.BufferDesc.Height = height;
	d.BufferDesc.Format = backBufferFormat;
	d.BufferCount = bufferCount;
	d.Windowed = windowed;
	d.SampleDesc.Count = sampleCount;
	d.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	d.OutputWindow = window;
	TL_HRESULT_HANDLER(dxgiFactory->CreateSwapChain(state.device, &d, &state.swapChain));

	if (FAILED(state.device->QueryInterface(&state.device2))) { 
		state.device2 = 0;
	}
	if (FAILED(state.device->QueryInterface(&state.device3))) { 
		state.device3 = 0;
	}

	state.initBackBuffer();
}

#endif

}}
#pragma warning(pop)
