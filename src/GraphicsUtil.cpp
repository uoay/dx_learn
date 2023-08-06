#include "GraphicsUtil.h"

#include <sstream>

GraphicsUtil::Exception::Exception(const char* file, int line, HRESULT errorCode) :GameException(file, line, errorCode) {}

const char* GraphicsUtil::Exception::what() const noexcept {
	std::ostringstream oss;
	oss << "[Error Code]" << GetErrorCode() << std::endl
		<< "[Description]" << GetErrorString() << std::endl
		<< GetExceptionLocation();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* GraphicsUtil::Exception::GetType() const {
	return "DirectX Exception";
}

void GraphicsUtil::CreateDevice(Microsoft::WRL::ComPtr<ID3D12Device10>& device) {
	ID3D12Device* tmpDevice = nullptr;
	HRESULT hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_2, IID_PPV_ARGS(&tmpDevice));
	if (FAILED(hr)) {
		throw GFX_EXPECTION(hr);
	}
	hr = tmpDevice->QueryInterface(IID_PPV_ARGS(&device));
	if (FAILED(hr)) {
		throw GFX_EXPECTION(hr)
	}
	tmpDevice->Release();
}

void GraphicsUtil::CreateFence(
	Microsoft::WRL::ComPtr<ID3D12Device10>& device,
	UINT64 initialValue, D3D12_FENCE_FLAGS flags,
	Microsoft::WRL::ComPtr<ID3D12Fence1>& fence
) {
	ID3D12Fence* tmpFence = nullptr;
	HRESULT hr = device->CreateFence(initialValue, flags, IID_PPV_ARGS(&tmpFence));
	if (FAILED(hr)) {
		throw GFX_EXPECTION(hr);
	}
	hr = tmpFence->QueryInterface(IID_PPV_ARGS(&fence));
	if (FAILED(hr)) {
		throw GFX_EXPECTION(hr);
	}
	tmpFence->Release();
}

void GraphicsUtil::CreteCommandObjects(
	Microsoft::WRL::ComPtr<ID3D12Device10>& device,
	D3D12_COMMAND_QUEUE_DESC* cmdQueueDesc,
	Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue,
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator>& cmdAllocator,
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7>& cmdList
) {
	HRESULT hr = device->CreateCommandQueue(cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));
	if (FAILED(hr)) {
		throw GFX_EXPECTION(hr);
	}

	hr = device->CreateCommandAllocator(cmdQueueDesc->Type, IID_PPV_ARGS(&cmdAllocator));
	if (FAILED(hr)) {
		throw GFX_EXPECTION(hr);
	}

	ID3D12GraphicsCommandList* tmpCmdList = nullptr;
	hr = device->CreateCommandList(
		0,
		cmdQueueDesc->Type,
		cmdAllocator.Get(), 
		nullptr,
		IID_PPV_ARGS(&tmpCmdList)
	);
	if (FAILED(hr)) {
		throw GFX_EXPECTION(hr);
	}
	hr = tmpCmdList->QueryInterface(IID_PPV_ARGS(&cmdList));
	if (FAILED(hr)) {
		throw GFX_EXPECTION(hr);
	}
	cmdList->Close();
	tmpCmdList->Release();
}

void GraphicsUtil::CreateFactory(DWORD factoryFlags, Microsoft::WRL::ComPtr<IDXGIFactory7>& factory) {
	IDXGIFactory2* tmpFactory = nullptr;
	HRESULT hr = CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&tmpFactory));
	if (FAILED(hr)) {
		throw GFX_EXPECTION(hr);
	}
	hr = tmpFactory->QueryInterface(IID_PPV_ARGS(&factory));
	if (FAILED(hr)) {
		throw GFX_EXPECTION(hr);
	}
	tmpFactory->Release();
}

void GraphicsUtil::CreateSwapChain(
	Microsoft::WRL::ComPtr<IDXGIFactory7>& factory,
	Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue,
	HWND hWnd,
	DXGI_SWAP_CHAIN_DESC1* swapChainDesc,
	DXGI_SWAP_CHAIN_FULLSCREEN_DESC* swapChainFullscreenDesc,
	Microsoft::WRL::ComPtr<IDXGISwapChain4>& swapChain
) {
	IDXGISwapChain1* tmpSwapChain = nullptr;
	HRESULT hr = factory->CreateSwapChainForHwnd(
		cmdQueue.Get(),
		hWnd,
		swapChainDesc,
		swapChainFullscreenDesc,
		nullptr,
		&tmpSwapChain);
	if (FAILED(hr)) {
		throw GFX_EXPECTION(hr);
	}
	hr = tmpSwapChain->QueryInterface(IID_PPV_ARGS(&swapChain));
	if (FAILED(hr)) {
		throw GFX_EXPECTION(hr);
	}
	tmpSwapChain->Release();
}

void GraphicsUtil::CreateRtvDescriptorHeap(
	Microsoft::WRL::ComPtr<ID3D12Device10>& device,
	D3D12_DESCRIPTOR_HEAP_DESC* rtvHeapDesc,
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap
) {
	HRESULT hr = device->CreateDescriptorHeap(rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));
	if (FAILED(hr)) {
		throw GFX_EXPECTION(hr);
	}
}

void GraphicsUtil::CreateDsvDescriptorHeap(
	Microsoft::WRL::ComPtr<ID3D12Device10>& device,
	D3D12_DESCRIPTOR_HEAP_DESC* dsvHeapDesc,
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap
) {
	HRESULT hr = device->CreateDescriptorHeap(dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
	if (FAILED(hr)) {
		throw GFX_EXPECTION(hr);
	}
}