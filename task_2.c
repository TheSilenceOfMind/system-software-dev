/* task #7 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>

#define ALPHABET_SIZE 26

char letters[ALPHABET_SIZE] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};

pthread_rwlock_t lock;

void* invert_case(void* arg);
void* reverse_order(void* arg);
void* count_uppercase(void* arg);

int main(int argc, char* argv[]) {

    pthread_t thread_addr;
    int i;

    if (pthread_rwlock_init(&lock, NULL) != 0) {
        perror("rwlock_init ");
        exit(EXIT_FAILURE);
    }
    if (pthread_create(&thread_addr, NULL, invert_case, NULL) != 0) {
        perror("pthread_create invert_case ");
        exit(EXIT_FAILURE);
    }
    if (pthread_create(&thread_addr, NULL, reverse_order, NULL) != 0) {
        perror("pthread_create reverse_order ");
        exit(EXIT_FAILURE);
    }
    if (pthread_create(&thread_addr, NULL, count_uppercase, NULL) != 0) {
        perror("pthread_create count_uppercase ");
        exit(EXIT_FAILURE);
    }
    while (1) {
        if (pthread_rwlock_rdlock(&lock) != 0) {
            perror("pthread_rwlock_rdlock ");
            exit(EXIT_FAILURE);
        }
        for (i = 0; i < ALPHABET_SIZE; i++) {
            printf("%c ", letters[i]);
        }
        printf("\n");
        if (pthread_rwlock_unlock(&lock) != 0) {
            perror("pthread_rwlock_unlock ");
            exit(EXIT_FAILURE);
        }
        usleep(5*1000000);
    }
    return EXIT_SUCCESS;
}

void* invert_case(void* arg) {
    int i;
    while (1) {
        if (pthread_rwlock_wrlock(&lock) != 0) {
            perror ("pthread_rw_wrlock ");
            exit(EXIT_FAILURE);
        }
        for (i = 0; i < ALPHABET_SIZE; i++) {
            if ((letters[i] >= 'A') && (letters[i] <= 'Z')) {
                letters[i] += 32;
            } else {
                letters[i] -= 32;
            }
        }
        if (pthread_rwlock_unlock(&lock) != 0) {
            perror ("pthread_rw_wrlock ");
            exit(EXIT_FAILURE);
        }
        usleep(4*1000000);
    }
}

void* reverse_order(void* arg) {
    int i;
    char c;
    while (1) {
        if (pthread_rwlock_wrlock(&lock) != 0) {
            perror ("pthread_rw_wrlock ");
            exit(EXIT_FAILURE);
        }
        for (i = 0; i < ALPHABET_SIZE / 2; i++) {
            c = letters[i];
            letters[i] = letters[ALPHABET_SIZE - 1 - i];
            letters[ALPHABET_SIZE - 1 - i] = c;
        }
        if (pthread_rwlock_unlock(&lock) != 0) {
            perror ("pthread_rw_wrlock ");
            exit(EXIT_FAILURE);
        }
        usleep(3*1000000);
    }
}

void* count_uppercase(void* arg) {
    int i;
    while (1) {
        int counter = 0;
        if (pthread_rwlock_wrlock(&lock) != 0) {
            perror ("pthread_rw_wrlock ");
            exit(EXIT_FAILURE);
        }
        for (i = 0; i < ALPHABET_SIZE; i++) {
            if (letters[i] >= 'A' && letters[i] <= 'Z') {
                counter++;
            }
        }
        printf("Number of the uppercase letters: %d\n", counter);
        if (pthread_rwlock_unlock(&lock) != 0) {
            perror ("pthread_rw_wrlock ");
            exit(EXIT_FAILURE);
        }
        usleep(2000000);
    }
    
}
