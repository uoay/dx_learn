#pragma once

#include "descriptor_heap/DescriptorHeap.h"

class RenderTargetViewHeap : public DescriptorHeap {
public:
	RenderTargetViewHeap(ID3D12Device* device, unsigned int numBackBuffer);
};