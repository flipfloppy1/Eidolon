#pragma once
#include "ItemObject.h"

class Fountain : public ItemObject
{
  public:
    Fountain(ObjectContext context) : ItemObject(context)
    {
        kind = "flowing fountain";
        health = 1000;
        maxHealth = health;
        weight = 1200;
        resistance = 10;
    }
};
