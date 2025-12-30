#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"    // NOTE: Declares global (extern) variables and screens functions
#include "raymath.h"
#include <math.h>
#include <iostream>
#include "animation.hpp"
#include "tilemaploader.hpp"

#define RINI_IMPLEMENTATION
#include "rini.h"
#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif
//----------------------------------------------------------------------------------
// Global Variables Definition (local to this module)
//----------------------------------------------------------------------------------
static const float screenWidth = 800;
static const float screenHeight = 450;
static const float SPRITE_DIM = 192/4;


enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
};
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
    rlImGuiSetup(true);
    
    SetTargetFPS(60);       // Set our game to run at 60 frames-per-second

    Texture2D texture = LoadTexture("resources/sprites/Characters/walk.png");
    Rectangle walk = {0,0,SPRITE_DIM, SPRITE_DIM};
    Vector2 position = {screenWidth/2 - SPRITE_DIM/2, screenHeight/2 - SPRITE_DIM/2};
    float tempSpeed = 5.f;

    int frameCount = 4;

    int currentFrame = 0;
    int frameCounter = 0;
    int frameSpeed = 8;
    
    Texture2D grass_temp = LoadTexture("resources/sprites/Tilesets/Grass.png");
    const int grassWidth = 176/11;
    const int grassHeight = 112/7;
    Rectangle grass = { 0, grassHeight*6, grassWidth, grassHeight};
    Vector2 grassPosition = {0,0};

    Texture2D actions = LoadTexture("resources/sprites/Characters/playerActions.png");
    const int toolActionWidth = 96/2;
    const int toolActionHeight = 576/12;
    Rectangle workingSprite = {0,0, toolActionWidth, toolActionHeight};
    Animation testanim = Animation(actions, toolActionWidth ,toolActionHeight,
                                   7, 2, 20.f, 0.f, true, 12, 2);
    Vector2 testPosition = {100,50};
    bool chop = false;
    bool testIsMoving = false;


    Texture2D biomis = LoadTexture("resources/sprites/Objects/Basic_Grass_Biom_things.png");

    /*{
        actions,
        toolActionWidth,
        toolActionHeight,
        3,
        2,
        20.f,
        1.f,
        true,
        12,
        2,

    };*/
    Texture2D background = LoadTexture("resources/sprites/Tilesets/Hills.png");
    TileMap testTile = TileMap(&background, "resources/TileMap/mapped.tmj");

    Camera2D camera = { 0 };
    camera.target = { screenWidth/2, screenHeight/2 };//position; // Camera will follow this point (e.g., player position)
    camera.offset = { screenWidth/2, screenHeight/2 };  // Camera view position (center of the screen)
    camera.rotation = 0.0f;                 // No rotation
    camera.zoom = 1.0f;  
    
    rini_data config = rini_load(NULL);
    if (FileExists("config.ini")) {
      config = rini_load("config.ini");
      const char* temp = rini_get_value_text(config, "frameSpeed");
      frameSpeed = atoi(temp);
      printf("%d\n", frameSpeed);

      const char *temp2 = rini_get_value_text(config, "movementSpeed");
      tempSpeed = atof(temp2);
    } else {
      config = rini_load(NULL);
    }

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    //--------------------------------------------------------------------------------------

    bool isImGuiHovered = true;
    bool isMoving = false;
    // Main game loop

    
    
        int currentDirection = UP;
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        float dt = GetFrameTime();
      testanim.UpdateAnimation(dt);
        //UpdateAnimation(&testanim, dt);
        
        // Zoom based on mouse wheel
        float wheel = GetMouseWheelMove();
        Vector2 movement = {0, 0};
        
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
        
        // if(IsKeyDown(KEY_T)) {
        //     testanim.updateRecSelection(1, 2);
        // }

        // if(IsKeyDown(KEY_R)) {
        //     testanim.updateRecSelection(3, 2);
        // }
        
        if (IsKeyDown(KEY_UP)) 
        {
          movement.y -= 1 * dt;
          walk.y = SPRITE_DIM;
          currentDirection = UP;
        }
        if (IsKeyDown(KEY_DOWN)) 
        {
          movement.y += 1 * dt;
          walk.y = 0;
          currentDirection = DOWN;
        }
        if (IsKeyDown(KEY_LEFT)) 
        {
          movement.x -= 1 * dt;
          walk.y = SPRITE_DIM*2;
          currentDirection = LEFT;
        }
        if (IsKeyDown(KEY_RIGHT)) 
        {
          movement.x += 1 * dt;
          walk.y = SPRITE_DIM * 3;
          currentDirection = RIGHT;
        }

        if(IsKeyDown(KeyboardKey::KEY_A)) {
            chop = true;
             int nextStart = 0;
             int frameCount = 2;
             switch (currentDirection) {
             case UP:    nextStart = 3; break;
             case DOWN:  nextStart = 1; break;
             case LEFT:  nextStart = 5; break;
             case RIGHT: nextStart = 7; break;
             }
             //std::cout << "Before updating: " << testanim.getStartFrame() << "\n";
             // ONLY update if the animation range has actually changed
             if (testanim.getStartFrame() != nextStart ) {
                 testanim.updateRecSelection(nextStart, frameCount);
             }
             //std::cout << "After updating: " << testanim.getStartFrame() << "\n";
        }

        // if (IsKeyDown(KEY_W)) {
        //     testanim.updateRecSelection(5, 4);
        //     testMovement.y -= 1 * dt;
        // }

        // if (IsKeyDown(KEY_S)) {
        //     testanim.updateRecSelection(1, 4);
        //     testMovement.y += 1 * dt;
        // }

        // if (IsKeyDown(KEY_A)) {
        //     testanim.updateRecSelection(9, 4);
        //     testMovement.x -= 1 * dt;
        // }

        // if (IsKeyDown(KEY_D)) {
        //     testanim.updateRecSelection(13, 4);
        //     testMovement.y += 1 * dt;
        // }

        // if(testMovement.x != 0 || testMovement.y != 0) {
        //     testMovement = Vector2Normalize(testMovement);
        //     testIsMoving = true;
        // } else {
        //     testIsMoving = false;
        // }

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
        
        position = position + movement*tempSpeed;
        // testPosition = testPosition + testMovement*2.f;
        // Update animation

        // if(testIsMoving) {
        //     testanim.UpdateAnimation(dt);
        // } else {
        //     testanim.updateRecSelection(1, 4);
        // }
        
        if(isMoving){
            frameCounter++;
            if (frameCounter >= frameSpeed)
            {
                frameCounter = 0;
                currentFrame++;
                

                if (currentFrame >= frameCount)
                    currentFrame = 0;

                walk.x = currentFrame * SPRITE_DIM;
            }
        } else {
            // If not moving, reset to standing frame (e.g., first frame of animation)
            currentFrame = 0;
            walk.x = currentFrame * SPRITE_DIM; // Stay on the standing frame
            //walk.y = 0;
        }
        
        // printf("niga\n");


        // walk.x = 0;
        BeginDrawing();
        ClearBackground(RAYWHITE);
        // // Check if the player is out of bounds in X or Y
        // if (position.x < camera.target.x - screenWidth/2 || position.x > camera.target.x + screenWidth/2 ||
        //     position.y < camera.target.y - screenHeight/2 || position.y > camera.target.y + screenHeight/2)
        // {
        //     // Retarget the camera to the middle of the player
        //     camera.target = position;
        // }

        BeginMode2D(camera);
        testTile.draw();
        // DrawTextureEx(texture, 50, 50, );
        // for (int i = 0; i < screenWidth; i += grassWidth) {
        //     for (int j = 0; j < screenHeight; j += grassHeight) {
        //         grassPosition.y = j;
        //         grassPosition.x = i;
        //         DrawTextureRec(grass_temp, grass, grassPosition, WHITE);
                
        //     }
            
        // }

        //DrawAnimation(&testanim, {50,50});
        // testanim.DrawAnimation(testPosition);

        //This draw main character
            // std::cout <<"Current: " << testanim.getStartFrame() <<"\n";
        if(!chop) {
            DrawTextureRec(texture, walk, position, WHITE);
        } else {
            
            testanim.DrawAnimation(position);
            chop = false;
        }
        // DrawTexturePro(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint);

        EndMode2D();
        // All immediate Gui should only be drawn later after the main rendering
        rlImGuiBegin();
        ImGui::Begin("Debug");
        isImGuiHovered = ImGui::IsAnyItemHovered() || ImGui::IsWindowHovered();
        ImGui::Text("ImGui is working!");
        ImGui::SliderInt("frameSpeed", &frameSpeed, 1, 20);
        ImGui::SliderFloat("movementSpeed", &tempSpeed, 0.f, 6.f);
        ImGui::End();
        rlImGuiEnd();
        

        EndDrawing();
        // isImGuiHovered = false;
    }
#endif
    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    
    // Do close thinging here
    char buffer[64] = {0};
    snprintf(buffer, sizeof(buffer), "%d", frameSpeed);
    rini_set_value_text(&config, "frameSpeed", buffer, "frame speed for the animation");

    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%.4f", tempSpeed);
    rini_set_value_text(&config, "movementSpeed", buffer,
                        "movementSpeed speed for the character");
    rini_save(config, "config.ini");
    
    return 0;
}

