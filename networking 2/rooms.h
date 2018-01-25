#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>

struct room* createRooms();
void printRoom(struct room*);
struct room* moveRooms(struct room*, int direction);

struct room {
  int id;
  
  /*
  struct room *up;
  struct room *down;
  */
  struct room *left;
  struct room *right;
  int readingPipe;
  int writingPipe;
};

//creates rooms and returns starting room
struct room * createRooms() {
  struct room *a = (struct room *)malloc(sizeof(struct room));
  struct room *b = (struct room *)malloc(sizeof(struct room));
  struct room *c = (struct room *)malloc(sizeof(struct room));

  a->id = 0;
  a->left = b;
  a->right = c;
  
  b->id = 1;
  b->left = c;
  b->right = a;

  c->id = 2;
  c->left = a;
  c->right = b;

  return a;

}

void printRoom(struct room* roomz){

  printf("[%d]: LEFT:%d RIGHT:%d \n", roomz->id, roomz->left->id, roomz->right->id);

}

struct room* moveRooms(struct room* roomz, int direction){

  if (direction == 1) {//left
    return roomz->left;
  }
  else if (direction == 2) {//right
    return roomz->right;
  }
  else {//idk
    return roomz;
  }

}
