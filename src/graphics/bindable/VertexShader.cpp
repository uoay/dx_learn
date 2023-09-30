#include "bindable/VertexShader.h"

#include "GraphicsAccessor.h"

VertexShader::VertexShader(std::string path) : Shader(path) {}

void VertexShader::Bind(Graphics& graphics) const noexcept {
	GraphicsAccessor::GetPipelineStateDesc(graphics).VS = {
		reinterpret_cast<BYTE*>(mByteCode->GetBufferPointer()),
		mByteCode->GetBufferSize()
	};
}