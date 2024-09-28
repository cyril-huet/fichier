#include <err.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define PRINT(fmt_, ...) printf("%i: " fmt_ "\n", getpid() __VA_OPT__(,) __VA_ARGS__);

int main()
{
    // Initializes x.
    int  x = 0;

    // Forks the process.
    // (Creates a new process.)
    int id = fork();

    // Exits if error.
    if (id == -1)
        err(EXIT_FAILURE, "fork()");

    // Parent process.
    else if (id)
    {
        PRINT("Inside parent");
        PRINT("Child id : %i", id);
        PRINT("Initial x: %i", x);
        x = -42;
    }

    // Child process.
    else
    {
        PRINT("Inside child");
        PRINT("Initial x: %i", x);
        x = 42;
    }

    // Parent and child processes.
    PRINT("Parent id: %i", getppid());
    PRINT("New x: %i", x);

    return EXIT_SUCCESS;
}
