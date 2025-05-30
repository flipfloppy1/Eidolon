#pragma once
#include "raylib.h"
#include "Damage.h"

class GameObject;
class CreatureObject;
class ItemObject;
class Inventory;

// Parent class for interactions with items
class Ability
{
    // Does nothing, but marks Ability as polymorphic
    virtual void polymorphicMarker() {};
    public:
        virtual ~Ability() = default;
};

class Shout : public Ability
{
    public:
    int strength;
    float radius;
    Damage damage;
    Vector2 origin;
    int cooldown;
};

class Spray : public Ability
{
    public:
    Damage damage;
    Vector2 origin;
    Vector2 direction;
    int distance;
    int spread;
    int cooldown;
};

class Talk : public Ability
{
    public:
    CreatureObject* recipient;
};
