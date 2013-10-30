//Threadpool.c
#include "list.h"
#include "threadpool.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sched.h>
#include <unistd.h>

static void * work(void * args);



/* Create a new thread pool with n threads. */
struct thread_pool * thread_pool_new(int nthreads) {
	struct thread_pool * pool = (struct thread_pool *)malloc(sizeof(struct thread_pool));
	pthread_mutex_init(&pool->thread_list_lock, NULL);
	pthread_cond_init(&pool->thread_list_cond, NULL);
	list_init(&pool->threads);

	pthread_mutex_init(&pool->future_list_lock, NULL);
	//pthread_mutex_init(&pool->future_list_not_empty_lock, NULL);
	pthread_cond_init(&pool->future_list_not_empty, NULL);
	pthread_cond_init(&pool->future_list_empty, NULL);
	list_init(&pool->futures);

	pthread_mutex_init(&pool->shutdown_lock, NULL);

	pthread_cond_init(&pool->threads_ready, NULL);
	pthread_mutex_init(&pool->startup_lock, NULL);


	int i = 0;
	for(; i < nthreads; i++) {
		thread_t * t = (thread_t *)malloc(sizeof(thread_t));
		if(pthread_create(&t->thread, NULL, work, pool)) {
			fprintf(stderr, "Thread creation failed.\n");
			exit(EXIT_FAILURE);	
		}
		list_push_back(&pool->threads, &t->elem);
	}

	pthread_mutex_lock(&pool->shutdown_lock);
	pool->shutting_down = 0;
	pool->ready = 0;
	pthread_mutex_unlock(&pool->shutdown_lock);
	return pool;
}

/* Shutdown this thread pool.  May or may not execute already queued tasks. */
void thread_pool_shutdown(struct thread_pool * pool) {
	pthread_mutex_lock(&pool->shutdown_lock);
	pool->shutting_down = 1;
	pthread_mutex_unlock(&pool->shutdown_lock);

	while(!list_empty(&pool->futures)) {
		sched_yield();
	}

	pthread_mutex_lock(&pool->future_list_lock);
	pthread_cond_broadcast(&pool->future_list_not_empty);
	pthread_mutex_unlock(&pool->future_list_lock);
}

/* Submit a callable to thread pool and return future.
 * The returned future can be used in future_get() and future_free()
 */
struct future * thread_pool_submit(struct thread_pool * pool, thread_pool_callable_func_t callable, 
        void * callable_data) {
	future_t * f = (future_t *)malloc(sizeof(future_t));
	f->callable = callable;
	f->callable_data = callable_data;
	f->result = NULL;
	f->fin = 0;

	pthread_mutex_init(&f->future_lock, NULL);
	pthread_cond_init(&f->finished, NULL);

	// pthread_cond_wait(&pool->threads_ready, &pool->startup_lock);
	// pthread_mutex_unlock(&pool->startup_lock);
	// while(!pool->ready) {
	// 	sched_yield();
	// }

	pthread_mutex_lock(&pool->future_list_lock);
	list_push_back(&pool->futures, &f->elem);
	pthread_mutex_unlock(&pool->future_list_lock);

	pthread_mutex_lock(&pool->future_list_lock);
	pthread_cond_signal(&pool->future_list_not_empty);
	pthread_mutex_unlock(&pool->future_list_lock);


	return f;
}

/* Make sure that thread pool has completed executing this callable,
 * then return result. */
void * future_get(struct future * f) {
	//fprintf(stderr, "Waiting for future-lock #%p\n", (void *) pthread_self());
	//pthread_cond_wait(&f->finished, &f->future_lock);
	pthread_mutex_lock(&f->future_lock);
	while(!f->fin) {
		pthread_mutex_unlock(&f->future_lock);
		sched_yield();
		pthread_mutex_lock(&f->future_lock);
	}
	pthread_mutex_unlock(&f->future_lock);
	//fprintf(stderr, "Done waiting for future-lock #%p\n", (void *) pthread_self());
	return f->result;
}

/* Deallocate this future.  Must be called after future_get() */
void future_free(struct future * f) {
	free(f);	
}

static void * work(void * args) {
	struct thread_pool * pool = (struct thread_pool *)args;
	for(;;) {
		pthread_mutex_lock(&pool->future_list_lock);
		while(list_empty(&pool->futures)) {

			pthread_mutex_lock(&pool->shutdown_lock);
			if(pool->shutting_down) {
				pthread_mutex_unlock(&pool->shutdown_lock);
				pthread_cond_signal(&pool->future_list_empty);
				pthread_mutex_unlock(&pool->future_list_lock);
				pthread_exit(0);
			}
			pthread_mutex_unlock(&pool->shutdown_lock);
			//pthread_mutex_unlock(&pool->future_list_lock);
			//pthread_cond_signal(&pool->threads_ready);
			pthread_cond_wait(&pool->future_list_not_empty, &pool->future_list_lock);
			//make a new lock for future list not empty to use here ^ ^ ^ ^
		}
		future_t * f = list_entry(list_pop_front(&pool->futures), future_t, elem);

		pthread_mutex_unlock(&pool->future_list_lock);
		pthread_mutex_lock(&f->future_lock);
		//f->result = (f->callable) (f->callable_data);
		f->result = f->callable(f->callable_data);		
		f->fin = 1;
		pthread_mutex_unlock(&f->future_lock);
		// if(pthread_cond_signal(&f->finished)) {
		// 	perror("pthread_cond_signal");
		// 	exit(EXIT_FAILURE);	
		// }
		//fprintf(stderr, "Finished, Unlocked, and Signaled #%p\n", (void *) pthread_self());

		// if(pool->shutting_down && list_empty(&pool->futures)) {
		// 		pthread_cond_signal(&pool->future_list_empty);
		// 		fprintf(stderr, "Get in outer loop\n");
		// 		pthread_exit(0);
		// }

	}
	return NULL;
}
