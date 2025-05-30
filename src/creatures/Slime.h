#include "Abilities.h"
#include "CreatureObject.h"
#include "World.h"
#include "raymath.h"
#include <cmath>
#include "Goo.h"

class Slime : public CreatureObject
{
  public:
    int moveCooldown = 0; // waits one turn before moving
    Slime(ObjectContext context) : CreatureObject(context)
    {
        kind = "slime";
        Spray* sprayAbility = new Spray();
        health = 10;
        maxHealth = health;
        resistance = 5;

        sprayAbility->damage.value = stats.contains("damage") ? stats.at("damage") : 3;
        sprayAbility->damage.penetration = 1;
        sprayAbility->damage.type = POISON;

        sprayAbility->origin = Vector2{0, 0};
        sprayAbility->direction = Vector2{1, 0};
        sprayAbility->distance = 3;
        sprayAbility->spread = 2;
        validAbilities["spray"] = sprayAbility;
    }
    bool Do(Ability* ability) override
    {
        if (Spray* spray = dynamic_cast<Spray*>(ability))
        {
            // get this creatures position (origin of the spray)
            TileCoords pos = {(int)spray->origin.x, (int)spray->origin.y};
            Vector2 origin = spray->origin;
            // get spray parameters
            Vector2 dir = spray->direction;
            int distance = (float)spray->distance;
            int spreadAngle = (float)spray->spread;
            // loop over all creatures in the game
            for (int x = origin.x - distance; x < origin.x + distance; x++)
            {
                for (int y = origin.y - distance; y < origin.y + distance; y++)
                {
                    // get target's position
                    TileCoords targetPos = TileCoords{x, y};
                    Vector2 targetVec = {(float)targetPos.x, (float)targetPos.y};
                    // vector from spray origin to the target
                    Vector2 toTarget = Vector2Subtract(targetVec, origin);
                    // distance to target
                    float distToTarget = Vector2Length(toTarget);
                    // check if too far
                    if (distToTarget > distance)
                        continue;
                    // normalise the vector for angle calc
                    Vector2 toTargetNorm = Vector2Normalize(toTarget);
                    // calc angle between spray direction and target direction
                    float angle = acosf(Vector2DotProduct(dir, toTargetNorm)) * (180.0f / PI);
                    // skip targets outside of spray cone
                    if (angle > spreadAngle / 2.0f)
                        continue;

                    // target is within rage and cone, apply damage
                    std::optional<CreatureObject*> target = context.world->GetTile(targetPos)->creature;
                    if (target && (*target) != this)
                    {
                        (*target)->Damage(spray->damage);
                    }
                    context.world->Create(targetPos, new Goo(context));
                }
            }

            return true;
        }
        else
        {
            return CreatureObject::Do(ability);
        }
        return false;
    }
    // makes the slime slower than the player
    void TakeTurn(TileCoords playerPos) override
    {
        if (moveCooldown > 0)
        {
            moveCooldown--;
            return;
        }
        Spray* spray = dynamic_cast<Spray*>(validAbilities["spray"]);
        if (spray)
        {
            if (spray->cooldown > 0)
            {
                spray->cooldown--;
            }
            TileCoords myPos = *context.world->GetCreaturePos(this);
            Vector2 myVec = {(float)myPos.x, (float)myPos.y};
            Vector2 playerVec = {(float)playerPos.x, (float)playerPos.y};

            Vector2 toPlayer = Vector2Subtract(playerVec, myVec);
            float distToPlayer = Vector2Length(toPlayer);
            Vector2 dirToPlayer = Vector2Normalize(toPlayer);

            float angle = acosf(Vector2DotProduct(spray->direction,dirToPlayer)) * (180.0f / PI);

            // within distance, spread cone, cooldown ready, and 25% chance
            if (distToPlayer <= spray->distance &&
                angle <= spray->spread / 2.0f &&
                spray->cooldown == 0 &&
                GetRandomValue(0,3) == 0)
            {
                Do(spray);
                spray->cooldown = 5;
                context.messageLog->Push("sprayed");
                return;
            }
        }

        CreatureObject::TakeTurn(playerPos);
        moveCooldown = 1;
    }
};
