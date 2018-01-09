#include "cmd.h"

char* cmds[] = {
    "help",
};

char* cmd_descriptions[] = {
    "Prints help"
};

void handle_cmd(char* cmd) {
    if (strcmp(cmd, "help") == 0) {
        print_help_and_exit();
    }
}
