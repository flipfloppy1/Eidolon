#pragma once
#include "ItemFactory.h"
#include "Items.h"
#include "Statues.h"

class Sculptor : public ItemFactory
{
  public:
    Sculptor(ObjectContext context) : ItemFactory(context) {}
    ItemObject* Create(int index) override
    {
        switch (index)
        {
        case 0:
            return new SnailStatue(context);
        case 1:
            return new CatStatue(context);
        }
        return nullptr;
    }
    ItemObject* Create() override { return Create(GetRandomValue(0, 1)); }
};
