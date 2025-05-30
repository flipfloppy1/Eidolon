#include "Inventory.h"
#include <algorithm>

Inventory::Inventory() {}

Inventory::Inventory(std::vector<ItemObject*> items) { Inventory::items = items; }

Inventory::~Inventory()
{
    for (ItemObject*& item : items)
    {
        delete item;
    }
}

std::vector<ItemObject*> Inventory::SetItems(std::vector<ItemObject*> items)
{
    auto oldItems = Inventory::items;
    Inventory::items = items;
    return oldItems;
}

std::vector<ItemObject*>& Inventory::GetItems()
{
    return items;
}

bool Inventory::Contains(ItemObject* item)
{
    return std::find(items.begin(), items.end(), item) != items.end();
}

void Inventory::AddItem(ItemObject* item) { items.push_back(item); }

void Inventory::AddItem(ItemObject* item, int index) { items.insert(items.begin() + index, item); }

ItemObject* Inventory::TakeItem(int index)
{
    ItemObject* obj = items[index];
    items.erase(items.begin() + index);
    return obj;
}

ItemObject* Inventory::TakeItem(ItemObject* item)
{
    for (int i = 0; i < items.size(); i++)
        if (items[i] == item)
            return TakeItem(i);

    return nullptr;
}
