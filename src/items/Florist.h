#pragma once
#include "Items.h"
#include "ItemFactory.h"
#include "Flowers.h"

class Florist: public ItemFactory
{
  private:
    ObjectContext context;

  public:
    Florist(ObjectContext context) : ItemFactory(context) {}
    ItemObject* Create(int index) override
    {
        switch (index)
        {
        case 0:
            return new PinkFlowers(context);
        case 1:
            return new RedFlowers(context);
        case 2:
            return new WhiteFlowers(context);
        case 3:
            return new YellowFlowers(context);
        }
        return nullptr;
    }
    ItemObject* Create() override { return Create(GetRandomValue(0, 3)); }
};
