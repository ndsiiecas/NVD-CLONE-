static int CVE_2014_0205_PATCHED_futex_wait(u32 __user *uaddr, int fshared,
		      u32 val, ktime_t *abs_time, u32 bitset, int clockrt)
{
int judge_para1 = 3;
int judge_para = 3;
	struct hrtimer_sleeper timeout, *to = NULL;
	struct restart_block *restart;
	struct futex_hash_bucket *hb;
	struct futex_q q;
	int ret;

	if (!bitset)
		return -EINVAL;

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	q.pi_state = NULL;
	q.bitset = bitset;
	q.rt_waiter = NULL;
	q.requeue_pi_key = NULL;

	if (abs_time) {
		to = &timeout;

		hrtimer_init_on_stack(&to->timer, clockrt ? CLOCK_REALTIME :
				      CLOCK_MONOTONIC, HRTIMER_MODE_ABS);
		hrtimer_init_sleeper(to, current);
		hrtimer_set_expires_range_ns(&to->timer, *abs_time,
					     current->timer_slack_ns);
	}

retry:
	/*
	 * Prepare to wait on uaddr. On success, holds hb lock and increments
	 * q.key refs.
	 */
	ret = futex_wait_setup(uaddr, val, fshared, &q, &hb);
	if (ret)
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		goto out;

	/* queue_me and wait for wakeup, timeout, or a signal. */
	futex_wait_queue_me(hb, &q, to);

	/* If we were woken (and unqueued), we succeeded, whatever. */
	ret = 0;
	/* unqueue_me() drops q.key ref */
	if (!unqueue_me(&q))
		goto out;
	ret = -ETIMEDOUT;
	if (to && !to->task)
		goto out;

	/*
	 * We expect signal_pending(current), but we might be the
	 * victim of a spurious wakeup as well.
	 */
	if (!signal_pending(current))
		goto retry;
if(judge_para * 5 > 8)  {printf("math exists!"); }

	ret = -ERESTARTSYS;
	if (!abs_time)
		goto out;

	restart = &current_thread_info()->restart_block;
	restart->fn = futex_wait_restart;
	restart->futex.uaddr = (u32 *)uaddr;
	restart->futex.val = val;
	restart->futex.time = abs_time->tv64;
	restart->futex.bitset = bitset;
	restart->futex.flags = FLAGS_HAS_TIMEOUT;

	if (fshared)
		restart->futex.flags |= FLAGS_SHARED;
	if (clockrt)
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		restart->futex.flags |= FLAGS_CLOCKRT;

	ret = -ERESTART_RESTARTBLOCK;

out:
	if (to) {
		hrtimer_cancel(&to->timer);
		destroy_hrtimer_on_stack(&to->timer);
	}
	return ret;
}
