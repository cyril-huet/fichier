#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>

const size_t BUFFER_SIZE = 32;

int main(int argc, char** argv)
{
    if (argc < 3)
        errx(EXIT_FAILURE, "Usage: %s FILE COMMAND [ARGUMENTS]...", argv[0]);

    // Open the text file
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1)
        err(EXIT_FAILURE, "Could not open '%s'", argv[1]);

    // Create two pipes
    int pipe1[2], pipe2[2];
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1)
        err(EXIT_FAILURE, "Pipe error");

    pid_t pid = fork();
    if (pid == -1)
        err(EXIT_FAILURE, "Fork error");

    if (pid == 0) // Child process (Worker)
    {
        // Redirect stdin to pipe1 read end, stdout to pipe2 write end
        dup2(pipe1[0], STDIN_FILENO);
        dup2(pipe2[1], STDOUT_FILENO);

        // Close unused pipe ends
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);

        // Execute worker with the provided arguments
        execvp(argv[2], &argv[2]);
        err(EXIT_FAILURE, "Could not execute '%s'", argv[2]);
    }
    else // Parent process (Manager)
    {
        // Close unused pipe ends
        close(pipe1[0]);
        close(pipe2[1]);

        // Send the contents of the text file to the worker (via pipe 1)
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;
        while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
        {
            if (write(pipe1[1], buffer, bytes_read) != bytes_read)
                errx(EXIT_FAILURE, "Could not write data to worker");
        }

        // Close the write end of pipe 1
        close(pipe1[1]);
        close(fd);

        // Read the response from the worker (via pipe 2) and send to stdout
        while ((bytes_read = read(pipe2[0], buffer, BUFFER_SIZE)) > 0)
        {
            if (write(STDOUT_FILENO, buffer, bytes_read) != bytes_read)
                errx(EXIT_FAILURE, "Could not write data to stdout");
        }

        // Close the read end of pipe 2
        close(pipe2[0]);

        // Wait for the child process to finish
        int status;
        if (waitpid(pid, &status, 0) == -1)
            err(EXIT_FAILURE, "Waitpid error");

        // Check if the worker terminated with an error
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
            errx(EXIT_FAILURE, "Could not read data from %s", argv[2]);
    }

    return EXIT_SUCCESS;
}

