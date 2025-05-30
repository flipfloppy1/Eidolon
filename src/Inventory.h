#pragma once
#include "ItemObject.h"
#include <vector>

class Inventory
{
  private:
    std::vector<ItemObject*> items;

  public:
    Inventory();
    Inventory(std::vector<ItemObject*> items);
    ~Inventory();
    std::vector<ItemObject*> SetItems(std::vector<ItemObject*> items);
    std::vector<ItemObject*>& GetItems();
    bool Contains(ItemObject* item);
    void AddItem(ItemObject* item);
    void AddItem(ItemObject* item, int index);

    // Deletes an item from the inventory and returns it as an ItemObject*. Caller is responsible for deletion after taking it
    ItemObject* TakeItem(int index);
    ItemObject* TakeItem(ItemObject* item);
    int IndexOfItem(ItemObject* item);
};
