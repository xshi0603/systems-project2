#include "game_state.h"

struct room {
  // tiles[i][j] gets the game object at (i, j).
  struct game_object*[][] tiles;
};

struct room current_room;

void game_init() {
}

char* game_peek() {
  
}
