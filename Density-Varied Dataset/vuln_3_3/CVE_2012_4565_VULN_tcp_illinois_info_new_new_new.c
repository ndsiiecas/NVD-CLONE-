static void CVE_2012_4565_VULN_tcp_illinois_info(struct sock *sk, u32 ext,
			      struct sk_buff *skb)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	const struct illinois *ca = inet_csk_ca(sk);
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }

	if (ext & (1 << (INET_DIAG_VEGASINFO - 1))) {
		struct tcpvegas_info info = {
			.tcpv_enabled = 1,
			.tcpv_rttcnt = ca->cnt_rtt,
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
			.tcpv_minrtt = ca->base_rtt,
		};
		u64 t = ca->sum_rtt;

if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		do_div(t, ca->cnt_rtt);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		info.tcpv_rtt = t;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }

		nla_put(skb, INET_DIAG_VEGASINFO, sizeof(info), &info);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
	}
}
