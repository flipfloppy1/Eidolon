#pragma once
#include "GameStateEnum.h"
#include "nlohmann/json.hpp"
#include <iostream>

// An interchangeable section of the application with discrete state and procedures
class GameState
{
  public:
    virtual void Load(bool *windowShouldClose, GameStates* nextState, nlohmann::json config);
    virtual void Unload() = 0;

    // Draws the current frame into the program
    virtual void Draw() = 0;

    // Accepts and processes user input
    virtual void Input() = 0;

    // Updates effects that happen over time and GameState logic
    virtual void Update(float deltaTime) = 0;

    virtual ~GameState()
    {
        std::cout << "Deleted GameState\n";
    }

  protected:
    nlohmann::json settings;
    bool *windowShouldClose;
    GameStates* nextState;
};
