#pragma once

#include <array>
#include <unordered_map>

#include <DirectXColors.h>

#include "directx/d3dx12.h"

#include "MathUtil.h"

class Direct3DUtil{
public:
	struct ObjectConstants {
		DirectX::XMFLOAT4X4 worldViewProjection = MathUtil::IdentityMatrix4x4();
	};
	struct Vertex {
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};
	struct SubmeshGeometry {
		UINT IndexCount = 0;
		UINT StartIndexLocation = 0;
		INT BaseVertexLocation = 0;
	};
	struct MeshGeometry {
		std::wstring name;

		Microsoft::WRL::ComPtr<ID3DBlob> VertexBufferCPU;
		Microsoft::WRL::ComPtr<ID3DBlob> IndexBufferCPU;

		Microsoft::WRL::ComPtr<ID3D12Resource2> VertexBufferGPU;
		Microsoft::WRL::ComPtr<ID3D12Resource2> IndexBufferGPU;

		Microsoft::WRL::ComPtr<ID3D12Resource2> VertexBufferUploader;
		Microsoft::WRL::ComPtr<ID3D12Resource2> IndexBufferUploader;

		UINT VertexByteStride = 0;
		UINT VertexBufferByteSize = 0;
		DXGI_FORMAT IndexFormat = DXGI_FORMAT_R16_UINT;
		UINT IndexBufferByteSize = 0;

		std::unordered_map<std::string, SubmeshGeometry> DrawArgs;
	};

public:
	static void CreateDefaultBuffer(
		Microsoft::WRL::ComPtr<ID3D12Device10>& device,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7>& commandList,
		const void* initData,
		size_t byteSize,
		Microsoft::WRL::ComPtr<ID3D12Resource2>& uploadBuffer,//uploadBuffer不能立即销毁
		Microsoft::WRL::ComPtr<ID3D12Resource2>& defaultBuffer
	);
	static size_t CaculateConstantBufferByteSize(size_t byteSize);
	static Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(
		const std::wstring& fileName,
		const D3D_SHADER_MACRO* defines,
		const std::string& enteryPoint,
		const std::string& target
	);
 };