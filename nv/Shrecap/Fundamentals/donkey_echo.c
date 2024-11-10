#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) 
{
    if (argc < 0)
    {
        return EXIT_FAILURE;
    }
    if (argc != 3) 
    {
        return EXIT_FAILURE;
    }
    if (argc == 3)
    {
        char *res = argv[1];
        int nb = atoi(argv[2]);

        if (atoi(argv[2]) <= 0) 
        {
            return EXIT_FAILURE;
        }

        for (int i = 0; i < atoi(argv[2]); i++) 
        {
            pid_t pid;
            pid = fork();

            if (pid < 0) 
            {
                return EXIT_FAILURE;
            } 
            else if (pid == 0) 
            {
                printf("[Child PID: %d] %s\n", getpid(), res);
                exit(EXIT_SUCCESS); 
            }
        }

        for (int i = 0; i < nb; i++) 
        {
            int st;
            pid_t pid = wait(&st);
            if (pid == -1) 
            {
                return EXIT_FAILURE;
            }
        }
        printf("All echoes completed.\n");

        return EXIT_SUCCESS;
    }
    else
    {
        return EXIT_FAILURE;
    }
}

