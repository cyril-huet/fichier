#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <err.h>
#include <sys/wait.h>

double diff_timespec(const struct timespec *t1, const struct timespec *t0)
{
    double second = difftime(t1->tv_sec, t0->tv_sec);
    return second + ((double)t1->tv_nsec - (double)t0->tv_nsec) / 1e9;
}

int main(int argc, char **argv)
{
    // If the number of arguments is not valid,
    // exits with an error message.
    if (argc == 1)
        errx(EXIT_FAILURE, "Usage: %s COMMAND [ARGUMENTS]...", argv[0]);

    // TODO

    return EXIT_SUCCESS;
}
