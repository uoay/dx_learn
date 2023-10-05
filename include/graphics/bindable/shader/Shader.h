#pragma once

#include <wrl/client.h>

#include "bindable/Bindable.h"

class Shader : public Bindable {
public:
	Shader(std::wstring path);
	virtual bool ByPipelineState() const noexcept override;
	virtual bool ByGraphicsCommandList() const noexcept override;
	ID3DBlob* GetByteCodeBuffer() const noexcept;
protected:
	Microsoft::WRL::ComPtr<ID3DBlob> mByteCodeBuffer;
};