#include "raylib.h"

struct HealthBarStyling
{
    Font font;
    int fontSize = 16;
    Color fontColor = Color{255, 255, 255, 255};
    Color backgroundColor = Color{0, 0, 0, 255};
    Color foregroundColor = Color{20, 180, 20, 255};
};

class HealthBar
{
  private:
    Rectangle bounds;
    HealthBarStyling styles;
    float pos;
    float max;

  public:
    HealthBar(Rectangle bounds, float pos, float max, HealthBarStyling styles);
    void Draw();
    void SetBounds(Rectangle bounds);
    void Progress(float progress);
    void Progress(float pos, float max);
};
