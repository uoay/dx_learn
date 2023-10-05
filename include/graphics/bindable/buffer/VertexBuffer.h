#pragma once

#include <d3dcompiler.h>

#include "Direct3DUtil.h"
#include "GraphicsAccessor.h"
#include "Buffer.h"
#include "GameException.h"

class VertexBuffer : public Buffer {
public:
	template<typename T>
	VertexBuffer(Graphics& graphics, const std::vector<T>& vertices) {
		mView.StrideInBytes = sizeof(T);
		mView.SizeInBytes = static_cast<unsigned int>(vertices.size() * mView.StrideInBytes);

		GFX_THROW_IF_FAILED(D3DCreateBlob(mView.SizeInBytes, &mBufferCpu));
		memcpy(mBufferCpu->GetBufferPointer(), vertices.data(), mView.SizeInBytes);

		Direct3DUtil::CreateDefaultBuffer(
			GraphicsAccessor::GetDevice(graphics), GraphicsAccessor::GetGraphicsCommandList(graphics),
			vertices.data(), mView.SizeInBytes,
			mBufferUploader, mBufferGpu
		);
		mView.BufferLocation = mBufferGpu->GetGPUVirtualAddress();
	}
	virtual void Bind(
		Graphics& graphics,
		D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipelineStateDesc = nullptr
	) const noexcept override {
		GraphicsAccessor::GetGraphicsCommandList(graphics)->IASetVertexBuffers(0, 1, &mView);
	}
private:
	D3D12_VERTEX_BUFFER_VIEW mView;
	Microsoft::WRL::ComPtr<ID3DBlob> mBufferCpu;
	Microsoft::WRL::ComPtr<ID3D12Resource2> mBufferUploader;
	Microsoft::WRL::ComPtr<ID3D12Resource2> mBufferGpu;
};