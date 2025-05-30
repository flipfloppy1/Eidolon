#pragma once
#include "ItemObject.h"

class Fist : public ItemObject
{
  public:
    Fist(ObjectContext context): ItemObject(context) { kind = "fist"; weight = 1; stats["sharpness"] = 1; stats["sharpbonus"] = 1; stats["dmgnum"] = 1; stats["dmgsize"] = 2; }
    bool Use(Usage* usage) override
    {
        if (Attack* attackData = dynamic_cast<Attack*>(usage))
        {
            return ItemObject::Use(usage);
        }
        return false;
    }
};
