#pragma once

#include <directx/d3dx12.h>

#include "bindable/Bindable.h"

class Buffer : public Bindable {
public:
	virtual bool ByPipelineState() const noexcept override;
	virtual bool ByGraphicsCommandList() const noexcept override;
	virtual ~Buffer() noexcept = 0;
};