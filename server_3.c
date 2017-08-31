/* varian #8 : UDS - UNIX Domain Socket */
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h> /* perror() read write*/
#include <stdlib.h> /* exit */
#include <string.h> /* memset strncpy*/
#include <unistd.h> /* unlink */
#include <sys/loadavg.h>
#include <sys/time.h>

struct data {
    int pid,
        uid,
        gid;
    time_t live_time;
    double load_avg[3];
};

char* socket_path = "socket";

int main (int argc, char *argv[]) {

    struct sockaddr_un 
        clnt_addr,
        srv_addr;
    int srv, clnt;
    
    time_t start_time = time(NULL);
    struct data info = {0, 0, 0, 0, {0, 0, 0}};

    srv = socket(AF_UNIX, SOCK_STREAM, 0);
    if (srv == -1) {
        perror("Socket server error");
        exit(EXIT_FAILURE);
    }
    /* clean memory before using */
    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sun_family = AF_UNIX;
    strncpy(srv_addr.sun_path, socket_path, sizeof(srv_addr.sun_path)-1);
    unlink(socket_path);

    if (bind(srv, (struct sockaddr*)&srv_addr, sizeof(srv_addr)) == -1) {
        perror("bind error");
        exit(EXIT_FAILURE);
    }

    /* listen for socket connections and limit the queue of incoming connections */
    if (listen(srv, 5) == -1) {
        perror("listen error");
        exit(EXIT_FAILURE);
    }

    /* save necessary info */
    {
        if ((info.pid = getpid()) < 0) {
            perror("getpid");
            exit(EXIT_FAILURE);
        }
        if ((info.uid = getuid()) < 0) {
            perror("getuid");
            exit(EXIT_FAILURE);
        }
        if ((info.gid = getgid()) < 0) {
            perror("getgid");
            exit(EXIT_FAILURE);
        }
    }
    
    while (1) {
        fd_set fds;
        struct timeval timeout;
        int rc;
        char c[11]=  "Connected!\n";

        timeout.tv_sec = (time_t) 1;
        timeout.tv_usec = (time_t) 0;
        
        FD_ZERO(&fds);
        FD_SET(srv, &fds);

        info.live_time = time(NULL) - start_time;
        if (getloadavg(info.load_avg, 3) == -1) {
            perror("getloadavg error");
            exit(EXIT_FAILURE);
        }

        /* rc reterns number of ready socket descriptors */
        rc = select(sizeof(fds), &fds, NULL, NULL, &timeout);
        if (rc == -1) {
            perror("select error");
            exit(EXIT_FAILURE);
        }
        if (rc > 0) {
            write(1, &c, 11);
            if (FD_ISSET(srv, &fds)) {
                size_t clnt_size = sizeof(clnt_addr);
                clnt = accept(srv, (struct sockaddr*)&clnt_addr, (socklen_t*)&clnt_size);
                if (clnt == -1) {
                    perror("accept client error");
                    exit(EXIT_FAILURE);
                }
                if (write(clnt, &info, sizeof(info)) < 0) {
                    perror("write in clnt error");
                    exit(EXIT_FAILURE);
                }
                if (close(clnt) == -1) {
                    perror("close client fd error");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    return EXIT_SUCCESS;
}
