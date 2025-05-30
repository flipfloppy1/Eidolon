#include "GameStateEnum.h"
#include "GameState.h"

class Client
{
  public:
    int Run();

  private:
    void Load();
    void LoadConfig();
    void Unload();
    bool windowShouldClose = false;
    nlohmann::json config;
    GameState* currState = nullptr;
    GameStates prevState = MENU;
    GameStates* nextState = nullptr;
};
