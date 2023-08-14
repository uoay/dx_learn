#pragma once

#include <memory>

#include <windows.h>


#include "GameException.h"
#include "Graphics.h"
#include "Keyboard.h"

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
        ~WindowClass();
        WindowClass(const WindowClass&) = delete;
        WindowClass& operator=(const WindowClass&) = delete;
        static constexpr const wchar_t* mWndClassName = L"window";
        static WindowClass mWndClass;
        HINSTANCE mHInstance;
    };
public:
    Window(int clientWidth, int clientHeight, const wchar_t* wndName);
    ~Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    static std::optional<int> ProgressMessage();
    Graphics& GetGraphics();
private:
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
    int mClientWidth;
    int mClientHeight;
    HWND mHWnd;
    std::unique_ptr<Graphics> mGraphics;
public:
    Keyboard mKeyboard;
};