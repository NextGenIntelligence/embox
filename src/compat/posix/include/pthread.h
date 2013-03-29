/**
 * @file
 * @brief
 *
 * @date   27.03.2013
 * @author Anton Bulychev
 */

#ifndef PTHREAD_H_
#define PTHREAD_H_

#include <time.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#include <kernel/thread/types.h>
#include <kernel/thread/sync/cond.h>
#include <kernel/thread/sync/mutex.h>
#include <kernel/thread/sync/rwlock.h>

typedef struct thread *pthread_t;

typedef struct pthread_attr {

} pthread_attr_t;


typedef struct cond pthread_cond_t;

typedef struct pthread_condattr {

} pthread_condattr_t;


typedef struct mutex pthread_mutex_t;

typedef struct pthread_mutexattr {

} pthread_mutexattr_t;


typedef struct pthread_rwlock {

} pthread_rwlock_t;

typedef struct pthread_rwlockattr {

} pthread_rwlockattr_t;


typedef struct pthread_key {

} *pthread_key_t;


typedef struct pthread_once {

} pthread_once_t;


struct sched_param {

};

__BEGIN_DECLS

/*
 * TODO: Implement!
 */

//extern int   pthread_attr_destroy(pthread_attr_t *);
//extern int   pthread_attr_getdetachstate(const pthread_attr_t *, int *);
//extern int   pthread_attr_getguardsize(const pthread_attr_t *, size_t *);
//extern int   pthread_attr_getinheritsched(const pthread_attr_t *, int *);
//extern int   pthread_attr_getschedparam(const pthread_attr_t *, struct sched_param *);
//extern int   pthread_attr_getschedpolicy(const pthread_attr_t *, int *);
//extern int   pthread_attr_getscope(const pthread_attr_t *, int *);
//extern int   pthread_attr_getstackaddr(const pthread_attr_t *, void **);
//extern int   pthread_attr_getstacksize(const pthread_attr_t *, size_t *);
//extern int   pthread_attr_init(pthread_attr_t *);
//extern int   pthread_attr_setdetachstate(pthread_attr_t *, int);
//extern int   pthread_attr_setguardsize(pthread_attr_t *, size_t);
//extern int   pthread_attr_setinheritsched(pthread_attr_t *, int);
//extern int   pthread_attr_setschedparam(pthread_attr_t *, const struct sched_param *);
//extern int   pthread_attr_setschedpolicy(pthread_attr_t *, int);
//extern int   pthread_attr_setscope(pthread_attr_t *, int);
//extern int   pthread_attr_setstackaddr(pthread_attr_t *, void *);
//extern int   pthread_attr_setstacksize(pthread_attr_t *, size_t);

//extern int   pthread_cancel(pthread_t);
//extern void  pthread_cleanup_push(void (*)(void *), void *arg);
//extern void  pthread_cleanup_pop(int);

extern int   pthread_cond_broadcast(pthread_cond_t *);
extern int   pthread_cond_destroy(pthread_cond_t *);
extern int   pthread_cond_init(pthread_cond_t *, const pthread_condattr_t *);
extern int   pthread_cond_signal(pthread_cond_t *);
//extern int   pthread_cond_timedwait(pthread_cond_t *, pthread_mutex_t *, const struct timespec *);
extern int   pthread_cond_wait(pthread_cond_t *, pthread_mutex_t *);

//extern int   pthread_condattr_destroy(pthread_condattr_t *);
//extern int   pthread_condattr_getpshared(const pthread_condattr_t *, int *);
//extern int   pthread_condattr_init(pthread_condattr_t *);
//extern int   pthread_condattr_setpshared(pthread_condattr_t *, int);

extern int   pthread_create(pthread_t *, const pthread_attr_t *, void *(*)(void *), void *);
extern int   pthread_detach(pthread_t);
extern int   pthread_equal(pthread_t, pthread_t);
extern void  pthread_exit(void *);
//extern int   pthread_getconcurrency(void);
//extern int   pthread_getschedparam(pthread_t, int *, struct sched_param *);
//extern void *pthread_getspecific(pthread_key_t);
extern int   pthread_join(pthread_t, void **);
//extern int   pthread_key_create(pthread_key_t *, void (*)(void *));
//extern int   pthread_key_delete(pthread_key_t);

extern int   pthread_mutex_destroy(pthread_mutex_t *);
//extern int   pthread_mutex_getprioceiling(const pthread_mutex_t *, int *);
extern int   pthread_mutex_init(pthread_mutex_t *, const pthread_mutexattr_t *);
extern int   pthread_mutex_lock(pthread_mutex_t *);
//extern int   pthread_mutex_setprioceiling(pthread_mutex_t *, int, int *);
extern int   pthread_mutex_trylock(pthread_mutex_t *);
extern int   pthread_mutex_unlock(pthread_mutex_t *);

//extern int   pthread_mutexattr_destroy(pthread_mutexattr_t *);
//extern int   pthread_mutexattr_getprioceiling(const pthread_mutexattr_t *, int *);
//extern int   pthread_mutexattr_getprotocol(const pthread_mutexattr_t *, int *);
//extern int   pthread_mutexattr_getpshared(const pthread_mutexattr_t *, int *);
//extern int   pthread_mutexattr_gettype(const pthread_mutexattr_t *, int *);
//extern int   pthread_mutexattr_init(pthread_mutexattr_t *);
//extern int   pthread_mutexattr_setprioceiling(pthread_mutexattr_t *, int);
//extern int   pthread_mutexattr_setprotocol(pthread_mutexattr_t *, int);
//extern int   pthread_mutexattr_setpshared(pthread_mutexattr_t *, int);
//extern int   pthread_mutexattr_settype(pthread_mutexattr_t *, int);

//extern int   pthread_once(pthread_once_t *, void (*)(void));

//extern int   pthread_rwlock_destroy(pthread_rwlock_t *);
//extern int   pthread_rwlock_init(pthread_rwlock_t *, const pthread_rwlockattr_t *);
//extern int   pthread_rwlock_rdlock(pthread_rwlock_t *);
//extern int   pthread_rwlock_tryrdlock(pthread_rwlock_t *);
//extern int   pthread_rwlock_trywrlock(pthread_rwlock_t *);
//extern int   pthread_rwlock_unlock(pthread_rwlock_t *);
//extern int   pthread_rwlock_wrlock(pthread_rwlock_t *);

//extern int   pthread_rwlockattr_destroy(pthread_rwlockattr_t *);
//extern int   pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *, int *);
//extern int   pthread_rwlockattr_init(pthread_rwlockattr_t *);
//extern int   pthread_rwlockattr_setpshared(pthread_rwlockattr_t *, int);

extern pthread_t
             pthread_self(void);
//extern int   pthread_setcancelstate(int, int *);
//extern int   pthread_setcanceltype(int, int *);
//extern int   pthread_setconcurrency(int);
//extern int   pthread_setschedparam(pthread_t, int, const struct sched_param *);
//extern int   pthread_setspecific(pthread_key_t, const void *);
//extern void  pthread_testcancel(void);

__END_DECLS

#endif /* PTHREAD_H_ */