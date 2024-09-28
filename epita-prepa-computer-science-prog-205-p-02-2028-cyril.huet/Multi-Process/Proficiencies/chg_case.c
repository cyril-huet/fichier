#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <err.h>

const size_t BUFFER_SIZE = 32;

void to_lower(int in_fd, int out_fd)
{
    char buffer[BUFFER_SIZE];
    ssize_t r;

    while ((r = read(in_fd, buffer, BUFFER_SIZE)))
    {
        if (r == -1)
            err(EXIT_FAILURE, "Could not read data");

        for (size_t i = 0; i < (size_t)r; i++)
            if (buffer[i] >= 'A' && buffer[i] <= 'Z')
                buffer[i] += 'a' - 'A';

        if (write(out_fd, buffer, r) != r)
            errx(EXIT_FAILURE, "Could not write data");
    }
}

void to_upper(int in_fd, int out_fd)
{
    char buffer[BUFFER_SIZE];
    ssize_t r;

    while ((r = read(in_fd, buffer, BUFFER_SIZE)))
    {
        if (r == -1)
            err(EXIT_FAILURE, "Could not read data");

        for (size_t i = 0; i < (size_t)r; i++)
            if (buffer[i] >= 'a' && buffer[i] <= 'z')
                buffer[i] += 'A' - 'a';

        if (write(out_fd, buffer, r) != r)
            errx(EXIT_FAILURE, "Could not write data");
    }
}

int main(int argc, char** argv)
{
    if (argc == 1)
        to_lower(STDIN_FILENO, STDOUT_FILENO);

    else if (argc == 2)
    {
        if (strcmp(argv[1], "-l") == 0)
            to_lower(STDIN_FILENO, STDOUT_FILENO);
        else if (strcmp(argv[1], "-U") == 0)
            to_upper(STDIN_FILENO, STDOUT_FILENO);
        else
            errx(EXIT_FAILURE, "Usage: %s [-l|-U]", argv[0]);
    }

    else
        errx(EXIT_FAILURE, "Usage: %s [-l|-U]", argv[0]);

    return EXIT_SUCCESS;
}
