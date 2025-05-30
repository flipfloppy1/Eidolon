#include "Slider.h"
#include "raylib.h"
#include "Events.h"
#include <algorithm>

Slider::Slider() : Slider({0, 0, 0, 0}, 0.0f, 1.0f) {}
Slider::Slider(Rectangle bounds, float start, float end) : Slider(bounds, start, end, (end - start) / 2) {}
Slider::Slider(Rectangle bounds, float start, float end, float val) : Slider(bounds, start, end, val, SliderStyling()) {}
Slider::Slider(Rectangle bounds, float start, float end, float val, SliderStyling styles)
{
    Slider::bounds = bounds;
    Slider::start = start;
    Slider::end = end;
    Slider::val = val;
    Slider::styles = styles;
    held = false;
    progress = (end - start != 0.0f ? val / (end - start) : 0.0f);
    handler = MouseHandler(bounds.x + bounds.width * progress - (bounds.height + styles.blipOverflow) / 2.0f, bounds.y - styles.blipOverflow, bounds.height + styles.blipOverflow * 2,
                            bounds.height + styles.blipOverflow * 2);
}

void Slider::Draw()
{
    DrawRectangle(bounds.x, bounds.y, bounds.width, bounds.height, styles.backgroundColor);
    DrawRectangle(bounds.x, bounds.y, bounds.width * progress, bounds.height, styles.barColor);
    DrawRectangleLinesEx(bounds, 1.0f, styles.borderColor);
    DrawRectangle(bounds.x + bounds.width * progress - (bounds.height + styles.blipOverflow) / 2, bounds.y - styles.blipOverflow, bounds.height + styles.blipOverflow * 2,
                    bounds.height + styles.blipOverflow * 2, styles.blipColor);
}

void Slider::SetBounds(Rectangle bounds)
{
    Slider::bounds = bounds;
    handler = MouseHandler(bounds.x + bounds.width * progress - (bounds.height + styles.blipOverflow) / 2.0f, bounds.y - styles.blipOverflow, bounds.height + styles.blipOverflow,
                            bounds.height + styles.blipOverflow);
}

void Slider::Update()
{
    mouseLeft.UpdatePrev();
    mouseLeft.SetCurr(IsMouseButtonDown(MOUSE_BUTTON_LEFT));
    handler.Update();
    if (handler.OnClick(MOUSE_BUTTON_LEFT) && mouseLeft.OnStart())
    {
        held = true;
        lastMouseX = GetMouseX();
    }

    if (held)
    {
        int currMouseX = GetMouseX();
        float mouseDeltaX = currMouseX - lastMouseX;
        lastMouseX = currMouseX;
        val += ((float)mouseDeltaX / bounds.width) * 100.0f;
        val = std::max(val, start);
        val = std::min(val, end);
    }

    if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        held = false;

    progress = (end - start != 0.0f ? val / (end - start) : 0.0f);
    handler.SetRegion(Rectangle{bounds.x + bounds.width * progress - (bounds.height + styles.blipOverflow) / 2.0f, bounds.y - styles.blipOverflow, bounds.height + styles.blipOverflow,
                                bounds.height + styles.blipOverflow});
}
