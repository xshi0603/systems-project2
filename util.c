#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cmd.h"

void print_help() {
  printf("List of commands:\n");
  for (size_t i = 0; ; i++) {
    if (cmds[i] == NULL) {
      break;
    }
    printf("%s - %s\n", cmds[i], cmd_descriptions[i]);
  }
}
