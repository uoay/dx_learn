#pragma once

#include <wrl/client.h>

#include "Bindable.h"

class Shader : public Bindable {
public:
	Shader(const std::string path);
	//virtual void Bind(const Graphics& graphics) const noexcept = 0;
	ID3DBlob* GetBytecode() const noexcept;
protected:
	Microsoft::WRL::ComPtr<ID3DBlob> mByteCode;
};