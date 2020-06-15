#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define SIZE 4

void *tarefa_print_i(void *arg)
{
    int *i = (int *)arg;
    printf("i: %d\n", *i);
}

int main()
{
    int *vi = malloc(SIZE * sizeof(int));

    pthread_t tids[SIZE];

    for (int i = 0; i < SIZE; i++)
    {
        vi[i] = i;
        int error = pthread_create(&tids[i], NULL, tarefa_print_i, &vi[i]);
        pthread_join(tids[i], NULL);
    }

    return 0;
}