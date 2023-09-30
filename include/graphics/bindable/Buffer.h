#pragma once

#include <directx/d3dx12.h>

#include "Bindable.h"

class Buffer : public Bindable {
protected:
	Microsoft::WRL::ComPtr<ID3DBlob> mBufferCpu;
	Microsoft::WRL::ComPtr<ID3D12Resource2> mBufferUploader;
	Microsoft::WRL::ComPtr<ID3D12Resource2> mBufferGpu;
};