#include "Application.h"
#include "Window.h"

Application::Application() :mWnd(1024, 768, L"Ãû×Ö") {}

int Application::Run() {
    while (true) {
        const auto ecode = Window::ProgressMessage();
        if (ecode) {
            return *ecode;
        }
        DoFrame();
    }
}

void Application::DoFrame() {
    mWnd.GetGraphics().Draw();
}