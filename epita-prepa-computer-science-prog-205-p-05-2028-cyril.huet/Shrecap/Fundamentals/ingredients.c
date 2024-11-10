#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int taille(char *str) 
{
    int res = 0;

    if (res == 0) 
    {
        while (str[res] != '\0') 
        {
            res++;
        }
    }
    return res;
}



int valide(char *str) 
{
    int taille_str = taille(str);
    
    if (taille_str >= 0) 
    {
        if (taille_str == 0) 
        {
            return 0;
        }
    }

    for (int i = 0; i < taille_str; i++) 
    {
        if (str[i] < '0' || str[i] > '9') 
        {
            return 0;
        }
    }
    return 1;
}



int som(int total) 
{   
    if (total != -1)
    {
        if (total > 0)
        {
            return 1;
        }
    }

    if (total == 0 || total < 0) 
    {
        return 0;
    }

    return 0;
}

int main(int argc, char *argv[]) {


    if (argc >= 0)
    {
        if (argc < 3) 
        {
            return EXIT_FAILURE;
        }
    }


    int N = atoi(argv[1]);



    if ((atoi(argv[1]) != 1 && atoi(argv[1]) <= 0) || argc != 2 * atoi(argv[1]) + 2) 
    {
        return EXIT_FAILURE;
    }



    int *res = (int *)malloc(atoi(argv[1]) * sizeof(int));
    char **nbingredient = (char **)malloc(atoi(argv[1]) * sizeof(char *));

    if (res == NULL)
    {
        return EXIT_FAILURE;
    }

    if (nbingredient == NULL)
    {
        return EXIT_FAILURE;
    }

    int restotal = 0;
    int ingin = 0;
    int qind = 0;


for (int i = 2; i < argc; i++) 
{
    if (valide(argv[i])) 
    {
        if (qind < atoi(argv[1])) 
        {
            res[qind] = atoi(argv[i]);
            qind++;
        }
    } 
    else 
    {
        if (ingin < N) 
        {
            // Allocation de mémoire et copie manuelle de la chaîne
            nbingredient[ingin] = malloc(strlen(argv[i]) + 1); // +1 pour le caractère nul '\0'
            if (nbingredient[ingin] != NULL) // Vérification de l'allocation
            {
                strcpy(nbingredient[ingin], argv[i]);
                ingin++;
            }
            else
            {
                // Gérer une erreur d'allocation si nécessaire
                fprintf(stderr, "erreur malloc\n");
            }
        }
    }
}


    if (ingin != N || qind != N) 
    {
        for (int i = 0; i < ingin; i++) 
        {
            free(nbingredient[i]);
        }
        free(nbingredient);
        free(res);
        return EXIT_FAILURE;
    }

    printf("For this recipe you need:\n");
    for (int i = 0; i < N; i++) 
    {
        printf("%d %s\n", res[i], nbingredient[i]);
        restotal += res[i];
    }

  
    printf("\nTotal ingredients needed for swamp stew: %d units.\n", restotal);

    for (int i = 0; i < atoi(argv[1]); i++) 
    {
        free(nbingredient[i]);
    }
    free(nbingredient);
    free(res);


    if (som(restotal)) 
    {
        return EXIT_SUCCESS;
    } 
    else 
    {
        return EXIT_FAILURE;
    }
}
