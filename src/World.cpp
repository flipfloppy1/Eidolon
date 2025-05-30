#include "World.h"
#include "Creatures.h"
#include "DungeonWall.h"
#include "GameObject.h"
#include "IceWall.h"
#include "items/Sculptor.h"
#include "items/Florist.h"
#include "Player.h"
#include "CreatureObject.h"
#include "PlayerKnowledge.h"
#include "RoomEater.h"
#include "Tiles.h"
#include "WorldData.h"
#include "WorldEater.h"
#include "raylib.h"
#include <algorithm>
#include <iostream>

#define DUNGEON_RADIUS 80.0f

World::World(MessageLog* messageLog) { World::messageLog = messageLog; }

void World::Load(int seed, int* gameScale)
{
    World::gameScale = gameScale;
    int viewportWidth = *gameScale * VIEWPORT_MIN_WIDTH;
    int viewportHeight = *gameScale * VIEWPORT_MIN_HEIGHT;
    viewport.x = GetRandomValue(0, WORLD_WIDTH - viewportWidth);
    viewport.y = GetRandomValue(0, WORLD_HEIGHT - viewportHeight);

    worldCenter = viewport + TileCoords{viewportWidth / 2, viewportHeight / 2};
    Generate(seed);

    playerCoords = {viewport.x + GetRandomValue(0, viewportWidth - 1), viewport.y + GetRandomValue(0, viewportHeight - 1)};
    while (data[playerCoords]->items.size() > 0)
    {
        std::cout << viewportWidth << ", " << viewportHeight << '\n';
        if (viewportWidth > WORLD_WIDTH / 2 || viewportHeight > WORLD_HEIGHT / 2)
        {
            (*gameScale)--;
            viewportWidth = *gameScale * VIEWPORT_MIN_WIDTH;
            viewportHeight = *gameScale * VIEWPORT_MIN_HEIGHT;
        }
        else if (viewportWidth == 0 || viewportHeight == 0)
        {
            (viewportWidth) = 15;
            (viewportHeight) = 9;
        }
        playerCoords = {viewport.x + GetRandomValue(0, viewportWidth - 1), viewport.y + GetRandomValue(0, viewportHeight - 1)};
    }
    data[playerCoords]->creature = new Human(ObjectContext{this, messageLog});
    data[playerCoords]->creature.value()->SetAsPlayer(true);
    attachedCreatures[*data[playerCoords]->creature] = playerCoords;
    tiles = new Tiles();
    tiles->Load();
    Sound morphSound = LoadSound("./assets/sounds/morph.mp3");
    player = new Player(data[playerCoords]->creature.value(), new PlayerKnowledge(this), morphSound);
    playerView = data[playerCoords]->creature.value()->See(playerCoords);
    for (auto& coords : playerView)
    {
        player->GetKnowledge()->ExploreTile(coords, GetTile(coords));
    }
    player->GetKnowledge()->SetMorphable(data[playerCoords]->creature.value()->GetKind(), data[playerCoords]->creature.value());
    player->GetCreature()->GetInventory().AddItem(new RedFlowers({this, messageLog}));
}

void World::Unload()
{
    tiles->Unload();
    // The below crashes on loading world second time -- should be investigated
    //UnloadSound(morphSound);
    std::cout << "Unloading attachedCreatures\n";
    for (auto& creature : attachedCreatures)
        delete creature.first;
    std::cout << "Unloaded attachedCreatures\n";
    std::cout << "Unloading detachedCreatures\n";
    for (auto& creature : detachedCreatures)
        if (!attachedCreatures.contains(creature))
            delete creature;
    std::cout << "Unloaded detachedCreatures\n";
}

void World::Create(TileCoords position, GameObject* obj)
{
    if (TileData* tile = GetTile(position))
    {
        if (CreatureObject* creature = dynamic_cast<CreatureObject*>(obj))
        {
            tile->creature = creature;
            attachedCreatures[creature] = position;
            auto iter = detachedCreatures.find(creature);
            if (iter != detachedCreatures.end())
                detachedCreatures.erase(iter);
        }
        else if (ItemObject* item = dynamic_cast<ItemObject*>(obj))
        {
            tile->items.push_back(item);
        }
    }
}

std::vector<CreatureObject*> World::GetCreatures()
{
    std::vector<CreatureObject*> creatures;
    for (auto& [creature, val] : attachedCreatures)
        creatures.push_back(creature);

    return creatures;
}

void World::Destroy(TileCoords position, int idx)
{
    if (TileData* tile = GetTile(position))
    {
        ItemObject* item = tile->items[idx];
        tile->items.erase(tile->items.begin() + idx);
        delete item;
    }
}

CreatureObject* World::Detach(TileCoords position)
{
    if (TileData* tile = GetTile(position))
    {
        if (tile->creature.has_value())
        {
            CreatureObject* creature = *tile->creature;
            detachedCreatures.emplace(creature);

            auto iter = attachedCreatures.find(creature);
            if (iter != attachedCreatures.end())
                attachedCreatures.erase(iter);

            tile->creature.reset();
            return creature;
        }
    }
    return nullptr;
}

Tiles* World::GetTileLoader()
{
    return tiles;
}

Player* World::GetPlayer()
{
    return player;
}

void World::Kill(TileCoords position)
{
    if (TileData* tile = GetTile(position))
    {
        if (tile->creature.has_value())
        {
            CreatureObject* creature = *tile->creature;
            tile->creature.reset();

            auto attachedIter = attachedCreatures.find(creature);
            if (attachedIter != attachedCreatures.end())
                attachedCreatures.erase(attachedIter);
            detachedCreatures.emplace(creature);
        }
    }
}
ItemObject* World::Detach(TileCoords position, ItemObject* item)
{
    if (TileData* tile = GetTile(position))
    {
        auto iter = std::find(tile->items.begin(), tile->items.end(), item);
        ItemObject* item = nullptr;
        if (iter != tile->items.end())
            item = *iter;

        tile->items.erase(iter);
        return item;
    }

    return nullptr;
}
void World::MovePlayer(Direction dir)
{
    TileData* currTile = GetTile(playerCoords);
    TileData* nextTile = GetTile(playerCoords.Move(dir));
    if (nextTile && currTile)
    {
        if (currTile->creature.value()->IsPlayer())
        {
            for (auto& item : nextTile->items)
            {
                if (item->GetWeight() > 1000)
                {
                    messageLog->Push("The " + item->GetKind() + " here is too big to move past!");
                    return;
                }
            }
            CreatureObject* player = currTile->creature.value();
            if (nextTile->creature.has_value())
            {
                CreatureObject* adjCreature = nextTile->creature.value();
                messageLog->Push("You attack the " + adjCreature->GetKind() + " with your " + player->GetEquipment(MAIN_HAND)->GetKind() + "!");
                player->Attack(adjCreature, playerCoords.Move(dir));
            }
            else
            {
                nextTile->creature = player;
                currTile->creature.reset();
                playerCoords = playerCoords.Move(dir);
                attachedCreatures[*data[playerCoords]->creature] = playerCoords;
            }
        }
    }
    else
    {
        messageLog->Push("You can't go that way.");
    }
    playerView = data[playerCoords]->creature.value()->See(playerCoords);
    for (TileCoords const& coords : playerView)
        player->GetKnowledge()->ExploreTile(coords, data[coords]);
}

void World::AddMorphable(CreatureObject* creature)
{
    player->GetKnowledge()->SetMorphable(creature->GetKind(), creature);
}

void World::Generate(int seed)
{
    noise.reseed(seed);
    SetRandomSeed(seed);
    std::vector<TileCoords> rooms;
    for (int y = 0; y < WORLD_HEIGHT; y++)
    {
        for (int x = 0; x < WORLD_WIDTH; x++)
        {
            float dist = worldCenter <=> TileCoords{x, y};
            if (noise.normalizedOctave2D(x, y, 4, 0.1) > -0.4f)
            {
                if (dist < DUNGEON_RADIUS)
                    data[{x, y}]->items.push_back(new DungeonWall({this, messageLog}));
                else
                    data[{x, y}]->items.push_back(new IceWall({this, messageLog}));
            }
            else
            {
                if (dist > DUNGEON_RADIUS)
                    eaters.push_back(new WorldEater(this, {x, y}, 100));
                else
                    rooms.push_back({x, y});
            }
        }
    }

    for (TileCoords& coords : rooms)
    {
        roomEaters.push_back(new RoomEater(this, coords, rooms, GetRandomValue(3, 12), GetRandomValue(2, 6)));
    }

    for (int y = 1; y < WORLD_HEIGHT - 1; y++)
    {
        for (int x = 1; x < WORLD_WIDTH - 1; x++)
        {
            if (TileData* tData = data[{x, y}])
            {
                if (tData->items.size() > 0)
                {
                    if (CountNeighbours({x, y}) < 3) // Remove small clusters of tiles
                    {
                        tData->items.erase(tData->items.begin());
                    }
                }
            }
        }
    }
    tileBg = LoadTexture("./assets/images/TileBackground.png");
    for (auto& eater : eaters)
    {
        while (eater->Update())
        {
        }
    }

    for (auto& eater : roomEaters)
    {
        eater->Eat();
    }

    for (int y = 1; y < WORLD_HEIGHT - 1; y++)
    {
        for (int x = 1; x < WORLD_WIDTH - 1; x++)
        {
            if (TileData* tData = data[{x, y}])
            {
                if (tData->items.size() > 0)
                {
                    if (CountNeighbours({x, y}, true) < 2 && tData->items[0]->GetKind() != "ice wall" ||
                        CountNeighbours({x, y}) < 2 && tData->items[0]->GetKind() == "ice wall") // Remove small clusters of tiles
                    {
                        tData->items.erase(tData->items.begin());
                    }
                }
            }
        }
    }

    // Place statues in dungeon
    Sculptor sculptor = Sculptor({this, messageLog});
    for (int i = 0; i < 10; i++)
    {
        bool placed = false;
        while (!placed)
        {
            TileCoords pos = {GetRandomValue(worldCenter.x - DUNGEON_RADIUS, worldCenter.x + DUNGEON_RADIUS), GetRandomValue(worldCenter.y - DUNGEON_RADIUS, worldCenter.y + DUNGEON_RADIUS)};
            if (pos <=> worldCenter < DUNGEON_RADIUS)
            {
                if (TileData* tile = GetTile(pos))
                {
                    if (!tile->Blocking())
                    {
                        tile->items.push_back(sculptor.Create());
                        placed = true;
                    }
                }
            }
        }
    }

    // Place a fountain somewhere
    Florist florist = Florist({this, messageLog});
    bool fountainPlaced = false;
    while (!fountainPlaced)
    {
        TileCoords pos = {GetRandomValue(worldCenter.x - DUNGEON_RADIUS, worldCenter.x + DUNGEON_RADIUS), GetRandomValue(worldCenter.y - DUNGEON_RADIUS, worldCenter.y + DUNGEON_RADIUS)};
        if (pos <=> worldCenter < DUNGEON_RADIUS)
        {
            if (TileData* tile = GetTile(pos))
            {
                if (!tile->Blocking() && GetBlockingNeighbours(pos).size() == 0)
                {
                    tile->items.push_back(new Fountain({this, messageLog}));
                    Direction flow1 = (Direction)GetRandomValue(0, 7);
                    Direction flow2 = flow1;
                    while (flow1 == flow2)
                        flow2 = (Direction)GetRandomValue(0, 7);

                    if (TileData* flowTile1 = GetTile(pos.Move(flow1)))
                    {
                        flowTile1->items.push_back(florist.Create());
                    }
                    if (TileData* flowTile2 = GetTile(pos.Move(flow2)))
                    {
                        flowTile2->items.push_back(florist.Create());
                    }
                    fountainPlaced = true;
                }
            }
        }
    }
    // spawn goblins
    for (int i = 0; i < 50; i++)
    {
        bool placed = false;
        while (!placed)
        {
            TileCoords pos = {GetRandomValue(worldCenter.x - DUNGEON_RADIUS, worldCenter.x + DUNGEON_RADIUS), GetRandomValue(worldCenter.y - DUNGEON_RADIUS, worldCenter.y + DUNGEON_RADIUS)};
            if (pos <=> worldCenter < DUNGEON_RADIUS)
            {
                if (TileData* tile = GetTile(pos))
                {
                    if (!tile->Blocking() && !tile->creature.has_value())
                    {
                        Goblin* goblin = new Goblin({this, messageLog});
                        tile->creature = goblin;
                        attachedCreatures[goblin] = pos;
                        placed = true;
                    }
                }
            }
        }
    }
    // spawn slimes
        for (int i = 0; i < 25; i++)
    {
        bool placed = false;
        while (!placed)
        {
            TileCoords pos = {GetRandomValue(worldCenter.x - DUNGEON_RADIUS, worldCenter.x + DUNGEON_RADIUS), GetRandomValue(worldCenter.y - DUNGEON_RADIUS, worldCenter.y + DUNGEON_RADIUS)};
            if (pos <=> worldCenter < DUNGEON_RADIUS)
            {
                if (TileData* tile = GetTile(pos))
                {
                    if (!tile->Blocking() && !tile->creature.has_value())
                    {
                        Slime* slime = new Slime({this, messageLog});
                        tile->creature = slime;
                        attachedCreatures[slime] = pos;
                        placed = true;
                    }
                }
            }
        }
    }
}

std::vector<Direction> World::GetBlockingNeighbours(TileCoords const& coords)
{
    std::vector<Direction> dir;

    for (int i = 0; i < 8; i++)
    {
        TileCoords dirCoords = coords.Move((Direction)i);
        TileData* tile = GetTile(dirCoords);
        if (tile)
        {
            bool isBlocking = false;
            for (auto const& item : tile->items)
                isBlocking |= item->GetWeight() > 1000;

            if (isBlocking)
            {
                dir.push_back((Direction)i);
            }
        }
        else
        {
            dir.push_back((Direction)i);
        }
    }

    return dir;
}

std::vector<std::pair<Texture2D, float>> World::GetWallTextures(TileCoords coords)
{
    std::string wallName;
    std::string suffix;
    std::string kind;
    std::vector<std::pair<Texture2D, float>> tileRender;
    for (auto& item : data[coords]->items)
        if (item->GetWeight() >= 10000)
            kind = item->GetKind();

    if (kind == "")
        return tileRender;

    std::vector<Direction> neighbours = GetBlockingNeighbours(coords);
    auto contains = [&neighbours](Direction dir) { return std::find(neighbours.begin(), neighbours.end(), dir) != neighbours.end(); };

    if (neighbours.size() == 8 || (contains(NORTH) && contains(SOUTH) && contains(EAST) && contains(WEST)))
    {
        suffix = "empty";
        wallName = "";
    }

    if (kind.size() > 5)
        wallName = kind.substr(0, kind.size() - 5);

    tileRender.push_back({tiles->GetTile(wallName + "/wall").value(), 0.0f});

    if (!contains(NORTH))
    {
        if (contains(WEST) && contains(EAST))
            tileRender.push_back({tiles->GetTile(wallName + "/side_full").value(), 0.0f});
        else if (contains(EAST))
            tileRender.push_back({tiles->GetTile(wallName + "/side_left").value(), 0.0f});
        else if (contains(WEST))
            tileRender.push_back({tiles->GetTile(wallName + "/side_right").value(), 0.0f});
        else
            tileRender.push_back({tiles->GetTile(wallName + "/side_both").value(), 0.0f});
    }
    if (!contains(SOUTH))
    {
        if (contains(WEST) && contains(EAST))
            tileRender.push_back({tiles->GetTile(wallName + "/side_full").value(), 180.0f});
        else if (contains(EAST))
            tileRender.push_back({tiles->GetTile(wallName + "/side_right").value(), 180.0f});
        else if (contains(WEST))
            tileRender.push_back({tiles->GetTile(wallName + "/side_left").value(), 180.0f});
        else
            tileRender.push_back({tiles->GetTile(wallName + "/side_both").value(), 180.0f});
    }
    if (!contains(WEST))
    {
        if (contains(NORTH) && contains(SOUTH))
            tileRender.push_back({tiles->GetTile(wallName + "/side_full").value(), 270.0f});
        else if (contains(SOUTH))
            tileRender.push_back({tiles->GetTile(wallName + "/side_right").value(), 270.0f});
        else if (contains(NORTH))
            tileRender.push_back({tiles->GetTile(wallName + "/side_left").value(), 270.0f});
        else
            tileRender.push_back({tiles->GetTile(wallName + "/side_both").value(), 270.0f});
    }
    if (!contains(EAST))
    {
        if (contains(NORTH) && contains(SOUTH))
            tileRender.push_back({tiles->GetTile(wallName + "/side_full").value(), 90.0f});
        else if (contains(SOUTH))
            tileRender.push_back({tiles->GetTile(wallName + "/side_left").value(), 90.0f});
        else if (contains(NORTH))
            tileRender.push_back({tiles->GetTile(wallName + "/side_right").value(), 90.0f});
        else
            tileRender.push_back({tiles->GetTile(wallName + "/side_both").value(), 90.0f});
    }
    if (!contains(NORTHEAST) && contains(NORTH) && contains(EAST))
        tileRender.push_back({tiles->GetTile(wallName + "/corner").value(), 0.0f});
    if (!contains(SOUTHEAST) && contains(SOUTH) && contains(EAST))
        tileRender.push_back({tiles->GetTile(wallName + "/corner").value(), 90.0f});
    if (!contains(SOUTHWEST) && contains(SOUTH) && contains(WEST))
        tileRender.push_back({tiles->GetTile(wallName + "/corner").value(), 180.0f});
    if (!contains(NORTHWEST) && contains(NORTH) && contains(WEST))
        tileRender.push_back({tiles->GetTile(wallName + "/corner").value(), 270.0f});

    if (kind == "dungeon wall")
    {
    }
    else if (kind == "ice wall")
    {
    }

    return tileRender;
}

TileCoords World::PlayerInteract()
{
    if (playerDir)
        return playerCoords.Move(*playerDir);
    else
        return playerCoords;
}

void World::Draw()
{
    bool yOff = GetRenderWidth() / GetRenderHeight() > VIEWPORT_MIN_WIDTH / VIEWPORT_MIN_HEIGHT;
    int renderBorders;
    if (yOff)
        renderBorders = (float)GetRenderWidth() - GetRenderHeight() * (float)(VIEWPORT_MIN_WIDTH / (float)VIEWPORT_MIN_HEIGHT);
    else
        renderBorders = (float)GetRenderHeight() - GetRenderWidth() * (float)(VIEWPORT_MIN_HEIGHT / (float)VIEWPORT_MIN_WIDTH);

    int viewportWidth = *gameScale * VIEWPORT_MIN_WIDTH;
    int viewportHeight = *gameScale * VIEWPORT_MIN_HEIGHT;
    Vector2 renderOffset = {yOff ? renderBorders / 2.0f : 0.0f, !yOff ? renderBorders / 2.0f : 0.0f};
    Vector2 renderDims = {(float)GetRenderWidth() - renderOffset.x * 2.0f, (float)GetRenderHeight() - renderOffset.y * 2.0f};
    viewport.x = std::max(std::min(playerCoords.x - viewportWidth / 2, WORLD_WIDTH - viewportWidth), 0);

    viewport.y = std::max(std::min(playerCoords.y - viewportHeight / 2, WORLD_HEIGHT - viewportHeight), 0);

    const float dotsWidth = 20.0f;
    const float dotsHeight = 20.0f;
    DrawTexturePro(tileBg, Rectangle{0, 0, (float)tileBg.width * dotsWidth, (float)tileBg.height * dotsHeight},
                   Rectangle{renderOffset.x + std::round(renderDims.x / viewportWidth) * (playerCoords.x - viewport.x - dotsWidth / 2.0f) + std::round(renderDims.x / viewportWidth) / 2.0f,
                             renderOffset.y + std::round(renderDims.y / viewportHeight) * (playerCoords.y - viewport.y - dotsHeight / 2.0f) + std::round(renderDims.y / viewportHeight) / 2.0f,
                             std::round(renderDims.x / viewportWidth) * dotsWidth, std::round(renderDims.y / viewportHeight) * dotsHeight},
                   {std::round(renderDims.x / viewportWidth / 2.0f), std::round(renderDims.y / viewportHeight / 2.0f)}, 0.0f, Color{255, 255, 255, 255});
    for (int y = viewport.y; y < viewport.y + viewportHeight; y++)
    {
        for (int x = viewport.x; x < viewport.x + viewportWidth; x++)
        {
            // if (GetTile({x, y})->items.size())
            // {
            //     for (auto const& item : GetTile({x, y})->items)
            //     {
            //         if (item->GetKind() == "dungeon wall")
            //         {
            //             DrawRectangle(std::round(renderDims.x / viewportWidth) * (x - viewport.x), std::round(renderDims.y / viewportHeight) * (y - viewport.y),
            //                           std::round(renderDims.x / viewportWidth), std::round(renderDims.y / viewportHeight), Color{52, 41, 52, 255});
            //         }
            //         else if (item->GetKind() == "ice wall")
            //         {
            //             DrawRectangle(std::round(renderDims.x / viewportWidth) * (x - viewport.x), std::round(renderDims.y / viewportHeight) * (y - viewport.y),
            //                           std::round(renderDims.x / viewportWidth), std::round(renderDims.y / viewportHeight), Color{52, 110, 160, 255});
            //         }
            //     }
            // }
            if (playerView.contains({x, y}))
            {
                for (auto const& item : data[{x, y}]->items)
                {
                    std::string wallName;
                    std::string kind = item->GetKind();
                    if (kind.size() > 5)
                    {
                        wallName = kind.substr(0, kind.size() - 6);
                    }
                    if (item->GetKind() == "dungeon wall")
                    {
                        auto wallTexes = GetWallTextures({x, y});
                        for (auto& [tex, rotation] : wallTexes)
                        {
                            DrawTexturePro(tex, Rectangle{0, 0, (float)tex.width, (float)tex.height},
                                           Rectangle{renderOffset.x + std::round(renderDims.x / viewportWidth) * (x - viewport.x) + std::round(renderDims.x / viewportWidth) / 2.0f,
                                                     renderOffset.y + std::round(renderDims.y / viewportHeight) * (y - viewport.y) + std::round(renderDims.y / viewportHeight) / 2.0f,
                                                     std::round(renderDims.x / viewportWidth), std::round(renderDims.y / viewportHeight)},
                                           {std::round(renderDims.x / viewportWidth / 2.0f), std::round(renderDims.y / viewportHeight / 2.0f)}, rotation, Color{255, 255, 255, 255});
                        }
                        if (!(wallTexes.size() > 0))
                        {
                            DrawRectangle(renderOffset.x + std::round(renderDims.x / viewportWidth) * (x - viewport.x), renderOffset.y + std::round(renderDims.y / viewportHeight) * (y - viewport.y),
                                          std::round(renderDims.x / viewportWidth), std::round(renderDims.y / viewportHeight), Color{52, 41, 52, 255});
                        }
                    }
                    else if (item->GetKind() == "ice wall")
                    {
                        DrawRectangle(renderOffset.x + std::round(renderDims.x / viewportWidth) * (x - viewport.x), renderOffset.y + std::round(renderDims.y / viewportHeight) * (y - viewport.y),
                                      std::round(renderDims.x / viewportWidth), std::round(renderDims.y / viewportHeight), Color{52, 110, 160, 255});
                    }
                    else
                    {
                        std::optional<Texture2D> tex = tiles->GetTile(item->GetKind());
                        if (tex.has_value())
                        {
                            DrawTexturePro(tex.value(), Rectangle{0, 0, (float)tex->width, (float)tex->height},
                                           Rectangle{renderOffset.x + std::round(renderDims.x / viewportWidth) * (x - viewport.x) + std::round(renderDims.x / viewportWidth) / 2.0f,
                                                     renderOffset.y + std::round(renderDims.y / viewportHeight) * (y - viewport.y) + std::round(renderDims.y / viewportHeight) / 2.0f,
                                                     std::round(renderDims.x / viewportWidth), std::round(renderDims.y / viewportHeight)},
                                           {std::round(renderDims.x / viewportWidth) / 2.0f, std::round(renderDims.y / viewportHeight) / 2.0f}, 0.0f, Color{255, 255, 255, 255});
                        }
                    }
                }
            }
            else
            {
                switch (player->GetKnowledge()->GetTileKnowledge({x, y}))
                {
                case EMPTY:
                    DrawRectangle(renderOffset.x + std::round(renderDims.x / viewportWidth) * (x - viewport.x), renderOffset.y + std::round(renderDims.y / viewportHeight) * (y - viewport.y),
                                  std::round(renderDims.x / viewportWidth), std::round(renderDims.y / viewportHeight), Color{0, 0, 0, 255});
                    break;
                default:
                    auto wallTexes = GetWallTextures({x, y});
                    for (auto& [tex, rotation] : wallTexes)
                    {
                        DrawTexturePro(tex, Rectangle{0, 0, (float)tex.width, (float)tex.height},
                                       Rectangle{renderOffset.x + std::round(renderDims.x / viewportWidth) * (x - viewport.x) + std::round(renderDims.x / viewportWidth) / 2.0f,
                                                 renderOffset.y + std::round(renderDims.y / viewportHeight) * (y - viewport.y) + std::round(renderDims.y / viewportHeight) / 2.0f,
                                                 std::round(renderDims.x / viewportWidth), std::round(renderDims.y / viewportHeight)},
                                       {std::round(renderDims.x / viewportWidth) / 2.0f, std::round(renderDims.y / viewportHeight) / 2.0f}, rotation, Color{60, 60, 60, 255});
                    }
                }
            }
            if (GetTile({x, y})->creature.has_value() && playerView.contains({x,y}))
            {
                Texture2D tileTexture = tiles->GetTile(GetTile({x, y})->creature.value()->GetKind()).value();
                DrawTexturePro(tileTexture, Rectangle{0, 0, (float)tileTexture.width, (float)tileTexture.height},
                               Rectangle{renderOffset.x + std::round(renderDims.x / viewportWidth) * (x - viewport.x) + std::round(renderDims.x / viewportWidth) / 2.0f,
                                         renderOffset.y + std::round(renderDims.y / viewportHeight) * (y - viewport.y) + std::round(renderDims.y / viewportHeight) / 2.0f,
                                         std::round(renderDims.x / viewportWidth), std::round(renderDims.y / viewportHeight)},
                               {std::round(renderDims.x / viewportWidth) / 2.0f, std::round(renderDims.y / viewportHeight) / 2.0f}, 0.0f, Color{255, 255, 255, 255});
            }
        }
    }

    if (playerDir)
    {
        TileCoords coords = playerCoords.Move(*playerDir);
        auto tex = tiles->GetTile("arrow");
        DrawTexturePro(*tex, Rectangle{0, 0, (float)tex->width, (float)tex->height},
                       Rectangle{renderOffset.x + std::round(renderDims.x / viewportWidth) * (coords.x - viewport.x) + std::round(renderDims.x / viewportWidth) / 2.0f,
                                 renderOffset.y + std::round(renderDims.y / viewportHeight) * (coords.y - viewport.y) + std::round(renderDims.y / viewportHeight) / 2.0f, std::round(renderDims.x / viewportWidth),
                                 std::round(renderDims.y / viewportHeight)},
                       {std::round(renderDims.x / viewportWidth) / 2.0f, std::round(renderDims.y / viewportHeight) / 2.0f}, (float)(*playerDir) * 45.0f - 90.0f, Color{255, 255, 255, 255});
    }
    // Draw aspect ratio bars over everything else
    DrawRectangle(0, 0, !yOff ? GetRenderWidth() : renderOffset.x, !yOff ? renderOffset.y : GetRenderHeight(), Color{0,0,0,255});
    DrawRectangle(renderOffset.x + (yOff ? renderDims.x : 0.0f), renderOffset.y + (!yOff ? renderDims.y : 0.0f), !yOff ? GetRenderWidth() : renderOffset.x, !yOff ? renderOffset.y : GetRenderHeight(), Color{0,0,0,255});
}

void World::AdvanceTurn() {
    for (auto& [creature,_] : attachedCreatures) {
        if (!creature || creature->GetHealth() == 0) continue; // skip dead or null creatures
        if (creature->IsPlayer()) continue; // skip player
        creature->TakeTurn(playerCoords);
    }
}

std::optional<TileCoords> World::GetCreaturePos(CreatureObject* creature)
{
    if (attachedCreatures.contains(creature))
        return attachedCreatures.at(creature);

    return std::optional<TileCoords>();
}

void World::MoveCreature(CreatureObject* creature, TileCoords newPos)
{
    TileCoords oldPos = newPos;
    if (GetCreaturePos(creature))
        oldPos = *GetCreaturePos(creature);

    if (newPos == oldPos)
        return; // no movement

    TileData* oldTile = GetTile(oldPos);
    TileData* newTile = GetTile(newPos);

    // check validity of move
    if (!newTile || newTile->Blocking() || newTile->creature.has_value())
    {
        return;
    }

    // detach from old tile if it contains the creature
    if (oldTile && oldTile->creature.has_value() && oldTile->creature.value() == creature)
    {
        oldTile->creature.reset();
    }

    // attach to new tile
    newTile->creature = creature;
    attachedCreatures[creature] = newPos;
}

std::vector<CreatureObject*> allCreatures;
