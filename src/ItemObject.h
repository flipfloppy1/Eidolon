#pragma once
#include "GameObject.h"
#include "Usage.h"
#include <map>

// The most abstract representation of an inanimate object in the game world
class ItemObject : public GameObject
{
  protected:
    std::map<std::string, Usage*> validUsages;

  public:
    ItemObject(ObjectContext context) : GameObject(context) { kind = "generic item"; stats["dmgnum"] = 1; stats["dmgsize"] = weight; }
    // Returns true on success, false on failure
    virtual bool Use(Usage* usage);
    virtual ~ItemObject()
    {
        for (auto& [name, usage] : validUsages)
        {
            delete usage;
        }
    }
};
