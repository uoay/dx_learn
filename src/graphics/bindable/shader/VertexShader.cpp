#include "bindable/shader/VertexShader.h"

#include "GraphicsAccessor.h"

VertexShader::VertexShader(std::wstring path) : Shader(path) {}

void VertexShader::Bind(
	Graphics& graphics,
	D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipelineStateDesc
) const noexcept {
	pipelineStateDesc->VS = {
		reinterpret_cast<BYTE*>(mByteCodeBuffer->GetBufferPointer()),
		mByteCodeBuffer->GetBufferSize()
	};
}