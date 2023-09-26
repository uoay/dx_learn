#pragma once

#include <directx/d3d12.h>

#include "Graphics.h"

class Bindable {
public:
	virtual void Bind(const Graphics& graphics) const noexcept = 0;
	virtual ~Bindable() = default;
};