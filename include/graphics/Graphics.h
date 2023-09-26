#pragma once

#include <fstream>
#include <sstream>

#include <dxgi1_6.h>
#include <wrl.h>

#include "GraphicsBase.h"

class Graphics : public GraphicsBase {
    friend class GraphicsAccessor;
public:
    Graphics(const HWND hwnd, const int clientWidth, const int clientHeight);
    Graphics(const Graphics&) = delete;
    Graphics& operator=(const Graphics&) = delete;
    virtual void Draw();
public:
    virtual void OnResize(int clientWidth, int clientHeight);
protected:
    D3D12_VIEWPORT mViewport;
    D3D12_RECT mScissorRect;
};