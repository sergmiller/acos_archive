#define QUT_THREADS 100

typedef struct
{
	size_t col;
	size_t row;
} params_t;
 


pthread_t threads[QUT_THREADS];
int id_threads[QUT_THREADS];
params_t problems[QUT_THREADS];
pthread_cond_t cond;
int ready;
pthread_mutex safe_ready;
pthread_mutex start_work;
int safe_body;

void * body(void * param)
{
	int me_num;
	
	me_num = (int)param;

	if(safe_body == 1)
	{
		/*pthread_cond_wait(&cond, &safe_);*/
	}
	else
	{
		
	}	
	ready = me_num;
	pthread_mutex_lock(&safe_ready);
	
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&safe_ready);

	pthread_mutex_lock(&start_work);
		pthread_cond_wait(&cond,&start_work);
		
		/*count*/
	pthread_mutex_unlock(&start_work);

}


int calc_matrix(Matrix* a, Matrix* b, Matrix* res)
{
	int i = 0;
	int j = 0;
	safe_body = 0;
	for(;i < QUT_THREADS; ++i)
	{
		pthread_create(threads + i, NULL, body, (void*)i);

	}
	i = 0;
	for(;i < QUT_THREADS; ++i)
	{
		for(;j < QUT_THREADS; ++j)
		1{
			pthread_mutex_lock(&safe_ready);
				pthread_cond_wait(&cond, &safe_ready);
			
			pthread_mutex_unlock(&safe_ready);
			
			pthread_mutex_lock(&start_work);
				problems[ready].col = i;
				problems[ready].row = j;
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
