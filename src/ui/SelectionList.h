#pragma once

#include "Events.h"
#include "raylib.h"
#include <optional>
#include <string>
#include <vector>

struct SelectionListStyling
{
    Font font;
    std::optional<Sound> clickSound;
    int fontSize = 22;
    int borderWidth = 1;
    int padding = 10;
    int itemMargin = 4;
    int itemHeight = 40;
    Color textColor = Color{255, 255, 255, 255};
    Color backgroundColor = Color{0, 0, 0, 255};
    Color itemBackground = Color{20, 20, 20, 255};
    Color itemHoverBackground = Color{40, 40, 40, 255};
    Color itemClickBackground = Color{60, 60, 60, 255};
    Color borderColor = Color{0, 255, 255, 255};
};

typedef std::vector<std::string> SelectionListItems;

class SelectionList
{
  public:
    SelectionList(Rectangle bounds, SelectionListItems items, SelectionListStyling styles);
    void Draw();
    void Update(float deltaTime);
    void SetBounds(Rectangle bounds);
    void SetItems(SelectionListItems items);
    void Open();
    void Close();
    bool IsOpen();
    bool OnSelect();
    void Deselect();
    std::string Val();

  private:
    Rectangle bounds;
    SelectionListStyling styles;
    SelectionListItems items;
    std::vector<MouseHandler> handlers;
    bool selected;
    bool open;
    std::string value;
};
