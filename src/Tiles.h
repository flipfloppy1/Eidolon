#pragma once
#include "raylib.h"
#include <map>
#include <string>
#include <vector>
#include <optional>
#include <iostream>

class Tiles
{
    const std::vector<std::string> tileTypes = {
        "goblin",
        "slime",
        "human",
        "arrow",
        "dungeon/corner",
        "dungeon/wall",
        "dungeon/side_both",
        "dungeon/side_full",
        "dungeon/side_left",
        "dungeon/side_right",
        "dungeon wall",
        "ice/corner",
        "ice/wall",
        "ice/side_both",
        "ice/side_full",
        "ice/side_left",
        "ice/side_right",
        "flowing fountain",
        "snail statue",
        "cat statue",
        "red flowers",
        "white flowers",
        "yellow flowers",
        "pink flowers",
        "shortsword",
        "goo"
    };

  public:
    void Load()
    {
        for (auto& tileName : tileTypes)
        {
            tiles[tileName] = LoadTexture(("./assets/tiles/" + tileName + ".png").c_str());
        }
    }
    void Unload()
    {
        std::cout << "Unloading textures\n";
        for (auto& [key, val] : tiles)
            UnloadTexture(val);
        std::cout << "Unloaded textures\n";
    }
    std::optional<Texture2D> GetTile(std::string kind) { if (tiles.contains(kind)) return tiles.at(kind); else return std::optional<Texture2D>(); }
    std::map<std::string, Texture2D> tiles;
};
