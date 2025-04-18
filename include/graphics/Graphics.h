#pragma once

#include <fstream>
#include <sstream>

#include <dxgi1_6.h>
#include <wrl.h>

#include "Direct3DUtil.h"

class Graphics {
    friend class GraphicsAccessor;
public:
    Graphics(HWND hwnd, int clientWidth, int clientHeight);
    Graphics(const Graphics&) = delete;
    Graphics& operator=(const Graphics&) = delete;
    void Draw();
    void BeginFrame();
    void EndFrame();
    void FlushCommandQueue();
    Microsoft::WRL::ComPtr<ID3D12PipelineState> mPipelineState;
protected:
    D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
    D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;

    void CreateCommandObjects();

    void CreateSwapChain(HWND hWnd, int clientWidth, int clientHeight);
    void GetDescriptorSize();
    void CreateRtvAndDsvDescriptorHeap();
    void CreateRenderTargetView();
    void CreateDepthStencilView(int clientWidth, int clientHeight);
    void CreateViewPortAndScissorRect(int clientWidth, int clientHeight);
public:
    virtual void OnResize(int clientWidth, int clientHeight);
protected:
    static const DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    static const DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    static const UINT mBackBufferCount = 2;
    int mCurrentBackBuffer = 0;
    Microsoft::WRL::ComPtr<ID3D12Device10> mDevice;
    Microsoft::WRL::ComPtr<ID3D12Fence1> mFence;
    UINT64 mCurrentFence = 0;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mCommandAllocator;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> mCommandList;
    Microsoft::WRL::ComPtr<IDXGIFactory7> mFactory;
    Microsoft::WRL::ComPtr<IDXGISwapChain4> mSwapChain;
    Microsoft::WRL::ComPtr<ID3D12Resource2> mBackBuffer[mBackBufferCount];
    Microsoft::WRL::ComPtr<ID3D12Resource2> mDepthStencilBuffer;
    UINT mRtvDescriptorSize;
    UINT mDsvDescriptorSize;
    UINT mCbvSrvUavDescriptorSize;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvHeap;
    D3D12_VIEWPORT mScreenViewport;
    D3D12_RECT mScissorRect;


};