#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

struct question {
    char *question;
    char *answer;
};
typedef struct question question_t;

struct timer_args{
    pthread_cond_t *cond;
    pthread_mutex_t  *timerMutex;
    int *timeup;
};
typedef struct timer_args timer_args_t;

struct ask_args {
    int *score;
    question_t *question;
    pthread_mutex_t *mutex;
    int q_number;

};
typedef struct ask_args ask_args_t;

void *timer(void *args) {
    timer_args_t *timerArgs = args;
    int *timeup = timerArgs->timeup;
    pthread_mutex_lock(timerArgs->timerMutex);
    sleep(5);
    printf("\n");
    printf("5s have passed\n");
    *timeup = 1;
    pthread_mutex_unlock(timerArgs->timerMutex);
    pthread_cond_broadcast(timerArgs->cond);
    pthread_exit(NULL);
}

void *ask(void *args) {
    ask_args_t *ask_args = args;
    pthread_mutex_lock(ask_args->mutex);
    int *score = ask_args->score;

    printf("%s? ", ask_args->question->question);

    char input[128];
    fgets(input, 128, stdin);

    // strip newline
    input[strcspn(input, "\n")] = '\0';

    if (!strcmp(input, ask_args->question->answer)) {
        printf("Correct!\n");
        *score = *score+1;
    } else {
        printf("Incorrect :-(\n");
    }

    printf("You answered question %d\n",ask_args->q_number);
    ///
    pthread_mutex_unlock(ask_args->mutex);
    ///
    pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {
    question_t questions[] = {
        {.question = "3*2", .answer = "6"},
        {.question = "50/10", .answer = "5"},
        {.question = "2+1+1+1", .answer = "5"},
        {.question = "3^3", .answer = "27"},
        {.question = "3+3", .answer = "6"},
        {.question = "4/2", .answer = "2"}};

    ///

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t  timerMutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t timeup = PTHREAD_COND_INITIALIZER;
    int timeupFlag = 0;


    ///

    pthread_t timer_thread;
    timer_args_t timerArgs = {.cond=&timeup,.timeup = &timeupFlag,.timerMutex = &timerMutex};
    if (pthread_create(&timer_thread, NULL, timer, &timerArgs)) {
        printf("Error creating timer thread.\n");
    }

    int score = 0;
    for (int i = 0; i < 6; ++i) {
        ask_args_t *ask_args = malloc(sizeof(ask_args_t));
        ask_args_t ask_arg = {.score = &score, .question = &questions[i],.mutex=&mutex,.q_number=i};
        *ask_args = ask_arg;
        pthread_t ask_thread;
        if (pthread_create(&ask_thread, NULL, ask, ask_args)) {
            printf("Error creating asker thread.\n");
        }
    }

    while (timeupFlag != 1){
        pthread_cond_wait(&timeup,&timerMutex);
    }


    printf("End of questions, final score %d\n", score);
    return 0;
}
