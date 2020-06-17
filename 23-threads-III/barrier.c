#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define N 10
volatile int t_count;

typedef struct
{
    int id;
    pthread_mutex_t *m;
    sem_t *s;
} thread_barrier_args;

void *thread_n(void *v)
{
    thread_barrier_args *args = (thread_barrier_args *)v;

    printf("Parte 1 thread %d\n", args->id);

    // TODO: a barreira vai aqui

    pthread_mutex_lock(args->m);
    t_count++;

    if (t_count == N)
    {
        for (int i = 0; i < N; i++)
            sem_post(args->s);
    }

    pthread_mutex_unlock(args->m);
    sem_wait(args->s);

    printf("Pós barreira thread %d\n", args->id);

    return NULL;
}

int main()
{
    // TODO: inicializar todas variáveis da barreira

    t_count = 0;

    pthread_mutex_t m;
    pthread_mutex_init(&m, NULL);

    sem_t s;
    sem_init(&s, 0, 0);

    // TODO: criar e esperar N threads

    pthread_t ids[N];
    thread_barrier_args args[N];

    for (int i = 0; i < N; i++)
    {
        args[i].id = i;
        args[i].s = &s;
        args[i].m = &m;
        pthread_create(&ids[i], NULL, thread_n, &args[i]);
    }

    for (int i = 0; i < N; i++)
    {
        pthread_join(ids[i], NULL);
    }

    sem_destroy(&s);
    pthread_mutex_destroy(&m);

    return 0;
}
