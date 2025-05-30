#include "GameState.h"

void GameState::Load(bool *windowShouldClose, GameStates* nextState, nlohmann::json config) { GameState::windowShouldClose = windowShouldClose; GameState::nextState = nextState; settings = config; }
