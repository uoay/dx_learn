#include "GraphicsAccessor.h"

ID3D12Device10* GraphicsAccessor::GetDevice(Graphics& graphics) {
	return graphics.mDevice.Get();
}

ID3D12CommandQueue* GraphicsAccessor::GetCommandQueue(Graphics& graphics) {
	return graphics.mCommandQueue.Get();
}

ID3D12GraphicsCommandList7* GraphicsAccessor::GetGraphicsCommandList(Graphics& graphics) {
	return graphics.mCommandList.Get();
}

ID3D12CommandAllocator* GraphicsAccessor::GetCommandAllocator(Graphics& graphics) {
	return graphics.mCommandAllocator.Get();
}

Microsoft::WRL::ComPtr<ID3D12PipelineState>& GetPipelineState(Graphics& graphics) {
	return graphics.mPipelineState;
}