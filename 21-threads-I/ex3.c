#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define SIZE 4

typedef struct
{
    int x;
    int y;
    int value;
} comms;

void *tarefa_print_i(void *arg)
{
    comms *args = (comms *)arg;
    args->value = args->x * args->y;
    printf("Dentro da thread!\nx: %d\ty: %d\tvalue: %d\n", args->x, args->y, args->value);
}

int main()
{
    pthread_t tids[SIZE];

    for (int i = 0; i < SIZE; i++)
    {
        comms args = {.x = i, .y = 2};
        int error = pthread_create(&tids[i], NULL, tarefa_print_i, &args);
        pthread_join(tids[i], NULL);
        printf("Fora da thread!\nx: %d\ty: %d\tvalue: %d\n", args.x, args.y, args.value);
    }

    return 0;
}