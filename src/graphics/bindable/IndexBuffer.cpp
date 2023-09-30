#include "bindable/IndexBuffer.h"

#include <d3dcompiler.h>

#include "Direct3DUtil.h"
#include "GraphicsAccessor.h"

IndexBuffer::IndexBuffer(
	const Graphics& graphics,
	const std::vector<uint16_t>& indices,
	DXGI_FORMAT indexFormat
) : mIndexCount(static_cast<unsigned int>(indices.size())) {
	const size_t ibByteSize = mIndexCount * sizeof(std::uint16_t);
	
	GFX_THROW_IF_FAILED(D3DCreateBlob(ibByteSize, &mBufferCpu));
	memcpy(mBufferCpu->GetBufferPointer(), indices.data(), ibByteSize);

	Direct3DUtil::CreateDefaultBuffer(
		GraphicsAccessor::GetDevice(), GraphicsAccessor::GetGraphicsCommandList(graphics),
		indices.data(), ibByteSize,
		mBufferUploader.Get(), mBufferGpu.Get()
	);
	
	mView.BufferLocation = mBufferGpu->GetGPUVirtualAddress();
	mView.Format = indexFormat;
	mView.SizeInBytes = static_cast<unsigned int>(ibByteSize);
}

void IndexBuffer::Bind(
	Graphics& graphics
) const noexcept {
	GraphicsAccessor::GetGraphicsCommandList(graphics)->IASetIndexBuffer(&mView);
}

unsigned int IndexBuffer::GetIndexCount() const noexcept{
	return mIndexCount;
}