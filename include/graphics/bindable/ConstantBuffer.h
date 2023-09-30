#pragma once

#include "../descriptor_heap/ConstantBufferViewHeap.h"
#include "../Direct3DUtil.h"
#include "../GraphicsAccessor.h"
#include "Buffer.h"

template<typename T>
class ConstantBuffer : public Buffer {
public:
	ConstantBuffer(Graphics& graphics, const T constants) : mViewHeap(GraphicsAccessor::GetDevice()) {
		mElementByteSize = Direct3DUtil::CaculateConstantBufferByteSize(sizeof(T));

		GFX_THROW_IF_FAILED(GraphicsAccessor::GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(mElementByteSize * elementCount),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mBuffer)
		));
	}
	void Update(const T constants) {
		/*
		BYTE* mMappedData = nullptr;
		GFX_THROW_IF_FAILED(mBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData)));
		memcpy(mMappedData, &constants, sizeof(constants));
		GFX_THROW_IF_FAILED(mBuffer->Unmap(,))*/
		
	}
	void Bind(Graphics& graphics) const noexcept override {
		GraphicsAccessor::GetGraphicsCommandList(graphics)->SetDescriptorHeaps()
	}
private:
	Microsoft::WRL::ComPtr<ID3D12Resource2> mBuffer;
	ConstantBufferViewHeap mViewHeap;
};