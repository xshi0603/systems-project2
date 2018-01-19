#include "networking.h"

int main() {
  int listen_socket, client_socket, f;
  int subserver_count = 0;
  char buffer[BUFFER_SIZE];

  fd_set read_fds;

  listen_socket = server_setup();

  while(1) {
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);
    FD_SET(listen_socket, &read_fds);

    select(listen_socket + 1, &read_fds, NULL, NULL, NULL);

    if (FD_ISSET(listen_socket, &read_fds)) {
      client_socket = server_connect(listen_socket);

      f = fork();
      if (!f)
	subserver(client_socket);
      else {
	subserver_count++;
	//add to list

	
	close(client_socket);
      }
    }

    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      //server or admin commands
      fgets(buffer, sizeof(buffer), stdin);
      //if buffer do stuff

    }
  }
}

void subserver(int client_socket) {
  char buffer[BUFFER_SIZE];

  strncpy(buffer, "connected", sizeof(buffer));
  write(client_socket, buffer, sizeof(buffer));

  while (read(client_socket, buffer, sizeof(buffer))) {
    printf("[%s] %s\n", "name", buffer);
    //write to all clients in room


  }
  //close stuff necessary
  exit(0);
}
