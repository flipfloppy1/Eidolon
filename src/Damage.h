#pragma once

enum DamageType
{
    PHYSICAL,
    HEAT,
    COLD,
    POISON,
    // ...
};

struct Damage
{
    int value = 0;
    int penetration = 0;
    DamageType type = PHYSICAL;
};
