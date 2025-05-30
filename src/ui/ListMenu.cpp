#include "ListMenu.h"
#include "raylib.h"

ListMenu::ListMenu(Rectangle bounds, std::vector<const char *> buttons, ListMenuStyling styles): ListMenu(bounds, buttons, styles, 0) {}

ListMenu::ListMenu(Rectangle bounds, std::vector<const char *> buttons, ListMenuStyling styles, int selectedIndex)
{
    ListMenu::styles = styles;
    selector = EaseOut(0.0f, 0.0f, styles.selectorSpeed);
    selector.On();
    onClick = false;
    listItems = buttons;
    SetBounds(bounds);
    SetSelectedIndex(selectedIndex, false);
}

void ListMenu::AddItem(const char *name)
{
    listItems.push_back(name);
    UpdateHandlerPositions();
}

void ListMenu::AddItem(const char *name, int index)
{
    listItems.insert(listItems.begin() + index, name);
    UpdateHandlerPositions();
}

void ListMenu::RemoveItem(int index)
{
    handlers.erase(handlers.begin() + index);
    listItems.erase(listItems.begin() + index);
    UpdateHandlerPositions();
}

void ListMenu::SetBounds(Rectangle bounds)
{
    ListMenu::bounds = bounds;
    UpdateHandlerPositions();
}

Rectangle ListMenu::GetBounds() { return bounds; }

int ListMenu::GetSelectedIndex() { return selectedIndex; }

bool ListMenu::OnClick()
{
    bool tmpClick = onClick;
    onClick = false;
    return tmpClick;
}

bool ListMenu::OnSelect()
{
    return OnClick() || selectedIndexHandler.OnStart();
}

void ListMenu::SetSelectedIndex(int index, bool makeSound = true)
{
    if (styles.clickSound.has_value() && makeSound)
    {
        PlaySound(styles.clickSound.value());
    }
    selectedIndex = index;
    int length = 0;
    for (int j = 0; j < index; j++)
    {
        length += styles.padding * 2 + MeasureTextEx(styles.font, listItems[j], styles.fontSize, 1).y;
    }
    selector.Jump(length + styles.padding +
                  (MeasureTextEx(styles.font, listItems[index], styles.fontSize, 1).y - styles.selectorSize) /
                      2);
}

void ListMenu::Update(float deltaTime)
{
    selector.Update(deltaTime);
    selectedIndexHandler.UpdatePrev();
    for (int i = 0; i < handlers.size(); i++)
    {
        handlers[i].Update();
        if (handlers[i].OnClick(MOUSE_BUTTON_LEFT))
        {
            onClick = true;
            SetSelectedIndex(i);
        }
    }
    selectedIndexHandler.SetCurr(selectedIndex);
}

void ListMenu::Draw()
{
    DrawRectangleRec(bounds, styles.backgroundColor);
    DrawRectangle(bounds.x, bounds.y + selector.Val(), bounds.width, styles.selectorSize, styles.selectorColor);
    int drawHead = bounds.y;
    for (int i = 0; i < listItems.size(); i++)
    {
        drawHead += styles.padding;
        Vector2 textSize = MeasureTextEx(styles.font, listItems[i], styles.fontSize, 1);
        DrawTextEx(styles.font, listItems[i],
                   Vector2{bounds.x + (bounds.width - textSize.x) / 2, (float)drawHead}, styles.fontSize, 1,
                   styles.textColor);
        drawHead += textSize.y + styles.padding;
    }
}

void ListMenu::UpdateHandlerPositions()
{
    int length = bounds.y;
    handlers.clear();
    for (int i = 0; i < listItems.size(); i++)
    {
        length += styles.padding;
        Vector2 textSize = MeasureTextEx(styles.font, listItems[i], styles.fontSize, 1);
        MouseHandler newHandler = MouseHandler(Rectangle{bounds.x, (float)length, bounds.width, textSize.y});
        handlers.push_back(newHandler);
        length += textSize.y + styles.padding;
    }
}
