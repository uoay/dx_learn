#include "GraphicsBase.h"

GraphicsBase::GraphicsBase(
	const HWND hWnd,
	const unsigned int clientWidth,
	const unsigned int clientHeight
) : mBackBuffer(), mDepthStencilBuffer() {

	auto device = GetDevice();
	auto commandQueue = GetCommandQueue();

	GFX_THROW_IF_FAILED(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence)));

	GFX_THROW_IF_FAILED(device->CreateCommandAllocator(commandQueue->GetDesc().Type, IID_PPV_ARGS(&mCommandAllocator)));

	GFX_THROW_IF_FAILED(device->CreateCommandList(
		0,
		commandQueue->GetDesc().Type,
		mCommandAllocator.Get(),
		nullptr,
		IID_PPV_ARGS(&mGraphicsCommandList)
	));

	GFX_THROW_IF_FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&mFactory)));

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = clientWidth;
	swapChainDesc.Height = clientHeight;
	swapChainDesc.Format = BackBuffer::sFormat;
	swapChainDesc.Stereo = FALSE;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = BackBuffer::sCount;
	swapChainDesc.Scaling = DXGI_SCALING_NONE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	IDXGISwapChain1* tmpSwapChain = nullptr;
	GFX_THROW_IF_FAILED(mFactory->CreateSwapChainForHwnd(
		commandQueue,
		hWnd,
		&swapChainDesc,
		nullptr,
		nullptr,
		&tmpSwapChain
	));
	GFX_THROW_IF_FAILED(tmpSwapChain->QueryInterface(IID_PPV_ARGS(&mSwapChain)));
	tmpSwapChain->Release();
	
	mBackBuffer = std::make_unique<BackBuffer>(device, mSwapChain.Get());
	
	mDepthStencilBuffer =
		std::make_unique<DepthStencilBuffer>(
			device,
			mGraphicsCommandList.Get(),
			clientWidth, clientHeight
		);

	GFX_THROW_IF_FAILED(mGraphicsCommandList->Close());
}

ID3D12Device10* GraphicsBase::GetDevice() {
	static Microsoft::WRL::ComPtr<ID3D12Device10> sDevice;
	if (sDevice == nullptr) {
		GFX_THROW_IF_FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&sDevice)));
	}
	return sDevice.Get();
}

ID3D12CommandQueue* GraphicsBase::GetCommandQueue() {
	static Microsoft::WRL::ComPtr<ID3D12CommandQueue> sCommandQueue;
	if (sCommandQueue == nullptr) {
		D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
		commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		commandQueueDesc.NodeMask = 0;
		GFX_THROW_IF_FAILED(GetDevice()->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&sCommandQueue)));
	}
	return sCommandQueue.Get();
}

void GraphicsBase::FlushCommandQueue() {
	++mCurrentFence;
	GFX_THROW_IF_FAILED(GetCommandQueue()->Signal(mFence.Get(), mCurrentFence));
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

void GraphicsBase::OnResize(const unsigned int clientWidth, const unsigned int clientHeight) {
	FlushCommandQueue(); 

	GFX_THROW_IF_FAILED(mGraphicsCommandList->Reset(mCommandAllocator.Get(), nullptr));

	mBackBuffer.reset();
	mDepthStencilBuffer.reset();
	
	GFX_THROW_IF_FAILED(mSwapChain->ResizeBuffers(
		BackBuffer::sCount,
		clientWidth, clientHeight,
		BackBuffer::sFormat,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
	));

	mBackBuffer = std::make_unique<BackBuffer>(GetDevice(), mSwapChain.Get());
	mCurrentBackBuffer = 0;
	mDepthStencilBuffer = std::make_unique<DepthStencilBuffer>(
		GetDevice(), mGraphicsCommandList.Get(), clientWidth, clientHeight
	);

	mGraphicsCommandList->Close();
	ID3D12CommandList* cmdsLists[]{ mGraphicsCommandList.Get() };
	GetCommandQueue()->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	FlushCommandQueue();
}

