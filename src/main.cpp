#include "TheGame.h"
#include "../test/GeometryTest.h"

int main(int argc, char* args[]) {
  bool run_tests = true;
  if (run_tests) {
    runTests();
  }
  TheGame the_game;
  if (!the_game.init()) {
    return -1;
  }

  the_game.runLoop();
  return 0;
}