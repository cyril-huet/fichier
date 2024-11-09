#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


int longueur(const char *str) 
{
    int res = 0;

    if (str == NULL) 
    {
        return 0;
    }

    while (str[res] != '\0') 
    {
        res++;
    }
    
    return res;
}


int camarche(char c) 
{
    return (c >= '0' && c <= '9');
}


int vraicar(const char *str) 
{
    if (str == NULL) 
    {
        return 0;
    }
    
    int lon = longueur(str);
    
    if (lon == 0) 
    {
        return 0;
    }
    
    for (int i = 0; i < lon; i++) 
    {
        if (!camarche(str[i])) 
        {
            return 0;
        }
    }
    
    return 1;
}

int main(int argc, char *argv[]) 
{
    if (argc == 1)
    {
        return EXIT_FAILURE;
    }

    if (argc<2 )
    {
        return EXIT_FAILURE;
    }
    if (argc > 2)
    {

        int var = atoi(argv[1]);

        if (var <= 0) 
        {




            return EXIT_FAILURE;
        }

      
        for (int i = 2; i < argc; i++) 
        {
            if (!vraicar(argv[i])) 
            {
               
               
               
                return EXIT_FAILURE;
            }
        }

        int pipes[var][2];

        for (int i = 0; i < var; i++) 
        {
            if (pipe(pipes[i]) == -1) 
            {
                return EXIT_FAILURE;
            }

pid_t pid = fork();

if (pid < 0) 
{
    return EXIT_FAILURE;
}

if (pid == 0) 
{  
    close(pipes[i][0]); 
    int res = 0;





    int k = 2;
    for (int j = 0; j < i; j++) 
    {
        if (j < ((argc - 2) % var)) 
        {
            k += ((argc - 2) / var) + 1;
        } 
        else 
                    {
                        k += ((argc - 2) / var);
                    }
                }

                int fin = (argc - 2) / var;
                if (i < ((argc - 2) % var)) 
                {
                    fin += 1;
                }




                for (int j = 0; j < fin; j++) 
                {
                    if (k + j < argc) 
                    {
                        res += atoi(argv[k + j]);
                    }
                }



                write(pipes[i][1], &res, sizeof(int));
                close(pipes[i][1]);

                exit(0);
            }
        }

        int res2 = 0;

 
        for (int i = 0; i < var; i++) 
        {
            close(pipes[i][1]);
            int ouais2;
            read(pipes[i][0], &ouais2, sizeof(int));
            close(pipes[i][0]);

            printf("Pile %d total: %d\n", i+1, ouais2); 
            res2 += ouais2;

            wait(NULL);
        }

        printf("Dragon's total treasure value: %d\n", res2);

        return 0;
    }
    if (argc<2 )
    {
        return EXIT_FAILURE;
    }
}

