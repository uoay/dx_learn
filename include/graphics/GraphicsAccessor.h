#pragma once

#include "Graphics.h"

class GraphicsAccessor {
public:
	static ID3D12Device* GetDevice();
	static ID3D12CommandQueue* GetCommandQueue();
	static ID3D12GraphicsCommandList* GetGraphicsCommandList(const Graphics& graphics);
	static D3D12_GRAPHICS_PIPELINE_STATE_DESC& GetGraphicspipelineStateDesc(const Graphics& graphics);
};