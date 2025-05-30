#include "ItemObject.h"
#include "Inventory.h"
#include <cmath>
#include <algorithm>
#include "World.h"

bool ItemObject::Use(Usage* usage)
{
    if (Throw* throwData = dynamic_cast<Throw*>(usage))
    {
        throwData->inv->TakeItem(throwData->itemIndex);
        Vector2 origin = throwData->origin;
        Vector2 traj = throwData->trajectory;
        int viewDistance = throwData->strength;
        int accuracy = throwData->accuracy;
        traj.x += (GetRandomValue(-10, 10) / 100.0f) / std::max(throwData->accuracy, 1);
        traj.y += (GetRandomValue(-10, 10) / 100.0f) / std::max(throwData->accuracy, 1);
        Vector2 target = {traj.x * viewDistance + origin.x, traj.y * viewDistance + origin.y};
        traj = {traj.x + origin.x, traj.y + origin.y};
        float dx = (target.x - traj.x);
        float dy = (target.y - traj.y);
        float x,y;
        float step;
        int i = 0;

        if (std::fabsf(dx) >= std::fabsf(dy))
            step = std::fabsf(dx);
        else
            step = std::fabsf(dy);

        dx = dx / step;
        dy = dy / step;
        x = traj.x;
        y = traj.y;

        GameObject* targetObj = nullptr;
        while (i <= step)
        {
            TileCoords currCoords = {(int)std::round(x),(int)std::round(y)};
            if (TileData* tile = context.world->GetTile(currCoords))
            {
                if (tile->items.size() > 0)
                {
                    int maxWeight = tile->items[0]->GetWeight();
                    int idx = 0;
                    for (int i = 0; i < tile->items.size(); i++)
                    {
                        if (tile->items[i]->GetWeight() > maxWeight)
                        {
                            maxWeight = tile->items[i]->GetWeight();
                            idx = i;
                        }
                    }
                    if (tile->creature.has_value())
                    {
                        if (tile->creature.value()->GetWeight() > maxWeight)
                            targetObj = tile->creature.value();
                        else
                            targetObj = tile->items[idx];
                    }
                    else
                    {
                        targetObj = tile->items[idx];
                    }
                    break;
                }
                else
                {
                    continue;
                }
            }
            else
            {
                break;
            }
            x += dx;
            y += dy;
            i++;
        }
        if (targetObj != nullptr)
        {
            int pen;
            if (stats.contains("sharpness") && stats.contains("sharpbonus"))
                pen = std::min(throwData->strength + stats["sharpbonus"], stats["sharpness"]);
            else
                pen = std::min(throwData->strength, 4);
            struct Damage dmg = {};
            dmg.penetration = pen;
            dmg.type = PHYSICAL;
            for (int i = 0; i < stats["dmgnum"]; i++)
            {
                dmg.value += GetRandomValue(1, stats["dmgsize"]);
            }
            targetObj->Damage(dmg);
        }
        return true;
    }
    else if (Attack* attackData = dynamic_cast<Attack*>(usage))
    {
        int pen = std::min(attackData->strength + stats["sharpbonus"], stats["sharpness"]);
        struct Damage dmg = {};
        dmg.penetration = pen;
        dmg.type = PHYSICAL;
        for (int i = 0; i < stats["dmgnum"]; i++)
        {
            dmg.value += GetRandomValue(1, stats["dmgsize"]);
        }
        attackData->target->Damage(dmg);
        if (attackData->target->GetHealth() == 0)
        {
            if (attackData->attacker.has_value() && attackData->attacker.value()->IsPlayer())
            {
                if (CreatureObject* creature = dynamic_cast<CreatureObject*>(attackData->target))
                {
                    context.world->AddMorphable(context.world->Detach(attackData->targetPos));
                }
            }
            else if (attackData->attacker.has_value())
            {
                context.world->Kill(attackData->targetPos);
            }
        }
        return true;
    }

    return false;
};
