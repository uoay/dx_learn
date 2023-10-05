#pragma once

#include <directx/d3d12.h>

#include "Graphics.h"

class Bindable {
public:
	virtual void Bind(
		Graphics& graphics,
		D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipelineStateDesc
	) const noexcept = 0;
	virtual bool ByPipelineState() const noexcept = 0;
	virtual bool ByGraphicsCommandList() const noexcept = 0;
	virtual ~Bindable() = default;
};