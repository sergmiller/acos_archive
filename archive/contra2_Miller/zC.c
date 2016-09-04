#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <memory.h>


#define QUT_THREADS 100

typedef struct
{
	size_t col;
	size_t row;
	safe_vector* res_cont;
	int type;
} params_t;
 

struct safe_3d_vector_array
{
	safe_3d_vector* data;
	size_t size;
};

struct safe_3d_vector
{
	safe_2d_vector* floats;
	size_t size;
}

struct safe_2d_vector
{
	safe_d_vector* lines;
	size_t size;
}

struct safe_vector
{
	int* values;
	size_t size;
}
pthread_t threads[QUT_THREADS];
int id_threads[QUT_THREADS];
params_t problems[QUT_THREADS];
struct safe_3d_vector_array* data_array;
pthread_cond_t cond
int ready;
pthread_mutex_t safe_ready;
pthread_mutex_t start_work;
pthread_mutex_t safe_read[QUT_THREADS];
int safe_body;

void * body(void * param)
{
	int me_num;
	int i;
	safe_vector* flg;
	me_num = (int)param;

	pthread_mutex_lock(&safe_ready);
		ready = me_num;
		pthread_cond_signal(&cond);
	pthread_mutex_unlock(&safe_ready);

	if(problems[me_num].type == 0)/*write*/
	{
		pthread_mutex_lock(&safe_write);
			pthread_mutex_lock(&safe_read[me_num]);
				flg = realloc(data_array[problems[me_num].row][problems[me_num].col].values, problems[me_num].res_cont->size * sizeof(int));
				if(!flg)
				{
					exit(1);
				}

				data_array[problems[me_num].row][problems[me_num].col].values = flg;
					
				for(i = 0; i < problems[me_num].res_cont->size; ++i)
				{
					data_array[problems[me_num].row][problems[me_num].col].values[i] = problems[me_num].res_cont->values[i];
				}
			pthread_mutex_lock(&safe_read[me_num]);
		pthread_mutex_lock(&safe_write);
	}
	else/*read*/
	{
		pthread_mutex_lock(&safe_read[me_num]);

			pthread_cond_wait
			flg = realloc(problems[me_num].res_cont->values, data_array[problems[me_num].row][problems[me_num].col].size * sizeof(int));
			if(!flg)
			{
				exit(1);
			}

			problems[me_num].res_cont->values = flg;
				
			for(i = 0; i < data_array[problems[me_num].row][problems[me_num].col].size; ++i)
			{
				problems[me_num].res_cont->values[i] = data_array[problems[me_num].row][problems[me_num].col].values[i];
			}
		pthread_mutex_unlock(&safe_read[me_num]);
	}


	pthread_mutex_lock(&start_work);
		pthread_cond_wait(&cond,&start_work);
	pthread_mutex_unlock(&start_work);

}


int array_access(struct safe_3d_vector_array *array, size_t position, int op, struct vector_3d* op_arg_or_result)
{
/*
	if(op == 0)/*write
	{
		pthread_mutex_lock(&safe_write);
		flg = realloc(array[position], 
		pthread_mutex_unlock(&safe_write);
	}
	else
	{
	
	}*/
	data_array = array;

	int i = 0;
	int j = 0;
	safe_body = 0;
	for(;i < QUT_THREADS; ++i)
	{
		pthread_create(threads + i, NULL, body, (void*)i);

	}
	i = 0;

	for(i = 0;i < array[position].size; ++i)
	{
		for(j = 0;j < array[position][i].size;++i)
		{
			pthread_mutex_lock(&safe_ready);
				pthread_cond_wait(&cond, &safe_ready);
			pthread_mutex_unlock(&safe_ready);
			
			pthread_mutex_lock(&start_work);
				problems[ready].col = i;
				problems[ready].row = j;
				problems[ready].res_cont = &op_arg_or_result[i][j];
				problems[ready].type = op;
				pthread_cond_signal(&cond);
			pthread_mutex_unlock(&start_work);
		}
	}


	i = 0;
	for(;i < QUT_THREADS;++i)
	{
		pthread_cansel(threads[i], NULL);
	}


	return 0;
}
