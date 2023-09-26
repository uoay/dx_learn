#pragma once

#include <directx/d3dx12.h>
#include <wrl.h>

class DescriptorHeap {
public:
	virtual ~DescriptorHeap() = default;
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandleForHeapStart();
protected:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mHeap;
};