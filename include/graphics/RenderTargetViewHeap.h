#pragma once

#include <directx/d3dx12.h>

#include "DescriptorHeap.h"
#include "GameException.h"

class RenderTargetViewHeap : public DescriptorHeap {
public:
	RenderTargetViewHeap(ID3D12Device& device, unsigned int numBackBuffer);
};