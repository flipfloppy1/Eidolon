#include "GameObjectList.h"
#include "Tiles.h"
#include "GameObject.h"

GameObjectList::GameObjectList(Rectangle bounds, std::vector<GameObject*> list, Tiles* tiles, GameObjectListStyling styles) : downKey(KEY_S), upKey(KEY_W)
{
    GameObjectList::tiles = tiles;
    SetBounds(bounds);
    GameObjectList::list = list;
    GameObjectList::styles = styles;
    open = false;
    useMouse = false;
    selectedItem = nullptr;
    selectedIndex = -1;
}

void GameObjectList::SetItems(std::vector<GameObject*> items)
{
    list = items;
    handlers.erase(handlers.begin(), handlers.end());
    SetBounds(bounds);
}

void GameObjectList::Draw()
{
    if (!open)
        return;

    DrawRectangle(bounds.x,bounds.y,bounds.width,bounds.height,styles.backgroundColor);
    const int tileOffset = 10;
    DrawRectangleLinesEx(bounds, styles.borderWidth, styles.borderColor);
    for (int i = 0; i < list.size(); i++)
    {
        GameObject* obj = list[i];
        std::string objName = obj->GetKind();
        const char* objText = objName.c_str();
        auto tile = tiles->GetTile(obj->GetKind());
        if (tile)
        {
            DrawRectangle(bounds.x + styles.padding, bounds.y + styles.elemMargin + (styles.elemMargin + styles.elemHeight) * i, bounds.width - styles.padding * 2, styles.elemHeight, handlers[i].Hover() ? styles.elemHoverColor : styles.elemBackgroundColor);
            DrawTexturePro(*tile, Rectangle{0,0,(float)tile->width,(float)tile->height}, Rectangle{bounds.x + styles.padding + tileOffset, bounds.y + styles.padding + styles.elemMargin + (styles.elemHeight + styles.elemMargin) * (i - 0.5f) + (styles.elemHeight + styles.elemMargin - tile->width) / 2.0f, (float)styles.elemHeight, (float)styles.elemHeight}, {0.0f,0.0f}, 0.0f, Color{255,255,255,255});
            Vector2 objNameText = MeasureTextEx(styles.font, objText, styles.fontSize, 1);
            DrawTextEx(styles.font, objText, {bounds.x + (bounds.width + styles.padding + tileOffset - objNameText.x) / 2.0f, bounds.y + styles.padding + (styles.elemHeight + styles.elemMargin) * i + (styles.elemHeight + styles.elemMargin - objNameText.y) / 2.0f}, styles.fontSize, 1, styles.textColor);
        }
    }

}

void GameObjectList::Open()
{
    open = true;
}

void GameObjectList::Close()
{
    open = false;
}

bool GameObjectList::IsOpen()
{
    return open;
}

void GameObjectList::Update(float deltaTime)
{
    if (!open)
        return;

    for (int i = 0; i < handlers.size(); i++)
    {
        handlers[i].Update();
        if (handlers[i].OnClick(MOUSE_BUTTON_LEFT))
        {
            selectedItem = list[i];
            selectedIndex = i;
        }
    }
}

void GameObjectList::SetBounds(Rectangle bounds)
{
    GameObjectList::bounds = bounds;
    for (int i = 0; i < list.size(); i++)
    {
        Rectangle handlerBounds = Rectangle{bounds.x + styles.padding, bounds.y + styles.padding + (float)styles.elemHeight * i, bounds.width - styles.padding * 2, (float)styles.elemHeight };
        if (i < handlers.size())
            handlers[i].SetRegion(handlerBounds);
        else
            handlers.push_back(MouseHandler(handlerBounds));
    }
}

bool GameObjectList::OnClick()
{
    for (auto& handler : handlers)
    {
        if (handler.OnClick(MOUSE_BUTTON_LEFT))
            return true;
    }

    return false;
}

GameObject* GameObjectList::SelectedItem()
{
    return selectedItem;
}
