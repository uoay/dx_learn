#include "Window.h"

#include <sstream>

Window::Exception::Exception(const char* file, int line, HRESULT errorCode) :GameException(file, line, errorCode) {}

const char* Window::Exception::what() const noexcept {
    std::ostringstream oss;
    oss << "[Error Code]" << GetErrorCode() << std::endl
        << "[Description]" << GetErrorString() << std::endl
        << GetExceptionLocation();
    mWhatBuffer = oss.str();
    return mWhatBuffer.c_str();
}

const char* Window::Exception::GetType() const {
    return "Game Window Exception";
}

Window::WindowClass Window::WindowClass::mWndClass;

Window::WindowClass::WindowClass() :mHInstance(GetModuleHandle(nullptr)) {
    WNDCLASSEX wcex{};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = GetHInstance();
    wcex.hIcon = nullptr;
    wcex.hIconSm = nullptr;
    wcex.hCursor = nullptr;
    wcex.hbrBackground = nullptr;
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = GetWndClassName();
    if(!RegisterClassEx(&wcex))
        throw WND_THROW_LAST_EXCEPTION();
}

Window::WindowClass::~WindowClass() {
    UnregisterClass(mWndClassName, GetHInstance());
}

const wchar_t* Window::WindowClass::GetWndClassName() {
    return mWndClassName;
}

HINSTANCE Window::WindowClass::GetHInstance() {
    return mWndClass.mHInstance;
}

Window::Window(int clientWidth, int clientHeight, const wchar_t* wndName) :mClientWidth(clientWidth), mClientHeight(clientHeight) {
    RECT rect{ 0, 0, clientWidth, clientHeight };
    AdjustWindowRect(&rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, false);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    mHWnd = CreateWindow(
        WindowClass::GetWndClassName(),
        wndName,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        width,
        height,
        nullptr,
        nullptr,
        WindowClass::GetHInstance(),
        nullptr
    );
    if (mHWnd == nullptr) {
        throw WND_THROW_LAST_EXCEPTION();
    }
    ShowWindow(mHWnd, SW_SHOWDEFAULT);
    UpdateWindow(mHWnd);
    mGraphics = std::make_unique<Graphics>(mHWnd, clientWidth, clientHeight);
}

Window::~Window() {
    DestroyWindow(mHWnd);
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

Graphics& Window::GetGraphics() {
    return *mGraphics;
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CLOSE:PostQuitMessage(0);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}