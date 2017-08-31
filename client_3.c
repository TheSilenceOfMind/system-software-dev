/* varian #8 : UDS - UNIX Domain Socket */

#include <sys/socket.h>
#include <stdio.h> /* perror() */
#include <stdlib.h> /* exit */
#include <string.h> /* memset strncpy*/
#include <unistd.h> /* unlink close */

struct data {
    int pid,
        uid,
        gid;
    time_t live_time;
    double load_avg[3];
};

char* socket_path = "socket";

int main(int argc, char* argv[]) {

    struct sockaddr_un addr;
    int s;
    struct data in = {0, 0, 0, 0, {0, 0, 0}};

    s = socket(AF_UNIX, SOCK_STREAM, 0);
    if (s == -1) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }
    
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);

    if (connect(s, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("connect error");
        exit(EXIT_FAILURE);
    }
    if (read(s, &in, sizeof(in)) < 0) {
        perror("read error");
        exit(EXIT_FAILURE);
    }
    if (close(s) == -1) {
        perror("close error");
        exit(EXIT_FAILURE);
    }

    printf("--------------------------------\n");
    printf("pid: %d\n", in.pid);
    printf("uid: %d\n", in.uid);
    printf("gid: %d\n", in.gid);
    printf("Load average:\n");
    printf("1m: %f\n", in.load_avg[0]);
    printf("5m: %f\n", in.load_avg[1]);
    printf("15m: %f\n", in.load_avg[2]);
    printf("--------------------------------\n");

    return EXIT_SUCCESS;
}
