#pragma once

#include <directx/d3dx12.h>

#include "DescriptorHeap.h"
#include "GameException.h"

class DepthStencilViewHeap : public DescriptorHeap {
public:
	DepthStencilViewHeap(ID3D12Device& device);
};