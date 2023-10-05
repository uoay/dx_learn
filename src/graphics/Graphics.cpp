#include "Graphics.h"

#include <sstream>

#include <d3dcompiler.h>
#include <DirectXColors.h>

#include "Direct3DUtil.h"
#include "GameException.h"

Graphics::Graphics(HWND hWnd, int clientWidth, int clientHeight) {
	GFX_THROW_IF_FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&mDevice)));

	GFX_THROW_IF_FAILED(mDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence)));

	CreateCommandObjects();

	GFX_THROW_IF_FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&mFactory)));

	CreateSwapChain(hWnd, clientWidth, clientHeight);

	GetDescriptorSize();

	CreateRtvAndDsvDescriptorHeap();

	OnResize(clientWidth, clientHeight);

}

void Graphics::CreateCommandObjects() {
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	commandQueueDesc.NodeMask = 0;

	GFX_THROW_IF_FAILED(mDevice->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&mCommandQueue)));

	GFX_THROW_IF_FAILED(mDevice->CreateCommandAllocator(commandQueueDesc.Type, IID_PPV_ARGS(&mCommandAllocator)));

	GFX_THROW_IF_FAILED(mDevice->CreateCommandList(
		0,
		commandQueueDesc.Type,
		mCommandAllocator.Get(),
		nullptr,
		IID_PPV_ARGS(&mCommandList)
	));
	GFX_THROW_IF_FAILED(mCommandList->Close());
}

void Graphics::CreateSwapChain(HWND hWnd, int clientWidth, int clientHeight) {
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Height = clientHeight;
	swapChainDesc.Width = clientWidth;
	swapChainDesc.Format = mBackBufferFormat;
	swapChainDesc.Stereo = FALSE;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = mBackBufferCount;
	swapChainDesc.Scaling = DXGI_SCALING_NONE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	IDXGISwapChain1* tmpSwapChain = nullptr;
	GFX_THROW_IF_FAILED(mFactory->CreateSwapChainForHwnd(
		mCommandQueue.Get(),
		hWnd,
		&swapChainDesc,
		nullptr,
		nullptr,
		&tmpSwapChain
	));
	GFX_THROW_IF_FAILED(tmpSwapChain->QueryInterface(IID_PPV_ARGS(&mSwapChain)));
	tmpSwapChain->Release();
}

void Graphics::GetDescriptorSize()
{
	mRtvDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	mDsvDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	mCbvSrvUavDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void Graphics::CreateRtvAndDsvDescriptorHeap() {
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.NumDescriptors = mBackBufferCount;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = 0;
	GFX_THROW_IF_FAILED(mDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&mRtvHeap)));

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NodeMask = 0;
	GFX_THROW_IF_FAILED(mDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&mDsvHeap)));
}

void Graphics::CreateRenderTargetView() {
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(mRtvHeap->GetCPUDescriptorHandleForHeapStart());
	for (int i = 0; i < mBackBufferCount; ++i) {
		GFX_THROW_IF_FAILED(mSwapChain->GetBuffer(i, IID_PPV_ARGS(&mBackBuffer[i])));
		mDevice->CreateRenderTargetView(mBackBuffer[i].Get(), nullptr, rtvHeapHandle);
		rtvHeapHandle.Offset(1, mRtvDescriptorSize);
	}
}

void Graphics::CreateDepthStencilView(int clientWidth, int clientHeight) {
	D3D12_RESOURCE_DESC dsvResourceDesc{};
	dsvResourceDesc.Alignment = 0;
	dsvResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	dsvResourceDesc.DepthOrArraySize = 1;
	dsvResourceDesc.Height = clientHeight;
	dsvResourceDesc.Width = clientWidth;
	dsvResourceDesc.MipLevels = 1;
	dsvResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	dsvResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	dsvResourceDesc.Format = mDepthStencilFormat;
	dsvResourceDesc.SampleDesc.Count = 1;
	dsvResourceDesc.SampleDesc.Quality = 0;

	CD3DX12_CLEAR_VALUE optClear{};
	optClear.Format = mDepthStencilFormat;
	optClear.DepthStencil.Depth = 1.0f;
	optClear.DepthStencil.Stencil = 0;

	GFX_THROW_IF_FAILED(mDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&dsvResourceDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&optClear,
		IID_PPV_ARGS(&mDepthStencilBuffer)
	));

	mDevice->CreateDepthStencilView(
		mDepthStencilBuffer.Get(),
		nullptr,
		mDsvHeap->GetCPUDescriptorHandleForHeapStart()
	);
	/*
	mCommandList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(
			mDepthStencilBuffer.Get(),
			D3D12_RESOURCE_STATE_COMMON,
			D3D12_RESOURCE_STATE_DEPTH_WRITE
		)
	);*/
}

void Graphics::CreateViewPortAndScissorRect(int clientWidth, int clientHeight) {
	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;
	mScreenViewport.Width = static_cast<float>(clientWidth);
	mScreenViewport.Height = static_cast<float>(clientHeight);
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;

	mScissorRect = { 0, 0, clientWidth, clientHeight };
}


void Graphics::FlushCommandQueue() {
	++mCurrentFence;
	GFX_THROW_IF_FAILED(mCommandQueue->Signal(mFence.Get(), mCurrentFence));
	if (mFence->GetCompletedValue() < mCurrentFence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
		if (eventHandle == nullptr) {
			GFX_THROW_LAST_EXCEPTION();
		}
		mFence->SetEventOnCompletion(mCurrentFence, eventHandle);
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
}

void Graphics::OnResize(int clientWidth, int clientHeight) {
	assert(mDevice);
	assert(mSwapChain);
	assert(mCommandAllocator);

	FlushCommandQueue();
	GFX_THROW_IF_FAILED(mCommandList->Reset(mCommandAllocator.Get(), mPipelineState.Get()));

	for (int i = 0; i < mBackBufferCount; ++i)
		mBackBuffer[i].Reset();
	mDepthStencilBuffer.Reset();

	GFX_THROW_IF_FAILED(mSwapChain->ResizeBuffers(
		mBackBufferCount,
		clientWidth, clientHeight,
		mBackBufferFormat,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
	));
	mCurrentBackBuffer = 0;

	CreateRenderTargetView();
	CreateDepthStencilView(clientWidth, clientHeight);

	GFX_THROW_IF_FAILED(mCommandList->Close());
	ID3D12CommandList* cmdsLists[]{ mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	FlushCommandQueue();

	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;
	mScreenViewport.Width = static_cast<float>(clientWidth);
	mScreenViewport.Height = static_cast<float>(clientHeight);
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;

	mScissorRect = { 0, 0, clientWidth, clientHeight };
}

void Graphics::BeginFrame() {
	GFX_THROW_IF_FAILED(mCommandAllocator->Reset());
	GFX_THROW_IF_FAILED(mCommandList->Reset(mCommandAllocator.Get(), mPipelineState.Get()));
	mCommandList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(
			mBackBuffer[mCurrentBackBuffer].Get(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET
		)
	);
	mCommandList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(
			mDepthStencilBuffer.Get(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_DEPTH_WRITE
		)
	);
	mCommandList->RSSetViewports(1, &mScreenViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		mRtvHeap->GetCPUDescriptorHandleForHeapStart(),
		mCurrentBackBuffer,
		mRtvDescriptorSize
	);
	mCommandList->ClearRenderTargetView(rtvHandle, DirectX::Colors::LightSteelBlue, 0, nullptr);

	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = mDsvHeap->GetCPUDescriptorHandleForHeapStart();
	mCommandList->ClearDepthStencilView(
		dsvHandle,
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
		1.0f,
		0,
		0,
		nullptr
	);
	mCommandList->OMSetRenderTargets(1, &rtvHandle, true, &dsvHandle);
}

void Graphics::EndFrame() {
	mCommandList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(
			mBackBuffer[mCurrentBackBuffer].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT
		)
	);
	mCommandList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(
			mDepthStencilBuffer.Get(),
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			D3D12_RESOURCE_STATE_PRESENT

		)
	);
	GFX_THROW_IF_FAILED(mCommandList->Close());
	ID3D12CommandList* cmdsLists[]{ mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
	GFX_THROW_IF_FAILED(mSwapChain->Present(0, 0));
	mCurrentBackBuffer = (mCurrentBackBuffer + 1) % mBackBufferCount;
	FlushCommandQueue();
}