#include "Window.h"

#include <sstream>

Window::Exception::Exception(const char* file, int line, HRESULT errorCode) :GameException(file, line, errorCode) {}

const char* Window::Exception::what() const noexcept {
    std::ostringstream oss;
    oss << "[Error Code]" << GetErrorCode() << std::endl
        << "[Description]" << GetErrorString() << std::endl
        << GetExceptionLocation();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* Window::Exception::GetType() const {
    return "Game Window Exception";
}

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() :hInstance(GetModuleHandle(nullptr)) {
    WNDCLASSEX wcex{};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = GetInstance();
    wcex.hIcon = nullptr;
    wcex.hIconSm = nullptr;
    wcex.hCursor = nullptr;
    wcex.hbrBackground = nullptr;
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = GetWndClassName();
    RegisterClassEx(&wcex);
}

Window::WindowClass::~WindowClass() {
    UnregisterClass(wndClassName, GetInstance());
}

const wchar_t* Window::WindowClass::GetWndClassName() {
    return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() {
    return wndClass.hInstance;
}

Window::Window(int width, int height, const wchar_t* wndName) :width(width), height(height) {
    hWnd = CreateWindow(
        WindowClass::GetWndClassName(),
        wndName,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        width,
        height,
        nullptr,
        nullptr,
        WindowClass::GetInstance(),
        nullptr
    );
    ShowWindow(hWnd, SW_SHOWDEFAULT);
    if (hWnd == nullptr) {
        throw HWND_LAST_EXCEPTION();
    }
    graphics = std::make_unique<Graphics>(hWnd);
}

Window::~Window() {
    DestroyWindow(hWnd);
}

std::optional<int> Window::ProgressMessage() {
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return msg.wParam;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return {};
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CLOSE:PostQuitMessage(0);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}