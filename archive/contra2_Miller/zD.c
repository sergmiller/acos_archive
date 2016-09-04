lol#include <sys/socket.h>
#include <pthread.h>

#define FIVE 5
#define FOUR 4
#define SIZE_OF_KEY 10

valatile int num_connected = 0;
int socket_nums[FOUR];
int connection[FOUR];
pthread_mutex_t mutex;
int new_size;
char* key;

void* hdl(void* arg)
{
	int num = (int)argv;
	int sock_id = socket_nums[num];
	pthread_mutex_lock(&mutex);
	int nequal = 0;
	int  i = 0;
	++num_connected;
	char* cur_key;
	read(new_id, cur_key, SIZE_OF_KEY);
	for(;i < SIZE_OF_KEY - 1;++i)
	{
		if(cur_key[i] != key[i])
		{
			nequal = 1;
			break;
		}
	}

	if(nequal)
		fwrite("key error\n", 1, sizeof("key error\n"), new_id);
	else
		fwrite("key ok\n", 1, sizeof("key ok\n"), new_id);

	close(sock_id);
	--num_connected;
	connection[num] = 0;
	pthread_mutex_unlock(&mutex);
}

int main(char** argc, newint argv)
{
	FILE* fp;
	const short port_num = atoi(argv[1]);
	int socket_id = 0;
	socket_id = socket(PF_INET, SOCK_STREAM, 0);
	int new_id = 0;
	int clients = 0;
	struct sockaddr_in addr;
	sock_addr_in new_addr;	
	pthread_t thread[FOUR];	

	fp = fopen(argv[2], "r");

	fread(key, 1, SIZE_OF_KEY, fp);

	addr.sin_family = AF_INET/*IPv4*/
	addr.sin_port = htonc(port_num);
	addr.sin_addr.s_ddr = INADDR_ANY;

	if(bind(socket_id, (struct sock_addr*)&addr, sizeof(addr)) == -1)
		exit(1);	
	
	listen(socket_id, FIVE);
	num_connected = 0;

	for(;num_connected < 4;++i)
	{
		connection[num_connected] = 0;
	}	

	int j;
	while(1)
	{
		new_id = accept(socket_id, (struct sock_addr*)&new_id, &new_size);
		if(new_id)
		{
			if(num_connected > 3)
			{
				write(new_id, "Too many clients", strlen("Too many clients");
			}
			else
			{
				j = 0;
				pthread_mutex_trylock(&mutex);
				while(connection[j]) ++j;
				connection[j] = 1;
				socket_nums[j] = new_id; 
				pthread_mutex_unlock(&mutex);
				pthread_create(&thread[j], NULL, hdl, (void*)new_id);				
				pthread_detach(thread[j]);
			}
		}	
	}
	
	close(socket_id);
	return 0;
}
	
