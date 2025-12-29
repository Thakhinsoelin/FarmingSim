#pragma once
#include "json.hpp"
#include "raylib.h"
using json = nlohmann::json;

class TileMap{
public:
    TileMap(Texture2D* texture, const char* tilemap_location);

    json getJsonData() {
        return data;
    }

    void draw();

private:

    std::vector<int> tiles;
    int tilesetColumns;
    int mapWidth ;
    int mapHeight ;
    int tileWidth ;
    int tileHeight;
    Texture2D texture;
    json data;
    void loadData(const char* filename);
};