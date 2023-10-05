#pragma once

#include "Shader.h"

class VertexShader : public Shader {
public:
	VertexShader(std::wstring path);
	void Bind(
		Graphics& graphics,
		D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipelineStateDesc
	) const noexcept override;
};