#pragma once

#include "directx/d3d12.h"

#include "Direct3DUtil.h"
#include "GameException.h"

template <typename T>
class UploadBuffer {
public:
	UploadBuffer(ID3D12Device* device, unsigned int elementCount,size_t elementByteSize) : mElementByteSize(elementByteSize) {
		GFX_THROW_IF_FAILED(device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(mElementByteSize * elementCount),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mBuffer)
		));
		GFX_THROW_IF_FAILED(mBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData)));
	}
	UploadBuffer(const UploadBuffer& uploadBuffer) = delete;
	UploadBuffer& operator=(const UploadBuffer& uploadBuffer) = delete;
	~UploadBuffer() {
		if (mBuffer != nullptr) {
			mBuffer->Unmap(0, nullptr);
		}
		mMappedData = nullptr;
	}
	ID3D12Resource2* GetResource() {
		return mBuffer.Get();
	}
	void CopyData(int elementIndex, const T& data) {
		memcpy(&mMappedData[elementIndex * mElementByteSize], &data, sizeof(T));
	}
private:
	Microsoft::WRL::ComPtr<ID3D12Resource2> mBuffer;
	BYTE* mMappedData = nullptr;
	size_t mElementByteSize;
};