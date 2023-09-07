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
    wcex.lpfnWndProc = HandleMessageSetup;
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

Window::Window(int clientWidth, int clientHeight, const wchar_t* wndName) :
    mWndName(wndName), mClientWidth(clientWidth), mClientHeight(clientHeight) {
    RECT rect{ 0, 0, clientWidth, clientHeight };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    mHWnd = CreateWindow(
        WindowClass::GetWndClassName(),
        wndName,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        width,
        height,
        nullptr,
        nullptr,
        WindowClass::GetHInstance(),
        this
    );
    if (mHWnd == nullptr) {
        throw WND_THROW_LAST_EXCEPTION();
    }
    ShowWindow(mHWnd, SW_SHOWDEFAULT);
    UpdateWindow(mHWnd);
    mGraphics = std::make_unique<Geometry>(mHWnd, clientHeight, clientWidth);
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

Geometry& Window::GetGraphics() {
    return *mGraphics;
}

void Window::CalculateFrameState() {
    static int frameCount = 0;
    static float elapsedTime = 0.0f;
    ++frameCount;
    if (mTimer.GetTotalTime() - elapsedTime >= 1.0f) {
        float fps = static_cast<float>(frameCount);
        float mspf = 1000.0f / fps;
        std::wstring fpsStr = std::to_wstring(fps);
        std::wstring mspfStr = std::to_wstring(mspf);
        std::wstring wndName = mWndName + L"  fps:" + fpsStr + L"  mspf:" + mspfStr;
        SetWindowText(mHWnd, wndName.c_str());
        frameCount = 0;
        elapsedTime += 1.0f;
    }
}


void Window::OnResize(LPARAM lParam) {
    mClientHeight = HIWORD(lParam);
    mClientWidth = LOWORD(lParam); 
    if (mGraphics != nullptr) {
       mGraphics->OnResize(mClientWidth, mClientHeight);
    } 
}

LRESULT Window::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
        case WM_SIZE:
            OnResize(lParam);
            return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMessageThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    Window* const wnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return wnd->HandleMessage(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_NCCREATE) {
        const CREATESTRUCT* const createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
        Window* const wnd = static_cast<Window*>(createStruct->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(wnd));
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMessageThunk));
        return wnd->HandleMessage(hWnd, msg, wParam, lParam);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}