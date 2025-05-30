#pragma once
#include "Events.h"
#include "raylib.h"
#include <string>
#include <optional>

// Styling data for Buttons, with reasonable defaults
struct ButtonStyling
{
    Font font;
    int fontSize = font.baseSize;
    float borderWidth = 1.0f;
    Color textColor = Color{255, 255, 255, 255};
    Color borderColor = Color{255, 255, 255, 255};
    Color backgroundColor = Color{0, 0, 0, 255};
    Color hoverColor = Color{255, 255, 255, 40};
    Color clickColor = Color{255, 255, 255, 80};
    std::optional<Sound> clickSound;
};

// UI class for a simple click button
class Button
{
  public:
    Button(Rectangle bounds, ButtonStyling styles);
    Button(Rectangle bounds, std::string label, ButtonStyling styles);

    // Update the button's state for the frame
    void Update();

    // Render the button to the screen
    void Draw();

    // Set the region the button inhabits
    void SetBounds(Rectangle);

    // Whether the button has just been clicked
    bool OnClick(MouseButton);
    // Whether the button has just been released
    bool OnRelease(MouseButton);
    // Whether the button is currently being pressed
    bool IsDown(MouseButton);
    // Whether the button has just been clicked
    bool OnClick() {return OnClick(MOUSE_BUTTON_LEFT);}
    // Whether the button has just been released
    bool OnRelease() {return OnRelease(MOUSE_BUTTON_LEFT);}
    // Whether the button is currently being pressed
    bool IsDown() {return IsDown(MOUSE_BUTTON_LEFT);}

    // Whether the button is being hovered over
    bool Hover();

  private:
    std::string label;
    MouseHandler handler;
    Color currBackground;
    ButtonStyling styles;
    Rectangle bounds;
};
