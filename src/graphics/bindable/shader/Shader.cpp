#include "bindable/shader/Shader.h"

#include <fstream>

#include <d3dcompiler.h>

#include "GameException.h"

Shader::Shader(std::wstring path) { //path相对于输出目录里的shader文件夹
	std::wstring p = L"../shader/" + path;
	D3DReadFileToBlob(p.c_str(), &mByteCodeBuffer);
	/*Failed
	std::ifstream shader("../shader/" + path, std::ifstream::binary | std::ifstream::in);
	if (!shader.is_open()) {
		GFX_THROW_EXCEPTION(L"无法打开shader");
	}
	shader.seekg(std::ifstream::end);
	size_t byteSize = shader.tellg();
	D3DCreateBlob(byteSize, &mByteCode);
	shader.seekg(std::ifstream::beg);
	shader.read(static_cast<char*>(mByteCode->GetBufferPointer()), byteSize);
	*/
}
bool Shader::ByPipelineState() const noexcept {
	return true;
}
bool Shader::ByGraphicsCommandList() const noexcept {
	return false;
}

ID3DBlob* Shader::GetByteCodeBuffer() const noexcept{
	return mByteCodeBuffer.Get();
}