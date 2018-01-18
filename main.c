#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cmd.h"
#include "server_intf.h"

int main() {
    printf("Welcome to Club Penguin!\n");
    printf("By Team Waddles\n");

    printf("Connecting to server...\n");
    int sockd = server_setup();
    set_sockd(sockd);

    printf("Type commands to interact with the game. For a list of commands, type \"help\".\n");

    char cmd[512];
    while (1) {
      if (fgets(cmd, sizeof(cmd), stdin) == NULL) {
	break;
      }
	
        handle_cmd(cmd);
    }

    exit(0);
}
