#include "GameScene.h"
#include "GameView.h"
#include <iostream>

int main(int argc, char *argv[]) {
  try {
    GameView *pGameView = NewGameView({800, 600});
    GameScene *pGameScene = NewGameScene(); 
    
    EnterGameLoop(*pGameView, UpdateGameScene, DrawGameScene, pGameScene);
  } catch (const std::exception &error) {
    std::cerr << error.what() << std::endl;
    return 1;
  }

  return 0;
}
