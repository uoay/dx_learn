#include "Application.h"

#include "Window.h"
#include "GraphicsAccessor.h"
Application::Application() :
    mWnd(1024, 768, L"名字"),
    mTestBox(std::make_unique<TestBox>(mWnd.GetGraphics())) 
{}

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
    mWnd.GetGraphics().BeginFrame();
    mWnd.mTimer.Tick();
    mWnd.CalculateFrameState();
    mTestBox->Draw();
    mTestBox->Update();
    mWnd.GetGraphics().EndFrame();
}