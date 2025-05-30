#pragma once

#include "GameState.h"
#include "GameStateEnum.h"
#include "Events.h"
#include "World.h"
#include "MessageList.h"
#include "StatScreen.h"
#include "HealthBar.h"

class PlayState : public GameState
{
  public:
    void Load(bool* windowShouldClose, GameStates* nextState, nlohmann::json config) override;
    void Unload() override;
    void Draw() override;
    void Input() override;
    void Update(float deltaTime) override;

  protected:
    World* world;
    MessageLog* messageLog;
    bool afterPlayerTurn;
    bool beforePlayerTurn;
    int runSeed;
    Font gameFont;
    int gameScale;
    int viewportWidth;
    int viewportHeight;
    MessageList* msgList;
    GameObjectList* interactableList;
    SelectionList* interactableOptions;
    TileCoords interactionPos;
    StatScreen* statScreen;
    HealthBar* playerBar;
    Vector2 lastRenderSize;
    Sound selectSound;
    KeyHandler ctrlKey = KeyHandler(KEY_LEFT_CONTROL);
    KeyHandler plusKey = KeyHandler(KEY_EQUAL);
    KeyHandler minusKey = KeyHandler(KEY_MINUS);
    KeyHandler spaceKey = KeyHandler(KEY_SPACE);
    KeyHandler eKey = KeyHandler(KEY_E);
    KeyHandler wKey = KeyHandler(KEY_W);
    KeyHandler aKey = KeyHandler(KEY_A);
    KeyHandler sKey = KeyHandler(KEY_S);
    KeyHandler dKey = KeyHandler(KEY_D);
    KeyHandler downKey = KeyHandler(KEY_DOWN);
    KeyHandler upKey = KeyHandler(KEY_UP);
    KeyHandler rightKey = KeyHandler(KEY_RIGHT);
    KeyHandler leftKey = KeyHandler(KEY_LEFT);
    KeyHandler shiftKey = KeyHandler(KEY_LEFT_SHIFT);
    KeyHandler escKey = KeyHandler(KEY_ESCAPE);
    KeyHandler capsKey = KeyHandler(KEY_CAPS_LOCK);
};
