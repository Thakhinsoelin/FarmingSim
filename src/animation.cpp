#include "animation.hpp"
#include <iostream>
Animation::Animation(Texture2D texture, int frameWidth, int frameHeight,
                     int startFrame, int totalFrames, float updateTime,
                     float elapsedTime, bool loop, int rows, int columns) {
  this->texture = texture;
  this->frameWidth = frameWidth;
  this->frameHeight = frameHeight;
  this->startFrame = startFrame;
  this->totalFrames = totalFrames;
  this->updateTime = updateTime;
  this->elapsedTime = elapsedTime;
  this->loop = loop;
  this->rows = rows;
  this->columns = columns;

  this->currentFrame = this->startFrame;

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

          if ((j * columns) + i >= currentFrame) {
            std::cout << "i: " << i << "j: " << j << "currentFrame: " << currentFrame << "\n";
            src.x = (i - 1) * frameWidth;
            src.y = j * frameHeight;
            break;
          }
          i++;
        }
        // int column = currentFrame % columns;
        // int row = currentFrame / columns;

        // src.x = (float)column * frameWidth;
        // src.y = (float)row * frameHeight;
        //currentFrame = (currentFrame + 1) % totalFrames;
        //int i = currentFrame % columns; // The remainder is your column
        //int j = currentFrame / rows; // The quotient is your row
        //if (i == 0) {
        //  std::cout << "I became zero\n";
        //  exit(1);
        //}
        //src.x = (i-1)*frameWidth;
        //src.y = j * frameHeight;
        // std::cout << "current Frame: " << currentFrame << "\n";
        if (currentFrame == startFrame+totalFrames-1) {
          currentFrame = startFrame;
          // std::cout << "resetted the current Frame \n";
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

void Animation::updateRecSelection(int from, int until){
    this->startFrame = from;
    this->totalFrames = until;
    this->frameCounter = 0;
    this->currentFrame = this->startFrame;
    src.x = 0;
    src.y = 0;
    
};