#include <sys/socket.h>


char* socket_path = "socket";

int main (int argc, char *argv[]) {

    struct sockaddr_un 
        client_addr,
        s_addr;
    int fd;
    
    int pid, uid, gid;
    time_t living_time;
    double load_avg[3];

    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }
    /* clean memory before using */
    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sun_family = AF_UNIX;
    strncpy(s_addr.sun_path, socket_path, sizeof(s_addr.sun_path)-1);
    unlink(socket_path);

    if (bind(fd, (struct sockaddr*)&s_addr, sizeof(s_addr)) == -1) {
        perror("bind error");
        exit(EXIT_FAILURE);
    }

    if (listen(fd, 5) == -1) {
        perror("listen error");
        exit(EXIT_FAILURE);
    }

    if ((pid = getpid()) < 0) {
        perror("getpid");
        exit(EXIT_FAILURE);
    }
    if ((uid = getuid()) < 0) {
        perror("getuid");
        exit(EXIT_FAILURE);
    }
    if ((gid = getgid()) < 0) {
        perror("getgid");
        exit(EXIT_FAILURE);
    }
    while (1) {
        
    }

    return EXIT_SUCCESS;
}
