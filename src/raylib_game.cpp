#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"    // NOTE: Declares global (extern) variables and screens functions
#include "raymath.h"
#include <math.h>
#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Shared Variables Definition (global)
// NOTE: Those variables are shared between modules through screens.h
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Global Variables Definition (local to this module)
//----------------------------------------------------------------------------------
// #define SPRITE_DIM 192/4

static const float screenWidth = 800;
static const float screenHeight = 450;
static const float SPRITE_DIM = 192/4;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Program main entry point
//----------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //---------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "raylib game template");
    // rlImguiSetup(true);
    rlImGuiSetup(true);
    
    SetTargetFPS(60);       // Set our game to run at 60 frames-per-second

    Texture2D texture = LoadTexture("resources/sprites/Characters/walk.png");
    Rectangle walk_front = {0,0,SPRITE_DIM, SPRITE_DIM};
    Vector2 position = {screenWidth/2 - SPRITE_DIM/2, screenHeight/2 - SPRITE_DIM/2};
    int frameCount = 4;


    int currentFrame = 0;
    int frameCounter = 0;
    int frameSpeed = 8;
    
    Camera2D camera = { 0 };
    camera.target = position; // Camera will follow this point (e.g., player position)
    camera.offset = { screenWidth/2, screenHeight/2 };  // Camera view position (center of the screen)
    camera.rotation = 0.0f;                 // No rotation
    camera.zoom = 1.0f;  
    

    // Load animation

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    //--------------------------------------------------------------------------------------

    bool isImGuiHovered = true;
    bool isMoving = false;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        
        // Zoom based on mouse wheel
        float wheel = GetMouseWheelMove();
        Vector2 movement = {0,0};
        if (!isImGuiHovered && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            Vector2 delta = GetMouseDelta();
            delta = Vector2Scale(delta, -1.0f/camera.zoom);
            camera.target = Vector2Add(camera.target, delta);
        }

        if (wheel != 0)
        {
            // Get the world point that is under the mouse
            Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

            // Set the offset to where the mouse is
            camera.offset = GetMousePosition();

            // Set the target to match, so that the camera maps the world space point
            // under the cursor to the screen space point under the cursor at any zoom
            camera.target = mouseWorldPos;

            // Zoom increment
            // Uses log scaling to provide consistent zoom speed
            float scale = 0.2f*wheel;
            camera.zoom = Clamp(expf(logf(camera.zoom)+scale), 0.125f, 64.0f);
            
        }
        
        if (IsKeyDown(KEY_W)) movement.y -= 1;
        if (IsKeyDown(KEY_S)) movement.y += 1;
        if (IsKeyDown(KEY_A)) movement.x -= 1;
        if (IsKeyDown(KEY_D)) movement.x += 1;

        // Normalize movement vector to maintain constant speed (diagonal movement)
        if (movement.x != 0 || movement.y != 0)
        {
            movement = Vector2Normalize(movement);  // Normalize direction
            isMoving = true;
        }
        else
        {
            isMoving = false;  // If no input, the player is not moving
        }

        // Update player position based on movement
        // position = Vector2Add(position, Vector2Scale(movement, 5.0f)); // Move at 5 pixels per frame
        position = position + movement*5.0f;
        // Update animation
        
        if(isMoving){
            frameCounter++;
            if (frameCounter >= frameSpeed)
            {
                frameCounter = 0;
                currentFrame++;
                

                if (currentFrame >= frameCount)
                    currentFrame = 0;

                walk_front.x = currentFrame * SPRITE_DIM;
            }
        } else {
            // If not moving, reset to standing frame (e.g., first frame of animation)
            currentFrame = 0;
            walk_front.x = currentFrame * SPRITE_DIM; // Stay on the standing frame
        }
        


        // walk_front.x = 0;
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        BeginMode2D(camera);
        // DrawTextureEx(texture, 50, 50, );
        DrawTextureRec(texture, walk_front, position, WHITE);
        // DrawTexturePro(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint);

        EndMode2D();
        // All immediate Gui should only be drawn later after the main rendering
        rlImGuiBegin();
        ImGui::Begin("Debug");
        isImGuiHovered = ImGui::IsAnyItemHovered() || ImGui::IsWindowHovered();
        ImGui::Text("ImGui is working!");
        ImGui::End();
        rlImGuiEnd();
        

        EndDrawing();
        // isImGuiHovered = false;
    }
#endif
    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

