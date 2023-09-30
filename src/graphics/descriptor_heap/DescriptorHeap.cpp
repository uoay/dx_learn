#include "descriptor_heap/DescriptorHeap.h"

ID3D12DescriptorHeap* DescriptorHeap::Get() const noexcept {
	return mHeap.Get();
}