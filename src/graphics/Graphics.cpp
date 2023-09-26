#include "Graphics.h"

#include <sstream>

#include <d3dcompiler.h>

#include "Direct3DUtil.h"
#include "GameException.h"

Graphics::Graphics(
	const HWND hWnd, const int clientWidth, const int clientHeight
) : GraphicsBase(hWnd, clientWidth, clientHeight) {

	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mViewport.Width = static_cast<float>(clientWidth);
	mViewport.Height = static_cast<float>(clientHeight);
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;

	mScissorRect = { 0, 0, clientWidth, clientHeight };
}

void Graphics::OnResize(const int clientWidth, const int clientHeight) {
	GraphicsBase::OnResize(clientWidth, clientHeight);

	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mViewport.Width = static_cast<float>(clientWidth);
	mViewport.Height = static_cast<float>(clientHeight);
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;

	mScissorRect = { 0, 0, clientWidth, clientHeight };
}

void Graphics::Draw() {

	GFX_THROW_IF_FAILED(mCommandAllocator->Reset());
	GFX_THROW_IF_FAILED(mGraphicsCommandList->Reset(mCommandAllocator.Get(), nullptr));
	
	mGraphicsCommandList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(
			mBackBuffer->GetBuffer(mCurrentBackBuffer),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET
			)
	);
	mGraphicsCommandList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(
			mDepthStencilBuffer->GetBuffer(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_DEPTH_WRITE
		)
	);

	mGraphicsCommandList->RSSetViewports(1, &mViewport);
	mGraphicsCommandList->RSSetScissorRects(1, &mScissorRect);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		mBackBuffer->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
		mCurrentBackBuffer,
		GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
	);
	mGraphicsCommandList->ClearRenderTargetView(rtvHandle, DirectX::Colors::LightSteelBlue, 0, nullptr);

	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = mDepthStencilBuffer->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	mGraphicsCommandList->ClearDepthStencilView(
		dsvHandle,
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
		1.0f,
		0,
		0,
		nullptr
	);
	mGraphicsCommandList->OMSetRenderTargets(1, &rtvHandle, true, &dsvHandle);
	mGraphicsCommandList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(
			mBackBuffer->GetBuffer(mCurrentBackBuffer),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT
		)
	);
	mGraphicsCommandList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(
			mDepthStencilBuffer->GetBuffer(),
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			D3D12_RESOURCE_STATE_PRESENT
			
		)
	);

	GFX_THROW_IF_FAILED(mGraphicsCommandList->Close());
	ID3D12CommandList* cmdsLists[]{ mGraphicsCommandList.Get() };
	GetCommandQueue()->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
	GFX_THROW_IF_FAILED(mSwapChain->Present(0, 0));
	mCurrentBackBuffer = (mCurrentBackBuffer + 1) % BackBuffer::sCount;
	FlushCommandQueue();
}