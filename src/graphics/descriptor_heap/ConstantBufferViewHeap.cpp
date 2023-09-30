#include "descriptor_heap/ConstantBufferViewHeap.h"

#include "GameException.h"

ConstantBufferViewHeap::ConstantBufferViewHeap(ID3D12Device* device) {
	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc{};
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvHeapDesc.NumDescriptors = 1;
	cbvHeapDesc.NodeMask = 0;
	GFX_THROW_IF_FAILED(device->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&mHeap)));
}