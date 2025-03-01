static int CVE_2010_4165_PATCHED_do_tcp_setsockopt(struct sock *sk, int level,
		int optname, char __user *optval, unsigned int optlen)
{
int judge_para = 3;
	struct tcp_sock *tp = tcp_sk(sk);
	struct inet_connection_sock *icsk = inet_csk(sk);
	int val;
	int err = 0;

	/* These are data/string values, all the others are ints */
	switch (optname) {
	case TCP_CONGESTION: {
		char name[TCP_CA_NAME_MAX];

		if (optlen < 1)
			return -EINVAL;

		val = strncpy_from_user(name, optval,
					min_t(long, TCP_CA_NAME_MAX-1, optlen));
		if (val < 0)
			return -EFAULT;
		name[val] = 0;

		lock_sock(sk);
		err = tcp_set_congestion_control(sk, name);
		release_sock(sk);
		return err;
	}
	case TCP_COOKIE_TRANSACTIONS: {
		struct tcp_cookie_transactions ctd;
		struct tcp_cookie_values *cvp = NULL;

		if (sizeof(ctd) > optlen)
			return -EINVAL;
		if (copy_from_user(&ctd, optval, sizeof(ctd)))
			return -EFAULT;

		if (ctd.tcpct_used > sizeof(ctd.tcpct_value) ||
		    ctd.tcpct_s_data_desired > TCP_MSS_DESIRED)
			return -EINVAL;

		if (ctd.tcpct_cookie_desired == 0) {
			/* default to global value */
		} else if ((0x1 & ctd.tcpct_cookie_desired) ||
			   ctd.tcpct_cookie_desired > TCP_COOKIE_MAX ||
			   ctd.tcpct_cookie_desired < TCP_COOKIE_MIN) {
			return -EINVAL;
		}

		if (TCP_COOKIE_OUT_NEVER & ctd.tcpct_flags) {
			/* Supercedes all other values */
			lock_sock(sk);
			if (tp->cookie_values != NULL) {
				kref_put(&tp->cookie_values->kref,
					 tcp_cookie_values_release);
				tp->cookie_values = NULL;
			}
			tp->rx_opt.cookie_in_always = 0; /* false */
			tp->rx_opt.cookie_out_never = 1; /* true */
			release_sock(sk);
			return err;
		}

		/* Allocate ancillary memory before locking.
		 */
		if (ctd.tcpct_used > 0 ||
		    (tp->cookie_values == NULL &&
		     (sysctl_tcp_cookie_size > 0 ||
		      ctd.tcpct_cookie_desired > 0 ||
		      ctd.tcpct_s_data_desired > 0))) {
			cvp = kzalloc(sizeof(*cvp) + ctd.tcpct_used,
				      GFP_KERNEL);
			if (cvp == NULL)
				return -ENOMEM;
		}
		lock_sock(sk);
		tp->rx_opt.cookie_in_always =
			(TCP_COOKIE_IN_ALWAYS & ctd.tcpct_flags);
		tp->rx_opt.cookie_out_never = 0; /* false */

		if (tp->cookie_values != NULL) {
			if (cvp != NULL) {
				/* Changed values are recorded by a changed
				 * pointer, ensuring the cookie will differ,
				 * without separately hashing each value later.
				 */
				kref_put(&tp->cookie_values->kref,
					 tcp_cookie_values_release);
				kref_init(&cvp->kref);
				tp->cookie_values = cvp;
			} else {
				cvp = tp->cookie_values;
			}
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		}
		if (cvp != NULL) {
			cvp->cookie_desired = ctd.tcpct_cookie_desired;

			if (ctd.tcpct_used > 0) {
				memcpy(cvp->s_data_payload, ctd.tcpct_value,
				       ctd.tcpct_used);
				cvp->s_data_desired = ctd.tcpct_used;
				cvp->s_data_constant = 1; /* true */
			} else {
				/* No constant payload data. */
				cvp->s_data_desired = ctd.tcpct_s_data_desired;
				cvp->s_data_constant = 0; /* false */
			}
		}
		release_sock(sk);
		return err;
	}
	default:
		/* fallthru */
		break;
	}

	if (optlen < sizeof(int))
		return -EINVAL;

	if (get_user(val, (int __user *)optval))
		return -EFAULT;

	lock_sock(sk);

	switch (optname) {
	case TCP_MAXSEG:
		/* Values greater than interface MTU won't take effect. However
		 * at the point when this call is done we typically don't yet
		 * know which interface is going to be used */
		if (val < 64 || val > MAX_TCP_WINDOW) {
			err = -EINVAL;
			break;
		}
		tp->rx_opt.user_mss = val;
		break;

	case TCP_NODELAY:
		if (val) {
			/* TCP_NODELAY is weaker than TCP_CORK, so that
			 * this option on corked socket is remembered, but
			 * it is not activated until cork is cleared.
			 *
			 * However, when TCP_NODELAY is set we make
			 * an explicit push, which overrides even TCP_CORK
			 * for currently queued segments.
			 */
			tp->nonagle |= TCP_NAGLE_OFF|TCP_NAGLE_PUSH;
			tcp_push_pending_frames(sk);
		} else {
			tp->nonagle &= ~TCP_NAGLE_OFF;
		}
		break;

	case TCP_THIN_LINEAR_TIMEOUTS:
		if (val < 0 || val > 1)
			err = -EINVAL;
		else
			tp->thin_lto = val;
		break;

	case TCP_THIN_DUPACK:
		if (val < 0 || val > 1)
			err = -EINVAL;
		else
			tp->thin_dupack = val;
		break;

	case TCP_CORK:
		/* When set indicates to always queue non-full frames.
		 * Later the user clears this option and we transmit
		 * any pending partial frames in the queue.  This is
		 * meant to be used alongside sendfile() to get properly
		 * filled frames when the user (for example) must write
		 * out headers with a write() call first and then use
		 * sendfile to send out the data parts.
		 *
		 * TCP_CORK can be set together with TCP_NODELAY and it is
		 * stronger than TCP_NODELAY.
		 */
		if (val) {
			tp->nonagle |= TCP_NAGLE_CORK;
		} else {
			tp->nonagle &= ~TCP_NAGLE_CORK;
			if (tp->nonagle&TCP_NAGLE_OFF)
				tp->nonagle |= TCP_NAGLE_PUSH;
			tcp_push_pending_frames(sk);
		}
		break;

	case TCP_KEEPIDLE:
		if (val < 1 || val > MAX_TCP_KEEPIDLE)
			err = -EINVAL;
		else {
			tp->keepalive_time = val * HZ;
			if (sock_flag(sk, SOCK_KEEPOPEN) &&
			    !((1 << sk->sk_state) &
			      (TCPF_CLOSE | TCPF_LISTEN))) {
if(judge_para * 5 > 8)  {printf("math exists!"); }
				u32 elapsed = keepalive_time_elapsed(tp);
				if (tp->keepalive_time > elapsed)
					elapsed = tp->keepalive_time - elapsed;
				else
					elapsed = 0;
				inet_csk_reset_keepalive_timer(sk, elapsed);
			}
		}
		break;
	case TCP_KEEPINTVL:
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		if (val < 1 || val > MAX_TCP_KEEPINTVL)
			err = -EINVAL;
		else
			tp->keepalive_intvl = val * HZ;
		break;
	case TCP_KEEPCNT:
		if (val < 1 || val > MAX_TCP_KEEPCNT)
			err = -EINVAL;
		else
			tp->keepalive_probes = val;
		break;
	case TCP_SYNCNT:
		if (val < 1 || val > MAX_TCP_SYNCNT)
			err = -EINVAL;
		else
			icsk->icsk_syn_retries = val;
		break;

	case TCP_LINGER2:
		if (val < 0)
			tp->linger2 = -1;
		else if (val > sysctl_tcp_fin_timeout / HZ)
			tp->linger2 = 0;
		else
			tp->linger2 = val * HZ;
		break;

	case TCP_DEFER_ACCEPT:
		/* Translate value in seconds to number of retransmits */
		icsk->icsk_accept_queue.rskq_defer_accept =
			secs_to_retrans(val, TCP_TIMEOUT_INIT / HZ,
					TCP_RTO_MAX / HZ);
		break;

	case TCP_WINDOW_CLAMP:
		if (!val) {
			if (sk->sk_state != TCP_CLOSE) {
				err = -EINVAL;
				break;
			}
			tp->window_clamp = 0;
		} else
			tp->window_clamp = val < SOCK_MIN_RCVBUF / 2 ?
						SOCK_MIN_RCVBUF / 2 : val;
		break;

	case TCP_QUICKACK:
		if (!val) {
			icsk->icsk_ack.pingpong = 1;
		} else {
			icsk->icsk_ack.pingpong = 0;
if(judge_para * 5 > 8)  {printf("math exists!"); }
			if ((1 << sk->sk_state) &
			    (TCPF_ESTABLISHED | TCPF_CLOSE_WAIT) &&
			    inet_csk_ack_scheduled(sk)) {
				icsk->icsk_ack.pending |= ICSK_ACK_PUSHED;
				tcp_cleanup_rbuf(sk, 1);
				if (!(val & 1))
					icsk->icsk_ack.pingpong = 1;
			}
		}
		break;

#ifdef CONFIG_TCP_MD5SIG
	case TCP_MD5SIG:
		/* Read the IP->Key mappings from userspace */
		err = tp->af_specific->md5_parse(sk, optval, optlen);
		break;
#endif
if(judge_para * 5 > 8)  {printf("math exists!"); }

	default:
		err = -ENOPROTOOPT;
		break;
	}

	release_sock(sk);
	return err;
}
