
#pragma once

#include "Events.h"
#include "GameState.h"
#include "Interp.h"
#include "raylib.h"
#include "ui/ListMenu.h"
#include "ui/Slider.h"
#include "ui/Button.h"

class SettingsState : public GameState
{
  public:
    void Load(bool *windowShouldClose, GameStates *nextState, nlohmann::json config) override;
    void Unload() override;
    void Draw() override;
    void Input() override;
    void Update(float deltaTime) override;

  private:
    const float iconSize = 120.0f;
    Font settingsFont;
    MouseHandler backBtn;
    EaseOut backUnderline = EaseOut(0.0f, 1.0f, 15.0f);
    Texture2D backIcon;
    Sound selectSound;
    Music ambientSound;
    int activeTab;
    Vector2 windowSize;
    Vector2 prevWindowSize;
    ListMenu* menuItems = nullptr;
    Slider ambienceVolume;
    Slider musicVolume;
    Slider effectVolume;
    KeyHandler leaveKey;
    KeyHandler altLeaveKey;
    KeyHandler downKey;
    KeyHandler upKey;
    Button* applyButton;
    // Draw the Audio settings page
    void DrawAudio();
    // Save the on-page settings back to json
    void SaveSettings();
};
