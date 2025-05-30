#pragma once

#include "GameState.h"
#include "Events.h"
#include "Interp.h"
#include "raylib.h"

class MenuState : public GameState
{
  public:
    void Load(bool *windowShouldClose, GameStates* nextState, nlohmann::json config) override;
    void Unload() override;
    void Draw() override;
    void Input() override;
    void Update(float deltaTime) override;

  private:
    Font titleFont;
    const int iconsWidth = 120;
    Color titleColor = Color{255,255,255,255};
    Sound selectSound;
    Sound wooshSound;
    Music ambientSound;
    Texture2D quitIcon;
    Texture2D settingsIcon;
    MouseHandler mouseTitle;
    MouseHandler mouseBack;
    MouseHandler mouseSettings;
    EaseOut woosh = EaseOut(0.0f, 1.0f, 5.0f);
    EaseOut titleUnderline = EaseOut(0.0f, 1.0f, 15.0f);
    EaseOut backUnderline = EaseOut(0.0f, 1.0f, 15.0f);
    EaseOut settingsUnderline = EaseOut(0.0f, 1.0f, 15.0f);
    bool playTransition = false;
    KeyHandler escKey;
    KeyHandler capsLockKey;
    KeyHandler settingsKey;
    KeyHandler playKey;
};
