/* thread.h
 * - Thread Abstraction Function Headers
 *
 * Copyright (c) 1999, 2000 the icecast team <team@icecast.org>
 * Copyright (c) 2019-2023 Karl Heyes <karl@kheyes.plus.com>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __THREAD_H__
#define __THREAD_H__

#include <pthread.h>

/* renamed from thread_t due to conflict on OS X */

typedef struct {
    /* the local id for the thread, and it's name */
    long thread_id;
    char *name;

    /* the time the thread was created */
    time_t create_time;

    /* the file and line which created this thread */
    const char *file;
    int line;

    /* is the thread running detached? */
    int detached;

#ifdef __OpenBSD__
    int running;
#endif

    /* the system specific thread */
    pthread_t sys_thread;
} thread_type;

typedef struct {
#ifdef THREAD_DEBUG
    /* the local id and name of the mutex */
    long mutex_id;
    char *name;

    /* the thread which is currently locking this mutex */
    long thread_id;

    /* time the lock was taken */
    unsigned long long lock_start;

#endif
    /* the file and line where the mutex was locked */
    char *file;
    int line;

    /* the system specific mutex */
    pthread_mutex_t sys_mutex;
} mutex_t;

typedef struct {
#ifdef THREAD_DEBUG
    long cond_id;
    char *name;
#endif

    int set;
    pthread_cond_t sys_cond;
} cond_t;

typedef struct {
#ifdef THREAD_DEBUG
    long rwlock_id;
    char *name;

    /* information on which thread and where in the code
    ** this rwlock was write locked
    */
    long thread_id;
    const char *file;
    int line;

    /* time the lock was taken */
    unsigned long long lock_start;
    long lock_count;
#endif

    pthread_rwlock_t sys_rwlock;
} rwlock_t;

#ifdef HAVE_PTHREAD_SPIN_LOCK
typedef struct
{
    pthread_spinlock_t lock;
} spin_t;

void thread_spin_create (spin_t *spin);
void thread_spin_destroy (spin_t *spin);
void thread_spin_lock (spin_t *spin);
void thread_spin_unlock (spin_t *spin);
#else
typedef mutex_t spin_t;
#define thread_spin_create(x)  thread_mutex_create(x)
#define thread_spin_destroy(x)   thread_mutex_destroy(x)
#define thread_spin_lock(x)      thread_mutex_lock(x)
#define thread_spin_unlock(x)    thread_mutex_unlock(x)
#endif

typedef int (*thread_mx_create_func)(void**m, int create);
typedef int (*thread_mx_lock_func)(void**m, int create);

int thread_mtx_create_callback (void **p, const char *filename, size_t line, int alloc);
int thread_mtx_lock_callback (void **p, const char *filename, size_t line, int lock);

#define thread_create(n,x,y,z) thread_create_c(n,x,y,z,__LINE__,__FILE__)
#define thread_mutex_create(x) thread_mutex_create_c(x,__LINE__,__FILE__)
#define thread_mutex_destroy(x) thread_mutex_destroy_c(x,__LINE__,__FILE__)
#define thread_mutex_lock(x) thread_mutex_lock_c(x,__LINE__,__FILE__)
#define thread_mutex_unlock(x) thread_mutex_unlock_c(x,__LINE__,__FILE__)
#define thread_cond_create(x) thread_cond_create_c(x,__LINE__,__FILE__)
#define thread_cond_signal(x) thread_cond_signal_c(x,__LINE__,__FILE__)
#define thread_cond_broadcast(x) thread_cond_broadcast_c(x,__LINE__,__FILE__)
#define thread_cond_wait(x) thread_cond_wait_c(x,__LINE__,__FILE__)
#define thread_cond_timedwait(x,m,t) thread_cond_timedwait_c(x,m,t,__LINE__,__FILE__)
#define thread_rwlock_create(x) thread_rwlock_create_c(__FILE__,(x),__LINE__,__FILE__)
#define thread_rwlock_rlock(x) thread_rwlock_rlock_c(x,__LINE__,__FILE__)
#define thread_rwlock_tryrlock(x) thread_rwlock_tryrlock_c(x,__LINE__,__FILE__)
#define thread_rwlock_wlock(x) thread_rwlock_wlock_c(x,__LINE__,__FILE__)
#define thread_rwlock_trywlock(x) thread_rwlock_trywlock_c(x,__LINE__,__FILE__)
#define thread_rwlock_unlock(x) thread_rwlock_unlock_c(x,__LINE__,__FILE__)
#define thread_exit(x) thread_exit_c(x,__LINE__,__FILE__)

#define MUTEX_STATE_NOTLOCKED -1
#define MUTEX_STATE_NEVERLOCKED -2
#define MUTEX_STATE_UNINIT -3
#define THREAD_DETACHED 1
#define THREAD_ATTACHED 0

#ifdef _mangle
# define thread_initialize _mangle(thread_initialize)
# define thread_use_log_id _mangle(thread_use_log_id)
# define thread_shutdown _mangle(thread_shutdown)
# define thread_create_c _mangle(thread_create_c)
# define thread_mutex_create_c _mangle(thread_mutex_create)
# define thread_mutex_lock_c _mangle(thread_mutex_lock_c)
# define thread_mutex_unlock_c _mangle(thread_mutex_unlock_c)
# define thread_mutex_destroy _mangle(thread_mutex_destroy_c)
# define thread_cond_create_c _mangle(thread_cond_create_c)
# define thread_cond_signal_c _mangle(thread_cond_signal_c)
# define thread_cond_broadcast_c _mangle(thread_cond_broadcast_c)
# define thread_cond_wait_c _mangle(thread_cond_wait_c)
# define thread_cond_timedwait_c _mangle(thread_cond_timedwait_c)
# define thread_cond_destroy _mangle(thread_cond_destroy)
# define thread_rwlock_create_c _mangle(thread_rwlock_create_c)
# define thread_rwlock_rlock_c _mangle(thread_rwlock_rlock_c)
# define thread_rwlock_wlock_c _mangle(thread_rwlock_wlock_c)
# define thread_rwlock_unlock_c _mangle(thread_rwlock_unlock_c)
# define thread_rwlock_destroy _mangle(thread_rwlock_destroy)
# define thread_exit_c _mangle(thread_exit_c)
# define thread_sleep _mangle(thread_sleep)
# define thread_library_lock _mangle(thread_library_lock)
# define thread_library_unlock _mangle(thread_library_unlock)
# define thread_self _mangle(thread_self)
# define thread_rename _mangle(thread_rename)
# define thread_join _mangle(thread_join)
#endif

/* init/shutdown of the library */
void thread_initialize(void);
void thread_shutdown(void);
void thread_use_log_id(int log_id);

/* creation, destruction, locking, unlocking, signalling and waiting */
thread_type *thread_create_c(char *name, void *(*start_routine)(void *),
        void *arg, int detached, int line, const char *file);
void thread_mutex_create_c(mutex_t *mutex, int line, const char *file);
void thread_mutex_lock_c(mutex_t *mutex, int line, const char *file);
void thread_mutex_unlock_c(mutex_t *mutex, int line, const char *file);
void thread_mutex_destroy_c (mutex_t *mutex, int line, const char *file);
void thread_cond_create_c(cond_t *cond, int line, char *file);
void thread_cond_signal_c(cond_t *cond, int line, char *file);
void thread_cond_broadcast_c(cond_t *cond, int line, char *file);
void thread_cond_wait_c(cond_t *cond, mutex_t *mutex, int line, char *file);
void thread_cond_timedwait_c(cond_t *cond, mutex_t *mutex, struct timespec *ts, int line, char *file);
void thread_cond_destroy(cond_t *cond);
void thread_rwlock_create_c(const char *name, rwlock_t *rwlock, int line, const char *file);
void thread_rwlock_rlock_c(rwlock_t *rwlock, int line, const char *file);
int  thread_rwlock_tryrlock_c(rwlock_t *rwlock, int line, const char *file);
void thread_rwlock_wlock_c(rwlock_t *rwlock, int line, const char *file);
int  thread_rwlock_trywlock_c(rwlock_t *rwlock, int line, const char *file);
void thread_rwlock_unlock_c(rwlock_t *rwlock, int line, const char *file);
void thread_rwlock_destroy(rwlock_t *rwlock);
void thread_exit_c(long val, int line, char *file);

/* sleeping */
void thread_sleep(unsigned long len);

/* for using library functions which aren't threadsafe */
void thread_library_lock(void);
void thread_library_unlock(void);
#define PROTECT_CODE(code) { thread_library_lock(); code; thread_library_unlock(); }

/* thread information functions */
thread_type *thread_self(void);

/* renames current thread */
void thread_rename(const char *name);

/* waits until thread_exit is called for another thread */
void thread_join(thread_type *thread);

void thread_get_timespec (struct timespec *now);
void thread_time_add_ms (struct timespec *now, unsigned long value);

#define THREAD_TIME_MS(X) ((X)->tv_sec*(uint64_t)1000+(X)->tv_nsec/1000000)
#define THREAD_TIME_SEC(X) ((X)->tv_sec)

#endif  /* __THREAD_H__ */
