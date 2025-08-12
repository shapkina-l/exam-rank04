#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

static volatile sig_atomic_t timeout_flag = 0;

void handle_alarm(int sig)
{
	(void)sig;
	timeout_flag = 1;
}

int sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	pid_t	cpid;
	pid_t	ret;
	int		status;
	int		exit_code;
	struct sigaction sa;

	sa.sa_handler = handle_alarm;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGALRM, &sa, NULL) == -1)
		return (-1);

	cpid = fork();
	if (cpid == -1)
	{
		return (-1);
	}
	if (cpid == 0)
	{
		f();
		exit(0);
	}
	alarm(timeout);
	while (1)
	{
		ret = waitpid(cpid, &status, 0);
		if (ret == cpid)
		{
			if (WIFEXITED(status))
			{
				exit_code = WEXITSTATUS(status);
				if (exit_code == 0)
				{
					if (verbose)
						printf("Nice function\n");
					return (0);
				}
				else 
				{
					if (verbose)
						printf("Bad function: exited with code %d\n", exit_code);
					return (1);
				}
			}
			else if (WIFSIGNALED(status))
			{
				if (verbose)
					printf("Bad function: %s\n", strsignal(WTERMSIG(status)));
				return (0);
			}
		}
		else if (timeout_flag)
		{
			kill(cpid, SIGKILL);
			waitpid(cpid, &status, 0);
			if (verbose)
				printf("Bad funciton: timed out after %d seconds\n", timeout);
			return (0);
		}
		usleep(1000);
	}
	return (-1);
}

// void nice_function(void)
// {
//     printf("Executing nice function...\n");
//     // Just return, exits with 0
// }

// void bad_function(void)
// {
//     printf("Executing bad function...\n");
//     exit(42); // Exits with non-zero code
// }

// void timeout_function(void)
// {
//     printf("Executing timeout function...\n");
//     while (1) ; // Infinite loop
// }

// int main(void)
// {
//     printf("== Testing nice function ==\n");
//     sandbox(nice_function, 2, true);

//     printf("\n== Testing bad function ==\n");
//     sandbox(bad_function, 2, true);

//     printf("\n== Testing timeout function ==\n");
//     sandbox(timeout_function, 2, true);

//     return 0;
// }
