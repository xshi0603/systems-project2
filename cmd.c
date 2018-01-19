#include "cmd.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_state.h"
#include "server_intf.h"
#include "util.h"

typedef void (*cmd_handler)(char**, size_t);

void get_cmd_name_and_args(char* cmd, char** cmd_name, char*** cmd_args, size_t* cmd_argc) {
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

  *cmd_argc = i;
}

char* str_join(char* delim, char** strvec, size_t strvec_len) {
  size_t total_len = (strvec_len - 1) * strlen(delim);
  size_t i = 0;
  for (; i < strvec_len; i++) {
    total_len += strlen(strvec[i]);
  }

  char* result = malloc(sizeof(char) * (total_len + 1));
  result[0] = '\0';
  
  for (; i < strvec_len - 1; i++) {
    strcat(result, strvec[i]);
    strcat(result, delim);
  }

  if (strvec_len != 0) {
    strcat(result, strvec[strvec_len - 1]);
  }

  return result;
}

void handle_help(char** args, size_t argc) {
  print_help();
}

void handle_up(char** args, size_t argc) {
  unsigned int num_to_move;
  if (argc != 1 || sscanf(args[0], "%u", &num_to_move) != 1) {
    print_help();
    return;
  }

  move_up(num_to_move);
}

void handle_down(char** args, size_t argc) {
  unsigned int num_to_move;
  if (argc != 1 || sscanf(args[0], "%u", &num_to_move) != 1) {
    print_help();
    return;
  }

  move_down(num_to_move);
}

void handle_left(char** args, size_t argc) {
  unsigned int num_to_move;
  if (argc != 1 || sscanf(args[0], "%u", &num_to_move) != 1) {
    print_help();
    return;
  }

  move_left(num_to_move);
}

void handle_right(char** args, size_t argc) {
  unsigned int num_to_move;
  if (argc != 1 || sscanf(args[0], "%u", &num_to_move) != 1) {
    print_help();
    return;
  }

  move_right(num_to_move);
}

void handle_peek(char** args, size_t argc) {
  if (argc != 0) {
    print_help();
    return;
  }

  char* peek_result = game_peek();
  printf("%s\n", peek_result);
  // No free: peek_result should be computed from a static lookup table.
  // No need to allocate.
}

void handle_chat(char** args, size_t argc) {
  if (argc == 0) {
    print_help();
    return;
  }
  
  char* message = str_join(" ", args, argc);
  send_chat_message(message);
  free(message);
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
  size_t cmd_argc;
  get_cmd_name_and_args(cmd, &cmd_name, &cmd_args, &cmd_argc);
  
  for (size_t i = 0; ; i++) {
    if (cmds[i] == NULL) {
      break;
    }
    if (strcmp(cmd_name, cmds[i]) == 0) {
      handlers[i](cmd_args, cmd_argc);
      break;
    }
  }

  free(cmd_args);
}
