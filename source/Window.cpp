#include "Window.hpp"


Window::Window(unsigned int width, unsigned int height) : screenWidth(width), screenHeight(height) 
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "AirplaneLive Clone");
    SetTargetFPS(60);
    // appIcon = LoadImage("resources/icon.png");
    // SetWindowIcon(appIcon);

    camera = { 0 };
    camera.target = { 0.0f, 0.0f };
    camera.offset = { 0.0f, 0.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}



Vector2 Window::positionRelativeToView(Vector2 position) const
{
    return GetScreenToWorld2D(position, camera);
}



void Window::updateWindowView(unsigned int width, unsigned int height) 
{
    float windowRatio = static_cast<float>(width) / static_cast<float>(height);
    float viewRatio = 1.0f;
    float sizeX = 1, sizeY = 1, posX = 0, posY = 0;

    if (windowRatio < viewRatio) {
        sizeX = viewRatio / windowRatio;
        posX = (1 - sizeX) / 2.0f;
    } else {
        sizeY = windowRatio / viewRatio;
        posY = (1 - sizeY) / 2.0f;
    }

    camera.zoom = sizeX;
}



void Window::run() 
{
    while (!WindowShouldClose()) {
        handleEvent();
        update();
        BeginDrawing();
        ClearBackground(RAYWHITE);
        internalRender();
        EndDrawing();
    }
}



void Window::update()
{
    internalUpdate();
}



void Window::handleEvent() 
{
    if (IsKeyPressed(KEY_ESCAPE)) {
        CloseWindow();
    }
}