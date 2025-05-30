#include "Client.h"
#include "MenuState.h"
#include "SettingsState.h"
#include "PlayState.h"
#include "raylib.h"
#include <fstream>

void Client::Load()
{
    SetTraceLogLevel(LOG_ERROR);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 720, "this->Title");
    #ifdef APPLE
        ToggleFullscreen();
    #endif
    SetTargetFPS(60);
    InitAudioDevice();
    SetExitKey(0);
    LoadConfig();

    currState = new MenuState();
    nextState = new GameStates(MENU);
    currState->Load(&windowShouldClose, nextState, config);
}

void Client::LoadConfig()
{
    std::ifstream infile("settings.json");
    config = nlohmann::json::parse(infile);
    infile.close();
}

int Client::Run()
{
    Load();

    while (!(WindowShouldClose() || windowShouldClose))
    {
        currState->Input();
        currState->Update(GetFrameTime());
        currState->Draw();

        if (*nextState != prevState)
        {
            WaitTime(0.05f); // Wait for short sounds to finish
            currState->Unload();

            switch (prevState)
            {
            case MENU:
                delete (MenuState*)currState;
                break;
            case SETTINGS:
                delete (SettingsState*)currState;
                break;
            case PLAY:
                delete (PlayState*)currState;
                break;
            default:
                break;
            }

            switch (*nextState)
            {
            case MENU:

                currState = new MenuState();
                break;
            case SETTINGS:
                currState = new SettingsState();
                break;
            case PLAY:
                currState = new PlayState();
                break;
            case RELOAD:
                LoadConfig();
                *nextState = prevState;
                break;
            }


            currState->Load(&windowShouldClose, nextState, config);
        }

        prevState = *nextState;
    }

    Unload();
    return 0;
}

void Client::Unload()
{
    WaitTime(0.05f); // Wait for short sounds to finish
    currState->Unload();
    CloseAudioDevice();
}
