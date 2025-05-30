#pragma once
#include "Wall.h"

class IceWall : public Wall
{
  public:
    IceWall(ObjectContext context) : Wall(context) { kind = "ice wall"; }
};
