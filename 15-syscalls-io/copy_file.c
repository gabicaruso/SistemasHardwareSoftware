/* POSIX headers */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* Standard C */
#include <stdio.h>

int main(int argc, char *argv[])
{
    char arq1[100]; // nomes do arquivo 1, recebido via scanf
    char arq2[100]; // nomes do arquivo 2, recebido via scanf
    char buf[1];    // local usado para guardar os dados lidos de arq2

    scanf("%s", arq1); // pegar o nome do arquivo 1
    scanf("%s", arq2); // pegar o nome do arquivo 2

    int fd1 = open(arq1, O_RDONLY);                 // abrir arquivo 1
    int fd2 = open(arq1, O_WRONLY | O_CREAT, 0700); // abrir arquivo 2

    int bytes_read = read(fd1, buf, 1);
    while (bytes_read != 0)
    {
        write(fd2, buf, 1);
        bytes_read = read(fd1, buf, 1);
    }
    close(fd1);

    return 0;
}
