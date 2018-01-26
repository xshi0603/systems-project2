#include "rooms.h"

int main () {

  struct room * initialRoom = createRooms();
  printf("inital room\n");
  printRoom(initialRoom);
  printf("left room\n");
  printRoom(initialRoom->left);
  printf("right room\n");
  printRoom(initialRoom->right);

  printf("moving left\n");
  initialRoom = moveRooms(initialRoom, 1);
  printRoom(initialRoom);
  printf("moving left\n");
  initialRoom = moveRooms(initialRoom, 1);
  printRoom(initialRoom);
  printf("moving left\n");
  initialRoom = moveRooms(initialRoom, 1);
  printRoom(initialRoom);
  printf("moving left\n");
  initialRoom = moveRooms(initialRoom, 1);
  printRoom(initialRoom);
  printf("moving left\n");
  initialRoom = moveRooms(initialRoom, 1);
  printRoom(initialRoom);

}

