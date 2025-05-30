#pragma once
#include "raylib.h"
#include "Events.h"

// Styling data for Sliders, with reasonable defaults
struct SliderStyling
{
    Color borderColor = Color{255, 255, 255, 255};
    Color backgroundColor = Color{0, 0, 0, 255};
    Color barColor = Color{255, 255, 255, 80};
    Color blipColor = Color{255, 255, 255, 255};
    int blipOverflow = 5;
};

// UI class to set interactive values between a minimum and maximum
class Slider
{
  public:
    Slider();
    Slider(Rectangle bounds, float start, float end);
    Slider(Rectangle bounds, float start, float end, float val);
    Slider(Rectangle bounds, float start, float end, float val, SliderStyling styles);

    void Draw();

    // Set the space on the screen the slider will occupy
    void SetBounds(Rectangle bounds);

    void Update();

    // Getter for the slider's current value
    float Val() { return val; }

  private:
    Rectangle bounds;
    float lastMouseX;
    float start, end, val;
    bool held;
    float progress;
    SliderStyling styles;
    MouseHandler handler;
    EventStream<> mouseLeft;
};
