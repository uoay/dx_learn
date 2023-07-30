#pragma once

#include <windows.h>

#include "Keyboard.h"

class Window {
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
private:
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
    int width;
    int height;
    HWND hWnd;
public:
    Keyboard keyboard;
};