#pragma once

#include <memory>

#include <windows.h>

#include "Keyboard.h"
#include "Graphics.h"

#define HWND_EXCEPTION(errorCode) Window::Exception(__FILE__, __LINE__, errorCode)
#define HWND_LAST_EXCEPTION() Window::Exception(__FILE__, __LINE__, GetLastError())

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
        static HINSTANCE GetInstance();
    private:
        WindowClass();
        ~WindowClass();
        WindowClass(const WindowClass&) = delete;
        WindowClass& operator=(const WindowClass&) = delete;
        static constexpr const wchar_t* wndClassName = L"window";
        static WindowClass wndClass;
        HINSTANCE hInstance;
    };
public:
    Window(int width, int height, const wchar_t* wndName);
    ~Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    static std::optional<int> ProgressMessage();
    Graphics& GetGraphics();
private:
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
    int width;
    int height;
    HWND hWnd;
    std::unique_ptr<Graphics> graphics;
public:
    Keyboard keyboard;
};