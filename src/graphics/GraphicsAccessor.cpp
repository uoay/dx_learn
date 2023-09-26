#include "GraphicsAccessor.h"

ID3D12Device* GraphicsAccessor::GetDevice() {
	return Graphics::GetDevice();
}

ID3D12CommandQueue* GraphicsAccessor::GetCommandQueue() {
	return Graphics::GetCommandQueue();
}

ID3D12GraphicsCommandList* GraphicsAccessor::GetGraphicsCommandList(const Graphics& graphics) {
	return graphics.mGraphicsCommandList.Get();
}