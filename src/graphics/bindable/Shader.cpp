#include "bindable/Shader.h"

#include <fstream>

#include <d3dcompiler.h>

#include "GameException.h"

Shader::Shader(const std::string path) {
	std::ifstream shader("../../shader" + path, std::ifstream::binary | std::ifstream::in);
	if (!shader.is_open()) {
		GFX_THROW_EXCEPTION(L"无法打开shader");
	}
	shader.seekg(std::ifstream::end);
	size_t byteSize = shader.tellg();
	D3DCreateBlob(byteSize, &mByteCode);
	shader.seekg(std::ifstream::beg);
	shader.read(static_cast<char*>(mByteCode->GetBufferPointer()), byteSize);
}

ID3DBlob* Shader::GetBytecode() const noexcept{
	return mByteCode.Get();
}