#pragma once

#include "CreatureObject.h"
#include "Abilities.h"
#include "raymath.h"
#include "World.h"

class Goblin : public CreatureObject
{
    public:
    Goblin(ObjectContext context) : CreatureObject(context)
    {
        kind = "goblin";
        Shout* shoutAbility = new Shout();
        health = 18;
        maxHealth = health;
        resistance = 2;
        shoutAbility->strength = stats.contains("strength") ? stats.at("strength") : 4;
        shoutAbility->origin = Vector2{0,0};
        validAbilities["shout"] = shoutAbility;

        shoutAbility->damage.value = shoutAbility->strength;
        shoutAbility->damage.penetration = 0;
        shoutAbility->damage.type = PHYSICAL;
        shoutAbility->radius = 2.5f;
    }
    bool Do(Ability* ability) override
    {
        if (Shout* shout = dynamic_cast<Shout*>(ability))
        {
            TileCoords pos = {(int)shout->origin.x, (int)shout->origin.y};
            Vector2 origin = {(float)pos.x, (float)pos.y};
            float radius = shout->radius;

            for (CreatureObject* target : context.world->GetCreatures())
            {
                if (target == this) continue;

                TileCoords targetPos = *context.world->GetCreaturePos(target);
                Vector2 targetVec = {(float)targetPos.x, (float)targetPos.y};

                float dist = Vector2Distance(origin, targetVec);
                if (dist > radius) continue;

                target->Damage(shout->damage);

            }
            return CreatureObject::Do(ability);
            return true;
        }
        return false;
    }

    void TakeTurn(TileCoords playerPos) override
    {
        TileCoords myPos = *context.world->GetCreaturePos(this);
        Vector2 myVec = {(float)myPos.x, (float)myPos.y};
        Vector2 playerVec = {(float)playerPos.x, (float)playerPos.y};
        float dist = Vector2Distance(myVec, playerVec);

        Shout* shout = dynamic_cast<Shout*>(validAbilities["shout"]);

        if (shout)
        {
            if (shout->cooldown > 0)
                shout->cooldown--;

            // only shout if player is in range, shout is off cooldown, and 50% chance
            if (dist <= shout->radius && shout->cooldown == 0 && GetRandomValue(0, 1) == 0)
            {
                Do(shout);
                shout->cooldown = 5; // set cooldown to 5 turns
                context.messageLog->Push("shout used");
                return;
            }
        }

        CreatureObject::TakeTurn(playerPos);
    }
};
