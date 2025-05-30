#include "World.h"

class PlayerKnowledge
{
    private:
    TileType knownTiles[WORLD_WIDTH * WORLD_HEIGHT] = {};
    World* world;
    std::map<std::string, CreatureObject*> morphables;
    public:
    void SetMorphable(std::string str, CreatureObject* creature)
    {
        if (!morphables.contains(str))
        {
            morphables[str] = creature;
        }
    }

    PlayerKnowledge(World* world)
    {
        PlayerKnowledge::world = world;
    }
    TileType GetTileKnowledge(TileCoords coords)
    {
        return knownTiles[coords.x * WORLD_HEIGHT + coords.y];
    }
    std::map<std::string, CreatureObject*> GetMorphables()
    {
        return morphables;
    }
    void ExploreTile(TileCoords coords, TileData* tile)
    {
        TileType type = EMPTY;
        if (!tile->items.empty())
        {
            for (auto& item: tile->items)
            {
                if (item->GetKind() == "dungeon wall")
                {
                    type = DUNGEON;
                    break;
                }
                else if (item->GetKind() == "ice wall")
                {
                    type = ICE;
                    break;
                }
            }
        }

        knownTiles[coords.x * WORLD_HEIGHT + coords.y] = type;
    }
};
