#include "bindable/buffer/IndexBuffer.h"

#include <d3dcompiler.h>

#include "Direct3DUtil.h"
#include "GraphicsAccessor.h"
#include "GameException.h"

IndexBuffer::IndexBuffer(
	Graphics& graphics,
	const std::vector<uint16_t>& indices,
	DXGI_FORMAT indexFormat
) : mIndexCount(static_cast<unsigned int>(indices.size())) {
	mView.SizeInBytes = static_cast<unsigned int>(mIndexCount * sizeof(std::uint16_t));
	
	GFX_THROW_IF_FAILED(D3DCreateBlob(mView.SizeInBytes, &mBufferCpu));
	memcpy(mBufferCpu->GetBufferPointer(), indices.data(), mView.SizeInBytes);

	Direct3DUtil::CreateDefaultBuffer(
		GraphicsAccessor::GetDevice(graphics), GraphicsAccessor::GetGraphicsCommandList(graphics),
		indices.data(), mView.SizeInBytes,
		mBufferUploader, mBufferGpu
	);
	
	mView.BufferLocation = mBufferGpu->GetGPUVirtualAddress();
	mView.Format = indexFormat;
}

void IndexBuffer::Bind(
	Graphics& graphics,
	D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipelineStateDesc
) const noexcept {
	GraphicsAccessor::GetGraphicsCommandList(graphics)->IASetIndexBuffer(&mView);
}

unsigned int IndexBuffer::GetIndexCount() const noexcept{
	return mIndexCount;
}