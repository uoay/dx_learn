#pragma once

#include "DescriptorHeap.h"

class ConstantBufferViewHeap : public DescriptorHeap {
public:
	ConstantBufferViewHeap(ID3D12Device* device);
};