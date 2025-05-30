#pragma once
#include "MessageLog.h"
#include "PerlinNoise.hpp"
#include "WorldData.h"
#include "raylib.h"

class GameObject;
class WorldEater;
class RoomEater;
class Player;
class Tiles;

class World
{
  private:
    WorldData data;
    siv::BasicPerlinNoise<float> noise;
    MessageLog* messageLog;
    Tiles* tiles;
    Player* player;
    TileCoords viewport;
    TileCoords playerCoords;
    TileCoords worldCenter;
    Texture2D tileBg;
    Sound morphSound;
    int* gameScale;
    std::optional<Direction> playerDir;
    std::set<TileCoords> playerView;
    std::vector<WorldEater*> eaters;
    std::map<CreatureObject*, TileCoords> attachedCreatures;
    std::set<CreatureObject*> detachedCreatures;
    std::vector<RoomEater*> roomEaters;
    std::vector<std::pair<Texture2D, float>> GetWallTextures(TileCoords coords);
    std::vector<Direction> GetBlockingNeighbours(TileCoords const& coords);

  public:
    World(MessageLog* messageLog);
    // Safe accessor for world, returns nullptr if invalid
    TileData* GetTile(TileCoords tile)
    {
        if (tile.x >= 0 && tile.x < WORLD_WIDTH && tile.y >= 0 && tile.y < WORLD_HEIGHT)
        {
            return data[tile];
        }
        else
        {
            return nullptr;
        }
    }
    // Create an item/creature
    void Create(TileCoords position, GameObject* object);
    // Destroy an item
    void Destroy(TileCoords position, int objIndex);
    // Kill a creature
    void Kill(TileCoords position);

    // Remove a creature from the tile without deleting it
    CreatureObject* Detach(TileCoords position);

    // Remove an item from the tile without deleting it
    ItemObject* Detach(TileCoords position, ItemObject* item);

    Player* GetPlayer();
    std::vector<CreatureObject*> GetCreatures();
    Tiles* GetTileLoader();
    void MovePlayer(Direction dir);
    void SetPlayerDirection(Direction dir) { playerDir = dir; }
    void UnsetPlayerDirection() { playerDir = std::optional<Direction>(); }
    TileCoords GetPlayerPos() { return playerCoords; }
    TileCoords PlayerInteract();
    std::optional<TileCoords> GetCreaturePos(CreatureObject* creature);

    void Load(int seed, int* gameScale);

    void AddMorphable(CreatureObject* creature);

    void Unload();

    int CountNeighbours(TileCoords coords, bool cardinal = false)
    {
        int sum = 0;
        if (!cardinal)
        {
            for (int x = coords.x - 1; x < coords.x + 2; x++)
            {
                for (int y = coords.y - 1; y < coords.y + 2; y++)
                {
                    if (!(x == coords.x && y == coords.y))
                    {
                        if (TileData* tile = data[{x, y}])
                        {
                            sum += tile->items.size() > 0;
                        }
                    }
                }
            }
        }
        else
        {
            if (TileData* tile = GetTile({coords.x - 1, coords.y}))
                sum += tile->items.size() > 0;
            if (TileData* tile = GetTile({coords.x + 1, coords.y}))
                sum += tile->items.size() > 0;
            if (TileData* tile = GetTile({coords.x, coords.y + 1}))
                sum += tile->items.size() > 0;
            if (TileData* tile = GetTile({coords.x, coords.y - 1}))
                sum += tile->items.size() > 0;
        }
        return sum;
    }

    // Draw the world to the screen
    void Draw();

    // Generate the world at a given layer
    void Generate(int seed);

    // for turns
    void AdvanceTurn();

    void MoveCreature(CreatureObject* creature, TileCoords newPos);
};
