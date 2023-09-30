#pragma once

#include "Graphics.h"

class GraphicsAccessor {
public:
	static ID3D12Device10* GetDevice();
	static ID3D12CommandQueue* GetCommandQueue();
	static ID3D12GraphicsCommandList7* GetGraphicsCommandList(const Graphics& graphics);
	static D3D12_GRAPHICS_PIPELINE_STATE_DESC& GetPipelineStateDesc(Graphics& graphics);
};