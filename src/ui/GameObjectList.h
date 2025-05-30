#include "Events.h"
#include "raylib.h"
#include <vector>

class GameObject;
class Tiles;

struct GameObjectListStyling
{
    Font font;
    int fontSize = 24;
    int padding = 5;
    int elemMargin = 5;
    int elemHeight = 64;
    int borderWidth = 1;
    Color textColor = Color{255, 255, 255, 255};
    Color backgroundColor = Color{0, 0, 0, 255};
    Color elemBackgroundColor = Color{20, 20, 20, 255};
    Color elemHoverColor = Color{50, 50, 50, 255};
    Color borderColor = Color{0, 255, 255, 255};
};

class GameObjectList
{
  private:
    std::vector<MouseHandler> handlers;
    std::vector<GameObject*> list;
    GameObject* selectedItem;
    Tiles* tiles;
    int selectedIndex;
    GameObjectListStyling styles;
    Rectangle bounds;
    bool useMouse;
    bool open;
    KeyHandler downKey;
    KeyHandler upKey;

  public:
    GameObjectList(Rectangle bounds, std::vector<GameObject*> list, Tiles* tiles, GameObjectListStyling styles);
    void Draw();
    void Update(float deltaTime);
    void SetBounds(Rectangle bounds);
    bool OnClick();
    bool IsOpen();
    void Open();
    void Close();
    void SetItems(std::vector<GameObject*> items);
    GameObject* SelectedItem();
};
