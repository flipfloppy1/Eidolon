#pragma once
#include <iostream>
#include "Abilities.h"
#include "Coordinates.h"
#include "GameObject.h"
#include "Inventory.h"
#include <set>

class TileData;
class WorldData;

typedef std::set<TileCoords> WorldView;

enum BodyParts
{
    MAIN_HAND,
    HANDS,
    TORSO,
    LEGS,
    FEET
};

// The most abstract representation of a creature in the game world
class CreatureObject : public GameObject
{
  public:
    bool TryMove(TileCoords target);
    virtual void TakeTurn(TileCoords playerPos);
    CreatureObject(ObjectContext context);
    virtual bool Do(Ability* ability);
    void Use(ItemObject* item, Usage* usage);
    virtual WorldView See(TileCoords viewpoint);
    void SetAsPlayer(bool state) { isPlayer = state; }
    bool IsPlayer() { return isPlayer; }
    void GiveItem(ItemObject* item) { inv.AddItem(item); }
    void Attack(GameObject* target, TileCoords targetPos);
    ItemObject* TakeItem(int index) { return inv.TakeItem(index); }
    ItemObject* GetEquipment(BodyParts part);
    bool EquipItem(BodyParts part, ItemObject* item);
    Inventory& GetInventory() { return inv; }
    void SetInventory(Inventory inv) { CreatureObject::inv = inv; }
    std::map<std::string, Ability*> GetAbilities() { return validAbilities; }
    void Heal(int health) { CreatureObject::health = std::min(maxHealth, CreatureObject::health + health); }
    std::map<std::string, int> GetStats() { return stats; }
    virtual ~CreatureObject()
    {
        std::cout << "Deleting abilities\n";
        for (auto& [name, ability] : validAbilities)
        {
            delete ability;
        }
        std::cout << "Deleted abilities\n";
        // for (auto& [name, item] : equipment)
        // {
        //     if (defaultEquipment[name] != item)
        //         delete item;
        // }
        std::cout << "Deleting equipment\n";
        for (auto& [part, eqmnt] : defaultEquipment)
        {
            delete eqmnt;
        }
        std::cout << "Deleted equipment\n";
    }

  protected:
    Inventory inv;
    bool isPlayer;
    std::map<BodyParts, ItemObject*> defaultEquipment;
    std::map<BodyParts, ItemObject*> equipment;
    std::map<std::string, Ability*> validAbilities;
    WorldView currView;
    std::vector<TileCoords> AStar(TileCoords start, TileCoords goal);
};
