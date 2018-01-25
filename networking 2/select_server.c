#include "networking.h"

void process(char *s);
void subserver(int from_client, int pipe1[], int pipe2);

int main() {
  
  int listen_socket;
  int client_socket;
  int f;
  int subservers[10][2];
  //int subservers1[10][2];
  int subserver_count = 0;
  char buffer[BUFFER_SIZE];

  //set of file descriptors to read from
  fd_set read_fds;
  
  int i;
  
  //mkfifo for server to read from
  int c2s, s2c;
  mkfifo("WKP", 0644);
  /*
  f = fork();
  if (f == 0){//child                                                                                                                                                         
    s2c = open("WKP", O_WRONLY);
    exit(0);
  }
  else {//parent                                                                                                                                                              
    c2s = open("WKP", O_RDONLY);
  }
  */

  s2c = open("WKP", O_WRONLY | O_NONBLOCK);
  c2s = open("WKP", O_RDONLY | O_NONBLOCK);
  s2c = open("WKP", O_WRONLY | O_NONBLOCK);

  printf("c2s: %d\n", c2s);
  printf("s2c: %d\n", s2c);

  //listen socket
  listen_socket = server_setup();

  while (1) {

    //select() modifies read_fds
    //we must reset it at each iteration
    FD_ZERO(&read_fds); //0 out fd set
    FD_SET(STDIN_FILENO, &read_fds); //add stdin to fd set
    FD_SET(c2s, &read_fds); //add socket to fd set
    FD_SET(listen_socket, &read_fds); //add socket to fd set
    //add each pipe
    /*
    for (i = 0; i < subserver_count; i++) {
      FD_SET(subservers1[i][READ], &read_fds);
      printf("fd_set %d\n", i);
    }
    */
    //select will block until either fd is ready
    select(listen_socket + 1, &read_fds, NULL, NULL, NULL);

    //if listen_socket triggered select
    if (FD_ISSET(listen_socket, &read_fds)) {
     
      client_socket = server_connect(listen_socket);
      //printf("client socket:%d\n", client_socket);     
      //unnamed pipes
      int fd1[2];
      pipe(fd1);
      //int fd2[2];
      //pipe(fd2);
      
      f = fork();
      if (f == 0){//child
	close (fd1[WRITE]);      
	//close (fd2[READ]);
	//s2c = open("WKP", O_WRONLY);       
	subserver(client_socket, fd1, s2c);
      }
      else {//parent    
	close (fd1[READ]);
	//close (fd2[WRITE]);
	//c2s = open("WKP", O_RDONLY | O_NONBLOCK);
	subservers[subserver_count][0] = fd1[0];
	subservers[subserver_count][1] = fd1[1];	
	//not working as planned?
	//subservers1[subserver_count][0] = fd2[0];
	//subservers1[subserver_count][1] = fd2[1];
	subserver_count++;
	close(client_socket);
      }
    }//end listen_socket select
    
    //if stdin triggered select
    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      //if you don't read from stdin, it will continue to trigger select()
      fgets(buffer, sizeof(buffer), stdin);
      //printf("[server] subserver count: %d\n", subserver_count);
      for (i = 0; i < subserver_count; i++) {
	write (subservers[i][WRITE], buffer, sizeof(buffer));
      }
    }//end stdin select
    if (FD_ISSET(c2s, &read_fds)) {
      read(c2s, buffer, sizeof(buffer));
      printf("[main] reading from pipe: %s\n", buffer);
      for (i = 0; i < subserver_count; i++) {
	write (subservers[i][WRITE], buffer, sizeof(buffer));
      }
    }//end pipe select  
    /*
    for (i = 0; i < subserver_count; i++) {
      if (FD_ISSET(subservers1[i][READ], &read_fds)) {
        printf("[main] reading from pipe");
        read(subservers1[i][READ], buffer, sizeof(buffer));
        int j;
        for (j = 0; j < subserver_count; j++) {
          write (subservers[j][WRITE], buffer, sizeof(buffer));
        }
      }//end pipe select    
    }
    */
  }
}

void subserver(int client_socket, int pipe1[], int pipe2) {

  fd_set read_fds;
  char buffer[BUFFER_SIZE];

  while (1) {

    //select() modifies read_fds
    //we must reset it at each iteration
    FD_ZERO(&read_fds); //0 out fd set
    FD_SET(client_socket, &read_fds); //add client to fd set
    FD_SET(pipe1[READ], &read_fds); //add pipe to fd set

    //select will block until either fd is ready
    select(pipe1[READ] + 1, &read_fds, NULL, NULL, NULL);

    //if pipe triggered select
    if (FD_ISSET(pipe1[READ], &read_fds)) {
      
      read(pipe1[READ], buffer, sizeof(buffer));
      printf("[SUBSERVER pipe] %s\n", buffer);
      write(client_socket, buffer, sizeof(buffer));

    }//end pipe select
    
    //if client triggered select
    if (FD_ISSET(client_socket, &read_fds)) {

      read(client_socket, buffer, sizeof(buffer));
      printf("[subserver %d] received: [%s]\n", getpid(), buffer);
      //printf("[subserver]: sending to client:%d\n", client_socket);
      process(buffer);
      
      //write(client_socket, buffer, sizeof(buffer));
      //blocks here? 
      
      printf("[subserver %d] writing to pipe\n", getpid());
      write(pipe2, buffer, sizeof(buffer)); //problem?
      
    }//end client select
  }
}

void process(char * s) {
  while (*s) {
    if (*s >= 'a' && *s <= 'z')
      *s = ((*s - 'a') + 13) % 26 + 'a';
    else  if (*s >= 'A' && *s <= 'Z')
      *s = ((*s - 'a') + 13) % 26 + 'a';
    s++;
  }
}
