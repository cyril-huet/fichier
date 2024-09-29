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
    struct timespec t0, t1;
    clock_gettime(CLOCK_REALTIME, &t0);
    pid_t p = fork();
    if (p < 0) {
        err(EXIT_FAILURE, "erreur de fork");
    }
    else if (p == 0) {
        execvp(argv[1], &argv[1]);
        err(EXIT_FAILURE, "erreur de execvp()");
    }
    else {
        int s;
        waitpid(p, &s, 0);
        clock_gettime(CLOCK_REALTIME, &t1);
        double res = diff_timespec(&t1, &t0);
        printf("time = %f s\n", res);
        if (WIFEXITED(s)) {
            return WEXITSTATUS(s);
        } else {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS; 
}
