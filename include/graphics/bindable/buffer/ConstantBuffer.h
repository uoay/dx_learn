#pragma once

#include "Direct3DUtil.h"
#include "GraphicsAccessor.h"
#include "Buffer.h"
#include "helper/UploadBuffer.h"

template<typename T>
class ConstantBuffer : public Buffer {
public:
	ConstantBuffer(Graphics& graphics, unsigned int elementCount)
		: mBuffer(
			GraphicsAccessor::GetDevice(graphics),
			elementCount,
			Direct3DUtil::CaculateConstantBufferByteSize(sizeof(T))
		  ) {
		D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc{};
		cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		cbvHeapDesc.NumDescriptors = 1;
		cbvHeapDesc.NodeMask = 0;
		GFX_THROW_IF_FAILED(GraphicsAccessor::GetDevice(graphics)->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&mDescriptorHeap)));

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
		cbvDesc.BufferLocation = mBuffer.GetResource()->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = static_cast<unsigned int>(Direct3DUtil::CaculateConstantBufferByteSize(sizeof(T)));
		GraphicsAccessor::GetDevice(graphics)->CreateConstantBufferView(&cbvDesc, mDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	}
	ConstantBuffer(const ConstantBuffer& uploadBuffer) = delete;
	ConstantBuffer& operator=(const ConstantBuffer& uploadBuffer) = delete;
	void Update(const T& constants) {
		mBuffer.CopyData(0, constants);
	}
	virtual void Bind(
		Graphics& graphics,
		D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipelineStateDesc = nullptr
	) const noexcept override {
		ID3D12DescriptorHeap* descriptorHeaps[] = { mDescriptorHeap.Get()};
		GraphicsAccessor::GetGraphicsCommandList(graphics)->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
		GraphicsAccessor::GetGraphicsCommandList(graphics)->SetGraphicsRootDescriptorTable(0, mDescriptorHeap.Get()->GetGPUDescriptorHandleForHeapStart());
	}
private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDescriptorHeap;
	UploadBuffer<T> mBuffer;
	BYTE* mMappedData = nullptr;
};