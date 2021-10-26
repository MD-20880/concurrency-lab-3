#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *hello_world(void *args) {
    int *n = args;
    printf("Hello from thread %d\n", *n);
    pthread_exit(NULL);
}

typedef struct Value {
    int value;
}Value;

int main(int argc, char const *argv[]) {
    pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t)*5);
    for(int n = 1;n<=5;n++){
        int* num = malloc(sizeof(int));
        *num = n;
        if (pthread_create(&threads[n-1], NULL, hello_world, num)) {
            printf("Error creating thread\n");
        }

    }
    for(int n = 1;n<=5;n++){
        if (pthread_join(threads[n-1], NULL)) {
            printf("Error joining thread\n");
        }

    }



}
