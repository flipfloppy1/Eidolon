#include "SelectionList.h"

SelectionList::SelectionList(Rectangle bounds, SelectionListItems items, SelectionListStyling styles)
{
    SelectionList::SetBounds(bounds);
    SelectionList::items = items;
    SelectionList::styles = styles;
    selected = false;
    open = false;
}

void SelectionList::SetItems(SelectionListItems items)
{
    if (items == SelectionList::items)
        return;

    SelectionList::items = items;

    // Re-register handlers
    handlers.erase(handlers.begin(), handlers.end());
    SelectionList::SetBounds(bounds);
}

void SelectionList::Draw()
{
    if (!open)
        return;

    DrawRectangleRec(bounds, styles.backgroundColor);

    for (int i = 0; i < items.size(); i++)
    {
        DrawRectangleRec(handlers[i].GetRegion(), handlers[i].IsDown(MOUSE_BUTTON_LEFT) ? styles.itemClickBackground : handlers[i].Hover() ? styles.itemHoverBackground : styles.itemBackground);
        Vector2 textSize = MeasureTextEx(styles.font, items[i].c_str(), styles.fontSize, 1);
        DrawTextEx(styles.font, items[i].c_str(),
                   Vector2{bounds.x + styles.padding + (bounds.width - styles.padding * 2 - textSize.x) / 2.0f,
                           bounds.y + styles.padding + (styles.itemHeight + styles.itemMargin) * i + (styles.itemHeight - textSize.y) / 2.0f},
                   styles.fontSize, 1, styles.textColor);
    }

    DrawRectangleLinesEx(bounds, styles.borderWidth, styles.borderColor);
}

void SelectionList::Update(float deltaTime)
{
    if (!open)
        return;
    selected = false;
    for (int i = 0; i < handlers.size(); i++)
    {
        handlers[i].Update();
        if (handlers[i].OnRelease(MOUSE_BUTTON_LEFT))
        {
            selected = true;
            value = items[i];
            if (styles.clickSound)
                PlaySound(*styles.clickSound);
        }
    }
}

void SelectionList::SetBounds(Rectangle bounds)
{
    SelectionList::bounds = bounds;
    for (int i = 0; i < items.size(); i++)
    {
        Rectangle handlerBounds =
            Rectangle{bounds.x + styles.padding, bounds.y + styles.padding + (styles.itemHeight + styles.itemMargin) * i, bounds.width - styles.padding * 2, (float)styles.itemHeight};
        if (i < handlers.size())
        {
            handlers[i].SetRegion(handlerBounds);
        }
        else
        {
            handlers.push_back(MouseHandler(handlerBounds));
        }
    }
}

bool SelectionList::IsOpen()
{
    return open;
}

void SelectionList::Close()
{
    Deselect();
    open = false;
}

void SelectionList::Open()
{
    open = true;
}

std::string SelectionList::Val()
{
    return value;
}

bool SelectionList::OnSelect()
{
    return selected;
}

void SelectionList::Deselect()
{
    selected = false;
    value = "";
}
