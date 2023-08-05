#include <windows.h>

#include "Application.h"

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    try {
        return Application{}.Run();
    }
    catch (const GameException& e) {
        MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
    }
    catch (const std::exception& e) {
        MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...) {
        MessageBoxA(nullptr, "No details available", "Unknow Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    return -1;
}