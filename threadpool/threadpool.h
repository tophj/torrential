#ifndef THREADPOOL_BITCH
#define THREADPOOL_BITCH

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sched.h>
#include <unistd.h>
#include "list.h"

/* A function pointer representing a 'callable' */
typedef void * (* thread_pool_callable_func_t) (void * data);


typedef struct _thread_t {
	pthread_t thread;
	struct list_elem elem;
} thread_t;

typedef struct future {
	pthread_cond_t finished;
	pthread_mutex_t future_lock;
	thread_pool_callable_func_t callable;
	void * callable_data;
	void * result;
	struct list_elem elem;
	int fin;
} future_t;

typedef struct thread_pool {
	pthread_mutex_t thread_list_lock;
	pthread_cond_t thread_list_cond;
	struct list threads;

	pthread_mutex_t future_list_lock;
	//pthread_mutex_t future_list_not_empty_lock;
	pthread_cond_t future_list_not_empty;
	pthread_cond_t future_list_empty;

	pthread_cond_t threads_ready;
	pthread_mutex_t startup_lock;

	struct list futures;

	pthread_mutex_t shutdown_lock;

	int shutting_down;
	int ready;
} tp_t;





/* Shutdown this thread pool.  May or may not execute already queued tasks. */
void thread_pool_shutdown(struct thread_pool*);

/* Create a new thread pool with n threads. */
tp_t * thread_pool_new(int nthreads);

/* Submit a callable to thread pool and return future.
 * The returned future can be used in future_get() and future_free()
 */
future_t * thread_pool_submit(
        tp_t *, 
        thread_pool_callable_func_t callable, 
        void * callable_data);

/* Make sure that thread pool has completed executing this callable,
 * then return result. */
void * future_get(struct future *);

/* Deallocate this future.  Must be called after future_get() */
void future_free(struct future *);

#endif
