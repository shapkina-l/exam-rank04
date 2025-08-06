#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

int	ft_popen(const char *file, char *const argv[], char type)
{
    int pipefd[2];
    pid_t id;

    if (type != 'r' && type != 'w')
        return (-1);
    if (pipe(pipefd) == -1)
        return (-1);
    id = fork();
    if (id == -1)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        return (-1);
    }
    if (id == 0)
    {
        if (type == 'r')
        {
            close(pipefd[0]);
            if (dup2(pipefd[1], STDOUT_FILENO) == -1)
                exit (EXIT_FAILURE);
            close(pipefd[1]);
        }
        else
        {
            close(pipefd[1]);
            if (dup2(pipefd[0], STDIN_FILENO) == -1)
                exit (EXIT_FAILURE);
            close(pipefd[0]);
        }
        execvp(file, argv);
        exit (EXIT_FAILURE);
    }
    if (type == 'r')
	{
		close(pipefd[1]);
		return(pipefd[0]);
	}
	if (type == 'w')
	{
		close(pipefd[0]);
		return (pipefd[1]);
	}
	return (-1);
}

// int main(void)
// {
//     // Prepare command: ls -l
//     char *cmd[] = {"ls", "-l", NULL};

//     // Get read end of pipe from ft_popen
//     int fd = ft_popen("ls", cmd, 'r');
//     if (fd == -1)
//     {
//         perror("ft_popen failed");
//         return EXIT_FAILURE;
//     }

//     // Read output from the command
//     char buffer[256];
//     ssize_t bytes_read;
//     while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0)
//     {
//         buffer[bytes_read] = '\0';  // Null-terminate the string
//         printf("%s", buffer);       // Print to stdout
//     }

//     close(fd);
//     return 0;
// }

// int main(void)
// {
//     char *cmd[] = {"cat", NULL};  // 'cat' reads from stdin

//     int fd = ft_popen("cat", cmd, 'w');
//     if (fd == -1)
//     {
//         perror("ft_popen failed");
//         return EXIT_FAILURE;
//     }

//     // Send input to 'cat'
//     const char *msg = "Hello from parent to cat!\n";
//     write(fd, msg, strlen(msg));
//     close(fd);  // Important to send EOF to 'cat'

//     // The child will echo the message back to stdout (from its stdin)

//     return 0;
// }