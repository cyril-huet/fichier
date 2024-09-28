#include <stdlib.h>
#include <unistd.h>
#include <err.h>

int main(int argc, char** argv)
{
    if (argc < 2)
        errx(EXIT_FAILURE, "Usage: %s COMMAND [ARGUMENTS]...", argv[0]);

    execvp(argv[1], &argv[1]);
    err(EXIT_FAILURE, "execvp()");
    return EXIT_SUCCESS;
}
