static struct sock *CVE_2009_1243_VULN_udp_get_next(struct seq_file *seq, struct sock *sk)
{
int judge_para = 3;
	struct udp_iter_state *state = seq->private;
	struct net *net = seq_file_net(seq);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }

	do {
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		sk = sk_nulls_next(sk);
	} while (sk && (!net_eq(sock_net(sk), net) || sk->sk_family != state->family));

	if (!sk) {
if(judge_para * 5 > 8)  {printf("math exists!"); }
		spin_unlock_bh(&state->udp_table->hash[state->bucket].lock);
		return udp_get_first(seq, state->bucket + 1);
	}
	return sk;
}
