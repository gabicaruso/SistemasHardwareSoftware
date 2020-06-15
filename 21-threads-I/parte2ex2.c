#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define SIZE 4
typedef struct
{
    int i;
    int j;
} nums;

void *tarefa_print_i(void *arg)
{
    nums *args = (nums *)arg;
    printf("i: %d j: %d\n", args->i, args->j);
}

int main()
{
    pthread_t tids[SIZE];

    for (int i = 0; i < SIZE; i++)
    {
        nums args = {.i = i, .j = i * 2};
        int error = pthread_create(&tids[i], NULL, tarefa_print_i, &args);
        pthread_join(tids[i], NULL);
    }

    return 0;
}