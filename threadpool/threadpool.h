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

struct thread_pool {
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
};




/* Create a new thread pool with n threads. */
struct thread_pool * thread_pool_new(int nthreads);

/* Shutdown this thread pool.  May or may not execute already queued tasks. */
void thread_pool_shutdown(struct thread_pool *);

/* A function pointer representing a 'callable' */
typedef void * (* thread_pool_callable_func_t) (void * data);

/* Submit a callable to thread pool and return future.
 * The returned future can be used in future_get() and future_free()
 */
struct future * thread_pool_submit(
        struct thread_pool *, 
        thread_pool_callable_func_t callable, 
        void * callable_data);

/* Make sure that thread pool has completed executing this callable,
 * then return result. */
void * future_get(struct future *);

/* Deallocate this future.  Must be called after future_get() */
void future_free(struct future *);



