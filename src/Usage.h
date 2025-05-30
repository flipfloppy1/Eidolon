#pragma once
#include "raylib.h"
#include "WorldData.h"

class GameObject;
class CreatureObject;
class ItemObject;
class Inventory;

// Parent class for interactions with items
class Usage
{
    // Does nothing, but marks Usage as polymorphic
    virtual void polymorphicMarker() {};
};

class Consume : public Usage
{
    public:
    Inventory* inv;
    CreatureObject* consumer;
};

class Attack : public Usage
{
    public:
    int strength;
    std::optional<CreatureObject*> attacker;
    GameObject* target;
    TileCoords targetPos;
};

class Throw : public Usage
{
    public:
    Inventory* inv;
    int itemIndex;
    int strength;
    int accuracy;
    Vector2 trajectory;
    Vector2 origin;
};

class Fire : public Usage
{
    public:
    Inventory* inv;
    ItemObject* ammo;
    int accuracy;
};
