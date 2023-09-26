#include "DepthStencilViewHeap.h"

DepthStencilViewHeap::DepthStencilViewHeap(ID3D12Device& device) {
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	heapDesc.NumDescriptors = 1;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	heapDesc.NodeMask = 0;
	GFX_THROW_IF_FAILED(device.CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&mHeap)));
}