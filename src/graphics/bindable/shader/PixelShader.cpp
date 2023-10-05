#include "bindable/shader/PixelShader.h"

#include "GraphicsAccessor.h"

PixelShader::PixelShader(std::wstring path) : Shader(path) {}

void PixelShader::Bind(
	Graphics& graphics,
	D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipelineStateDesc
) const noexcept {
	pipelineStateDesc->PS = {
		reinterpret_cast<BYTE*>(mByteCodeBuffer->GetBufferPointer()),
		mByteCodeBuffer->GetBufferSize()
	};
}