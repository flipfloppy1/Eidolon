#include "PlayState.h"
#include "MessageLog.h"
#include "World.h"
#include "raylib.h"
#include "Player.h"
#include <ctime>
#include "Shortsword.h"
#include <iostream>

void PlayState::Load(bool* windowShouldClose, GameStates* nextState, nlohmann::json config)
{
    GameState::Load(windowShouldClose, nextState, config);
    SetWindowTitle("this->Eidolon");
    gameFont = LoadFontEx("./assets/fonts/SystematicJ.ttf", 80, nullptr, 0);
    afterPlayerTurn = false;
    beforePlayerTurn = true;
    runSeed = (int)std::time(nullptr);
    messageLog = new MessageLog();
    MessageListStyling listStyles = {};
    listStyles.msgFont = gameFont;
    listStyles.borderWidth = 0;
    selectSound = LoadSound("./assets/sounds/MenuSelect.mp3");
    msgList = new MessageList({0, 0, 500, (float)GetScreenHeight()}, listStyles);
    world = new World(messageLog);
    gameScale = 3;
    world->Load(runSeed, &gameScale);
    StatScreenStyling statStyles;
    statStyles.font = gameFont;
    statStyles.listStyles.font = gameFont;
    statStyles.listStyles.clickSound = selectSound;
    statStyles.listStyles.padding = 5;
    statStyles.listStyles.selectorSize = 40;
    statStyles.inventoryStyles.font = gameFont;
    lastRenderSize = {(float)GetRenderWidth(), (float)GetRenderHeight()};
    world->GetPlayer()->GetCreature()->GiveItem(new Shortsword({world, messageLog}));
    std::cout << world->GetPlayer()->GetCreature()->GetInventory().GetItems()[0]->GetKind() << '\n';
    statScreen = new StatScreen(Rectangle{50, 50, (float)GetRenderWidth() - 100, (float)GetRenderHeight() - 100}, world->GetPlayer(), world->GetTileLoader(), statStyles);
    statScreen->Hide();
    interactableList = new GameObjectList({(GetRenderWidth() - 400) / 2.0f, (GetRenderHeight() - 500) / 2.0f, 400, 500}, {}, world->GetTileLoader(), {gameFont});
    interactableOptions = new SelectionList({(GetRenderWidth() - 400) / 2.0f, (GetRenderHeight() - 500) / 2.0f, 400, 500}, {}, {gameFont, selectSound});
    playerBar = new HealthBar({50, (float)GetRenderHeight() - 100, 1000, 50}, world->GetPlayer()->GetCreature()->GetHealth(), world->GetPlayer()->GetCreature()->GetMaxHealth(), {gameFont});
}

void PlayState::Unload()
{
    delete statScreen;
    std::cout << "statScreen unloaded\n";
    delete interactableList;
    std::cout << "interactableList unloaded\n";
    delete interactableOptions;
    std::cout << "interactableOptions unloaded\n";
    delete messageLog;
    std::cout << "messageLog unloaded\n";
    delete msgList;
    std::cout << "messageList unloaded\n";
    world->Unload();
    std::cout << "world unloaded\n";
    delete world;
    std::cout << "world deleted\n";
}

void PlayState::Draw()
{
    ClearBackground(Color{0, 0, 0, 255});
    BeginDrawing();

    world->Draw();
    msgList->Draw();
    playerBar->Draw();
    interactableList->Draw();
    interactableOptions->Draw();
    statScreen->Draw();

    EndDrawing();
}

void PlayState::Input()
{
    PollInputEvents();
    downKey.Update();
    upKey.Update();
    rightKey.Update();
    leftKey.Update();
    shiftKey.Update();
    wKey.Update();
    aKey.Update();
    sKey.Update();
    dKey.Update();
    plusKey.Update();
    ctrlKey.Update();
    minusKey.Update();
    spaceKey.Update();
    escKey.Update();
    capsKey.Update();
    eKey.Update();

    // gameScale += frameScroll;
    if ((minusKey.OnPress() || minusKey.OnRepeat()) && ctrlKey.IsDown())
        gameScale++;
    if ((plusKey.OnPress() || plusKey.OnRepeat()) && ctrlKey.IsDown())
        gameScale--;

    if (sKey.IsDown())
        world->SetPlayerDirection(SOUTH);

    if (wKey.IsDown())
        world->SetPlayerDirection(NORTH);

    if (aKey.IsDown())
    {
        if (wKey.IsDown())
            world->SetPlayerDirection(NORTHWEST);
        else if (sKey.IsDown())
            world->SetPlayerDirection(SOUTHWEST);
        else
            world->SetPlayerDirection(WEST);
    }

    if (dKey.IsDown())
    {
        if (wKey.IsDown())
            world->SetPlayerDirection(NORTHEAST);
        else if (sKey.IsDown())
            world->SetPlayerDirection(SOUTHEAST);
        else
            world->SetPlayerDirection(EAST);
    }

    if (spaceKey.OnPress())
    {
        interactionPos = world->PlayerInteract();
        TileData* tile = world->GetTile(interactionPos);
        if (tile)
        {
            std::vector<GameObject*> items;
            if (tile->creature)
                items.push_back((GameObject*)tile->creature.value());
            for (auto const& item : tile->items)
            {
                items.push_back((GameObject*)item);
            }
            interactableList->SetItems(items);
            interactableOptions->Close();
            interactableList->Open();
        }
    }

    if (eKey.OnPress() || eKey.OnRepeat())
    {
        statScreen->ToggleHidden();
    }

    if (!dKey.IsDown() && !aKey.IsDown() && !sKey.IsDown() && !wKey.IsDown())
        world->UnsetPlayerDirection();

    if (escKey.OnPress() || capsKey.OnPress())
    {
        if (!statScreen->IsHidden())
            statScreen->SendHide();
        else if (interactableOptions->IsOpen())
            interactableOptions->Close();
        else if (interactableList->IsOpen())
            interactableList->Close();
        else
            *nextState = MENU;
    }

    if (statScreen->IsHidden())
    {
        if (downKey.OnPress() || downKey.OnRepeat())
        {
            if (shiftKey.IsDown())
                world->MovePlayer(SOUTHEAST);
            else
                world->MovePlayer(SOUTH);

            world->AdvanceTurn();
        }
        if (upKey.OnPress() || upKey.OnRepeat())
        {
            if (shiftKey.IsDown())
                world->MovePlayer(NORTHWEST);
            else
                world->MovePlayer(NORTH);

            world->AdvanceTurn();
        }
        if (rightKey.OnPress() || rightKey.OnRepeat())
        {
            if (shiftKey.IsDown())
                world->MovePlayer(NORTHEAST);
            else
                world->MovePlayer(EAST);

            world->AdvanceTurn();
        }
        if (leftKey.OnPress() || leftKey.OnRepeat())
        {
            if (shiftKey.IsDown())
                world->MovePlayer(SOUTHWEST);
            else
                world->MovePlayer(WEST);

            world->AdvanceTurn();
        }
    }
    else
    {
        if (downKey.OnPress() || downKey.OnRepeat())
            statScreen->SetPageIndex((statScreen->GetPageIndex() + 1) % StatScreen::pages.size());
        if (upKey.OnPress() || upKey.OnRepeat())
            statScreen->SetPageIndex((statScreen->GetPageIndex() != 0) ? statScreen->GetPageIndex() - 1 : StatScreen::pages.size() - 1);
    }
}

void PlayState::Update(float deltaTime)
{
    playerBar->Progress(world->GetPlayer()->GetCreature()->GetHealth(), world->GetPlayer()->GetCreature()->GetMaxHealth());
    if (lastRenderSize.x != GetRenderWidth() || lastRenderSize.y != GetRenderHeight())
    {
        lastRenderSize = {(float)GetRenderWidth(), (float)GetRenderHeight()};
        statScreen->SetBounds(Rectangle{50, 50, (float)GetRenderWidth() - 100, (float)GetRenderHeight() - 100});
        msgList->SetRegion({0, 0, 500, (float)GetScreenHeight()});
        interactableOptions->SetBounds({(GetRenderWidth() - 400) / 2.0f, (GetRenderHeight() - 500) / 2.0f, 400, 500});
        interactableList->SetBounds({(GetRenderWidth() - 400) / 2.0f, (GetRenderHeight() - 500) / 2.0f, 400, 500});
    }
    gameScale = std::min(std::max(gameScale, 1), GetRenderWidth() / (VIEWPORT_MIN_WIDTH * 4));
    if (messageLog->NewMessage())
    {
        msgList->AddMessage(messageLog->ConsumeMessage());
    }
    msgList->Update(deltaTime);
    statScreen->Update(deltaTime);
    interactableOptions->Update(deltaTime);
    interactableList->Update(deltaTime);
    if (interactableOptions->OnSelect())
    {
        if (interactableOptions->Val() == "Pick up")
        {
            ItemObject* item = world->Detach(interactionPos, (ItemObject*)interactableList->SelectedItem());
            if (item)
            {
                world->GetPlayer()->GetCreature()->GiveItem(item);
            }
        }
        else if (interactableOptions->Val() == "Equip")
        {
            ItemObject* item = world->Detach(interactionPos, (ItemObject*)interactableList->SelectedItem());
            if (item)
                world->GetPlayer()->GetCreature()->EquipItem(MAIN_HAND, item);
        }
        interactableList->Close();
        interactableOptions->Close();
    }

    if (interactableList->OnClick())
    {
        interactableOptions->Open();
        GameObject* selectedItem = interactableList->SelectedItem();
        if (ItemObject* itemObject = dynamic_cast<ItemObject*>(selectedItem))
        {
            interactableOptions->SetItems({"Pick up", "Equip"});
        }
        else if (CreatureObject* creatureObject = dynamic_cast<CreatureObject*>(selectedItem))
        {
            interactableOptions->SetItems({"Attack", "Talk"});
        }
        interactableList->Update(deltaTime);
        interactableList->Close();
    }

}
