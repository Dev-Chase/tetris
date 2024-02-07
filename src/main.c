#include "game.h"
#include "raylib.h"
#include "settings.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TITLE_TEXT_LEN 25
typedef enum { Menu, Playing } GameState;

int main(void) {
    // Initializing Window & Camera
    InitWindow(W, H, "Snake in C!");
    SetTargetFPS(FPS);
    Camera2D camera = {
        .offset = (Vector2){0, 0},
        .target = (Vector2){0, 0},
        .zoom = 1.,
        .rotation = 0.,
    };

    // Seeding RNG
    srand(time(NULL));

    // Game Variables;
    GameState game_state = Menu;

    char title_text[TITLE_TEXT_LEN] = "Press Space to Start";
    char sub_title_text[TITLE_TEXT_LEN] = "";

    Game game;
    init_game(&game);

    // Game Loop
    while (!WindowShouldClose()) {
        // camera.offset.y = 30;
        BeginDrawing();
        BeginMode2D(camera);
        ClearBackground(BG_COLOUR);

        switch (game_state) {
        case Menu:
            DrawText(title_text,
                     (W / 2) - MeasureText(title_text, FONT_SIZE) / 2,
                     (H / 2) - (FONT_SIZE / 2), FONT_SIZE, FG_COLOUR);
            DrawText(sub_title_text,
                     (W / 2) - MeasureText(sub_title_text, FONT_SIZE) / 2,
                     (H / 2) - (FONT_SIZE / 2) - FONT_SIZE - 4, FONT_SIZE,
                     FG_COLOUR);
            if (IsKeyPressed(KEY_SPACE)) {
                game_state = Playing;
                reset_game(&game);
            }
            break;
        case Playing:

            // Game Over Checks
            if (update_game(&game, &camera) == GAME_OVER) {
                game_state = Menu;
                strncpy(sub_title_text, "Game Over.", 25);
                strncpy(title_text, "Press Space to Restart", 25);
                continue;
            }

            // Drawing Sprites
            draw_game(&game);

            // Pause Checks
            if (IsKeyPressed(KEY_Q)) {
                game_state = Menu;
                strncpy(sub_title_text, "Game Paused", 25);
                strncpy(title_text, "Press Space to Unpause", 25);
            }
            break;
        };
        EndMode2D();
        EndDrawing();
    }

    // Free Memory
    end_game(&game);
    CloseWindow();
    return EXIT_SUCCESS;
}
