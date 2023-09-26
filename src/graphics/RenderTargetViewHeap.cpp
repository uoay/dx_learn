#include "RenderTargetViewHeap.h"

RenderTargetViewHeap::RenderTargetViewHeap(ID3D12Device& device, unsigned int numBackBuffer) {
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NumDescriptors = numBackBuffer;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	heapDesc.NodeMask = 0;
	GFX_THROW_IF_FAILED(device.CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&mHeap)));
}
