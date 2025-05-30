#include "Button.h"
#include "raylib.h"

Button::Button(Rectangle bounds, ButtonStyling styles) : Button(bounds, "", styles) {}

Button::Button(Rectangle bounds, std::string label, ButtonStyling styles)
{
    Button::bounds = bounds;
    Button::label = label;
    Button::styles = styles;
    handler = MouseHandler(bounds);
    currBackground = styles.backgroundColor;
}

void Button::Draw()
{
    DrawRectangleRec(bounds, currBackground);
    DrawRectangleLinesEx(bounds, styles.borderWidth, styles.borderColor);
    if (label != "")
    {
        Vector2 textSize = MeasureTextEx(styles.font, label.c_str(), styles.fontSize, 1.0f);
        DrawTextEx(styles.font, label.c_str(), Vector2{bounds.x + (bounds.width - textSize.x) / 2, bounds.y + (bounds.height - textSize.y) / 2}, styles.fontSize, 1, styles.textColor);
    }
}

void Button::SetBounds(Rectangle bounds)
{
    Button::bounds = bounds;
    handler.SetRegion(bounds);
}

bool Button::OnClick(MouseButton mouseBtn) { return handler.OnRelease(mouseBtn); }
bool Button::OnRelease(MouseButton mouseBtn) { return handler.OnRelease(mouseBtn); }
bool Button::IsDown(MouseButton mouseBtn) { return handler.IsDown(mouseBtn); }
bool Button::Hover() { return handler.Hover(); }

void Button::Update()
{
    handler.Update();
    if (handler.Hover())
        currBackground = handler.IsDown(MOUSE_BUTTON_LEFT) ? styles.clickColor : styles.hoverColor;
    else
        currBackground = styles.backgroundColor;

    if (handler.OnClick(MOUSE_BUTTON_LEFT) && styles.clickSound.has_value())
        PlaySound(styles.clickSound.value());
}
