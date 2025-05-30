#pragma once
#include "raylib.h"

class PlayerKnowledge;
class CreatureObject;

class Player
{
  public:
    Player(CreatureObject* creature, PlayerKnowledge* knowledge, Sound morphSound);
    ~Player();
    CreatureObject* GetCreature();
    PlayerKnowledge* GetKnowledge();
    // Transfers the player to a new creature object,
    // copying over stats and inventory and returning
    // the old creature.
    CreatureObject* MorphCreature(CreatureObject* creature);

  private:
    CreatureObject* creature;
    PlayerKnowledge* knowledge;
    Sound morphSound;
};
