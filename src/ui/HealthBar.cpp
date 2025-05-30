#include "HealthBar.h"
#include <string>

HealthBar::HealthBar(Rectangle bounds, float pos, float max, HealthBarStyling styles)
{
    HealthBar::bounds = bounds;
    HealthBar::styles = styles;
    HealthBar::pos = pos;
    HealthBar::max = max;
}

void HealthBar::Draw()
{
    DrawRectangleRec(bounds, styles.backgroundColor);
    DrawRectangle(bounds.x, bounds.y, bounds.width * (pos / max), bounds.height, styles.foregroundColor);
    Vector2 textSize = MeasureTextEx(styles.font, (std::to_string((int)pos) + "/" + std::to_string((int)max)).c_str(), styles.fontSize, 1);
    DrawTextEx(styles.font, (std::to_string((int)pos) + "/" + std::to_string((int)max)).c_str(), {bounds.x + (bounds.width - textSize.x) / 2.0f, bounds.y + (bounds.height - textSize.y) / 2.0f},
               styles.fontSize, 1, styles.fontColor);
}

void HealthBar::SetBounds(Rectangle bounds)
{
    HealthBar::bounds = bounds;
}

void HealthBar::Progress(float pos, float max)
{
    HealthBar::pos = pos;
    HealthBar::max = max;
}

void HealthBar::Progress(float progress)
{
    HealthBar::pos = progress;
    max = 1.0f;
}
