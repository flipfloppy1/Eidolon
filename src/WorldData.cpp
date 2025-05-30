#include "WorldData.h"
#include "CreatureObject.h"
#include "ItemObject.h"
#include <iostream>

bool TileData::Blocking()
{
    bool isBlocking = false;
    for (auto const& item: items)
        isBlocking |= item->GetWeight() > 1000;

    return isBlocking;
}
TileData::~TileData()
{
    // Don't have to free creatures because the world tracks this
    if (creature.has_value())
    {
        // delete creature.value();
    }
    for (auto& item: items)
    {
        delete item;
    }
}

WorldData::WorldData()
{
    for (int i = 0; i < WORLD_WIDTH * WORLD_HEIGHT; i++)
        data[i] = new TileData();
}
TileData*& WorldData::operator[](TileCoords coords)
{
    return data[coords.x * WORLD_HEIGHT + coords.y];
}
WorldData::~WorldData()
{
    std::cout << "Unloading worldData\n";
    for (int i = 0; i < WORLD_WIDTH * WORLD_HEIGHT; i++)
        delete data[i];
    std::cout << "Unloaded worldData\n";
}
