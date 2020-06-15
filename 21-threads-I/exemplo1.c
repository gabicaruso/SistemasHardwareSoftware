
// #include <pthread.h>
// #include <stdio.h>

// void *minha_thread(void *arg)
// {
//     printf("Hello thread!\n");
//     return NULL;
// }

// int main()
// {
//     pthread_t tid;

//     int error = pthread_create(&tid, NULL, minha_thread, NULL);

//     printf("Hello main\n");

//     pthread_join(tid, NULL);

//     return 0;
// }

#include <pthread.h>
#include <stdio.h>

void *minha_thread1(void *arg)
{
    printf("1\n");
    return NULL;
}

void *minha_thread2(void *arg)
{
    printf("2\n");
    return NULL;
}

void *minha_thread3(void *arg)
{
    printf("3\n");
    return NULL;
}

void *minha_thread4(void *arg)
{
    printf("4\n");
    return NULL;
}
int main()
{
    pthread_t tid1;
    pthread_t tid2;
    pthread_t tid3;
    pthread_t tid4;

    int error1 = pthread_create(&tid1, NULL, minha_thread1, NULL);
    int error2 = pthread_create(&tid2, NULL, minha_thread2, NULL);
    int error3 = pthread_create(&tid3, NULL, minha_thread3, NULL);
    int error4 = pthread_create(&tid4, NULL, minha_thread4, NULL);

    printf("Hello main\n");

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);

    return 0;
}
