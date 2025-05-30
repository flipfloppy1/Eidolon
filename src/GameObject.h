#pragma once
#include "Damage.h"
#include <string>
#include "raylib.h"
#include <map>

class World;
class MessageLog;

struct ObjectContext
{
    World* world;
    MessageLog* messageLog;
};

// The most abstract representation of a "thing" in the game world
class GameObject
{
  public:
    // Used to inflict damage on this object. Returns the amount of damage inflicted minus the amount resisted.
    virtual int Damage(struct Damage damage);

    GameObject(ObjectContext context) : context(context)
    {
        health = 25;
        maxHealth = health;
        resistance = 4;
        weight = 10;
        kind = "generic game object";
    }

    std::string GetKind() { return kind; }
    int GetWeight() { return weight; }
    int GetHealth() { return health; }
    int GetMaxHealth() { return maxHealth; }
    ObjectContext GetContext() { return context; }

  protected:
    ObjectContext context;
    std::map<std::string, int> stats;
    Texture2D tile;
    int maxHealth;
    int health;
    int resistance;
    int weight;
    std::string kind;
};
