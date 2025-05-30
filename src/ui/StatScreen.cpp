#include "StatScreen.h"
#include "Player.h"
#include "World.h"
#include "SelectionList.h"
#include "Tiles.h"
#include "PlayerKnowledge.h"
#include "CreatureObject.h"
#include <algorithm>
#include <iostream>

const std::vector<const char*> StatScreen::pages = {"Stats", "Inventory", "Equipment", "Morphotypes", "Abilities"};

StatScreen::StatScreen(Rectangle bounds, Player* player, Tiles* tiles, StatScreenStyling styling)
{
    inv = &player->GetCreature()->GetInventory();
    StatScreen::bounds = bounds;
    StatScreen::styles = styling;
    StatScreen::player = player;
    StatScreen::tiles = tiles;
    menu = new ListMenu(Rectangle{bounds.x + styles.menuPadding, bounds.y + styles.menuPadding, std::min(bounds.width, 300.0f) - styles.menuPadding * 2, bounds.height - styles.menuPadding * 2}, pages,
                        styling.listStyles);
    int menuWidth = menu->GetBounds().width + styles.menuPadding * 2;
    std::vector<GameObject*> invGameObjects;
    std::vector<ItemObject*> const& playerItems = player->GetCreature()->GetInventory().GetItems();
    std::cout << playerItems.size() << '\n';
    for (auto& item : playerItems)
    {
        invGameObjects.push_back((GameObject*)item);
    }
    inventoryList = new GameObjectList(
        Rectangle{bounds.x + menuWidth + styles.contentSpacing, bounds.y + styles.contentSpacing, bounds.width - menuWidth - styles.contentSpacing * 2, bounds.height - styles.contentSpacing * 2},
        invGameObjects, tiles, styles.inventoryStyles);
    selectedPage = pages[0];
    shown = true;
    SelectionListStyling selectionStyles = {styles.font, styles.listStyles.clickSound};
    selectionStyles.itemHeight = 50;
    selectionStyles.itemMargin = 5;
    selectionList = new SelectionList({bounds.x + (bounds.width - 200) / 2.0f, bounds.y + (bounds.height - 400) / 2.0f, 200, 400}, {}, selectionStyles);
    inventoryList->Open();
    std::vector<GameObject*> morphotypeObjs;
    for (auto& [kind, creature] : player->GetKnowledge()->GetMorphables())
    {
        morphotypeObjs.push_back((GameObject*)creature);
    }
    morphotypeList = new GameObjectList(
        Rectangle{bounds.x + menuWidth + styles.contentSpacing, bounds.y + styles.contentSpacing, bounds.width - menuWidth - styles.contentSpacing * 2, bounds.height - styles.contentSpacing * 2},
        morphotypeObjs, tiles, styles.inventoryStyles);
    morphotypeList->Open();
    abilityList = new SelectionList({bounds.x + menuWidth + styles.contentSpacing, bounds.y + styles.contentSpacing, bounds.width - menuWidth - styles.contentSpacing * 2, bounds.height - styles.contentSpacing * 2}, {}, selectionStyles);
    abilityList->Open();
}

void StatScreen::SetBounds(Rectangle bounds)
{
    StatScreen::bounds = bounds;
    menu->SetBounds(Rectangle{bounds.x + styles.menuPadding, bounds.y + styles.menuPadding, std::min(bounds.width, 300.0f) - styles.menuPadding * 2, bounds.height - styles.menuPadding * 2});
    inventoryList->SetBounds(Rectangle{bounds.x + menu->GetBounds().width + styles.menuPadding * 2 + styles.contentSpacing, bounds.y + styles.contentSpacing,
                                       bounds.width - (menu->GetBounds().width + styles.menuPadding * 2 + styles.contentSpacing * 2), bounds.height - styles.contentSpacing * 2});
    selectionList->SetBounds({bounds.x + (bounds.width - 200) / 2.0f, bounds.y + (bounds.height - 400) / 2.0f, 200, 400});
    abilityList->SetBounds(Rectangle{bounds.x + menu->GetBounds().width + styles.menuPadding * 2 + styles.contentSpacing, bounds.y + styles.contentSpacing,
                                           bounds.width - (menu->GetBounds().width + styles.menuPadding * 2 + styles.contentSpacing * 2), bounds.height - styles.contentSpacing * 2});
    morphotypeList->SetBounds(Rectangle{bounds.x + menu->GetBounds().width + styles.menuPadding * 2 + styles.contentSpacing, bounds.y + styles.contentSpacing,
                                           bounds.width - (menu->GetBounds().width + styles.menuPadding * 2 + styles.contentSpacing * 2), bounds.height - styles.contentSpacing * 2});
}

bool StatScreen::IsHidden() { return !shown; }

void StatScreen::Draw()
{
    if (!shown)
        return;

    DrawRectangleRec(bounds, styles.backgroundColor);
    menu->Draw();

    int menuWidth = menu->GetBounds().width + styles.menuPadding * 2;
    DrawRectangle(bounds.x + menu->GetBounds().width + styles.menuPadding * 2 + styles.contentSpacing, bounds.y + styles.contentSpacing,
                  bounds.width - menu->GetBounds().width + styles.menuPadding * 2 - styles.contentSpacing * 2, bounds.height - styles.contentSpacing * 2, styles.contentBackgroundColor);

    DrawRectangleLinesEx(
        Rectangle{bounds.x + menuWidth + styles.contentSpacing, bounds.y + styles.contentSpacing, bounds.width - menuWidth - styles.contentSpacing * 2, bounds.height - styles.contentSpacing * 2},
        styles.contentBorderWidth, styles.contentBorderColor);

    if (selectedPage == "Inventory")
    {
        inventoryList->Draw();
    }
    else if (selectedPage == "Morphotypes")
    {
        morphotypeList->Draw();
    }
    else if (selectedPage == "Abilities")
    {
        abilityList->Draw();
    }

    DrawRectangleLinesEx(bounds, styles.borderWidth, styles.borderColor);

    selectionList->Draw();
}

void StatScreen::SetPageIndex(int index)
{
    selectedPage = pages[index];
    return menu->SetSelectedIndex(index, true);
}

int StatScreen::GetPageIndex() { return menu->GetSelectedIndex(); }

void StatScreen::Update(float deltaTime)
{
    std::vector<GameObject*> itemObjs;
    for (auto& item : player->GetCreature()->GetInventory().GetItems())
    {
        itemObjs.push_back((GameObject*)item);
    }
    inventoryList->SetItems(itemObjs);
    std::vector<GameObject*> morphotypeObjs;
    for (auto& [kind, creature] : player->GetKnowledge()->GetMorphables())
    {
        morphotypeObjs.push_back((GameObject*)creature);
    }
    morphotypeList->SetItems(morphotypeObjs);
    menu->Update(deltaTime);
    inventoryList->Update(deltaTime);
    morphotypeList->Update(deltaTime);
    selectionList->Update(deltaTime);

    if (selectedPage == "Abilities")
    {
        std::vector<std::string> abilities;
        for (auto& [name, ability] : player->GetCreature()->GetAbilities())
            abilities.push_back(name);

        abilityList->SetItems(abilities);
        abilityList->Update(deltaTime);
    }

    if (menu->OnClick())
    {
        selectedPage = pages[menu->GetSelectedIndex()];
    }
    if (selectedPage == "Inventory" && inventoryList->OnClick())
    {
        selectionList->SetItems({"Equip", "Drop", "Use"});
        selectionList->Open();
    }
    else if (selectedPage == "Morphotypes" && morphotypeList->OnClick())
    {
        selectionList->SetItems({"Morph"});
        selectionList->Open();
    }
    if (selectionList->OnSelect() && selectedPage == "Inventory")
    {
        if (selectionList->Val() == "Drop")
        {
            selectionList->Close();
            ItemObject* item = player->GetCreature()->GetInventory().TakeItem((ItemObject*)inventoryList->SelectedItem());
            if (item)
            {
                player->GetCreature()->GetContext().world->Create(player->GetCreature()->GetContext().world->GetPlayerPos(), item);
            }
        }
        else if (selectionList->Val() == "Equip")
        {
            selectionList->Close();
            ItemObject* item = player->GetCreature()->GetInventory().TakeItem((ItemObject*)inventoryList->SelectedItem());
            if (item)
            {
                player->GetCreature()->EquipItem(MAIN_HAND, item);
            }
        }
    }
    else if (selectionList->OnSelect() && selectedPage == "Morphotypes")
    {
        if (selectionList->Val() == "Morph")
        {
            player->MorphCreature((CreatureObject*)morphotypeList->SelectedItem());
        }
    }

    if (abilityList->OnSelect() && selectedPage == "Abilities")
    {
        std::cout << abilityList->Val();
        player->GetCreature()->Do(player->GetCreature()->GetAbilities()[abilityList->Val()]);
        SendHide();
        SendHide();
        SendHide();
    }
}

void StatScreen::SendHide()
{
    if (selectionList->IsOpen())
        selectionList->Close();
    else if (shown)
        Hide();
}

void StatScreen::Show() { shown = true; }

void StatScreen::Hide() { shown = false; }

void StatScreen::ToggleHidden() { shown = !shown; }

StatScreen::~StatScreen()
{
    delete inventoryList;
    delete menu;
    delete selectionList;
    delete morphotypeList;
    delete abilityList;
}
