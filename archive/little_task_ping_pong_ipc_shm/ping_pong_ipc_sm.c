#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define BALL_LIMIT 40
/* предполагается запуск из 3 терминалов с аргументами 1, 2 и 3. 
 *Процесс запущенный с аргументом 1 создает и удаляет IPC объекты, 
 *поэтому необходимо запускать его первым.
 */

int shm_id;
int sem_id;
int* mem;
int number;

void ipc_clear()
{
	shmdt(mem);
	shmctl(shm_id,IPC_RMID,NULL);

	semctl(sem_id, 0, IPC_RMID);
	semctl(sem_id, 1, IPC_RMID);
	semctl(sem_id, 2, IPC_RMID);

	printf("IPC DELETED\n");
}

void handler(int s)
{
	if(!number)
		ipc_clear();
	exit(4);
}


struct sembuf wait_for_ready[3];
struct sembuf lock[3];
struct sembuf unlock[3];


int main(int argc, char** argv)
{
	key_t key;
	int i = 0;
	int param;
	struct sigaction act;
	sigset_t set;

	number = atoi(argv[1]) - 1;

	for(i = 1;i < 32;++i)
	{
		sigaddset(&set, i);
	}

	act.sa_handler = (void*)handler;
	act.sa_mask = set;
	
	for(i = 1;i < 32;++i)
	{
		sigaction(i, &act, NULL);
	}

	for(i = 0;i < 3;++i)
	{
		wait_for_ready[i].sem_num = i;
		wait_for_ready[i].sem_op = 0;
		wait_for_ready[i].sem_flg = 0;
		lock[i].sem_num = i;
		lock[i].sem_op = 1;
		lock[i].sem_flg = 0;
		unlock[i].sem_num = i;
		unlock[i].sem_op = -1;
		unlock[i].sem_flg = 0;
	}

	key = ftok("sem3.txt", 1);

	if(number == 0)
		param = IPC_CREAT|0666;
	else
		param = 0;

	shm_id = shmget(key, sizeof(int), param);
	if(shm_id == -1)
	{
		printf("Can't connect to shared memory\n");
		ipc_clear();
		exit(1);
	}

	sem_id = semget(key, 3, param);
	if(sem_id == -1)
	{
		printf("Can't connect to the semaphors\n");
		ipc_clear();
		exit(2);
	}

	if(!number)
	{
		semop(sem_id, &(lock[1]), 1);
		semop(sem_id, &(lock[2]), 1);
	}
	

	mem = (int*)shmat(shm_id, NULL, 0);
	if(mem == (void *)-1)
	{
		printf("Can't get address\n");
		if(!number)
		{
			ipc_clear();
			exit(3);
		}
	}

	if(!number)
		mem[0] = 0;
	
	while(1)
	{
		semop(sem_id, &(wait_for_ready[number]), 1);
		if(mem[0] == BALL_LIMIT)
		{
			semop(sem_id, &(lock[number]), 1);
			semop(sem_id, &(unlock[((number + 1)%3)]), 1);

			if(!number)
				ipc_clear();
			return 0;
		}

		printf("get ball: %d\n",++mem[0]);

		semop(sem_id, &(lock[number]), 1);
		semop(sem_id, &(unlock[((number + 1)%3)]), 1);
	}
}