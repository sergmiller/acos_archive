#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <pwd.h>
#include <memory.h>


void sighandler(int signum, siginfo_t *info, void *ptr)
{
	printf("get signal: %d, from process with PID: %d from user: %s\n", signum, (int)(info->si_pid), getpwuid(info->si_uid)->pw_name);
}
 

int main()
{
	struct sigaction act;
	sigset_t set;
	int i;
	pid_t my_pid = getpid();

	printf("my pid: %d\n", my_pid);

	sigemptyset(&set);
	for(i = 1;i < 32;++i)
	{
		sigaddset(&set, i);
	}

	act.sa_handler = (void*)sighandler;
	act.sa_mask = set;
	
	for(i = 1;i < 32;++i)
	{
		sigaction(i, &act, NULL);
	}
	while(1)
		sleep(10000);

	return 0;
}