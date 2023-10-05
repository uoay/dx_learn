#pragma once

#include <directx/d3dx12.h>

#include "Bindable.h"
#include "Graphics.h"

class RootSignature : public Bindable {
public:
	RootSignature(Graphics& graphics);
	virtual bool ByPipelineState() const noexcept override;
	virtual bool ByGraphicsCommandList() const noexcept override;
	virtual void Bind(
		Graphics& graphics,
		D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipelineStateDesc = nullptr
	) const noexcept override;
private:
	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature;
};