#include "MenuState.h"
#include "raylib.h"
#include <algorithm>

void MenuState::Load(bool *windowShouldClose, GameStates *nextState, nlohmann::json config)
{
    GameState::Load(windowShouldClose, nextState, config);
    SetWindowTitle("this->Title");
    titleFont = LoadFontEx("./assets/fonts/CourierScreenplay.ttf", 120, nullptr, 0);
    selectSound = LoadSound("./assets/sounds/MenuSelect.mp3");
    wooshSound = LoadSound("./assets/sounds/Woosh.wav");
    SetSoundVolume(wooshSound, (float)settings["audio"]["effects"] / 100.0f);
    SetSoundVolume(selectSound, (float)settings["audio"]["effects"] / 100.0f);
    ambientSound = LoadMusicStream("./assets/sounds/CaveAmbience.mp3");
    quitIcon = LoadTexture("./assets/icons/ArrowBack.png");
    settingsIcon = LoadTexture("./assets/icons/Settings.png");
    SeekMusicStream(ambientSound, GetTime());
    SetMusicVolume(ambientSound, (float)settings["audio"]["ambience"] / 100.0f);
    PlayMusicStream(ambientSound);
    escKey = KeyHandler(KEY_ESCAPE);
    capsLockKey = KeyHandler(KEY_CAPS_LOCK);
    settingsKey = KeyHandler(KEY_S);
    playKey = KeyHandler(KEY_ENTER);
}

void MenuState::Draw()
{
    ClearBackground(Color{0, 0, 0, 255});

    BeginDrawing();

    // Title and hover indicator
    int titleSize = 120.0f * (woosh.Val() + 1.0f);
    Vector2 tDim = MeasureTextEx(titleFont, "this->Eidolon", titleSize, 1);
    Vector2 thisDim = MeasureTextEx(titleFont, "this->", titleSize, 1);
    DrawTextEx(titleFont, "this->Eidolon",
               Vector2{(GetRenderWidth() - tDim.x) / 2, std::min(200, GetRenderHeight() / 4) - tDim.y / 2}, titleSize, 1,
               mouseTitle.IsDown(MOUSE_BUTTON_LEFT) ? Color{20, 180, 50, 255} : titleColor);
    DrawRectangle((GetRenderWidth() - tDim.x) / 2, std::min(200, GetRenderHeight() / 4) - 20 + tDim.y / 2,
                  thisDim.x * titleUnderline.Val(), 10,
                  mouseTitle.IsDown(MOUSE_BUTTON_LEFT) ? Color{20, 180, 50, 255} : Color{255, 255, 255, 255});

    // Back button and hover indicator
    DrawTextureEx(quitIcon, {0.0f, (float)GetRenderHeight() - iconsWidth}, 0.0f,
                  (float)iconsWidth / (float)quitIcon.width,
                  mouseBack.IsDown(MOUSE_BUTTON_LEFT) ? Color{200, 50, 50, 255} : Color{255, 255, 255, 255});
    DrawRectangle(0, GetRenderHeight() - 10, 120 * backUnderline.Val(), 10,
                  mouseBack.IsDown(MOUSE_BUTTON_LEFT) ? Color{200, 50, 50, 255} : Color{255, 255, 255, 255});

    // Settings button and hover indicator
    DrawTextureEx(settingsIcon, {(float)GetRenderWidth() - iconsWidth, (float)GetRenderHeight() - iconsWidth}, 0.0f,
                  (float)iconsWidth / (float)settingsIcon.width,
                  mouseSettings.IsDown(MOUSE_BUTTON_LEFT) ? Color{180, 180, 180, 255} : Color{255, 255, 255, 255});
    DrawRectangle(GetRenderWidth() - iconsWidth, GetRenderHeight() - 10, 120 * settingsUnderline.Val(), 10,
                  mouseSettings.IsDown(MOUSE_BUTTON_LEFT) ? Color{180, 180, 180, 255} : Color{255, 255, 255, 255});

    // Transition Overlay
    DrawRectangle(0, 0, GetRenderWidth(), GetRenderHeight(), Color{0,0,0,(unsigned char)(255.0f * woosh.Val())});

    EndDrawing();
}

void MenuState::Input()
{
    PollInputEvents();
    playKey.Update();
    escKey.Update();
    capsLockKey.Update();
    settingsKey.Update();
    int titleSize = 120.0f * (woosh.Val() + 1.0f);
    Vector2 tDim = MeasureTextEx(titleFont, "this->Eidolon", titleSize, 1);
    Vector2 thisDim = MeasureTextEx(titleFont, "this->", titleSize, 1);
    mouseTitle.SetRegion((GetRenderWidth() - tDim.x) / 2, std::min(200, GetRenderHeight() / 4) - tDim.y / 2, thisDim.x,
                         tDim.y);
    mouseTitle.Update();
    mouseBack.SetRegion(0, GetRenderHeight() - iconsWidth, iconsWidth, iconsWidth);
    mouseBack.Update();
    mouseSettings.SetRegion(GetRenderWidth() - iconsWidth, GetRenderHeight() - iconsWidth, iconsWidth, iconsWidth);
    mouseSettings.Update();
}

void MenuState::Update(float deltaTime)
{
    titleUnderline.Update(deltaTime, mouseTitle.Hover() || playKey.IsDown());
    backUnderline.Update(deltaTime, mouseBack.Hover() || escKey.IsDown() || capsLockKey.IsDown());
    settingsUnderline.Update(deltaTime, mouseSettings.Hover() || settingsKey.IsDown());
    woosh.Update(deltaTime, playTransition);
    UpdateMusicStream(ambientSound);
    if (mouseTitle.OnClick(MOUSE_BUTTON_LEFT))
        PlaySound(selectSound);
    if (mouseTitle.OnRelease(MOUSE_BUTTON_LEFT) || playKey.OnRelease())
    {
        PlaySound(wooshSound);
        playTransition = true;
    }
    if (mouseBack.OnClick(MOUSE_BUTTON_LEFT))
        PlaySound(selectSound);
    if (mouseBack.OnClick(MOUSE_BUTTON_LEFT))
        PlaySound(selectSound);
    if (mouseBack.OnRelease(MOUSE_BUTTON_LEFT) || escKey.OnRelease() || capsLockKey.OnRelease())
        *windowShouldClose = true;
    if (mouseSettings.OnClick(MOUSE_BUTTON_LEFT))
        PlaySound(selectSound);
    if (mouseSettings.OnRelease(MOUSE_BUTTON_LEFT) || settingsKey.OnRelease())
        *nextState = SETTINGS;

    if (woosh.Val() >= 0.999f)
        *nextState = PLAY;
}

void MenuState::Unload()
{
    StopMusicStream(ambientSound);
    UnloadMusicStream(ambientSound);
    UnloadFont(titleFont);
    UnloadSound(selectSound);
    UnloadSound(wooshSound);
    UnloadTexture(quitIcon);
    UnloadTexture(settingsIcon);
}
