#pragma once

#include "Shader.h"

class PixelShader : public Shader {
public:
	PixelShader(std::wstring path);
	virtual void Bind(
		Graphics& graphics,
		D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipelineStateDesc
	)  const noexcept override;
};