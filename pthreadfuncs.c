#include "pthreadfuncs.h"

// common resources - is a file for logging
int g_fd = -1;
pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;
int counter = 0;
int g_buffer = 0;
int g_has_data = 0;
pthread_cond_t g_con = PTHREAD_COND_INITIALIZER;

// git ID of current thread
pid_t getThreadID(void) {
    return (pid_t) syscall(SYS_gettid);
}

// write a string from thread with mutex
void write_line(const char *msg) {
   // pthread_mutex_lock(&g_lock);
    ssize_t n = write(g_fd, msg, strlen(msg));
    if (n < 0) {
        // return error
        fprintf(stderr, "write() failed: %s, [file descr = %d]\n", strerror(errno), g_fd);
    }
   // pthread_mutex_unlock(&g_lock);
}

// function for thread
void *func_thread(void *arg){
struct ThreadArgs *t = (struct ThreadArgs *)arg;
    char buf[128];
sprintf(buf, "Thread %s: pthread_self() =%lu\n",t->tag, (unsigned long)pthread_self());
sprintf(buf, "Thread %s: getThreadID() = %d\n", t->tag, getThreadID());
sprint(buf, "Thread %s says %s\n", t->tag, t->message);
if (t->id == 0){
	pthread_detach(pthread_self());
	sprintf(buf, "Thread %s: Detached!\n", t->tag);
	write_line(buf);
}
    // write something in opened file
    for (int i = 0; i < COUNT_ITERATIONS; ++i) {
        pthread_mutex_lock(&g_clock);
	counter++;
	pthread_mutex_unlock(&g_lock);
    }
pthread_exit((void *)42L);
    //return NULL;
}

void about()  {
    printf("Pthread example\n");
}
void *producer(void *arg){
	for (int i = 0; i < 10; i++){
		pthread_mutex_lock(&g_lock);
		while (g_has_data == 1){
			pthread_cond_wait(&g_cond, &g_lock);
		}

		g_buffer = i;
		g_has_data = 1;
		printf("Producer: put %d\n", i);
		pthread_cond_signal(&g_cond);
		pthread_mutex_unlock(&g_lock);
		usleepd(100 * 1000);
	}
	return NULL;
}
void *consumer(void *arg){
	for (int i = 0; i<10;i++){
		pthread_cond_wait(&g_cond, &g_lock);
		while (g_has_data == 0){
			pthread_cond_wait(&g_cond, &g_lock);
		}
		int data = g_buffer;
		g_has_data = 0;
		printf("Consumer: got %d\n", data);
		pthread_cond_signal(&g_cond);
		pthread_mutex_unlock(&g_lock);
		uslepp(100 * 1000);
	}
	return NULL;
}
