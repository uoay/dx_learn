#pragma once

#include <vector>

#include "Bindable.h"

class InputLayout : public Bindable {
public:
	InputLayout(const std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElementDesc) noexcept;
	virtual bool ByPipelineState() const noexcept override;
	virtual bool ByGraphicsCommandList() const noexcept override;
	virtual void Bind(
		Graphics& graphics,
		D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipelineStateDesc
	) const noexcept override;
private:
	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputElementDesc;
};