#pragma once

#include "DepthStencilViewHeap.h"

class DepthStencilBuffer {
public:
	DepthStencilBuffer(
		ID3D12Device& device,
		ID3D12GraphicsCommandList& graphicsCommandList,
		const unsigned int clientWidth,
		const unsigned int clientHeight
	);
	ID3D12Resource2* GetBuffer();
	DepthStencilViewHeap* GetDescriptorHeap();
private:
	static constexpr DXGI_FORMAT sFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	Microsoft::WRL::ComPtr<ID3D12Resource2> mBuffer;
	std::unique_ptr<DepthStencilViewHeap> mViewHeap;
};