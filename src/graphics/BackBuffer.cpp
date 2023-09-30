#include "BackBuffer.h"

#include <directx/d3dx12.h>

BackBuffer::BackBuffer(ID3D12Device* device, IDXGISwapChain* swapChain)
	: mViewHeap(device, sCount) {

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(mViewHeap.Get()->GetCPUDescriptorHandleForHeapStart());

	for (int i = 0; i < sCount; ++i) {
		swapChain->GetBuffer(i, IID_PPV_ARGS(&mBuffer[i]));
		device->CreateRenderTargetView(mBuffer[i].Get(), nullptr, rtvHeapHandle);
		rtvHeapHandle.Offset(1, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	}
}

ID3D12Resource2* BackBuffer::GetBuffer(const unsigned int i) const noexcept {
	return mBuffer[i].Get();
}

RenderTargetViewHeap& BackBuffer::GetDescriptorHeap() noexcept {
	return mViewHeap;
}