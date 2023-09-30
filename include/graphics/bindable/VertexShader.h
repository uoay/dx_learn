#pragma once

#include "Shader.h"

class VertexShader : public Shader {
public:
	VertexShader(std::string path);
	void Bind(Graphics& graphics) const noexcept override;
private:
	//Microsoft::WRL::ComPtr<ID3DBlob> mByteCode;
};