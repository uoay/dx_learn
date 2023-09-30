#pragma once

#include "DescriptorHeap.h"

class DepthStencilViewHeap : public DescriptorHeap {
public:
	DepthStencilViewHeap(ID3D12Device* device);
};