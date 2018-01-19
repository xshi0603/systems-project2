int main() {
    fd_set read_fds;
    fd_set write_fds;
    fd_set except_fds;

    build_fd_sets(&read_fds, &write_fds, &except_fds);

    while (1) {
        select();
    }

    exit(0);
}
