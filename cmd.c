#include "cmd.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

typedef void (*cmd_handler)(char**);

void get_cmd_name_and_args(char* cmd, char** cmd_name, char*** cmd_args) {
  *cmd_name = cmd;
  *cmd_args = (char**)calloc(sizeof(char*), 512);

  size_t i = 0;
  while (1) {
    strsep(&cmd, " ");
    if (cmd == NULL) {
      break;
    }
    (*cmd_args)[i] = cmd;
    i++;
  }
}

void handle_help(char** args) {
  print_help_and_exit();
}

void handle_up(char** args) {
}

void handle_down(char** args) {
}

void handle_left(char** args) {
}

void handle_right(char** args) {
}

void handle_peek(char** args) {
}

void handle_chat(char** args) {
}

char* cmds[] = {
    "help",
    "up",
    "down",
    "left",
    "right",
    "peek",
    "chat",
    NULL
};

char* cmd_descriptions[] = {
  "Prints help",
  "Moves up",
  "Moves down",
  "Moves left",
  "Moves right",
  "Inspect object in front of you",
  "Enter chat room",
    NULL
};

cmd_handler handlers[] = {
  handle_help,
  handle_up,
  handle_down,
  handle_left,
  handle_right,
  handle_peek,
  handle_chat,
  NULL
};

void handle_cmd(char* cmd) {
  // Note: these don't allocate strings. Don't free the strings.
  // get_cmd_args does allocate, but only for the pointers to the strings.
  char* cmd_name;
  char** cmd_args;
  get_cmd_name_and_args(cmd, &cmd_name, &cmd_args);
  
  for (size_t i = 0; ; i++) {
    if (cmds[i] == NULL) {
      break;
    }
    if (strcmp(cmd_name, cmds[i]) == 0) {
      handlers[i](cmd_args);
      break;
    }
  }

  free(cmd_args);
}
