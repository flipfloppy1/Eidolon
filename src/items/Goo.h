#pragma once

#include "ItemObject.h"

class Goo : public ItemObject
{
  public:
    Goo(ObjectContext context) : ItemObject(context)
    {
        health = 1;
        maxHealth = 1;
        resistance = 1;
        kind = "goo";
    }
};
