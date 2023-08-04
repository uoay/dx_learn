#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

class Graphics {
public:
    Graphics(HWND hwnd);
    Graphics(const Graphics&) = delete;
    Graphics& operator=(const Graphics&) = delete;
private:
    void CreateDevice();
    void CreateFence();
    void CreteCommandObjects();
    void CreateFactory();
    void CreateSwapChain(HWND hwnd);
    void CreateDescriptorHeap();
private:
    static const unsigned int bufferCount = 2;
    int currBackBuffer = 0;
    Microsoft::WRL::ComPtr<ID3D12Device10> device;
    Microsoft::WRL::ComPtr<ID3D12Fence1> fence;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> cmdQueue;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmdAllocator;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> cmdList;
    Microsoft::WRL::ComPtr<IDXGIFactory7> factory;
    Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;
    unsigned int rtvDescriptorSize;
    unsigned int dsvDescriptorSize;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap;
};