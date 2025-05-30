#pragma once

#include <optional>
#include <vector>
#include "Coordinates.h"

class CreatureObject;
class ItemObject;

#define WORLD_WIDTH 700
#define WORLD_HEIGHT 300
#define VIEWPORT_MIN_WIDTH 5
#define VIEWPORT_MIN_HEIGHT 3

enum WorldZone
{
    ZONE_DUNGEON,
    ZONE_ICE_CAVERN
};

struct TileData
{
    std::optional<CreatureObject*> creature;
    std::vector<ItemObject*> items;
    bool Blocking();
    ~TileData();
};

struct WorldData
{
    TileData* data[WORLD_WIDTH * WORLD_HEIGHT];
    WorldData();
    TileData*& operator[](TileCoords coords);
    ~WorldData();
};
