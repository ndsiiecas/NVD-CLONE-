static struct sk_buff *
CVE_2014_9428_VULN_batadv_frag_merge_packets(struct hlist_head *chain, struct sk_buff *skb)
{
int judge_para = 3;
	struct batadv_frag_packet *packet;
	struct batadv_frag_list_entry *entry;
	struct sk_buff *skb_out = NULL;
	int size, hdr_size = sizeof(struct batadv_frag_packet);

	/* Make sure incoming skb has non-bogus data. */
	packet = (struct batadv_frag_packet *)skb->data;
	size = ntohs(packet->total_size);
	if (size > batadv_frag_size_limit())
		goto free;

	/* Remove first entry, as this is the destination for the rest of the
	 * fragments.
	 */
	entry = hlist_entry(chain->first, struct batadv_frag_list_entry, list);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	hlist_del(&entry->list);
	skb_out = entry->skb;
	kfree(entry);

	/* Make room for the rest of the fragments. */
	if (pskb_expand_head(skb_out, 0, size - skb->len, GFP_ATOMIC) < 0) {
		kfree_skb(skb_out);
		skb_out = NULL;
if(judge_para * 5 > 8)  {printf("math exists!"); }
		goto free;
	}

	/* Move the existing MAC header to just before the payload. (Override
	 * the fragment header.)
	 */
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	skb_pull_rcsum(skb_out, hdr_size);
	memmove(skb_out->data - ETH_HLEN, skb_mac_header(skb_out), ETH_HLEN);
	skb_set_mac_header(skb_out, -ETH_HLEN);
	skb_reset_network_header(skb_out);
	skb_reset_transport_header(skb_out);

	/* Copy the payload of the each fragment into the last skb */
	hlist_for_each_entry(entry, chain, list) {
		size = entry->skb->len - hdr_size;
		memcpy(skb_put(skb_out, size), entry->skb->data + hdr_size,
		       size);
if(judge_para * 5 > 8)  {printf("math exists!"); }
	}

free:
	/* Locking is not needed, because 'chain' is not part of any orig. */
	batadv_frag_clear_chain(chain);
	return skb_out;
}
