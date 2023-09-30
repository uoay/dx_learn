#pragma once

#include <memory>

#include "BackBuffer.h"
#include "DepthStencilBuffer.h"
#include "GameException.h"

class GraphicsBase {
public:
	GraphicsBase(const HWND hWnd, const unsigned int clientWidth, const unsigned int clientHeight);
	virtual void Draw() = 0;
protected:
	static ID3D12Device10* GetDevice();
	static ID3D12CommandQueue* GetCommandQueue();
	void FlushCommandQueue();
	void OnResize(const unsigned int clientWidth, const unsigned int clientHeight);
protected:
	Microsoft::WRL::ComPtr<ID3D12Fence1> mFence;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mCommandAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> mGraphicsCommandList;
	Microsoft::WRL::ComPtr<IDXGIFactory7> mFactory;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> mSwapChain;

	std::unique_ptr<BackBuffer> mBackBuffer;
	std::unique_ptr<DepthStencilBuffer> mDepthStencilBuffer;

	unsigned int mCurrentBackBuffer = 0;
	uint64_t mCurrentFence = 0;
	D3D12_VIEWPORT mViewport;
	D3D12_RECT mScissorRect;
};