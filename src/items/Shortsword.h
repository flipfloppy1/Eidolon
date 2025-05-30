#pragma once
#include "ItemObject.h"

class Shortsword : public ItemObject
{
  public:
    Shortsword(ObjectContext context): ItemObject(context) { kind = "shortsword"; weight = 1; stats["sharpness"] = 5; stats["sharpbonus"] = 2; stats["dmgnum"] = 3; stats["dmgsize"] = 2; }
    bool Use(Usage* usage) override
    {
        if (Throw* throwData = dynamic_cast<Throw*>(usage))
        {
            ItemObject::Use(usage);
            return true;
        }
        else if (Attack* attackData = dynamic_cast<Attack*>(usage))
        {
            ItemObject::Use(usage);
            return true;
            int pen = std::min(attackData->strength + stats["sharpbonus"], stats["sharpness"]);
            struct Damage dmg = {};
            dmg.penetration = pen;
            dmg.type = PHYSICAL;
            for (int i = 0; i < stats["dmgnum"]; i++)
            {
                dmg.value += GetRandomValue(1, stats["dmgsize"]);
            }
            attackData->target->Damage(dmg);
            return true;
        }
        return false;
    }
};
