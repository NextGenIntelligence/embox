/**
 * @file
 * @brief
 *
 * @author  Vita Loginova
 * @date    Jul 31, 2014
 */
#include <util/err.h>
#include <embox/test.h>
#include <kernel/sched.h>
#include <kernel/sched/waitq.h>
#include <kernel/sched/current.h>
#include <kernel/lthread/lthread.h>
#include <kernel/lthread/lthread_priority.h>
#include <kernel/thread.h>
#include <kernel/time/ktime.h>
#include <kernel/sched/sync/mutex.h>
#include <kernel/lthread/sync/mutex.h>
#include <kernel/thread/sync/mutex.h>

static struct lthread *high;
static struct thread *low;
static struct mutex m;

EMBOX_TEST_SUITE("test for lthread API");

TEST_SETUP(setup);

TEST_CASE("General") {
	test_assert_zero(thread_launch(low));
	test_assert_zero(thread_join(low, NULL));
	test_assert_emitted("abcde");
}

static void *low_run(void *arg) {
	test_emit('a');
	mutex_lock(&m);
	test_emit('b');
	lthread_launch(high);
	test_emit('c');
	mutex_unlock(&m);
	test_emit('e');
	return NULL;
}

static void *high_run(void *arg) {
	if (mutex_trylock_lthread(&m) == -EAGAIN) {
		return NULL;
	}
	test_emit('d');
	mutex_unlock_lthread(&m);
	return NULL;
}

static int setup(void) {
	sched_priority_t l = 200, h = 210;

	mutex_init_schedee(&m);

	low = thread_create(THREAD_FLAG_SUSPENDED, low_run, NULL);
	test_assert_zero(err(low));

	high = lthread_create(high_run, NULL);
	test_assert_zero(err(high));

	test_assert_zero(thread_set_priority(low, l));
	test_assert_zero(lthread_priority_set(high, h));
	return 0;
}
