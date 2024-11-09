#include <stdio.h>
#include <stdlib.h>



int positive(int n)
{
    if (n < 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
int main(int argc, char *argv[]) {

    if (argc < 0)
    {
        return EXIT_FAILURE;
    }
    if (argc < 2)
    {
        return EXIT_FAILURE;
    }
    if (argc > 2){

    int arg = argc-1;
    int *res = malloc((arg) * sizeof(int));

    if (res == NULL)
    {

        return EXIT_FAILURE;
    }

    int compteur = arg;

    for (int i = 0; i < compteur; i++)
    {
        int temps = atoi(argv[i + 1]);

        if (positive(temps) == 0)
        {
            free(res);


            return EXIT_FAILURE;
        }

        if(compteur >= 0)

        {
            res[i] = temps;
        }

    }


    int i = 0;

    while (i < compteur)
    {

        if (res[i] % 4 == 0)
        {
            int prem = res[i] / 4;
            res[i] -= prem;

            int compteur2 = compteur +1;
            int *res2 = realloc(res, (compteur2) * sizeof(int));


            if (res2 == NULL)
            {
                free(res);
                return EXIT_FAILURE;
            }

            res = res2;
            res[compteur] = prem;

            compteur++;
        }
        else
        {
            i++;
        }
    }


    for (int i = 0; i < compteur; i++)
    {
        printf("%d ", res[i]);
    }
    printf("\n");
    free(res);
    return 0;
    }
}

