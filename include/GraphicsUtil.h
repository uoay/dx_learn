#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "GameException.h"

#define GFX_EXPECTION(errorCode) GraphicsUtil::Exception(__FILE__, __LINE__, errorCode);

class GraphicsUtil {
	friend class Graphics;
public:
	class Exception : public GameException {
	public:
		Exception(const char* file, int line, HRESULT errorCode);
		virtual const char* what() const noexcept;
		virtual const char* GetType() const;
	};
private:
	static void CreateDevice(Microsoft::WRL::ComPtr<ID3D12Device10>& device);
	static void CreateFence(
		Microsoft::WRL::ComPtr<ID3D12Device10>& device,
		UINT64 initialValue,
		D3D12_FENCE_FLAGS flags,
		Microsoft::WRL::ComPtr<ID3D12Fence1>& fence
	);
	static void CreteCommandObjects(
		Microsoft::WRL::ComPtr<ID3D12Device10>& device,
		D3D12_COMMAND_QUEUE_DESC* cmdQueueDesc,
		Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue,
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator>& cmdAllocator,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7>& cmdList
	);
	static void CreateFactory(DWORD factoryFlags, Microsoft::WRL::ComPtr<IDXGIFactory7>& factory);
	static void CreateSwapChain(
		Microsoft::WRL::ComPtr<IDXGIFactory7>& factory,
		Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue,
		HWND hWnd,
		DXGI_SWAP_CHAIN_DESC1* swapChainDesc,
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC* swapChainFullscreenDesc,
		Microsoft::WRL::ComPtr<IDXGISwapChain4>& swapChain
	);
	static void CreateRtvDescriptorHeap(
		Microsoft::WRL::ComPtr<ID3D12Device10>& device,
		D3D12_DESCRIPTOR_HEAP_DESC* rtvHeapDes,
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap
	);
	static void CreateDsvDescriptorHeap(
		Microsoft::WRL::ComPtr<ID3D12Device10>& device,
		D3D12_DESCRIPTOR_HEAP_DESC* dsvHeapDes,
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap
	);
};
