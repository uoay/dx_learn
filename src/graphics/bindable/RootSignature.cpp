#include "bindable/RootSignature.h"

#include "GraphicsAccessor.h"
#include "GameException.h"

RootSignature::RootSignature(Graphics& graphics) {
	CD3DX12_ROOT_PARAMETER slotRootParameter[1];

	CD3DX12_DESCRIPTOR_RANGE cbvTable;
	cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	slotRootParameter[0].InitAsDescriptorTable(1, &cbvTable);

	CD3DX12_ROOT_SIGNATURE_DESC rootDesc(1, slotRootParameter, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	Microsoft::WRL::ComPtr<ID3DBlob> serializedRootSig;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
	D3D12_VERSIONED_ROOT_SIGNATURE_DESC versionDesc;
	versionDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_0;
	versionDesc.Desc_1_0 = rootDesc;
	HRESULT hr = D3D12SerializeVersionedRootSignature(&versionDesc, &serializedRootSig, &errorBlob);

	if (errorBlob != nullptr) {
		OutputDebugString(static_cast<wchar_t*>(errorBlob->GetBufferPointer()));
	}
	GFX_THROW_IF_FAILED(hr);

	GFX_THROW_IF_FAILED(GraphicsAccessor::GetDevice(graphics)->CreateRootSignature(
		0,
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(),
		IID_PPV_ARGS(&mRootSignature)
	));
}

bool RootSignature::ByPipelineState() const noexcept {
	return true;
}
bool RootSignature::ByGraphicsCommandList() const noexcept {
	return true;
}

void RootSignature::Bind(
	Graphics& graphics,
	D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipelineStateDesc
) const noexcept {
	if (pipelineStateDesc == nullptr) {
		GraphicsAccessor::GetGraphicsCommandList(graphics)->SetGraphicsRootSignature(mRootSignature.Get());
	}
	else {
		pipelineStateDesc->pRootSignature = mRootSignature.Get();
	}
}