#include "Interp.h"

void Interp::Update(float deltaTime)
{
    if (state)
    {
        if (start < end ? val < end : end < val)
            val = Increment(deltaTime);
        else
            val = end;
    }
    else
    {
        if (start < end ? val > start : start > val)
            val = Decrement(deltaTime);
        else
            val = start;
    }
}

void Interp::Update(float deltaTime, bool state)
{
    Interp::state = state;
    Interp::Update(deltaTime);
}

float Lerp::Increment(float deltaTime) { return val + deltaTime * speed; }

float Lerp::Decrement(float deltaTime) { return val - deltaTime * speed; }

float EaseOut::Increment(float deltaTime) { return val + (end - val) * deltaTime * speed; }

float EaseOut::Decrement(float deltaTime) { return val - val * deltaTime * speed; }

float EaseIn::Increment(float deltaTime) { return val + (val / (end - start)) * deltaTime + deltaTime * speed / 4.0f; }

float EaseIn::Decrement(float deltaTime) { return val - (end - val) / (end - start) * deltaTime + deltaTime * speed / 4.0f; }
