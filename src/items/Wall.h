#pragma once
#include "ItemObject.h"

class Wall : public ItemObject
{
  public:
    Wall(ObjectContext context) : ItemObject(context)
    {
        kind = "generic wall";
        health = 10000;
        maxHealth = health;
        weight = 10000;
        resistance = 15;
    }
};
