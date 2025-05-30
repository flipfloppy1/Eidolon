#include "CreatureObject.h"
#include "WorldData.h"
#include "World.h"
#include <cmath>
#include <set>
#include "Fist.h"
#include "Usage.h"
#include <queue>
#include <map>
#include <vector>
#include <algorithm>

#define SEEALL false

typedef std::set<TileCoords> WorldView;

CreatureObject::CreatureObject(ObjectContext context): GameObject(context)
{
    equipment[MAIN_HAND] = new Fist(context);
    defaultEquipment[MAIN_HAND] = equipment[MAIN_HAND];
    stats["strength"] = 4;
    kind = "generic creature";
    isPlayer = false;
}

void CreatureObject::Attack(GameObject* target, TileCoords targetPos)
{
    class Attack* attackData = new class Attack();
    attackData->strength = stats["strength"];
    attackData->target = target;
    attackData->attacker = this;
    attackData->targetPos = targetPos;
    equipment[MAIN_HAND]->Use(attackData);
    delete attackData;
}

ItemObject* CreatureObject::GetEquipment(BodyParts part)
{
    if (equipment.contains(part))
        return equipment.at(part);
    else
        return nullptr;
}

void CreatureObject::Use(ItemObject* item, Usage* usage)
{
    context.messageLog->Push(this, usage, nullptr, item);
    bool success = item->Use(usage);
    if (!success)
    {
        context.messageLog->Push("The action failed!");
    }
}

bool CreatureObject::Do(Ability* ability)
{
    if (Shout* shout = dynamic_cast<Shout*>(ability))
    {
        return true;
    }
    else if (Spray* spray = dynamic_cast<Spray*>(ability))
    {
        return true;
    }
    return false;
}

bool CreatureObject::EquipItem(BodyParts part, ItemObject* item)
{
    if (item)
    {
        if (!inv.Contains(item))
            inv.AddItem(item);

        equipment[part] = item;
    }
    else
    {
        equipment[part] = defaultEquipment[part];
    }

    return true;
}

WorldView CreatureObject::See(TileCoords viewpoint)
{
    // Default sight implementation, copy and modify in derived classes as needed
    const int viewDistance = 5;
    std::set<TileCoords> view = std::set<TileCoords>();

    #if SEEALL
    for (int x = 0; x < WORLD_WIDTH; x++)
    {
        for (int y = 0; y < WORLD_HEIGHT; y++)
        {
            view.insert({x, y});
        }
    }
    return view;
    #endif

    for (float xComp = -1.0f; xComp <= 1.0f; xComp += 0.025f)
    {
        for (float yComp = 1.0f; yComp >= -1.0f; yComp -= 0.025f)
        {
            Vector2 curr = {xComp, yComp};
            Vector2 target = {xComp * viewDistance + viewpoint.x, yComp * viewDistance + viewpoint.y};
            curr = {xComp + viewpoint.x, yComp + viewpoint.y};
            float dx = (target.x - curr.x);
            float dy = (target.y - curr.y);
            float x,y;
            float step;
            int i = 0;

            if (std::fabsf(dx) >= std::fabsf(dy))
                step = std::fabsf(dx);
            else
                step = std::fabsf(dy);

            dx = dx / step;
            dy = dy / step;
            x = curr.x;
            y = curr.y;

            while (i <= step)
            {
                TileCoords currCoords = {(int)std::round(x),(int)std::round(y)};
                if (currCoords <=> curr > viewDistance) break;
                if (TileData* tile = context.world->GetTile(currCoords))
                {
                    view.insert(currCoords);

                    if (tile->Blocking())
                        break;
                }
                else
                {
                    break;
                }
                x += dx;
                y += dy;
                i++;
            }
        }
    }
    CreatureObject::currView = view;
    return view;
}

std::vector<TileCoords> CreatureObject::AStar(TileCoords start,TileCoords goal)
{
    struct  Node
    {
        TileCoords coords;
        int g, f;
        Node* parent;
    };

    auto heuristic = [](TileCoords a, TileCoords b)
    {
        return abs(a.x - b.x) + abs(a.y - b.y); // manhattan distance
    };

    auto compare = [](Node* a, Node* b)
    {
        return a->f > b->f;
    };

    std::priority_queue<Node*, std::vector<Node*>, decltype(compare)> open(compare);
    std::map<TileCoords, Node*> allNodes;
    std::set<TileCoords> closed;

    Node* startNode = new Node{start, 0, heuristic(start, goal), nullptr};
    open.push(startNode);
    allNodes[start] = startNode;

    while (!open.empty())
    {
        Node* current = open.top();
        open.pop();
        if (current->coords == goal)
        {
            std::vector<TileCoords> path;
            while (current)
            {
                path.push_back(current->coords);
                current = current->parent;
            }
            std::reverse(path.begin(), path.end());
            for (auto& [_, node] : allNodes) delete node;
            return path;
        }
        closed.insert(current->coords);

        for (int dx = -1; dx <= 1; dx++)
        {
            for (int dy = -1; dy <= 1; dy++)
            {
                if (std::abs(dx) + std::abs(dy) != 1) continue;
                TileCoords neighbour = {current->coords.x + dx, current->coords.y + dy};
                if (closed.find(neighbour) != closed.end()) continue;
                TileData* tile = context.world->GetTile(neighbour);
                if (!tile || tile->Blocking()) continue;
                int g = current->g + 1;
                int f = g + heuristic(neighbour, goal);
                if (allNodes.find(neighbour) != allNodes.end() && g >= allNodes[neighbour]->g) continue;
                Node* next = new Node{neighbour, g, f, current};
                open.push(next);
                allNodes[neighbour] = next;
            }
        }
    }

    for (auto& [_, node] : allNodes) delete node;
    return {};
}


void CreatureObject::TakeTurn(TileCoords playerPos)
{
    TileCoords position = *context.world->GetCreaturePos(this);
    if (position <=> playerPos > 20.0f)
        return;
    WorldView view = See(position);
    if (view.find(playerPos) != view.end())
    {
        std::vector<TileCoords> path = AStar(position, playerPos);
        if (path.size() > 1)
        {
            TryMove(path[1]); // move only one step towards the player
        }
    }
    else
    {
        // walk randomly
        std::vector<TileCoords> validNeighbours;

        for (int dx = -1; dx <= 1; dx++)
        {
            for (int dy = -1; dy <= 1; dy++)
            {
                if (dx == 0 && dy == 0) continue;

                TileCoords neighbour = {position.x + dx, position.y +dy};
                TileData* tile = context.world->GetTile(neighbour);

                if (tile && !tile->Blocking())
                {
                    validNeighbours.push_back(neighbour);
                }
            }
        }

        if (!validNeighbours.empty())
        {
            TileCoords target = validNeighbours[GetRandomValue(0,validNeighbours.size()-1)];
            TryMove(target);
        }
    }
}

bool CreatureObject::TryMove(TileCoords target)
{
    TileData* tile = context.world->GetTile(target);
    if (!tile || tile->Blocking() || tile->creature.has_value())
    {
        return false;
    }
    context.world->MoveCreature(this, target);
    return true;
}
