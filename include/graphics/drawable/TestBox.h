#pragma once

#include "drawable/Drawable.h"
#include "GraphicsAccessor.h"
#include "bindable/RootSignature.h"
#include "bindable/buffer/IndexBuffer.h"
#include "bindable/buffer/VertexBuffer.h"
#include "bindable/buffer/ConstantBuffer.h"
#include "bindable/InputLayout.h"
#include "bindable/shader/PixelShader.h"
#include "bindable/shader/VertexShader.h"

class TestBox : Drawable {
public:
	TestBox(Graphics& graphics);
	void CreateConstantBuffer();
	void CreateRootSignature();
	void CreateShadersAndInputLayout();
	void CreateCube();
	void CreatePipeLineStateObject();
	virtual void Draw();
	void Update();
public:
	Graphics& graphics;
	std::unique_ptr<RootSignature> mRootSignature;
	std::unique_ptr<IndexBuffer> mIndexBuffer;
	std::unique_ptr<VertexBuffer> mVertexBuffer;
	std::unique_ptr<ConstantBuffer<Direct3DUtil::ObjectConstants>> mConstantBuffer;
	std::unique_ptr<InputLayout> mInputLayout;
	std::unique_ptr<PixelShader> mPixelShader;
	std::unique_ptr<VertexShader> mVertexShader;
	//D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;
	//D3D12_INDEX_BUFFER_VIEW mIndexBufferView;
	//Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mCbvHeap;
	//std::unique_ptr<UploadBuffer<Direct3DUtil::ObjectConstants>> mObjectContantsBuffer;
	//Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature;
	//Microsoft::WRL::ComPtr<ID3DBlob> mVsBytecode;
	//Microsoft::WRL::ComPtr<ID3DBlob> mPsBytecode;
	//std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;
	//Microsoft::WRL::ComPtr<ID3D12PipelineState> mPSO = nullptr;

	DirectX::XMFLOAT4X4 mWorld = MathUtil::IdentityMatrix4x4();
	DirectX::XMFLOAT4X4 mView = MathUtil::IdentityMatrix4x4();
	DirectX::XMFLOAT4X4 mProj = MathUtil::IdentityMatrix4x4();
	std::unique_ptr<Direct3DUtil::MeshGeometry> mCube;
};