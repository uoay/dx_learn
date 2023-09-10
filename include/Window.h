#pragma once

#include <memory>

#include "GameException.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "Timer.h"
#include "Geometry.h"

class Window {
private:
    class WindowClass {
    public:
        static const wchar_t* GetWndClassName();
        static HINSTANCE GetHInstance();
    private:
        WindowClass();
        WindowClass(const WindowClass&) = delete;
        WindowClass& operator=(const WindowClass&) = delete;
        ~WindowClass();
        static constexpr const wchar_t* mWndClassName = L"window";
        static WindowClass mWndClass;
        HINSTANCE mHInstance;
    };
public:
    Window(int clientWidth, int clientHeight, const wchar_t* wndName);
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    ~Window();
    static std::optional<int> ProgressMessage();
    Geometry& GetGraphics();
    void CalculateFrameState();
private:
    void OnResize(LPARAM lParam);

    LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK HandleMessageThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
    std::wstring mWndName;
    int mClientWidth;
    int mClientHeight;
    HWND mHWnd;
    std::unique_ptr<Geometry> mGraphics;
public:
    Keyboard mKeyboard;
    Timer mTimer;
};