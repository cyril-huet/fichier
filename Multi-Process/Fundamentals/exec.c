#include <stdlib.h>
#include <unistd.h>
#include <err.h>

int main(int argc, char** argv)
{
    if (argc < 2)
        errx(EXIT_FAILURE, "Usage: %s COMMAND [ARGUMENTS]...", argv[0]);

    // TODO

    return EXIT_SUCCESS;
}