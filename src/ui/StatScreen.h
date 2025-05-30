#include "ListMenu.h"
#include "raylib.h"
#include "GameObjectList.h"
#include "SelectionList.h"
#include <string>

class CreatureObject;
class Player;
class Tiles;
class Inventory;

struct StatScreenStyling
{
    Font font;
    ListMenuStyling listStyles;
    GameObjectListStyling inventoryStyles;
    Color backgroundColor = Color{0, 0, 0, 255};
    Color borderColor = Color{0, 255, 255, 255};
    int contentSpacing = 20;
    Color contentBackgroundColor = Color{10, 10, 10, 255};
    Color contentBorderColor = Color{40, 40, 40, 255};
    int menuPadding = 10;
    int innerContentPadding = 5;
    int contentBorderWidth = 1;
    int borderWidth = 1;
};

class StatScreen
{
  public:
    StatScreen(Rectangle bounds, Player* player, Tiles* tiles, StatScreenStyling styling);
    ~StatScreen();
    void Show();
    void Hide();
    void SendHide();
    void ToggleHidden();
    bool IsHidden();
    void SetPageIndex(int index);
    int GetPageIndex();
    void Draw();
    void SetBounds(Rectangle bounds);
    void Update(float deltaTime);
    static const std::vector<const char*> pages;

  private:
    Rectangle bounds;
    StatScreenStyling styles;
    bool shown;
    Player* player;
    Inventory* inv;
    Tiles* tiles;
    ListMenu* menu;
    GameObjectList* inventoryList;
    GameObjectList* morphotypeList;
    std::string selectedPage;
    SelectionList* selectionList;
    SelectionList* abilityList;
};
