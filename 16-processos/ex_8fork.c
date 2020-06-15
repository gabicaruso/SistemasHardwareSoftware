#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    pid_t pai, filho;
    int id = 0;
    // processo pai aqui!
    pai = getpid();
    printf("eu sou o processo pai, pid=%d, meu id do programa é %d\n", pai, id);
    id++;

    for (int i = 0; i < 8; i++)
    {
        filho = fork();
        if (filho == 0)
        {
            // processo filhos aqui
            pai = getppid();
            filho = getpid();
            printf("eu sou o processo filho, pid=%d, pidd=%d, meu id do programa é %d\n", filho, pai, id);
            break;
        }
        id++;
    }

    int w;
    for (int i = 0; i < 8; i++)
    {
        wait(&w);
    }

    return 0;
}
