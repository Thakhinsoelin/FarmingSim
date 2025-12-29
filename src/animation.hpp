#pragma once
#include "raylib.h"

struct MyRect {
  unsigned int x;
  unsigned int y;
  unsigned int width;
  unsigned int height;

  Rectangle convert() { return {(float)this->x, (float)this->y, (float)this->width, (float)this->height};
  }
};

class Animation {
public:
    Texture2D texture;       // The sprite sheet texture
    int frameWidth;          // Width of one frame in the sprite sheet
    int frameHeight;         // Height of one frame in the sprite sheet
    
    int totalFrames;         // Total number of frames in the sprite sheet (e.g., columns * rows)
    float updateTime;        // Time between frame updates (how fast the animation plays)
    float elapsedTime;       // Elapsed time for frame updates
    bool loop;               // Should the animation loop when it reaches the end?
    int rows;                // Number of rows in the sprite sheet
    int columns;             // Number of columns in the sprite sheet
    int startFrame = 0;
    Animation(Texture2D texture, int frameWidth, int frameHeight,
              int startFrame, int totalFrames, float updateTime,
              float elapsedTime, bool loop, int rows, int columns);

    void UpdateAnimation(float deltaTime);
    void DrawAnimation(Vector2 position);
    void updateRecSelection(int from, int until);
    int getStartFrame() {
      return startFrame;
    }

private:
    int frameCounter = 0;
    Rectangle src = {0};
    int currentFrame;        // The current frame index in the animation
    int totalFramesToRender = 0;
};

void UpdateAnimation(Animation *anim, float deltaTime);
void DrawAnimation(Animation *anim, Vector2 position);
