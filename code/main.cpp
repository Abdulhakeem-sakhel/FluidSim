#include "playground.h"
#include <raylib.h>

Color backGround = {26, 34, 52, 255};
const int height = 720;
const int width = 1280;

int main() {
    InitWindow(width, height, "2d Fluid Sim");
    SetTargetFPS(60);

    Playground playground = Playground();
    while(WindowShouldClose() == false) {
        playground.update(GetFrameTime());
        playground.handelMouseInput();
        BeginDrawing();
            ClearBackground(DARKGRAY);
            playground.draw();
            DrawText(TextFormat("FPS: %d", GetFPS()), 0, 30, 30, RED);
        EndDrawing(); 
    }
    CloseWindow();

}