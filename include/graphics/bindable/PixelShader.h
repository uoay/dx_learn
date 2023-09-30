#pragma once

#include "Shader.h"

class PixelShader : public Shader {
public:
	PixelShader(std::string path);
	void Bind(Graphics& graphics) const noexcept override;
};