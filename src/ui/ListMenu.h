#pragma once
#include "raylib.h"
#include <vector>
#include "Events.h"
#include "Interp.h"
#include <optional>

// Styling data for ListMenus, with reasonable defaults
struct ListMenuStyling
{
    Font font;
    int fontSize = 40;
    int padding = 0;
    std::optional<Sound> clickSound;
    Color backgroundColor = Color{0, 0, 0, 255};
    Color textColor = Color{255, 255, 255, 255};
    Color selectorColor = Color{255, 255, 255, 80};
    int selectorSpeed = 10.0f;
    int selectorSize = 30;
};

// UI class for a list of interactive elements with a selection indicator
class ListMenu
{
  public:
    ListMenu(Rectangle bounds, std::vector<const char *> buttons, ListMenuStyling styles);

    ListMenu(Rectangle bounds, std::vector<const char *> buttons, ListMenuStyling styles, int selectedIndex);

    // Append an item with the given label to the list
    void AddItem(const char *name);
    // Insert an item with the given label to the list at some index
    void AddItem(const char *name, int index);
    // Remove the item at some index
    void RemoveItem(int index);

    // Set the region of the window the ListMenu occupies
    void SetBounds(Rectangle bounds);
    Rectangle GetBounds();

    int GetSelectedIndex();
    void SetSelectedIndex(int index, bool makeSound);

    // Whether the ListMenu has just been clicked
    bool OnClick();

    bool OnSelect();

    void Update(float deltaTime);

    void Draw();

  private:
    Rectangle bounds;
    ListMenuStyling styles;
    int selectedIndex;
    EaseOut selector;
    bool onClick;
    EventStream<int> selectedIndexHandler;
    std::vector<MouseHandler> handlers;
    std::vector<const char *> listItems;
    // Used internally wheneber the list changes to update the mouse handlers for each element
    void UpdateHandlerPositions();
};
