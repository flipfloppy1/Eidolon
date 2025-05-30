
#include "World.h"
#include "raylib.h"
class WorldEater
{
  public:
    WorldEater(World* world, TileCoords startPos, int hungriness)
    {
        WorldEater::world = world;
        WorldEater::hungriness = hungriness;
        WorldEater::lastPos = startPos;
    }
    bool Update()
    {
        if (hungriness > 0)
        {
            auto neighbours = CheckNeighbours();
            if (neighbours.size() > 0)
            {
                int index = GetRandomValue(0, neighbours.size() - 1);
                neighbours[index].second->items.erase(neighbours[index].second->items.begin());
                lastPos = neighbours[index].first;
            }
            else
            {
                return false;
            }
        }

        return hungriness > 0;
    }

  private:
    World* world;
    int hungriness;
    TileCoords lastPos;
    std::vector<std::pair<TileCoords, TileData*>> CheckNeighbours()
    {
        std::vector<std::pair<TileCoords, TileData*>> tiles;

        for (int isY = 0; isY < 2; isY++)
        {
            for (int offset = 0; offset < 2; offset++)
            {
                if (TileData* data = world->GetTile({lastPos.x + (offset * 2 - 1) * isY, lastPos.y + (offset * 2 - 1) * !isY}))
                {
                    if (data->items.size() > 0)
                    {
                        tiles.push_back({{lastPos.x + (offset * 2 - 1) * isY, lastPos.y + (offset * 2 - 1) * !isY}, data});
                    }
                }
            }
        }

        return tiles;
    }
};
