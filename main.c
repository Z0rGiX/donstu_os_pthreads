#include "pthreadfuncs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/syscall.h>


int main(void) {
    // headline
    about();
    // array with threads
    pthread_t threads[COUNT_THREADS];
    struct ThreadArgs args[COUNT_THREADS];
    pthread_t prod_thread, cons_thread;
    pthread_create(&prod_thread, NULL,producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);
    // sys call - open
    // file, modes, rights
    g_fd = open("output.log", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (g_fd < 0) {
        perror("open");
        return EXIT_FAILURE;
    }

    write_line("output.log");

    // create structs for threads 
    // create threads
    for (int i = 0; i < COUNT_THREADS; i++) {
	sprintf(args[i].tag, "Thread %d", i);
	sprintf(args[i].message, "Hello from thread %d!", i);
        int rc = pthread_create(&threads[i], NULL, func_thread, &args[i]);
        if (rc != 0) {
            fprintf(stderr, "pthread_create: %s\n", strerror(rc));
            return EXIT_FAILURE;
        }
    }

    // wait stoping all thread
    for (int i = 0; i < COUNT_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // sys call for close file
    if (close(g_fd) < 0) {
        perror("close");
        return EXIT_FAILURE;
    }
    // remove mutex
    pthread_mutex_destroy(&g_lock);
    write_line("output.log");
    return EXIT_SUCCESS;
}
