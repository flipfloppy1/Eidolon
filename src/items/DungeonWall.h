#pragma once
#include "Wall.h"

class DungeonWall : public Wall
{
  public:
    DungeonWall(ObjectContext context) : Wall(context) { kind = "dungeon wall"; }
};
