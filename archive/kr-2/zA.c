#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <pwd.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>


pid_t father_pid;
pid_t child[2];
pid_t grndch[2];
pid_t getting_pid;

struct buff_pid
{
	pid_t chld;
	pid_t grdch;
};

void sighandler(int signum, siginfo_t *info, void *ptr)
{
	getting_pid = info->si_pid;
}
 

int main()
{
	struct sigaction act;
	sigset_t set;
	int i;
	int j;
	int pipefd[2];
    pid_t cpid;
    struct buff_pid buff;
    

	/*printf("my pid: %d\n", my_pid);*/
	father_pid = getpid();

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

	if (pipe(pipefd) == -1)
	 {
        perror("pipe");
        exit(EXIT_FAILURE);
     }

           cpid = fork();
           if (cpid == -1) {
               perror("fork");
               exit(EXIT_FAILURE);
           }

	for(j = 0; j < 2; ++j)
    {
		if (child[j] = fork() == 0)
    	{
    		if(grndch[j] = fork() == 0)
    		{
    			pause();
    			child[j] = getting_pid;
    			pause();
    			father_pid = getting_pid;
    			printf("grndch %d :\n", j);
    			printf("father pid: %d\n", child[j]);
    			printf("grandfather pid: %d", father_pid);
    			wait(NULL);
    			exit(1);
    		}
    		else
    		{
    		   perror("fork");
               exit(EXIT_FAILURE);
    		}

    		pause();
    		kill(SIGUSR1, grndch[j]);
    	


    		if(j == 0)
    			{
    				close(pipefd[1]);          /* Close unused write end */

               		read(pipefd[0], &buff, sizeof(struct buff_pid));
                   	child[(j+1)%2] = buff.chld;
                   	grndch[(j+1)%2] = buff.grdch;

               		close(pipefd[0]);
               		sleep(1);
               		open(pipefd[1]);
               		buff.chld = child[j];
    				buff.grdch = grndch[j];
    				write(pipefd[1], &buff, sizeof(struct buff_pid));
    				close(pipefd[1]);
    			}
    			else
    			{
    				close(pipefd[0]);          /* Close unused read end */
    				buff.chld = child[j];
    				buff.grdch = grndch[j];
		              write(pipefd[1], &buff, sizeof(struct buff_pid));
		              close(pipefd[1]);          /* Reader will see EOF */
		              sleep(1);
		              open(pipefd[0]);
		              read(pipefd[0], &buff, size0of(buff));
	               	  child[(j+1)%2] = buff.chld;
	               	  grndch[(j+1)%2] = buff.grdch;
	               	  close(pipefd[0]);
    			}

    			printf("child %d :\n", j);
    			printf("father pid: %d\n", child[j]);
    			printf("grandfather pid: %d", father_pid);
    			wait(NULL);
    			exit(1);
    	}
    	else
		{
		   perror("fork");
           exit(EXIT_FAILURE);
		}

    	child[j] = getting_pid;
    }
    	kill(SIGUSR1, child[0]);
    	kill(SIGUSR1, child[1]);	
    	printf("father: %d\n", father_pid);
    	printf("grndch 1 : %d\n", grndch[0]);
    	printf("grndch 2 : %d\n", grndch[1]);
    
	return 0;
}

       













