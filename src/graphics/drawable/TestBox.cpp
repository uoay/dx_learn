#include "drawable/TestBox.h"

#include <d3dcompiler.h>

#include "GameException.h"

TestBox::TestBox(Graphics& graphics) : graphics(graphics) {
	GFX_THROW_IF_FAILED(GraphicsAccessor::GetGraphicsCommandList(graphics)->Reset(GraphicsAccessor::GetCommandAllocator(graphics), nullptr));

	CreateConstantBuffer();
	CreateRootSignature();
	CreateShadersAndInputLayout();
	CreateCube();
	CreatePipeLineStateObject();
	
	GFX_THROW_IF_FAILED(GraphicsAccessor::GetGraphicsCommandList(graphics)->Close());
	ID3D12CommandList* cmdsLists[]{ GraphicsAccessor::GetGraphicsCommandList(graphics) };
	GraphicsAccessor::GetCommandQueue(graphics)->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	graphics.FlushCommandQueue();
}

void TestBox::CreateConstantBuffer() {
	mConstantBuffer = std::make_unique<ConstantBuffer<Direct3DUtil::ObjectConstants>>(graphics, 1);
}

void TestBox::CreateRootSignature() {
	mRootSignature = std::make_unique<RootSignature>(graphics);
}
void TestBox::CreateShadersAndInputLayout() {
	mPixelShader = std::make_unique<PixelShader>(L"PixelShader.cso");
	mVertexShader = std::make_unique<VertexShader>(L"VertexShader.cso");

	mInputLayout = std::make_unique<InputLayout>(std::vector<D3D12_INPUT_ELEMENT_DESC>({
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	}));
}
void TestBox::CreateCube() {
	std::vector<Direct3DUtil::Vertex> vertices{
	Direct3DUtil::Vertex({ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(DirectX::Colors::White) }),
	Direct3DUtil::Vertex({ DirectX::XMFLOAT3(-1.0f, +1.0f, -1.0f), DirectX::XMFLOAT4(DirectX::Colors::Black) }),
	Direct3DUtil::Vertex({ DirectX::XMFLOAT3(+1.0f, +1.0f, -1.0f), DirectX::XMFLOAT4(DirectX::Colors::Red) }),
	Direct3DUtil::Vertex({ DirectX::XMFLOAT3(+1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(DirectX::Colors::Green) }),
	Direct3DUtil::Vertex({ DirectX::XMFLOAT3(-1.0f, -1.0f, +1.0f), DirectX::XMFLOAT4(DirectX::Colors::Blue) }),
	Direct3DUtil::Vertex({ DirectX::XMFLOAT3(-1.0f, +1.0f, +1.0f), DirectX::XMFLOAT4(DirectX::Colors::Yellow) }),
	Direct3DUtil::Vertex({ DirectX::XMFLOAT3(+1.0f, +1.0f, +1.0f), DirectX::XMFLOAT4(DirectX::Colors::Cyan) }),
	Direct3DUtil::Vertex({ DirectX::XMFLOAT3(+1.0f, -1.0f, +1.0f), DirectX::XMFLOAT4(DirectX::Colors::Magenta) })
	};
	std::vector<std::uint16_t> indices = {
		0, 1, 2,
		0, 2, 3,

		4, 6, 5,
		4, 7, 6,

		4, 5, 1,
		4, 1, 0,

		3, 2, 6,
		3, 6, 7,

		1, 5, 6,
		1, 6, 2,

		4, 0, 3,
		4, 3, 7
	};

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Direct3DUtil::Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

	mIndexBuffer = std::make_unique<IndexBuffer>(graphics, indices);
	mVertexBuffer = std::make_unique<VertexBuffer>(graphics, vertices);

	mCube = std::make_unique<Direct3DUtil::MeshGeometry>();
	mCube->name = L"cube";

	Direct3DUtil::SubmeshGeometry submesh;
	submesh.IndexCount = static_cast<UINT>(indices.size());
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;
	mCube->DrawArgs["box"] = submesh;
}

void TestBox::CreatePipeLineStateObject() {
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};
	//psoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
	mRootSignature->Bind(graphics, &psoDesc);

	mInputLayout->Bind(graphics, &psoDesc);
	//psoDesc.pRootSignature = mRootSignature.Get();
	mVertexShader->Bind(graphics, &psoDesc);
	mPixelShader->Bind(graphics, &psoDesc);
	//psoDesc.VS = { reinterpret_cast<BYTE*>(mVsBytecode->GetBufferPointer()), mVsBytecode->GetBufferSize() };
	//psoDesc.PS = { reinterpret_cast<BYTE*>(mPsBytecode->GetBufferPointer()), mPsBytecode->GetBufferSize() };
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleDesc.Quality = 0;
	GFX_THROW_IF_FAILED(GraphicsAccessor::GetDevice(graphics)->CreateGraphicsPipelineState(
		&psoDesc, IID_PPV_ARGS(&graphics.mPipelineState)
	));
}

void TestBox::Draw() {
	mRootSignature->Bind(graphics);
	mIndexBuffer->Bind(graphics);
	mVertexBuffer->Bind(graphics);
	mConstantBuffer->Bind(graphics);
	GraphicsAccessor::GetGraphicsCommandList(graphics)->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	GraphicsAccessor::GetGraphicsCommandList(graphics)->DrawIndexedInstanced(
		mCube->DrawArgs["box"].IndexCount,
		1, 0, 0, 0
	);
}


void TestBox::Update() {
	Direct3DUtil::ObjectConstants objConstants;
	float x = 0.0f;
	float y = 0.0f;
	float z = 5.0f;
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(x, y, z, 1.0f);
	DirectX::XMVECTOR target = DirectX::XMVectorZero();
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX v = DirectX::XMMatrixLookAtLH(pos, target, up);

	DirectX::XMMATRIX p = DirectX::XMMatrixPerspectiveFovLH(0.25f * 3.1416f, static_cast<float>(1024) / 768, 1.0f, 1000.0f);
	DirectX::XMMATRIX WVP_Matrix = v * p;
	DirectX::XMStoreFloat4x4(&objConstants.worldViewProjection, XMMatrixTranspose(WVP_Matrix));
	mConstantBuffer->Update(objConstants);
}