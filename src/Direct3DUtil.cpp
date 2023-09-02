#include "Direct3DUtil.h"

#include <sstream>

#include <d3dcompiler.h>

Direct3DUtil::Exception::Exception(const char* file, int line, HRESULT errorCode) :GameException(file, line, errorCode) {}

const char* Direct3DUtil::Exception::what() const noexcept {
	std::ostringstream oss;
	oss << "[Error Code]" << GetErrorCode() << std::endl
		<< "[Description]" << GetErrorString() << std::endl
		<< GetExceptionLocation();
	mWhatBuffer = oss.str();
	return mWhatBuffer.c_str();
}

const char* Direct3DUtil::Exception::GetType() const {
	return "Direct3D Exception";
}

void Direct3DUtil::CreateDefaultBuffer(
	Microsoft::WRL::ComPtr<ID3D12Device10>& device,
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7>& commandList,
	const void* initData,
	UINT64 byteSize,
	Microsoft::WRL::ComPtr<ID3D12Resource2>& uploadBuffer,
	Microsoft::WRL::ComPtr<ID3D12Resource2>& defaultBuffer
) {
	THROW_IF_FAILED(device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(byteSize),
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(&defaultBuffer)
	));

	THROW_IF_FAILED(device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(byteSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&uploadBuffer)
	));
	commandList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(
			defaultBuffer.Get(),
			D3D12_RESOURCE_STATE_COMMON,
			D3D12_RESOURCE_STATE_COPY_DEST
		)
	);
	D3D12_SUBRESOURCE_DATA subresourceData{};
	subresourceData.pData = initData;
	subresourceData.RowPitch = byteSize;
	subresourceData.SlicePitch = subresourceData.RowPitch;
	UpdateSubresources<1>(
		commandList.Get(),
		defaultBuffer.Get(),
		uploadBuffer.Get(),
		0, 0, 1,
		&subresourceData
	);
	commandList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(
			defaultBuffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_GENERIC_READ
		)
	);
}

size_t Direct3DUtil::CaculateConstantBufferByteSize(size_t byteSize) {
	return (byteSize + 255) & ~255;
}

Microsoft::WRL::ComPtr<ID3DBlob> Direct3DUtil::CompileShader(
	const std::wstring& fileName,
	const D3D_SHADER_MACRO* defines,
	const std::string& enteryPoint,
	const std::string& target
) {
	UINT compileFlags = 0;
#ifndef NDEBUG
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	Microsoft::WRL::ComPtr<ID3DBlob> byteCode;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
	HRESULT hr = D3DCompileFromFile(
		fileName.c_str(),
		defines,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		enteryPoint.c_str(),
		target.c_str(),
		compileFlags,
		0,
		&byteCode,
		&errorBlob
	);

	if (errorBlob != nullptr) {
		OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	THROW_IF_FAILED(hr);

	return byteCode;
}