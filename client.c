#include "networking.h"

int main(int argc, char **argv) {

  int server_socket;
  char buffer[BUFFER_SIZE];

  if (argc == 2)
    server_socket = client_setup( argv[1]);
  else
    server_socket = client_setup( TEST_IP);

  while (1) {
    printf("enter message: ");
    fgets(buffer, sizeof(buffer), stdin);
    *strchr(buffer, "\n") = 0;


    //need to be passively reading

    write(server_socket, buffer, sizeof(buffer));

    read(server_socket, buffer, sizeof(buffer));

    printf("recieved: [%s]\n", buffer);

  }

}
