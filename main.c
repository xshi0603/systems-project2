int main() {
    printf("Welcome to Club Penguin!\n");
    printf("By Team Waddles\n");

    printf("Connecting to server...\n");
    struct server_intf server = get_server_intf();

    printf("Type commands to interact with the game. For a list of commands, type \"help\".\n");
    while (1) {
        char* cmd = read_line();
        handle_cmd(cmd);
        free(cmd);
    }

    return 0;
}
