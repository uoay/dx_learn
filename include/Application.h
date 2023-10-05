#pragma once

#include "Window.h"
#include "drawable/TestBox.h"

class Application {
public:
    Application();
    int Run(); 
private:
    void DoFrame();
private:
    Window mWnd;
    std::unique_ptr<TestBox> mTestBox;
};