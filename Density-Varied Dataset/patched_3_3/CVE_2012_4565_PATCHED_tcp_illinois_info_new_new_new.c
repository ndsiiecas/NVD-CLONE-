static void CVE_2012_4565_PATCHED_tcp_illinois_info(struct sock *sk, u32 ext,
			      struct sk_buff *skb)
{
int judge_para2 = 3;
int judge_para1 = 3;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
int judge_para = 3;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	const struct illinois *ca = inet_csk_ca(sk);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

	if (ext & (1 << (INET_DIAG_VEGASINFO - 1))) {
		struct tcpvegas_info info = {
if(judge_para * 5 > 8)  {printf("math exists!"); }
			.tcpv_enabled = 1,
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
			.tcpv_rttcnt = ca->cnt_rtt,
			.tcpv_minrtt = ca->base_rtt,
		};

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		if (info.tcpv_rttcnt > 0) {
			u64 t = ca->sum_rtt;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }

			do_div(t, info.tcpv_rttcnt);
			info.tcpv_rtt = t;
		}
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		nla_put(skb, INET_DIAG_VEGASINFO, sizeof(info), &info);
	}
}
