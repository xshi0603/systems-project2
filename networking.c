#include "networking.h"

int server_setup() {
  int sd, i;

  sd = socket(AF_INET, SOCK_STREAM, 0);
  printf("[global server] socket created\n");

  struct addrinfo *hints, *results;
  hints = (struct addirinfo *)calloc(1, sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;
  hints->ai_flags = AI_PASSIVE;
  getaddrinfo(NULL, PORT, hints, &results);

  i = bind(sd, results->ai_addr, results->ai_addrlen);
  printf("[global server] socket bound\n");

  i = listen(sd, 32);
  printf("[global server] socket in listen state\n");

  free(hints);
  freeaddrinfo(results);
  return sd;
}

int server_connect(int sd) {
  int client_socket;
  socklen_t sock_size;
  struct sockaddr_storage client_address;

  client_socket = accept(sd, (struct sockaddr *)&client_address, &sock_size);
  return client_socket;
}

int client_setup(char *server) {
  int sd, i;

  sd = socket(AF_INET, SOCK_STREAM, 0);

  struct addrinfo *hints, *results;
  hints = (struct addrinfo *)calloc(1, sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;
  getaddrinfo(server, PORT, hints, &results);

  i = connect(sd, results->ai_addr, results->ai_addrlen);

  free(hints);
  freeaddrinfo(results);
  return sd;
}
