#include "bindable/InputLayout.h"

#include "GraphicsAccessor.h"

InputLayout::InputLayout(const std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElementDesc) noexcept
	: mInputElementDesc(inputElementDesc) {}

bool InputLayout::ByPipelineState() const noexcept {
	return true;
}

bool InputLayout::ByGraphicsCommandList() const noexcept {
	return false;
}

void InputLayout::Bind(
	Graphics& graphics,
	D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipelineStateDesc
) const noexcept {
	pipelineStateDesc->InputLayout = {
		mInputElementDesc.data(),
		static_cast<unsigned int>(mInputElementDesc.size())
	};
}