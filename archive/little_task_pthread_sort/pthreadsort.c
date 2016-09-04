#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <memory.h>

#define QUT_THREADS 20

#define SIZE_OF_STRING_PARTS 10000
struct params
{
	size_t start;
	size_t size;
	char data[SIZE_OF_STRING_PARTS];
};

typedef struct params params_t;

size_t size;
size_t current_position = 0;
FILE* fp;
FILE* write_fp;
char cnt = 1;
short count = 0;
pthread_t threads[QUT_THREADS];
int id_threads[QUT_THREADS];
params_t problems[QUT_THREADS];
char buff[SIZE_OF_STRING_PARTS];
pthread_cond_t cond_start_work;
pthread_cond_t cond_ready;
pthread_cond_t cond_exit;
int start_flag = 0;
int ready;
int main_is_break = 0;
int main_signal_flag = 0;
int get_new_task = 0;
int get_new_thread = 0;
size_t lock_cnt = 0;
pthread_mutex_t mutex_write;
pthread_mutex_t start_work;
pthread_mutex_t safe_ready;
pthread_mutex_t mutex_exit;

void * body(void * param)
{
	int me_num = *(int*)param;;
	char swap_buff;
	char out_cond = 0;
	size_t i = 0;
	size_t j;

	while(1)
	{
		pthread_mutex_lock(&mutex_exit);
			++lock_cnt;
			if(main_is_break == 1 && lock_cnt == QUT_THREADS)
				pthread_cond_signal(&cond_exit);
		pthread_mutex_unlock(&mutex_exit);


		pthread_mutex_lock(&start_work);
			pthread_mutex_lock(&safe_ready);
				get_new_thread = 1;
				if(get_new_task == 0)
				{
					pthread_cond_wait(&cond_ready, &safe_ready);
				}

				get_new_task = 0;

				problems[me_num].start = current_position;
	    		i = 0;
	    		for(;i < size;++i)
	    			problems[me_num].data[i] = buff[i];
	    		problems[me_num].size = size;

	    		pthread_cond_signal(&cond_ready);
    		pthread_mutex_unlock(&start_work);
		pthread_mutex_unlock(&safe_ready);

		pthread_mutex_lock(&mutex_exit);
			--lock_cnt;
		pthread_mutex_unlock(&mutex_exit);
		
		for(i = 0;i < problems[me_num].size - 1;++i)
		{
			for(j = 0;j < problems[me_num].size - i - 1;++j)
			{
				if(problems[me_num].data[j] > problems[me_num].data[j+1])
				{
					swap_buff = problems[me_num].data[j+1];
					problems[me_num].data[j + 1] = problems[me_num].data[j];
					problems[me_num].data[j] = swap_buff; 
				}
			}
		}

		pthread_mutex_lock(&mutex_write);
		//	if(problems[me_num].size > 0)
				fwrite(problems[me_num].data, 1, problems[me_num].size, write_fp);
				//fseek(write_fp, problems[me_num].size, SEEK_CUR);
				printf("WRITE\n");
			//fprintf(stdout, "%s", problems[me_num].data);
		pthread_mutex_unlock(&mutex_write);
		//printf("OK::\n
	}
}


int main(int argc, char** argv)
{
	int flg;
	int i = 0;
	

	fp = fopen(argv[1], "rb+");
	write_fp = fopen(argv[2], "wb+");

	if(!fp || !write_fp)
	{
		printf("ERROR: can't open file\n");
		exit(1);
	}
	
	//printf("OK0\n");
	
	for(;i < QUT_THREADS; ++i)
	{
		id_threads[i] = i;
		pthread_create(threads + i, NULL, body, &id_threads[i]);
	}

    while(1)
    {
     //   printf("OK1\n");

			size = fread(buff, 1,SIZE_OF_STRING_PARTS, fp);
			printf("size: %d\n", size);
    		if (size == 0)
    			break;

    		current_position += size;
			
			pthread_mutex_lock(&safe_ready);
				get_new_task = 1;

				if(get_new_thread == 0)
				{
					pthread_cond_wait(&cond_ready, &safe_ready);
				}

				get_new_thread = 0;
				get_new_task = 0;

				pthread_cond_signal(&cond_ready);
			pthread_mutex_unlock(&safe_ready);
    	//	fprintf(stdout, "thread %d 's job: %s\n", ready, problems[ready].data);
    }
    pthread_mutex_lock(&mutex_exit);
    	main_is_break = 1;
    	if(lock_cnt != QUT_THREADS)
		pthread_cond_wait(&cond_exit, &mutex_exit);    		
    pthread_mutex_unlock(&mutex_exit);
    /************************************************************/
	i = 0;
	for(;i < QUT_THREADS;++i)
	{
		pthread_cancel(threads[i]);
	}

	fclose(fp);
	fclose(write_fp);
	return 0;
}