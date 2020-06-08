#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct
{
    int s;
    int e;
    float *v;
    float res;
} var;

void *vector_sum(void *arg)
{
    printf("Dentro da thread!\n");

    var *args = (var *)arg;

    for (int i = args->s; i <= args->e; i++)
    {
        args->res = args->res + args->v[i];
        printf("Valor %d: %lf\n", i, args->v[i]);
    }

    printf("Start: %d\tEnd: %d\tResult: %lf\n", args->s, args->e, args->res);
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

    var args_1 = {.s = 0, .e = n / 2 - 1, .v = values, .res = 0};
    int error_1 = pthread_create(&tid_1, NULL, vector_sum, &args_1);

    var args_2 = {.s = n / 2, .e = n - 1, .v = values, .res = 0};
    int error_2 = pthread_create(&tid_2, NULL, vector_sum, &args_2);

    pthread_join(tid_1, NULL);
    pthread_join(tid_2, NULL);

    printf("Result: %lf\n", args_1.res + args_2.res);

    return 0;
}