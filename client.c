#include "networking.h"

int main(int argc, char **argv) {

  int server_socket;
  char buffer[BUFFER_SIZE];

  fd_set read_fds;

  if (argc == 2)
    server_socket = client_setup( argv[1]);
  else
    server_socket = client_setup( TEST_IP);

  while (1) {
    printf("enter message: ");

    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);
    FD_SET(server_socket, &read_fds);

    select(server_socket + 1, &read_fds, NULL, NULL, NULL);

    if (FD_ISSET(server_socket, &read_fds)) {

      read(server_socket, buffer, sizeof(buffer));

      printf("recieved: [%s]\n", buffer);

    }

    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      //server or admin commands            
      fgets(buffer, sizeof(buffer), stdin);
      //if buffer do stuff    
      *strchr(buffer, "\n") = 0;
      
      write(server_socket, buffer, sizeof(buffer));

      read(server_socket, buffer, sizeof(buffer));

      printf("recieved: [%s]\n", buffer);
                                                                                                                                                   
    }

  }

}
