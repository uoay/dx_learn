#include "Graphics.h"
#include <iostream>

Graphics::Graphics(HWND hWnd) {
#ifndef NDEBUG
	Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
	D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));
	debugController->EnableDebugLayer();
#endif
	CreateDevice();
	CreateFence();
	CreteCommandObjects();
	CreateFactory();
	CreateSwapChain(hWnd);
	CreateDescriptorHeap();
}

void Graphics::CreateDevice() {
	ID3D12Device* tmpDevice = nullptr;
	D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&tmpDevice));
	tmpDevice->QueryInterface(IID_PPV_ARGS(&device));
	tmpDevice->Release();
}

void Graphics::CreateFence() {
	ID3D12Fence* tmpFence = nullptr;
	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&tmpFence));
	tmpFence->QueryInterface(IID_PPV_ARGS(&fence));
	tmpFence->Release();
}

void Graphics::CreteCommandObjects() {
	D3D12_COMMAND_QUEUE_DESC queueDesc{};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.NodeMask = 0;

	device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&cmdQueue));
	
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAllocator));

	ID3D12GraphicsCommandList* tmpCmdList = nullptr;
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator.Get(), nullptr, IID_PPV_ARGS(&tmpCmdList));
	tmpCmdList->QueryInterface(IID_PPV_ARGS(&cmdList));
	cmdList->Close();
	tmpCmdList->Release();
}

void Graphics::CreateFactory() {
	DWORD dxgiFactoryFlags = 0;
#ifndef NDEBUG
	dxgiFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif
	IDXGIFactory2* tmpFactory = nullptr;
	CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&tmpFactory));
	tmpFactory->QueryInterface(IID_PPV_ARGS(&factory));
	tmpFactory->Release();
}

void Graphics::CreateSwapChain(HWND hWnd) {
	IDXGISwapChain1* tmpSwapChain = nullptr;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = 0;
	swapChainDesc.Height = 0;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.Stereo = FALSE;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = bufferCount;
	swapChainDesc.Scaling = DXGI_SCALING_NONE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapChainDesc.Flags = 0;
	factory->CreateSwapChainForHwnd(cmdQueue.Get(), hWnd, &swapChainDesc, nullptr, nullptr, &tmpSwapChain);
	tmpSwapChain->QueryInterface(IID_PPV_ARGS(&swapChain));
	tmpSwapChain->Release();
}

void Graphics::CreateDescriptorHeap() {
	rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	dsvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.NumDescriptors = bufferCount;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = 0;
	device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	rtvHeapDesc.NumDescriptors = bufferCount;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = 0;
	device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&dsvHeap));
}