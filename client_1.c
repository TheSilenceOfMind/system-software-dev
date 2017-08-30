#include <stdio.h> /* perror() */
#include <sys/stat.h> /* used to set mode_t vars */
#include <fcntl.h> /* file control */
#include <sys/mman.h>
#include <errno.h> /* error handling */
#include <stdlib.h> 
#include <unistd.h> /* for close(2) */


struct data {
    int pid,
        uid,
        gid;
    time_t live_time;
    double load_avg[3];
};

void print_err_msg(char str[]) {
    printf("%s\n", str);
    perror("client_1 ");
    errno = 0;
    exit(1);
}

int main (int argc, char* argv[]) {
    int fd;
    caddr_t mem;
    struct data *in_data;
    size_t size;

    fd = open("server_out_1", O_RDWR, S_IRUSR | S_IWUSR);
    if (fd < 0)
        print_err_msg("open");
    size = sizeof(struct data);
    
    mem = mmap((caddr_t)0, size, PROT_READ, MAP_SHARED, fd, 0);
    if (mem == MAP_FAILED)
        print_err_msg("mem");
    if (close(fd) == -1)
        print_err_msg("close");
    in_data = (struct data*)mem;
    printf("--------------------------------\n");
    printf("pid: %d\n", in_data->pid);
    printf("uid: %d\n", in_data->uid);
    printf("gid: %d\n", in_data->gid);
    printf("Load average:\n");
    printf("1m: %f\n", in_data->load_avg[0]);
    printf("5m: %f\n", in_data->load_avg[1]);
    printf("15m: %f\n", in_data->load_avg[2]);
    printf("--------------------------------\n");
    return EXIT_SUCCESS;
}
