
#include "World.h"
#include "raylib.h"
#include <limits>
class RoomEater
{
  public:
    RoomEater(World* world, TileCoords startPos, std::vector<TileCoords> rooms, int roomWidth, int roomHeight)
    {
        RoomEater::world = world;
        RoomEater::rooms = rooms;
        RoomEater::startPos = startPos;
        RoomEater::roomWidth = roomWidth;
        RoomEater::roomHeight = roomHeight;
    }
    void Eat()
    {
        for (int x = startPos.x; x < startPos.x + roomWidth; x++)
        {
            for (int y = startPos.y; y < startPos.y + roomHeight; y++)
            {
                if (TileData* tile = world->GetTile({x, y}))
                {
                    if (tile->items.size() > 0)
                        tile->items.erase(tile->items.begin());
                }
            }
        }

        float maxFloat = std::numeric_limits<float>::max();
        float closestRoomDist[5] = {maxFloat, maxFloat, maxFloat, maxFloat, maxFloat};
        for (auto& room : rooms)
        {
            float dist = room <=> startPos;
            for (int i = 0; i < 5; i++)
            {
                if (dist < closestRoomDist[i])
                {
                    closestRoomDist[i] = dist;
                    closestRooms[i] = room;
                    break;
                }
            }
        }
        TileCoords targetRoom = closestRooms[GetRandomValue(0, 4)];
        for (float p = 0.0f; p < 1.0f; p += 0.01f)
        {
            float x = startPos.x + (targetRoom.x - startPos.x) * p;
            float y = startPos.y + (targetRoom.y - startPos.y) * ((x - startPos.x) / (targetRoom.x - startPos.x));
            TileData* tile = world->GetTile({(int)x, (int)y});
            if (tile)
                if (tile->items.size() > 0)
                    tile->items.erase(tile->items.begin());
            TileData* tile2 = world->GetTile({(int)(x + 1), (int)y});
            if (tile2)
                if (tile2->items.size() > 0)
                    tile2->items.erase(tile2->items.begin());
        }
    }

  private:
    World* world;
    int roomWidth;
    int roomHeight;
    TileCoords closestRooms[5];
    TileCoords startPos;
    std::vector<TileCoords> rooms;
};
