#include "networking.h"
#include "rooms.h"

//void process(char *s);
void subserver(int from_client, int pipe1[], struct room*, int id);
char ** parse_args( char * line, char * parse_for );
char * trim(char *command);

int main() {
  
  int listen_socket;
  int client_socket;
  int f, i;
  //char** args = (char **)calloc(6, sizeof(char *));
  int ** subservers;

  subservers = (int **)calloc(10,sizeof(int*));

  for(i = 0; i < 10; i++){
    subservers[i] = (int *)calloc(3,sizeof(int));
  }

  //int subservers1[10][2];
  int subserver_count;
  subserver_count = (int) malloc (sizeof(int));
  subserver_count = 0;

  char buffer[BUFFER_SIZE];

  //set of file descriptors to read from
  fd_set read_fds;
  
  //mkfifo for server to read from
  //INITIAL = 0, LEFT = 1, RIGHT = 2
  int c2s0, s2c0;
  int c2s1, s2c1;
  int c2s2, s2c2;

  mkfifo("WKP0", 0644);
  mkfifo("WKP1", 0644);
  mkfifo("WKP2", 0644);

  s2c0 = open("WKP0", O_WRONLY | O_NONBLOCK);
  c2s0 = open("WKP0", O_RDONLY | O_NONBLOCK);
  s2c0 = open("WKP0", O_WRONLY | O_NONBLOCK);

  s2c1 = open("WKP1", O_WRONLY | O_NONBLOCK);
  c2s1 = open("WKP1", O_RDONLY | O_NONBLOCK);
  s2c1 = open("WKP1", O_WRONLY | O_NONBLOCK);

  s2c2 = open("WKP2", O_WRONLY | O_NONBLOCK);
  c2s2 = open("WKP2", O_RDONLY | O_NONBLOCK);
  s2c2 = open("WKP2", O_WRONLY | O_NONBLOCK);

  //printf("c2s: %d\n", c2s);
  //printf("s2c: %d\n", s2c);

  //listen socket
  listen_socket = server_setup();

  //----------ROOMS-------------

  struct room * initialRoom = createRooms();
  initialRoom->readingPipe = c2s0;
  initialRoom->writingPipe = s2c0;
  initialRoom->left->readingPipe = c2s1;
  initialRoom->left->writingPipe = s2c1;
  initialRoom->right->readingPipe = c2s2;
  initialRoom->right->writingPipe = s2c2;

  //---------END ROOMS----------

  while (1) {

    //select() modifies read_fds
    //we must reset it at each iteration
    FD_ZERO(&read_fds); //0 out fd set
    FD_SET(STDIN_FILENO, &read_fds); //add stdin to fd set
    FD_SET(c2s0, &read_fds); //add socket to fd set
    FD_SET(c2s1, &read_fds); //add socket to fd set
    FD_SET(c2s2, &read_fds); //add socket to fd set
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
	subserver(client_socket, fd1, initialRoom, subserver_count);
      }
      else {//parent    
	close (fd1[READ]);
	//close (fd2[WRITE]);
	//c2s = open("WKP", O_RDONLY | O_NONBLOCK);
	subservers[subserver_count][0] = fd1[0];
	subservers[subserver_count][1] = fd1[1];	
	subservers[subserver_count][2] = initialRoom->id;
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
      *strchr(buffer, '\n') = 0;
      //printf("[server] subserver count: %d\n", subserver_count);
      if (!strcmp(buffer, "exit")) {//close all
	printf("exiting\n");
	for (i = 0; i < subserver_count; i++) {
	  write (subservers[i][WRITE], buffer, sizeof(buffer));
	} 
	//sleep(3);
	exit(0);
      }
      else {
	for (i = 0; i < subserver_count; i++) {
	  write (subservers[i][WRITE], buffer, sizeof(buffer));
	}
      }
    }//end stdin select


    if (FD_ISSET(c2s0, &read_fds)) {
      read(c2s0, buffer, sizeof(buffer));

      char subbuff[5];
      memcpy( subbuff, buffer, 4 );
      subbuff[4] = '\0';

      //printf("[main] reading from pipe: %s\n", buffer);
      if (strstr(buffer, "moving") != NULL){
	//printf("[main0] MOVING writing to clientz: %s\n", buffer);
	//change room of dude

	char ** stuff = parse_args(buffer, "_");

	int y = atoi(stuff[0]);//subserver id
	int y2 = atoi(stuff[2]);//room id

	//printf("%d = ", y2);
	//subservers[y][2] = 0;
	subservers[y][2] = y2;
	//printf("%d\n", subservers[y][2]);

	for (i = 0; i < subserver_count; i++) {
	  if (subservers[i][2] == 0) {
	    write (subservers[i][WRITE], "Someone has joined this room\n" , sizeof("Someone has joined this room\n"));
	  }
	}
      }

      else if (!strcmp(subbuff, "/all")) {
	char subbuff2[BUFFER_SIZE - 5];
	memcpy( subbuff2, &buffer[5], sizeof(subbuff2));
	subbuff2[strlen(subbuff2)] = 0;
	for (i = 0; i < subserver_count; i++) {
	  write (subservers[i][WRITE], subbuff2, sizeof(subbuff2));
	}
      }

      else {
	printf("[main] writing to clients: %s\n", buffer);
	for (i = 0; i < subserver_count; i++) {
	  if (subservers[i][2] == 0) {
	    write (subservers[i][WRITE], buffer, sizeof(buffer));
	  }
	}
      }
    }//end pipe select  

    if (FD_ISSET(c2s1, &read_fds)) {
      read(c2s1, buffer, sizeof(buffer));

      char subbuff[5];
      memcpy( subbuff, buffer, 4 );
      subbuff[4] = '\0';

      //printf("[main] reading from pipe: %s\n", buffer);
      if (strstr(buffer, "moving") != NULL){
	//printf("[main1] MOVING writing to clientz: %s\n", buffer);
	//change room of dude

	char ** stuff = parse_args(buffer, "_");

	int y = atoi(stuff[0]);//room id
	int y2 = atoi(stuff[2]);//subserver id

	//printf("%d = ", y2);
	//subservers[y][2] = 0;
	subservers[y][2] = y2;
	//printf("%d\n", subservers[y][2]);

	for (i = 0; i < subserver_count; i++) {
	  if (subservers[i][2] == 1) {
	    write (subservers[i][WRITE], "Someone has joined this room\n" , sizeof("Someone has joined this room\n"));
	  }
	}
      }

      else if (!strcmp(subbuff, "/all")) {
	char subbuff2[BUFFER_SIZE - 5];
	memcpy( subbuff2, &buffer[5], sizeof(subbuff2));
	subbuff2[strlen(subbuff2)] = 0;
	for (i = 0; i < subserver_count; i++) {
	  write (subservers[i][WRITE], subbuff2, sizeof(subbuff2));
	}
      }


      else {
	printf("[main] writing to clients: %s\n", buffer);
	for (i = 0; i < subserver_count; i++) {
	  if (subservers[i][2] == 1) {
	    write (subservers[i][WRITE], buffer, sizeof(buffer));
	  }
	}
      }
    }//end pipe select  

    if (FD_ISSET(c2s2, &read_fds)) {

      char subbuff[5];
      memcpy( subbuff, buffer, 4 );
      subbuff[4] = '\0';

      read(c2s2, buffer, sizeof(buffer));
      //printf("[main] reading from pipe: %s\n", buffer);
      if (strstr(buffer, "moving") != NULL){
	//printf("[main2] MOVING writing to clientz: %s\n", buffer);
	//change room of dude

	char ** stuff = parse_args(buffer, "_");

	int y = atoi(stuff[0]);//room id
	int y2 = atoi(stuff[2]);//subserver id

	//printf("%d = ", y2);
	//subservers[y][2] = 0;
	subservers[y][2] = y2;
	//printf("%d\n", subservers[y][2]);

	for (i = 0; i < subserver_count; i++) {	  
	  if (subservers[i][2] == 2) {
	    write (subservers[i][WRITE], "Someone has joined this room\n" , sizeof("Someone has joined this room\n"));
	  }
	}
      }

      else if (!strcmp(subbuff, "/all")) {	
	char subbuff2[BUFFER_SIZE - 5];
	memcpy( subbuff2, &buffer[5], sizeof(subbuff2));
	subbuff2[strlen(subbuff2)] = 0;
	for (i = 0; i < subserver_count; i++) {
	  write (subservers[i][WRITE], subbuff, sizeof(subbuff2));
	}
      }

      else {
	printf("[main] writing to clients: %s\n", buffer);
	for (i = 0; i < subserver_count; i++) {
	  if (subservers[i][2] == 2) {
	    write (subservers[i][WRITE], buffer, sizeof(buffer));
	  }
	}
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

void subserver(int client_socket, int pipe1[], struct room* currRoom, int id) {

  fd_set read_fds;
  char buffer[BUFFER_SIZE];
  char buffer2[BUFFER_SIZE];
  int pipe2;

  while (1) {

    pipe2 = currRoom->writingPipe;

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
      
      if (!strcmp(buffer, "exit")) {
	write(client_socket, buffer, sizeof(buffer));
	exit(0);
      }
      else {
	write(client_socket, buffer, sizeof(buffer));
      }

    }//end pipe select
    
    //if client triggered select
    if (FD_ISSET(client_socket, &read_fds)) {

      read(client_socket, buffer, sizeof(buffer));      
      if (!strcmp(buffer, "left")){
	currRoom = moveRooms(currRoom, 1);
	pipe2 = currRoom->writingPipe;
	sprintf(buffer2, "%d_moving_%d", id, currRoom->id);
	write(pipe2, buffer2, sizeof(buffer2));
      }
      else if (!strcmp(buffer, "right")) {
	currRoom = moveRooms(currRoom, 2);
	pipe2 = currRoom->writingPipe;
	sprintf(buffer2, "%d_moving_%d", id, currRoom->id);
	write(pipe2, buffer2, sizeof(buffer2));
      }
      else if (!strcmp(buffer, "exit")) {
	sprintf(buffer2, "ignore this");
	write(pipe2, buffer2, sizeof(buffer2));
	exit(0);
      }
      else {
	//printf("[subserver %d] received: [%s]\n", getpid(), buffer);
	//printf("[subserver]: sending to client:%d\n", client_socket);
	//process(buffer);
	
	//write(client_socket, buffer, sizeof(buffer));
	//blocks here? 
	
	//printf("[subserver %d] writing to mainserver\n", getpid());
	write(pipe2, buffer, sizeof(buffer)); //problem?
      }
    }//end client select
  }
}

char ** parse_args( char * line, char * parse_for ) {
  char** args = (char **)calloc(6, sizeof(char *));
  int i;
  for(i = 0; line; i++) {
    args[i] = strsep(&line, parse_for);
    args[i] = trim(args[i]);
  }
  args[i] = NULL;
  return args;
}

char * trim(char *command) {

  char * end = command + strlen(command) - 1;
  
  while (*command && isspace(*command)) {
    command++;
  }

  while (isspace(*end)) {
    *end-- = '\0';
  }
  
  return command;

}

/*
void process(char * s) {
  while (*s) {
    if (*s >= 'a' && *s <= 'z')
      *s = ((*s - 'a') + 13) % 26 + 'a';
    else  if (*s >= 'A' && *s <= 'Z')
      *s = ((*s - 'a') + 13) % 26 + 'a';
    s++;
  }
}
*/
