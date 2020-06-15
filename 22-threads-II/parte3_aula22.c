#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

struct calculo_args
{
    double *vetor;
    int start, end;
    sem_t *semaphore_soma, *semaphore_s1, *semaphore_s2;
    int size;
};

double soma = 0;
double variancia = 0;
void *calculo(void *_arg)
{
    struct calculo_args *spa = _arg;

    for (int i = spa->start; i < spa->end; i++)
    {
        if (spa->semaphore_soma != NULL)
            sem_wait(spa->semaphore_soma);
        soma += spa->vetor[i];
        if (spa->semaphore_soma != NULL)
            sem_post(spa->semaphore_soma);
    }

    if (spa->start == 0)
    {
        sem_post(spa->semaphore_s1);
        sem_wait(spa->semaphore_s2);
    }
    else
    {
        sem_post(spa->semaphore_s2);
        sem_wait(spa->semaphore_s1);
    }

    double media = soma / spa->size;

    for (int i = spa->start; i < spa->end; i++)
    {
        if (spa->semaphore_soma != NULL)
            sem_wait(spa->semaphore_soma);
        variancia += (spa->vetor[i] - media) * (spa->vetor[i] - media) / (spa->size - 1);
        if (spa->semaphore_soma != NULL)
            sem_post(spa->semaphore_soma);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    double *vetor = NULL;
    int n;
    scanf("%d", &n);

    vetor = malloc(sizeof(double) * n);
    for (int i = 0; i < n; i++)
    {
        scanf("%lf", &vetor[i]);
    }

    pthread_t ids[2];
    struct calculo_args args[2];

    sem_t semaphore_soma;
    sem_init(&semaphore_soma, 1, 1);

    sem_t semaphore_s1;
    sem_init(&semaphore_s1, 1, 0);

    sem_t semaphore_s2;
    sem_init(&semaphore_s2, 1, 0);

    for (int i = 0; i < 2; i++)
    {
        args[i].vetor = vetor;
        args[i].start = i * n / 2;
        args[i].end = (i + 1) * n / 2;
        args[i].semaphore_soma = &semaphore_soma;
        args[i].semaphore_s1 = &semaphore_s1;
        args[i].semaphore_s2 = &semaphore_s2;
        args[i].size = n;

        if (i == 2 - 1)
            args[i].end = n;

        pthread_create(&ids[i], NULL, calculo, &args[i]);
    }

    for (int i = 0; i < 2; i++)
    {
        pthread_join(ids[i], NULL);
    }

    sem_destroy(&semaphore_soma);
    sem_destroy(&semaphore_s1);
    sem_destroy(&semaphore_s2);

    struct calculo_args aa;
    aa.vetor = vetor;
    aa.start = 0;
    aa.end = n;
    aa.semaphore_soma = NULL;
    aa.semaphore_s1 = NULL;
    aa.semaphore_s2 = NULL;
    aa.size = n;
    printf("soma: %lf\n", soma);
    printf("variancia: %lf\n", variancia);

    return 0;
}