#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct
{
    int start;
    int end;
    float *v;
    float result;
} comms;

void *vector_sum(void *arg)
{
    printf("Dentro da thread!\n");

    comms *args = (comms *)arg;

    for (int i = args->start; i <= args->end; i++)
    {
        args->result = args->result + args->v[i];
        printf("Valor %d: %lf\n", i, args->v[i]);
    }

    printf("Start: %d\tEnd: %d\tResult: %lf\n", args->start, args->end, args->result);
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("No arguments given.");
        return 0;
    }

    int n = atoi(argv[1]);
    float *values = malloc(n * sizeof(float));

    for (int i = 0; i < n; i++)
    {
        values[i] = atof(argv[i + 2]);
        printf("Value %d: %lf\n", i, values[i]);
    }

    pthread_t tid_1, tid_2;

    comms args_1 = {.start = 0, .end = n / 2 - 1, .v = values, .result = 0};
    int error_1 = pthread_create(&tid_1, NULL, vector_sum, &args_1);

    comms args_2 = {.start = n / 2, .end = n - 1, .v = values, .result = 0};
    int error_2 = pthread_create(&tid_2, NULL, vector_sum, &args_2);

    pthread_join(tid_1, NULL);
    pthread_join(tid_2, NULL);

    printf("Result: %lf\n", args_1.result + args_2.result);

    return 0;
}