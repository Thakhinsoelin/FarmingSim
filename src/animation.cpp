#include "animation.hpp"
#include <iostream>
// Initialize the animation system with a given texture and sprite sheet parameters
void InitAnimation(Animation *anim, Texture2D texture, int frameWidth, int frameHeight, int totalFrames, float updateTime, bool loop) {
    anim->texture = texture;
    anim->frameWidth = frameWidth;
    anim->frameHeight = frameHeight;
    anim->currentFrame = 0;
    anim->totalFrames = totalFrames;
    anim->updateTime = updateTime;
    anim->elapsedTime = 0.0f;
    anim->loop = loop;
}

// // Update the animation based on the delta time (frame time)
// void UpdateAnimation(Animation *anim, float deltaTime) {
//     anim->elapsedTime += deltaTime;
    
//     // Check if it's time to update the frame
//     if (anim->elapsedTime >= anim->updateTime) {
//         // Move to the next frame
//         anim->currentFrame++;
        
//         // If the animation reaches the end and should loop, reset to the first frame
//         if (anim->currentFrame >= anim->totalFrames) {
//             if (anim->loop) {
//                 anim->currentFrame = 0;  // Loop back to the start
//             } else {
//                 anim->currentFrame = anim->totalFrames - 1;  // Stay on the last frame
//             }
//         }
        
//         // Reset elapsed time
//         anim->elapsedTime = 0.0f;
//     }
// }


// Update the animation based on the delta time (frame time)
void UpdateAnimation(Animation *anim, float deltaTime) {
    anim->elapsedTime += deltaTime;
    
    // Check if it's time to update the frame
    if (anim->elapsedTime >= anim->updateTime) {
        // Move to the next frame
        anim->currentFrame++;
        
        // If the animation reaches the end and should loop, reset to the first frame
        if (anim->currentFrame >= anim->totalFrames) {
            if (anim->loop) {
                anim->currentFrame = 0;  // Loop back to the start
            } else {
                anim->currentFrame = anim->totalFrames - 1;  // Stay on the last frame
            }
        }
        
        // Reset elapsed time
        anim->elapsedTime = 0.0f;
    }
}


// // Draw the current frame of the animation
// void DrawAnimation(Animation *anim, Vector2 position) {
//     // Calculate the frame's x and y position in the texture based on the current frame index
//     int frameX = (anim->currentFrame % (anim->texture.width / anim->frameWidth)) * anim->frameWidth;
//     int frameY = (anim->currentFrame / (anim->texture.width / anim->frameWidth)) * anim->frameHeight;

//     // Draw the current frame at the specified position
//     Rectangle sourceRect = { (float)frameX, (float)frameY, (float)anim->frameWidth, (float)anim->frameHeight };
//     Rectangle destRect = { position.x, position.y, (float)anim->frameWidth, (float)anim->frameHeight };

//     DrawTexturePro(anim->texture, sourceRect, destRect, { 0, 0 }, 0.0f, WHITE);
// }

// Draw the current frame of the animation
void DrawAnimation(Animation *anim, Vector2 position) {
    // Calculate the row and column of the frame
    int frameX = (anim->currentFrame % anim->columns) * anim->frameWidth;  // X position of the frame
    int frameY = (anim->currentFrame / anim->rows) * anim->frameHeight; // Y position of the frame

    // Draw the current frame at the specified position
    Rectangle sourceRect = { (float)frameX, (float)frameY, (float)anim->frameWidth, (float)anim->frameHeight };
    Rectangle destRect = { position.x, position.y, (float)anim->frameWidth, (float)anim->frameHeight };

    DrawTexturePro(anim->texture, sourceRect, destRect, { 0, 0 }, 0.0f, WHITE);
}

Animation::Animation(Texture2D texture, int frameWidth, int frameHeight,
                     int currentFrame, int totalFrames, float updateTime,
                     float elapsedTime, bool loop, int rows, int columns) {
  this->texture = texture;
  this->frameWidth = frameWidth;
  this->frameHeight = frameHeight;
  this->currentFrame = currentFrame;
  this->totalFrames = totalFrames;
  this->updateTime = updateTime;
  this->elapsedTime = elapsedTime;
  this->loop = loop;
  this->rows = rows;
  this->columns = columns;

  src.x = 0;
  src.y = 0;
  src.width = frameWidth;
  src.height = frameHeight;

  startFrame = currentFrame;
  //totalFramesToRender = 
}

void Animation::UpdateAnimation(float deltaTime) {
    
    frameCounter++;
    if (frameCounter >= updateTime) {
        frameCounter = 0;

        //int mul = rows * columns;

        //for (int i = 0; i < columns; i++) {
        //  for (int j = 0; j < rows; j++) {
        //    
        //  }
        //}

        int i = 0; // column counter
        int j = 0; // row counter
        while (true) {
          if (i > columns) {
            i -= columns;
            j++;
          }

          if ((j * columns) + i == currentFrame) {
            std::cout << "i: " << i << "j: " << j << "currentFrame: " << currentFrame << "\n";
            src.x = (i - 1) * frameWidth;
            src.y = j * frameHeight;
            break;
          }
          i++;
        }
        //currentFrame = (currentFrame + 1) % totalFrames;
        //int i = currentFrame % columns; // The remainder is your column
        //int j = currentFrame / rows; // The quotient is your row
        //if (i == 0) {
        //  std::cout << "I became zero\n";
        //  exit(1);
        //}
        //src.x = (i-1)*frameWidth;
        //src.y = j * frameHeight;
        std::cout << "current Frame: " << currentFrame << "\n";
        if (currentFrame >= startFrame+totalFrames-1) {
          currentFrame = startFrame;
          std::cout << "resetted the current Frame \n";
        } else {
          currentFrame++;
        }

        /*src.x = currentFrame * frameWidth;
        src.y = currentFrame * frameHeight;*/
        
    }
    
    
}

void Animation::DrawAnimation(Vector2 position) {

    DrawTextureRec(this->texture, src, position, WHITE);
}
