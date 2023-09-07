#pragma once

#include "Graphics.h"
#include "UploadBuffer.h"

class Geometry : public Graphics {
public:
	Geometry(HWND hWnd, int clientHeight, int clientWidth);
	virtual void Draw() override;
	virtual void Update(int clientHeigh, int clientWidtht);

	virtual void OnResize(int clientHeight, int clientWidth) override;
private:
	void CreateConstantBufferViewDescriptorHeap();
	void CreateConstantBuffer();
	void CreateRootSignature();
	void CreateShadersAndInputLayout();
	void CreateCube();
	void CreatePipeLineStateObject();
	
protected:
	D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;
	D3D12_INDEX_BUFFER_VIEW mIndexBufferView;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mCbvHeap;
	std::unique_ptr<UploadBuffer<Direct3DUtil::ObjectConstants>> mObjectContantsBuffer;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature;
	Microsoft::WRL::ComPtr<ID3DBlob> mVsBytecode;
	Microsoft::WRL::ComPtr<ID3DBlob> mPsBytecode;
	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mPSO = nullptr;

	DirectX::XMFLOAT4X4 mWorld = MathUtil::IdentityMatrix4x4();
	DirectX::XMFLOAT4X4 mView = MathUtil::IdentityMatrix4x4();
	DirectX::XMFLOAT4X4 mProj = MathUtil::IdentityMatrix4x4();
private:
	std::unique_ptr<Direct3DUtil::MeshGeometry> mCube;
};