#include "SettingsState.h"
#include "raylib.h"
#include <fstream>

void SettingsState::Load(bool *windowShouldClose, GameStates *nextState, nlohmann::json config)
{
    GameState::Load(windowShouldClose, nextState, config);
    SetWindowTitle("this->Settings");
    settingsFont = LoadFontEx("./assets/fonts/SystematicJ.ttf", 80, nullptr, 0);
    backIcon = LoadTexture("./assets/icons/ArrowBack.png");
    selectSound = LoadSound("./assets/sounds/MenuSelect.mp3");
    SetSoundVolume(selectSound, (float)settings["audio"]["effects"] / 100.0f);
    ambientSound = LoadMusicStream("./assets/sounds/CaveAmbienceMuffled.mp3");
    SeekMusicStream(ambientSound, GetTime());
    SetMusicVolume(ambientSound, (float)settings["audio"]["ambience"] / 100.0f);
    PlayMusicStream(ambientSound);
    activeTab = 0;
    ListMenuStyling menuStyle;
    menuStyle.font = settingsFont;
    menuStyle.fontSize = 40;
    menuStyle.selectorSize = 40;
    menuStyle.clickSound = selectSound;
    menuItems =
        new ListMenu(Rectangle{10, 80, 200, GetRenderHeight() - 80 - iconSize}, {"Video", "Audio", "Input"}, menuStyle);
    windowSize = Vector2{(float)GetRenderWidth(), (float)GetRenderHeight()};
    prevWindowSize = windowSize;
    ;
    float txtMeasure = (float)MeasureTextEx(settingsFont, "Ambience", 36, 1).y;
    ambienceVolume = Slider(Rectangle{240.0f, 20.0f + txtMeasure, GetRenderWidth() - 270.0f, 30.0f}, 0.0f, 100.0f, settings["audio"]["ambience"]);
    txtMeasure += (float)MeasureTextEx(settingsFont, "Music", 36, 1).y + 40.0f;
    musicVolume = Slider(Rectangle{240.0f, 20.0f + txtMeasure, GetRenderWidth() - 270.0f, 30.0f}, 0.0f, 100.0f, settings["audio"]["music"]);
    txtMeasure += (float)MeasureTextEx(settingsFont, "Effects", 36, 1).y + 40.0f;
    effectVolume = Slider(Rectangle{240.0f, 20.0f + txtMeasure, GetRenderWidth() - 270.0f, 30.0f}, 0.0f, 100.0f, settings["audio"]["effects"]);
    ButtonStyling applyStyles{settingsFont};
    applyStyles.fontSize = 30;
    applyButton = new Button(Rectangle{GetRenderWidth() - 300.0f, GetRenderHeight() - 80.0f, 270.0f, 50.0f}, "Apply", applyStyles);
    leaveKey = KeyHandler(KEY_ESCAPE);
    altLeaveKey = KeyHandler(KEY_CAPS_LOCK);
    downKey = KeyHandler(KEY_DOWN);
    upKey = KeyHandler(KEY_UP);
}

void SettingsState::Unload()
{
    delete applyButton;
    delete menuItems;
    StopMusicStream(ambientSound);
    UnloadMusicStream(ambientSound);
    UnloadFont(settingsFont);
    UnloadTexture(backIcon);
    UnloadSound(selectSound);
}

void SettingsState::DrawAudio()
{
    int drawHead = 0;
    DrawTextEx(settingsFont, "Ambience", Vector2{240,20}, 36, 1, Color{255,255,255,255});
    drawHead += MeasureTextEx(settingsFont, "Ambience", 36, 1).y;
    ambienceVolume.Draw();
    DrawTextEx(settingsFont, "Music", Vector2{240,60.0f + drawHead}, 36, 1, Color{255,255,255,255});
    drawHead += MeasureTextEx(settingsFont, "Music", 36, 1).y;
    musicVolume.Draw();
    DrawTextEx(settingsFont, "Effects", Vector2{240,100.0f + drawHead}, 36, 1, Color{255,255,255,255});
    effectVolume.Draw();
}

void SettingsState::Draw()
{
    ClearBackground(Color{0, 0, 0, 255});
    BeginDrawing();

    DrawTextEx(settingsFont, "Settings", Vector2{10, 10}, 48, 1, Color{255, 255, 255, 255});

    menuItems->Draw();

    DrawRectangleLinesEx(Rectangle{220, 10, GetRenderWidth() - 230.0f, GetRenderHeight() - 20.0f}, 1.0f,
                         Color{255, 255, 255, 255});

    switch (activeTab)
    {
    default:
        break;

    case 0:
        break;
    case 1:
        DrawAudio();
        break;
    }

    applyButton->Draw();

    DrawTextureEx(backIcon, {0.0f, (float)GetRenderHeight() - iconSize}, 0.0f, (float)iconSize / (float)backIcon.width,
                  backBtn.IsDown(MOUSE_BUTTON_LEFT) ? Color{200, 50, 50, 255} : Color{255, 255, 255, 255});
    DrawRectangle(0, GetRenderHeight() - 10, 120 * backUnderline.Val(), 10,
                  backBtn.IsDown(MOUSE_BUTTON_LEFT) ? Color{200, 50, 50, 255} : Color{255, 255, 255, 255});

    EndDrawing();
}

void SettingsState::SaveSettings()
{
    settings["audio"]["ambience"] = ambienceVolume.Val();
    settings["audio"]["music"] = musicVolume.Val();
    settings["audio"]["effects"] = effectVolume.Val();
}

void SettingsState::Input()
{
    PollInputEvents();
    backBtn.SetRegion(0, GetRenderHeight() - iconSize, iconSize, iconSize);
    backBtn.Update();
    leaveKey.Update();
    altLeaveKey.Update();
    downKey.Update();
    upKey.Update();
    if (menuItems->OnSelect())
        activeTab = menuItems->GetSelectedIndex();

    if (applyButton->OnClick())
    {
        SaveSettings();
        std::ofstream outfile("settings.json");
        outfile << settings.dump(0);
        outfile.close();
        *nextState = RELOAD;
    }
}

void SettingsState::Update(float deltaTime)
{
    applyButton->Update();
    windowSize = Vector2{(float)GetRenderWidth(), (float)GetRenderHeight()};
    menuItems->Update(deltaTime);
    ambienceVolume.Update();
    musicVolume.Update();
    effectVolume.Update();
    if (windowSize.y != prevWindowSize.y)
    {
        prevWindowSize = windowSize;
        menuItems->SetBounds(Rectangle{10, 80, 200, GetRenderHeight() - 80 - iconSize});
        applyButton->SetBounds(Rectangle{GetRenderWidth() - 300.0f, GetRenderHeight() - 80.0f, 270.0f, 50.0f});

        float txtMeasure = (float)MeasureTextEx(settingsFont, "Ambience", 36, 1).y;
        ambienceVolume.SetBounds(Rectangle{240.0f, 20.0f + txtMeasure, GetRenderWidth() - 270.0f, 30.0f});
        txtMeasure += (float)MeasureTextEx(settingsFont, "Music", 36, 1).y + 40.0f;
        musicVolume.SetBounds(Rectangle{240.0f, 20.0f + txtMeasure, GetRenderWidth() - 270.0f, 30.0f});
        txtMeasure += (float)MeasureTextEx(settingsFont, "Effects", 36, 1).y + 40.0f;
        effectVolume.SetBounds(Rectangle{240.0f, 20.0f + txtMeasure, GetRenderWidth() - 270.0f, 30.0f});
    }
    backUnderline.Update(deltaTime, backBtn.Hover() || leaveKey.IsDown() || altLeaveKey.IsDown());
    UpdateMusicStream(ambientSound);
    if (backBtn.OnClick(MOUSE_BUTTON_LEFT))
        PlaySound(selectSound);
    if (backBtn.OnRelease(MOUSE_BUTTON_LEFT) || leaveKey.OnRelease() || altLeaveKey.OnRelease())
        *nextState = MENU;

    if (downKey.OnPress() || downKey.OnRepeat())
    {
        menuItems->SetSelectedIndex((menuItems->GetSelectedIndex() + 1) % 3, true);
    }
    if (upKey.OnPress() || upKey.OnRepeat())
    {
        menuItems->SetSelectedIndex((menuItems->GetSelectedIndex() - 1) < 0 ? 2 : menuItems->GetSelectedIndex() - 1, true);
    }
}
