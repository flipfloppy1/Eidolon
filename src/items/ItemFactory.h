#pragma once
#include "ItemObject.h"

class ItemFactory
{
  protected:
    ObjectContext context;

  public:
    ItemFactory(ObjectContext context) { ItemFactory::context = context; }
    virtual ItemObject* Create(int index) = 0;
    virtual ItemObject* Create() = 0;
};
