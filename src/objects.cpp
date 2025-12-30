#include "objects.hpp"
#include "raylib.h"
#include <cstring>

Object::Object(std::string name, int dps, int health, Vector2& position, bool attackable) {
    // this->name = name.c_str();
    memcpy(this->name, name.c_str(), name.size());
    this->dps = dps;
    this->health = health;
    this->position = position;
    this->canBeAttacked = attackable;
};