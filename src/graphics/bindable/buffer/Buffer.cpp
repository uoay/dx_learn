#include "bindable/buffer/Buffer.h"

bool Buffer::ByPipelineState() const noexcept {
	return false;
}

bool Buffer::ByGraphicsCommandList() const noexcept {
	return true;
}

Buffer::~Buffer() {}