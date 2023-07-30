#include "Application.h"
#include "Window.h"

Application::Application() :wnd(1024, 768, L"����") {}

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

}