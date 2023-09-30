#pragma once

#include <directx/d3d12.h>
#include <wrl/client.h>

class DescriptorHeap {
public:
	virtual ~DescriptorHeap() = default;
	ID3D12DescriptorHeap* Get() const noexcept;
protected:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mHeap;
};