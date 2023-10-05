#pragma once

#include "Graphics.h"

class GraphicsAccessor {
public:
	static ID3D12Device10* GetDevice(Graphics& graphics);
	static ID3D12CommandQueue* GetCommandQueue(Graphics& graphics);
	static ID3D12GraphicsCommandList7* GetGraphicsCommandList(Graphics& graphics);
	static ID3D12CommandAllocator* GetCommandAllocator(Graphics& graphics);
	static Microsoft::WRL::ComPtr<ID3D12PipelineState>& GetPipelineState(Graphics& graphics);
};