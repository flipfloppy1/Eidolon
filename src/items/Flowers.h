#pragma once
#include "Inventory.h"
#include "ItemObject.h"
#include "MessageLog.h"
#include "World.h"

class Flowers : public ItemObject
{
  public:
    Flowers(ObjectContext context) : ItemObject(context) { kind = "generic flowers"; weight = 1; }
    bool Use(Usage* usage)
    {
        if (Throw* throwData = dynamic_cast<Throw*>(usage))
        {
            auto flowers = throwData->inv->TakeItem(throwData->itemIndex);
            context.world->GetTile({(int)throwData->origin.x, (int)throwData->origin.y})->items.push_back(flowers);
            context.messageLog->Push("The " + kind + " float harmlessly to the floor.");
        }
        else if (Attack* attackData = dynamic_cast<Attack*>(usage))
        {
            context.messageLog->Push("The " + kind + " float harmlessly to the floor.");
            if (attackData->attacker)
            {
                (*attackData->attacker)->GetInventory().TakeItem(this);
                (*attackData->attacker)->EquipItem(MAIN_HAND, nullptr);
                context.world->Create(attackData->targetPos, this);
            }
        }

        return false;
    }
};

class RedFlowers : public Flowers
{
    public:
    RedFlowers(ObjectContext context) : Flowers(context) { kind = "red flowers"; }
};

class PinkFlowers : public Flowers
{
    public:
    PinkFlowers(ObjectContext context) : Flowers(context) { kind = "pink flowers"; }
};

class WhiteFlowers : public Flowers
{
    public:
    WhiteFlowers(ObjectContext context) : Flowers(context) { kind = "white flowers"; }
};

class YellowFlowers : public Flowers
{
    public:
    YellowFlowers(ObjectContext context) : Flowers(context) { kind = "yellow flowers"; }
};
