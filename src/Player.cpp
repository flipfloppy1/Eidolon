#include "Player.h"
#include "PlayerKnowledge.h"
#include "CreatureObject.h"

Player::Player(CreatureObject* creature, PlayerKnowledge* knowledge, Sound morphSound)
{
    Player::creature = creature;
    Player::knowledge = knowledge;
    Player::morphSound = morphSound;
}
CreatureObject* Player::GetCreature()
{
    return creature;
}
CreatureObject* Player::MorphCreature(CreatureObject* creature)
{
    PlaySound(morphSound);
    TileCoords playerPos = creature->GetContext().world->GetPlayerPos();
    CreatureObject* oldCreature = creature->GetContext().world->GetPlayer()->GetCreature();
    creature->GetContext().world->Detach(playerPos);
    creature->GetContext().world->Create(playerPos, creature);
    creature->SetAsPlayer(true);
    Player::creature = creature;
    auto oldItems = creature->GetInventory().SetItems(oldCreature->GetInventory().GetItems());
    if (creature->GetHealth() == 0)
    {
        creature->Heal(creature->GetMaxHealth());
    }
    for (auto& item : oldItems)
    {
        auto newItems = creature->GetInventory().GetItems();
        if (std::find(newItems.begin(), newItems.end(), item) == newItems.end())
            delete item;
    }
    oldCreature->GetInventory().SetItems(std::vector<ItemObject*>());
    return oldCreature;
}

PlayerKnowledge* Player::GetKnowledge()
{
    return knowledge;
}

Player::~Player()
{
    delete knowledge;
}
