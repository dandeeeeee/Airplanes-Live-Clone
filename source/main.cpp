#include "Window.hpp"
#include "raylib.h"

class TestWindow : public Window {
public:
    TestWindow() : Window(800, 600) {}

protected:
    void internalUpdate() override {}
    
    void internalRender() override {
        DrawText("Window Test Running...", 10, 10, 20, DARKGRAY);
    }

    void internalHandleEvent(const std::optional<int>& event) override {}
};

int main() {
    TestWindow app;
    app.run();
    return 0;
}
