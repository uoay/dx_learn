#include "bindable/PixelShader.h"

#include "GraphicsAccessor.h"

PixelShader::PixelShader(std::string path) : Shader(path) {}

void PixelShader::Bind(Graphics& graphics) const noexcept {
	GraphicsAccessor::GetPipelineStateDesc(graphics).PS = {
		reinterpret_cast<BYTE*>(mByteCode->GetBufferPointer()),
		mByteCode->GetBufferSize()
	};
}