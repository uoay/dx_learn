#pragma once

#include <memory>

#include "GameException.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "Timer.h"
#include "Geometry.h"

#define WND_EXCEPTION(errorCode) Window::Exception(__FILE__, __LINE__, errorCode)
#define WND_THROW_LAST_EXCEPTION() Window::Exception(__FILE__, __LINE__, GetLastError())

class Window {
public:
    class Exception : public GameException {
    public:
        Exception(const char* file, int line, HRESULT errorCode);
        virtual const char* what() const noexcept;
        virtual const char* GetType() const;
    };
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
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
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