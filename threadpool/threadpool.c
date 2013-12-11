#include "threadpool.h"

typedef struct future {
	void * (* function)(void *);
	void * args;
	void * result;
	sem_t semaphore;
	struct list_elem elem;
} future;

static void * grab_work(void * p);

/* Takes a function pointer and variable number of arguments, returns a future */
static future * init_future(void * (* thread_function)(void * arguments), void * args) {

	future * f = (future *) malloc(sizeof(future));
	if (f != NULL) {
		
		f->function=thread_function;
		f->args = args;
		f->result = NULL;
		//init semaphore to 1
		sem_init(&f->semaphore, 0, 1);
	}
	else {
		printf("malloc of future in init_future failed!\n");
	}

	return f;
}

/* wait for f's semaphore to unlock, return f's result*/
void * future_get(struct future * f) {
	sem_wait(&f->semaphore);
	return f->result;
}

/* free the passed in future */
void future_free(struct future * f) {
	free(f);
	f = NULL;
}

/* Initializes thread_pool and its components, returns the created thread_pool */
thread_pool * thread_pool_new(int nthreads){
	
	thread_pool * pool = (thread_pool *) malloc(sizeof(thread_pool));
	pool->shutdown = false;
	
	list_init(&pool->futures);
	pool->size = nthreads;
	
	pool->worker_threads = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
	pthread_mutex_init(&pool->lock, NULL);
	pthread_cond_init(&pool->condition, NULL);
	
	
	int i = 0;
    for(i = 0; i < nthreads; i++) {
		pthread_mutex_lock(&pool->lock);
		if (pthread_create(&(pool->worker_threads)[i], NULL, grab_work, pool) != 0) {
			printf("pthread_create error in thread_pool_new!!!\n");
		}
		pthread_mutex_unlock(&pool->lock);
	}

	return pool;
}

/* get a future from the worker queue and run it's function */
static void * grab_work(void * p) {
	
	thread_pool * pool = (thread_pool *)p;

	while (1==1) {

		pthread_mutex_lock(&pool->lock);
		
		//wait for a new future to be added to the queue
		while (list_empty(&pool->futures)) {
			if(pool->shutdown){
				pthread_mutex_unlock(&pool->lock);
				pthread_exit(NULL);
			}
			pthread_cond_wait(&pool->condition, &pool->lock);
			if(pool->shutdown){
				pthread_mutex_unlock(&pool->lock);
				pthread_exit(NULL);
			}
		}
		
		if(pool->shutdown){
			pthread_mutex_unlock(&pool->lock);
			pthread_exit(NULL);
		}

		struct list_elem * el = list_pop_front(&pool->futures);
		future * f = list_entry(el, future, elem);
		
		pthread_mutex_unlock(&pool->lock);

		f->result = f->function(f->args);

		pthread_mutex_lock(&pool->lock);
		//post to semaphore, indicating f's result is available
		sem_post(&f->semaphore);
		pthread_mutex_unlock(&pool->lock);
	}

	return NULL;
}

/* takes a thread pool and adds a future to it's futures list with the passed function ptr and args */
struct future * thread_pool_submit(
        struct thread_pool * pool, 
        thread_pool_callable_func_t callable, 
        void * callable_data) {
	
	future * f = init_future(callable, callable_data);
	sem_wait(&f->semaphore);

	pthread_mutex_lock(&pool->lock);

	list_push_back(&pool->futures, &f->elem);
	pthread_cond_signal(&pool->condition);
	
	pthread_mutex_unlock(&pool->lock);

	return f;
}

/* Shuts down the thread pool and its associated threads and dependencies cleanly. */
void thread_pool_shutdown(struct thread_pool * pool){
	
	pthread_mutex_lock(&pool->lock);
	pool->shutdown = true;
	pthread_cond_broadcast(&pool->condition);
	pthread_mutex_unlock(&pool->lock);

	int i = 0;
	for(i = 0; i < pool->size; i++){
		if (pthread_join((pool->worker_threads)[i], NULL) != 0) {
			printf("pthread_join error in shutdown\n");
		}
	}

	pthread_mutex_destroy(&pool->lock);
	pthread_cond_destroy(&pool->condition);
	free(pool);
}
