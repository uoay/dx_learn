#pragma once

#include <dxgi1_6.h>

#include "descriptor_heap/RenderTargetViewHeap.h"

class BackBuffer {
public:
	BackBuffer(ID3D12Device* device, IDXGISwapChain* swapChain);
	ID3D12Resource2* GetBuffer(const unsigned int i) const noexcept;
	RenderTargetViewHeap& GetDescriptorHeap() noexcept;
public:
	static constexpr DXGI_FORMAT sFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	static constexpr unsigned int sCount = 2;
private:
	Microsoft::WRL::ComPtr<ID3D12Resource2> mBuffer[sCount];
	RenderTargetViewHeap mViewHeap;
};