#include "TheGame.h"

int main(int argc, char* args[]) {
  TheGame the_game;
  if (!the_game.init()) {
    return -1;
  }

  the_game.runLoop();
  return 0;
}