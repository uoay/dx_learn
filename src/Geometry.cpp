#include "Geometry.h"

#include <d3dcompiler.h>

Geometry::Geometry(HWND hWnd, int clientWidth, int clientHeight) : Graphics(hWnd, clientWidth, clientHeight) {
	THROW_IF_FAILED(mCommandList->Reset(mCommandAllocator.Get(), nullptr));

	CreateConstantBufferViewDescriptorHeap();
	CreateConstantBuffer();
	CreateRootSignature();
	CreateShadersAndInputLayout();
	CreateVertexBufferAndBufferView();
	CreateIndexBufferAndBufferView();
	CreateCube();
	CreatePipeLineStateObject();

	THROW_IF_FAILED(mCommandList->Close());
	ID3D12CommandList* cmdsLists[]{ mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	FlushCommandQueue();
}

void Geometry::CreateVertexBufferAndBufferView() {

}

void Geometry::CreateIndexBufferAndBufferView() {

}

void Geometry::CreateConstantBufferViewDescriptorHeap() {
	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc{};
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvHeapDesc.NumDescriptors = 1;
	cbvHeapDesc.NodeMask = 0;
	THROW_IF_FAILED(mDevice->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&mCbvHeap)));
}

void Geometry::CreateConstantBuffer() {
	mObjectContantsBuffer = std::make_unique<UploadBuffer<Direct3DUtil::ObjectConstants>>(mDevice.Get(), 1, true);

	D3D12_GPU_VIRTUAL_ADDRESS address = mObjectContantsBuffer->GetResource()->GetGPUVirtualAddress();

	UINT objCBByteSize = Direct3DUtil::CaculateConstantBufferByteSize(sizeof(Direct3DUtil::ObjectConstants));

	//int index = 0;
	//address += index * objCBByteSize;

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = address;
	cbvDesc.SizeInBytes = static_cast<UINT>(Direct3DUtil::CaculateConstantBufferByteSize(sizeof(Direct3DUtil::ObjectConstants)));

	mDevice->CreateConstantBufferView(&cbvDesc, mCbvHeap->GetCPUDescriptorHandleForHeapStart());
}

void Geometry::CreateRootSignature() {
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
		OutputDebugStringA(static_cast<char*>(errorBlob->GetBufferPointer()));
	}
	THROW_IF_FAILED(hr);

	THROW_IF_FAILED(mDevice->CreateRootSignature(
		0,
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(),
		IID_PPV_ARGS(&mRootSignature)
	));
}

void Geometry::CreateShadersAndInputLayout() {
	mVsBytecode = Direct3DUtil::CompileShader(L"C:\\Users\\dista\\Source\\Repos\\uoay\\Game\\shader\\Shader.hlsl", nullptr, "VS", "vs_5_0");
	mPsBytecode = Direct3DUtil::CompileShader(L"C:\\Users\\dista\\Source\\Repos\\uoay\\Game\\shader\\Shader.hlsl", nullptr, "PS", "ps_5_0");

	mInputLayout = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
}

void Geometry::CreateCube() {
	std::array<Direct3DUtil::Vertex, 8> vertices {
		Direct3DUtil::Vertex({ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(DirectX::Colors::White) }),
		Direct3DUtil::Vertex({ DirectX::XMFLOAT3(-1.0f, +1.0f, -1.0f), DirectX::XMFLOAT4(DirectX::Colors::Black) }),
		Direct3DUtil::Vertex({ DirectX::XMFLOAT3(+1.0f, +1.0f, -1.0f), DirectX::XMFLOAT4(DirectX::Colors::Red) }),
		Direct3DUtil::Vertex({ DirectX::XMFLOAT3(+1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(DirectX::Colors::Green) }),
		Direct3DUtil::Vertex({ DirectX::XMFLOAT3(-1.0f, -1.0f, +1.0f), DirectX::XMFLOAT4(DirectX::Colors::Blue) }),
		Direct3DUtil::Vertex({ DirectX::XMFLOAT3(-1.0f, +1.0f, +1.0f), DirectX::XMFLOAT4(DirectX::Colors::Yellow) }),
		Direct3DUtil::Vertex({ DirectX::XMFLOAT3(+1.0f, +1.0f, +1.0f), DirectX::XMFLOAT4(DirectX::Colors::Cyan) }),
		Direct3DUtil::Vertex({ DirectX::XMFLOAT3(+1.0f, -1.0f, +1.0f), DirectX::XMFLOAT4(DirectX::Colors::Magenta) })
	};
	std::array<std::uint16_t, 36> indices = {
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

	mCube = std::make_unique<Direct3DUtil::MeshGeometry>();
	mCube->name = L"cube";

	THROW_IF_FAILED(D3DCreateBlob(vbByteSize, &mCube->VertexBufferCPU));
	CopyMemory(mCube->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);
	THROW_IF_FAILED(D3DCreateBlob(ibByteSize, &mCube->IndexBufferCPU));
	CopyMemory(mCube->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	Direct3DUtil::CreateDefaultBuffer(
		mDevice, mCommandList,
		vertices.data(), vbByteSize, mCube->VertexBufferUploader,
		mCube->VertexBufferGPU
	);
	Direct3DUtil::CreateDefaultBuffer(
		mDevice, mCommandList,
		indices.data(), ibByteSize, mCube->IndexBufferUploader,
		mCube->IndexBufferGPU
	);

	mCube->VertexByteStride = sizeof(Direct3DUtil::Vertex);
	mCube->VertexBufferByteSize = vbByteSize;
	mCube->IndexBufferByteSize = ibByteSize;
	mCube->IndexFormat = DXGI_FORMAT_R16_UINT;


	Direct3DUtil::SubmeshGeometry submesh;
	submesh.IndexCount = static_cast<UINT>(indices.size());
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;
	mCube->DrawArgs["box"] = submesh;

	mVertexBufferView.BufferLocation = mCube->VertexBufferGPU->GetGPUVirtualAddress();
	mVertexBufferView.SizeInBytes = mCube->VertexBufferByteSize;
	mVertexBufferView.StrideInBytes = mCube->VertexByteStride;

	mIndexBufferView.BufferLocation = mCube->IndexBufferGPU->GetGPUVirtualAddress();
	mIndexBufferView.Format = mCube->IndexFormat;
	mIndexBufferView.SizeInBytes = mCube->IndexBufferByteSize;
}

void Geometry::CreatePipeLineStateObject() {
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
	psoDesc.pRootSignature = mRootSignature.Get();
	psoDesc.VS = { reinterpret_cast<BYTE*>(mVsBytecode->GetBufferPointer()), mVsBytecode->GetBufferSize() };
	psoDesc.PS = { reinterpret_cast<BYTE*>(mPsBytecode->GetBufferPointer()), mPsBytecode->GetBufferSize() };
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = mBackBufferFormat;
	psoDesc.DSVFormat = mDepthStencilFormat;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleDesc.Quality = 0;

	THROW_IF_FAILED(mDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPSO)));
}

void Geometry::Draw() {
	THROW_IF_FAILED(mCommandAllocator->Reset());
	THROW_IF_FAILED(mCommandList->Reset(mCommandAllocator.Get(), mPSO.Get()));

	mCommandList->RSSetViewports(1, &mScreenViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);

	mCommandList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(
			mBackBuffer[mCurrentBackBuffer].Get(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET
		)
	);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		mRtvHeap->GetCPUDescriptorHandleForHeapStart(),
		mCurrentBackBuffer,
		mRtvDescriptorSize
	);
	mCommandList->ClearRenderTargetView(rtvHandle, DirectX::Colors::LightSteelBlue, 0, nullptr);
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = mDsvHeap->GetCPUDescriptorHandleForHeapStart();
	mCommandList->ClearDepthStencilView(
		dsvHandle,
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
		1.0f,
		0,
		0,
		nullptr
	);
	mCommandList->OMSetRenderTargets(1, &rtvHandle, true, &dsvHandle);

	ID3D12DescriptorHeap* descriptorHeaps[] = { mCbvHeap.Get() };
	mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	mCommandList->SetGraphicsRootSignature(mRootSignature.Get());
	mCommandList->IASetVertexBuffers(0, 1, &mVertexBufferView);
	mCommandList->IASetIndexBuffer(&mIndexBufferView);
	mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mCommandList->SetGraphicsRootDescriptorTable(0, mCbvHeap->GetGPUDescriptorHandleForHeapStart());

	mCommandList->DrawIndexedInstanced(
		mCube->DrawArgs["box"].IndexCount,
		1, 0, 0, 0
	);

	mCommandList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(
			mBackBuffer[mCurrentBackBuffer].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT
		)
	);

	THROW_IF_FAILED(mCommandList->Close());

	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	THROW_IF_FAILED(mSwapChain->Present(0, 0));
	mCurrentBackBuffer = (mCurrentBackBuffer + 1) % mBackBufferCount;

	FlushCommandQueue();
}

void Geometry::Update() {
	Direct3DUtil::ObjectConstants objConstants;
	float x = 0.0f;
	float y = 0.0f;
	float z = 5.0f;
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(x, y, z, 1.0f);
	DirectX::XMVECTOR target = DirectX::XMVectorZero();
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX v = DirectX::XMMatrixLookAtLH(pos, target, up);
	DirectX::XMMATRIX p = DirectX::XMMatrixPerspectiveFovLH(0.25f * 3.1416f, 1280.0f / 720.0f, 1.0f, 1000.0f);
	DirectX::XMMATRIX w = DirectX::XMLoadFloat4x4(&mWorld);
	DirectX::XMMATRIX WVP_Matrix = v * p;
	DirectX::XMStoreFloat4x4(&objConstants.worldViewProjection, XMMatrixTranspose(WVP_Matrix));
	mObjectContantsBuffer->CopyData(0, objConstants);
}