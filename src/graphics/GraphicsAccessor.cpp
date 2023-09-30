#include "GraphicsAccessor.h"

ID3D12Device10* GraphicsAccessor::GetDevice() {
	return Graphics::GetDevice();
}

ID3D12CommandQueue* GraphicsAccessor::GetCommandQueue() {
	return Graphics::GetCommandQueue();
}

ID3D12GraphicsCommandList7* GraphicsAccessor::GetGraphicsCommandList(const Graphics& graphics) {
	return graphics.mGraphicsCommandList.Get();
}

D3D12_GRAPHICS_PIPELINE_STATE_DESC& GraphicsAccessor::GetPipelineStateDesc(Graphics& graphics) {
	return graphics.mPipelineStateDesc;
}