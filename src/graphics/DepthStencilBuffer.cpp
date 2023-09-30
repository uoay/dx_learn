#include "DepthStencilBuffer.h"

#include <directx/d3dx12.h>

#include "GameException.h"

DepthStencilBuffer::DepthStencilBuffer(
	ID3D12Device* device,
	ID3D12GraphicsCommandList* graphicsCommandList,
	const unsigned int clientWidth,
	const unsigned int clientHeight
) : mViewHeap(device) {

	D3D12_RESOURCE_DESC dsvResourceDesc{};
	dsvResourceDesc.Alignment = 0;
	dsvResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	dsvResourceDesc.DepthOrArraySize = 1;
	dsvResourceDesc.Width = clientWidth;
	dsvResourceDesc.Height = clientHeight;
	dsvResourceDesc.MipLevels = 1;
	dsvResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	dsvResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	dsvResourceDesc.Format = sFormat;
	dsvResourceDesc.SampleDesc.Count = 1;
	dsvResourceDesc.SampleDesc.Quality = 0;

	CD3DX12_CLEAR_VALUE optClear{};
	optClear.Format = sFormat;
	optClear.DepthStencil.Depth = 1.0f;
	optClear.DepthStencil.Stencil = 0;

	GFX_THROW_IF_FAILED(device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&dsvResourceDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&optClear,
		IID_PPV_ARGS(&mBuffer)
	));

	device->CreateDepthStencilView(
		mBuffer.Get(),
		nullptr,
		mViewHeap.Get()->GetCPUDescriptorHandleForHeapStart()
	);
	/*
	graphicsCommandList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(
			mBuffer.Get(),
			D3D12_RESOURCE_STATE_COMMON,
			D3D12_RESOURCE_STATE_DEPTH_WRITE
		)
	);*/
}

ID3D12Resource2* DepthStencilBuffer::GetBuffer() {
	return mBuffer.Get();
}

DepthStencilViewHeap& DepthStencilBuffer::GetDescriptorHeap() {
	return mViewHeap;
}