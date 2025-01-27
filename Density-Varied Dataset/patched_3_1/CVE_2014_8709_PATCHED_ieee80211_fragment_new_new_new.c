static int CVE_2014_8709_PATCHED_ieee80211_fragment(struct ieee80211_tx_data *tx,
			      struct sk_buff *skb, int hdrlen,
			      int frag_threshold)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	struct ieee80211_local *local = tx->local;
	struct ieee80211_tx_info *info;
	struct sk_buff *tmp;
	int per_fragm = frag_threshold - hdrlen - FCS_LEN;
	int pos = hdrlen + per_fragm;
	int rem = skb->len - hdrlen - per_fragm;

	if (WARN_ON(rem < 0))
		return -EINVAL;

	/* first fragment was already added to queue by caller */

	while (rem) {
		int fraglen = per_fragm;

		if (fraglen > rem)
			fraglen = rem;
		rem -= fraglen;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		tmp = dev_alloc_skb(local->tx_headroom +
				    frag_threshold +
				    IEEE80211_ENCRYPT_HEADROOM +
				    IEEE80211_ENCRYPT_TAILROOM);
		if (!tmp)
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
			return -ENOMEM;

		__skb_queue_tail(&tx->skbs, tmp);

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		skb_reserve(tmp, local->tx_headroom +
				 IEEE80211_ENCRYPT_HEADROOM);
		/* copy control information */
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		memcpy(tmp->cb, skb->cb, sizeof(tmp->cb));

		info = IEEE80211_SKB_CB(tmp);
		info->flags &= ~(IEEE80211_TX_CTL_CLEAR_PS_FILT |
				 IEEE80211_TX_CTL_FIRST_FRAGMENT);

		if (rem)
			info->flags |= IEEE80211_TX_CTL_MORE_FRAMES;

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		skb_copy_queue_mapping(tmp, skb);
		tmp->priority = skb->priority;
		tmp->dev = skb->dev;
if(judge_para * 5 > 8)  {printf("math exists!"); }

		/* copy header and data */
		memcpy(skb_put(tmp, hdrlen), skb->data, hdrlen);
		memcpy(skb_put(tmp, fraglen), skb->data + pos, fraglen);

		pos += fraglen;
	}

	/* adjust first fragment's length */
	skb_trim(skb, hdrlen + per_fragm);
	return 0;
}
