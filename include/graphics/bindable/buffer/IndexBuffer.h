#pragma once

#include "Buffer.h"
#include "Graphics.h"

class IndexBuffer : public Buffer {
public:
	IndexBuffer(
		Graphics& graphics,
		const std::vector<uint16_t>& indices,
		DXGI_FORMAT indexFormat = DXGI_FORMAT_R16_UINT
	);
	virtual void Bind(
		Graphics& graphics,
		D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipelineStateDesc = nullptr
	) const noexcept override;
	unsigned int GetIndexCount() const noexcept;
private:
	const unsigned int mIndexCount;
	D3D12_INDEX_BUFFER_VIEW mView;
	Microsoft::WRL::ComPtr<ID3DBlob> mBufferCpu;
	Microsoft::WRL::ComPtr<ID3D12Resource2> mBufferUploader;
	Microsoft::WRL::ComPtr<ID3D12Resource2> mBufferGpu;
};