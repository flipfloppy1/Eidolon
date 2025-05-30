#pragma once
#include "ItemObject.h"

class Statue : public ItemObject
{
  public:
    Statue(ObjectContext context) : ItemObject(context)
    {
        kind = "generic statue";
        health = 1000;
        maxHealth = health;
        weight = 1200;
        resistance = 10;
    }
};

class SnailStatue : public Statue
{
  public:
    SnailStatue(ObjectContext context) : Statue(context) { kind = "snail statue"; }
};

class CatStatue : public Statue
{
  public:
    CatStatue(ObjectContext context) : Statue(context) { kind = "cat statue"; }
};
