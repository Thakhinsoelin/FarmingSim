#pragma once
#include "raylib.h"
#include <string>
class Object{
public:
    Object(std::string name, int dps, int health, Vector2& position, bool attackable);
    char* name;
    bool canBeAttacked;
    int health;
    int dps;
    bool gone = false;
    Vector2 position;
private:
};