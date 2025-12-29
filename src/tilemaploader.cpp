#include "tilemaploader.hpp"
#include <fstream>


TileMap::TileMap(Texture2D* texture, const char* tilemap_location) {
    loadData(tilemap_location);
    this->texture = *texture;

    mapWidth = data["width"];
    mapHeight = data["height"];
    tileWidth = data["tilewidth"];
    tileHeight = data["tileheight"];

    json layer = data["layers"][0];  // usually ground
    std::vector<int> temp = layer["data"];
    tiles = temp;

    tilesetColumns = (*texture).width / tileWidth;
    


}

void TileMap::loadData(const char* filename) {
    std::ifstream file(filename);
    json data;
    file >> data;
    this->data = data;
}

void TileMap::draw() {
    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            int index = y * mapWidth + x;
            int tileID = tiles[index];

            if (tileID == 0) continue; // empty tile

            tileID--; // Tiled → 0-based

            Rectangle src = {
                static_cast<float>((tileID % tilesetColumns) * tileWidth),
                static_cast<float>((tileID / tilesetColumns) * tileHeight),
                (float)tileWidth,
                (float)tileHeight
            };

            Rectangle dst = {
                (float)(x * tileWidth),
                (float)(y * tileHeight),
                (float)tileWidth,
                (float)tileHeight
            };

            DrawTexturePro(texture, src, dst, {0,0}, 0, WHITE);
        }
    }

}