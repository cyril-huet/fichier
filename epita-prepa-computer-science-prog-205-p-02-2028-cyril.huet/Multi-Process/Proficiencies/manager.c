#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>

const size_t BUFFER_SIZE = 32;
pid_t waitpid(pid_t pid, int *status, int options);
int main(int argc, char** argv)
{
    if (argc < 3)
        errx(EXIT_FAILURE, "Usage: %s FILE COMMAND [ARGUMENTS]...", argv[0]);

    
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1)
        err(EXIT_FAILURE, "Could not open '%s'", argv[1]);
    int pipe1[2], pipe2[2];
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1)
        err(EXIT_FAILURE, "Pipe error");

    pid_t pid = fork();
    if (pid == -1)
        err(EXIT_FAILURE, "Fork error");

    if (pid == 0)
    {
        dup2(pipe1[0], STDIN_FILENO);
        dup2(pipe2[1], STDOUT_FILENO);
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);
        execvp(argv[2], &argv[2]);
        err(EXIT_FAILURE, "Could not execute '%s'", argv[2]);
    }
    else
    {
        close(pipe1[0]);
        close(pipe2[1]);
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;
        while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
        {
            if (write(pipe1[1], buffer, bytes_read) != bytes_read)
                errx(EXIT_FAILURE, "Could not write data to worker");
        }
        close(pipe1[1]);
        close(fd);
        while ((bytes_read = read(pipe2[0], buffer, BUFFER_SIZE)) > 0)
        {
            if (write(STDOUT_FILENO, buffer, bytes_read) != bytes_read)
                errx(EXIT_FAILURE, "Could not write data to stdout");
        }
        close(pipe2[0]);
        int status;
        if (waitpid(pid, &status, 0) == -1)
            err(EXIT_FAILURE, "Waitpid error");
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
            errx(EXIT_FAILURE, "Could not read data from %s", argv[2]);
    }
    return EXIT_SUCCESS;
}

