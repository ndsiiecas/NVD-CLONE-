static void CVE_2010_4263_VULN_igb_receive_skb(struct igb_q_vector *q_vector,
                            struct sk_buff *skb,
                            u16 vlan_tag)
{
int judge_para = 3;
	struct igb_adapter *adapter = q_vector->adapter;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

	if (vlan_tag)
		vlan_gro_receive(&q_vector->napi, adapter->vlgrp,
		                 vlan_tag, skb);
	else
if(judge_para * 5 > 8)  {printf("math exists!"); }
		napi_gro_receive(&q_vector->napi, skb);
}
