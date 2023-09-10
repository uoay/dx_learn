#include "Application.h"
#include "Window.h"

Application::Application() :mWnd(1024, 768, L"名字") {}

int Application::Run() {
    mWnd.mTimer.Reset();
    while (true) {
        const auto ecode = Window::ProgressMessage();
        if (ecode) {
            return *ecode;
        }
        DoFrame();
    }
}

void Application::DoFrame() {
    mWnd.mTimer.Tick();
    mWnd.CalculateFrameState();
    mWnd.GetGraphics().Draw();
    mWnd.GetGraphics().Update(1024, 768);
}