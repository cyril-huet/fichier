#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>

const size_t BUFFER_SIZE = 32;

int main(int argc, char** argv)
{
    if (argc < 3)
        errx(EXIT_FAILURE, "Usage: %s FILE COMMAND [ARGUMENTS]...", argv[0]);

    // TODO

    return EXIT_SUCCESS;
}