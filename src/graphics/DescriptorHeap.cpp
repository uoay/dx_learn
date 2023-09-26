#include "DescriptorHeap.h"

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::GetCPUDescriptorHandleForHeapStart() {
	return mHeap->GetCPUDescriptorHandleForHeapStart();
}