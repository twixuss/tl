#pragma once
#include "thread.h"

#if OS_WINDOWS
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d3d11.h>
#include <d3d11_3.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

#ifndef TL_HRESULT_HANDLER
#define TL_HRESULT_HANDLER(hr) ASSERT(SUCCEEDED(hr))
#endif

#define TL_COM_RELEASE(x)	\
	do {					\
		if (x) {			\
			x->Release();	\
		}					\
		x = 0;				\
	} while(0)

namespace TL { namespace D3D11 {

struct StructuredBuffer {
	ID3D11Buffer *buffer;
	ID3D11ShaderResourceView *srv;
	D3D11_USAGE usage;
	u32 size;
};

struct RenderTarget {
	ID3D11RenderTargetView *rtv;
	ID3D11Texture2D *tex;
};

struct Texture {
	ID3D11ShaderResourceView *srv;
	ID3D11Texture2D *tex;
	u32 width;
	u32 height;
};

struct RenderTexture {
	ID3D11RenderTargetView *rtv;
	ID3D11ShaderResourceView *srv;
	ID3D11Texture2D *tex;
};

void defaultShaderHandler(HRESULT result, char const *messages) {
	ASSERT(SUCCEEDED(result));
}

struct State {
	IDXGISwapChain *swapChain;
	ID3D11Device  *device;
	ID3D11Device2 *device2;
	ID3D11Device3 *device3;
	ID3D11DeviceContext *immediateContext;
	RenderTarget backBuffer;
	RecursiveMutex immediateContextMutex;
	u32 syncInterval = 1;

	void createBackBuffer();
	StructuredBuffer createStructuredBuffer(u32 count, u32 stride, void const *data, D3D11_USAGE usage);
	RenderTexture createRenderTexture(u32 width, u32 height, u32 sampleCount, DXGI_FORMAT format, UINT cpuFlags = 0);
	Texture createTexture(u32 width, u32 height, DXGI_FORMAT format, void const *data, bool generateMips = false);
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

	void updateStructuredBuffer(StructuredBuffer &buffer, u32 count, u32 stride, void const *data, u32 firstElement = 0);

	void clearRenderTarget(ID3D11RenderTargetView *renderTarget, f32 const rgba[4]);
	void clearRenderTarget(RenderTarget &rt, f32 const rgba[4]) { clearRenderTarget(rt.rtv, rgba); }
	void clearRenderTarget(RenderTexture &rt, f32 const rgba[4]) { clearRenderTarget(rt.rtv, rgba); }

	template <class Fn>
	void useContext(Fn &&fn) {
		SCOPED_LOCK(immediateContextMutex);
		fn();
	}

	void draw(u32 vertexCount, u32 offset = 0) { 
		useContext([&] { immediateContext->Draw(vertexCount, offset); });
	}

	void present() {
		swapChain->Present(syncInterval, 0);
	}

	void resizeBackBuffer(u32 width, u32 height) {
		release(backBuffer);
		swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
		createBackBuffer();
	}

	void setTopology(D3D11_PRIMITIVE_TOPOLOGY topology) { useContext([&] { immediateContext->IASetPrimitiveTopology(topology); }); }
	void setVertexShader(ID3D11VertexShader *shader) { useContext([&] { immediateContext->VSSetShader(shader, 0, 0); }); }
	void setPixelShader(ID3D11PixelShader  *shader) { useContext([&] { immediateContext->PSSetShader(shader, 0, 0); }); }
	void setShaderResource(StructuredBuffer &buffer, u32 slot, char stage) { 
		useContext([&] { 
			switch (stage) {
				case 'V': immediateContext->VSSetShaderResources(slot, 1, &buffer.srv); break;
				case 'P': immediateContext->PSSetShaderResources(slot, 1, &buffer.srv); break;
				default: INVALID_CODE_PATH("bad stage"); break;
			}
		});
	}
    void setViewport(f32 x, f32 y, f32 w, f32 h, f32 depthMin, f32 depthMax);
	void setViewport(f32 x, f32 y, f32 w, f32 h) { setViewport(x, y, w, h, 0, 1); }
	void setViewport(f32 w, f32 h) { setViewport(0, 0, w, h); }
	void setRenderTarget(ID3D11RenderTargetView *renderTarget) { useContext([&] { immediateContext->OMSetRenderTargets(1, &renderTarget, 0); }); }
	void setRenderTarget(RenderTarget &renderTarget) { setRenderTarget(renderTarget.rtv); }

	u32 getMaxMsaaSampleCount(DXGI_FORMAT format);
};

u32 getBitsPerPixel(DXGI_FORMAT format) {
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
    }
}

void release(StructuredBuffer v) {
	TL_COM_RELEASE(v.buffer);
	TL_COM_RELEASE(v.srv);
}
void release(RenderTarget v) {
	TL_COM_RELEASE(v.rtv);
	TL_COM_RELEASE(v.tex);
}
void release(Texture v) {
	TL_COM_RELEASE(v.srv);
	TL_COM_RELEASE(v.tex);
}
void release(RenderTexture v) {
	TL_COM_RELEASE(v.rtv);
	TL_COM_RELEASE(v.srv);
	TL_COM_RELEASE(v.tex);
}

void initState(State &state, IDXGISwapChain *swapChain, ID3D11RenderTargetView *backBuffer, ID3D11Device *device, ID3D11DeviceContext *immediateContext) {
	state.swapChain = swapChain;
	state.device = device;
	state.immediateContext = immediateContext;
	state.backBuffer.rtv = backBuffer;
}
void initState(State &state, HWND window, u32 width, u32 height, DXGI_FORMAT backBufferFormat, u32 bufferCount, bool windowed, u32 sampleCount, u32 deviceFlags = 0) {
	DXGI_SWAP_CHAIN_DESC d = {};
	d.BufferDesc.Width = width;
	d.BufferDesc.Height = height;
	d.BufferDesc.Format = backBufferFormat;
	d.BufferCount = bufferCount;
	d.Windowed = windowed;
	d.SampleDesc.Count = sampleCount;
	d.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	d.OutputWindow = window;
	D3D_FEATURE_LEVEL maxFeature = D3D_FEATURE_LEVEL_11_1;
	if (FAILED(D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, 0, deviceFlags, &maxFeature, 1, D3D11_SDK_VERSION, &d, &state.swapChain, &state.device, 0, &state.immediateContext))) {
		TL_HRESULT_HANDLER(D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, 0, deviceFlags, 0, 0, D3D11_SDK_VERSION, &d, &state.swapChain, &state.device, 0, &state.immediateContext));
	}

	if (FAILED(state.device->QueryInterface(&state.device2))) { 
		state.device2 = 0;
	}
	if (FAILED(state.device->QueryInterface(&state.device3))) { 
		state.device3 = 0;
	}

	state.createBackBuffer();
}

StructuredBuffer State::createStructuredBuffer(u32 count, u32 stride, void const *data, D3D11_USAGE usage) {
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
			D3D11_SUBRESOURCE_DATA initialData{};
			initialData.pSysMem = data;
			TL_HRESULT_HANDLER(device->CreateBuffer(&d, &initialData, &result.buffer));
		} else {
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
RenderTexture State::createRenderTexture(u32 width, u32 height, u32 sampleCount, DXGI_FORMAT format, UINT cpuFlags) {
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
Texture State::createTexture(u32 width, u32 height, DXGI_FORMAT format, void const *data, bool generateMips) {
	Texture result;

    u32 bpp = getBitsPerPixel(format);
    ASSERT(bpp % 8 == 0);
	u32 pitch = width * bpp / 8;

	{
		D3D11_TEXTURE2D_DESC d = {};
		d.Format = format;
		d.ArraySize = 1;
		d.BindFlags = D3D11_BIND_SHADER_RESOURCE | (generateMips ? D3D11_BIND_RENDER_TARGET : 0);
		d.Width = width;
		d.Height = height;
		d.MipLevels = generateMips ? 0 : 1;
		d.SampleDesc = {1, 0};
		d.MiscFlags = generateMips ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

		if (data) {
			D3D11_SUBRESOURCE_DATA initialData{};
			initialData.pSysMem = data;
			initialData.SysMemPitch = pitch;
			TL_HRESULT_HANDLER(device->CreateTexture2D(&d, generateMips ? nullptr : &initialData, &result.tex));
		} else {
			TL_HRESULT_HANDLER(device->CreateTexture2D(&d, 0, &result.tex));
		}
	}
	D3D11_SHADER_RESOURCE_VIEW_DESC d = {};
	d.Format = format;
	d.Texture2D.MipLevels = generateMips ? -1 : 1;
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

void State::updateStructuredBuffer(StructuredBuffer &buffer, u32 count, u32 stride, void const *data, u32 firstElement) {
	u32 size = count * stride;
	u32 offset = firstElement * stride;
	ASSERT(size + offset <= buffer.size);
	if (buffer.usage == D3D11_USAGE_DYNAMIC) {
		D3D11_MAPPED_SUBRESOURCE mapped;
		useContext([&] { TL_HRESULT_HANDLER(immediateContext->Map(buffer.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)); });
		memcpy(mapped.pData, data, size);
		useContext([&] { immediateContext->Unmap(buffer.buffer, 0); });
	} else if (buffer.usage == D3D11_USAGE_DEFAULT) {
		D3D11_BOX box = {};
		box.left = offset;
		box.right = box.left + size;
		box.back = 1;
		box.bottom = 1;
		useContext([&] { immediateContext->UpdateSubresource(buffer.buffer, 0, &box, data, 0, 0); });
	}
}

void State::clearRenderTarget(ID3D11RenderTargetView *renderTarget, f32 const rgba[4]) {
	useContext([&] { immediateContext->ClearRenderTargetView(renderTarget, rgba); });
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
void State::createBackBuffer() {
    TL_HRESULT_HANDLER(swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer.tex)));
    TL_HRESULT_HANDLER(device->CreateRenderTargetView(backBuffer.tex, 0, &backBuffer.rtv));
}
}}
#endif
