#include "GameObject.h"
#include <algorithm>
#include <cmath>

int GameObject::Damage(struct Damage damage)
{
    // Reduce damage taken by 10% per resistance point greater than penetration, and increase by 10% for every penetration value greater than resistance
    int takenDamage = (int)(std::round(damage.value * (1.0f + (damage.penetration - resistance) / 10.0f)));
    takenDamage = std::max(takenDamage, 0);

    health -= takenDamage;
    if (health < 0)
        health = 0;

    return takenDamage;
}
