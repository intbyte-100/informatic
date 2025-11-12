#pragma once
#include "TmxLevel.h"

struct GameView;

struct GameScene
{
TmxLevel level;
TmxObject player;
std::vector<TmxObject> enemies;
std::vector<TmxObject> coins;
std::vector<TmxObject> blocks;
sf::Vector2f startPosition;

};

GameScene *NewGameScene();
void UpdateGameScene(void *pData, GameView &view, float deltaSec);
void DrawGameScene(void *pData, GameView &view);
void DestroyGameScene(GameScene *&pScene);