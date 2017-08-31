#include <sys/types.h> /* used to get pid_t, mode_t */
#include <sys/stat.h> /* used to set mode_t vars */
#include <fcntl.h> /* file control */
#include <unistd.h> /* for close(2) */
#include <stdio.h> /* perror() */
#include <stdlib.h> /* exit */
#include <sys/loadavg.h>
#include <errno.h> /* error handling */
#include <sys/mman.h> /* mem mapping */

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

int main (int argc, char *argv[]) {
    int fd;
    caddr_t mem;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    size_t size;

    struct data *in_data;
    time_t start_time = time(NULL);

    fd = open ("server_out_1", O_RDWR | O_TRUNC | O_CREAT, mode);
    if (fd < 0) 
        print_err_msg("open");
    size = sizeof(struct data);
    if( (ftruncate(fd, size)) < 0)
		print_err_msg("ftruncate");
    mem = mmap((caddr_t)0, size, PROT_WRITE, MAP_SHARED, fd, 0);
    if (mem == MAP_FAILED)
        print_err_msg("mem");
    if (close(fd) == -1)
        print_err_msg("close");
    

    in_data = (struct data*)(void*)mem;
    if ((in_data->pid = getpid()) < 0)
        print_err_msg("getpid");
    if ((in_data->uid = getuid()) < 0)
        print_err_msg("getuid");
    if ((in_data->gid = getgid()) < 0)
        print_err_msg("getgid");
    while (1) {
        in_data->live_time = time(NULL) - start_time;
        if (getloadavg(in_data->load_avg, 3) == -1)
            print_err_msg("getloadavg");
        sleep(1);
    }
    return EXIT_SUCCESS;
}
