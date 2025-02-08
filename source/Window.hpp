#pragma once

#include "raylib.h"
#include <optional>

class Window 
{
public:
    Window() = delete;
    Window(unsigned int width, unsigned int height);
    virtual ~Window() = default;

    virtual void run();

protected:
    Camera2D camera;
    int screenWidth;
    int screenHeight;

    Vector2 positionRelativeToView(Vector2 position) const;

    virtual void internalHandleEvent(const std::optional<int>& event) = 0;
    virtual void internalRender() = 0;
    virtual void internalUpdate() = 0;

private:
    void handleEvent();
    void update();
    void render();
    void updateWindowView(unsigned int width, unsigned int height);

    Image appIcon;
};
