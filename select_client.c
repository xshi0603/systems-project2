#include "networking.h"

int main(int argc, char **argv) {
  int server_socket;
  char buffer[BUFFER_SIZE];
  char name[BUFFER_SIZE];

  fd_set read_fds;

  if (argc == 2)
    server_socket = client_setup( argv[1]);
  else
    server_socket = client_setup( TEST_IP );

  printf("Please input your desired name:\n");

  fgets(name, sizeof(name), stdin);
  *strchr(name, '\n') = 0;

  printf("Welcome %s!\n", name);
  printf("Type anything to talk to your room. Type \"/left\" to move left or \"/right\" to move right. Preceed your message with \"/all\" to speak to all rooms.\n");

  while (1) {

    printf("chat> ");
    //the above printf does not have \n
    //flush the buffer to immediately print
    fflush(stdout);

    //select() modifies read_fds
    //we must reset it at each iteration
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds); //add stdin to fd set
    FD_SET(server_socket, &read_fds); //add socket to fd set

    //select will block until either fd is ready
    if (select(server_socket + 1, &read_fds, NULL, NULL, NULL) == -1) {
      printf("There was an error with the server.\n");
      exit(1);
    }

    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      fgets(buffer, sizeof(buffer), stdin);
      *strchr(buffer, '\n') = 0;
      
      if (!strcmp(buffer, "exit")) {
	write(server_socket, buffer, sizeof(buffer));
	exit(0);
      }
      else if (!strcmp(buffer, "/left") || !strcmp(buffer, "/right") || !strcmp(buffer, "exit") || (strstr(buffer, "/all") != NULL) ) {
	/*
	char buffer2[BUFFER_SIZE];
	memset(buffer2, 0, sizeof(buffer2));
	strcat(buffer2, name);
	strcat(buffer2, ":");
	strcat(buffer2, buffer);
	*/
	write(server_socket, buffer, sizeof(buffer));
	read(server_socket, buffer, sizeof(buffer));
	printf("%s\n", buffer);
      }
      else {
	char buffer2[BUFFER_SIZE];
	memset(buffer2, 0, sizeof(buffer2));
	strcat(buffer2, name);
	strcat(buffer2, ":");
	strcat(buffer2, buffer);

	write(server_socket, buffer2, sizeof(buffer2));
	read(server_socket, buffer, sizeof(buffer));
	printf("%s\n", buffer);
      }
    }//end stdin select

    //currently the server is not set up to
    //send messages to all the clients, but
    //this would allow for broadcast messages
    if (FD_ISSET(server_socket, &read_fds)) {
      read(server_socket, buffer, sizeof(buffer));
      if (!strcmp(buffer, "exit")) {
	  exit(0);
      }
      else {
        // Delete "chat> " and print the msg.
	      printf("\b\b\b\b\b\b%s            \n", buffer);
      }//end socket select       
    }//end loop
  }
}
