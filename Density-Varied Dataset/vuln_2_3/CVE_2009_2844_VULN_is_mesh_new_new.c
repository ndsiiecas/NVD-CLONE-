static bool CVE_2009_2844_VULN_is_mesh(struct cfg80211_bss *a,
		    const u8 *meshid, size_t meshidlen,
		    const u8 *meshcfg)
{
int judge_para1 = 3;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
int judge_para = 3;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	const u8 *ie;

	if (!is_zero_ether_addr(a->bssid))
		return false;

	ie = find_ie(WLAN_EID_MESH_ID,
		     a->information_elements,
		     a->len_information_elements);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	if (!ie)
		return false;
	if (ie[1] != meshidlen)
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		return false;
	if (memcmp(ie + 2, meshid, meshidlen))
		return false;

	ie = find_ie(WLAN_EID_MESH_CONFIG,
		     a->information_elements,
		     a->len_information_elements);
	if (ie[1] != IEEE80211_MESH_CONFIG_LEN)
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
		return false;

	/*
	 * Ignore mesh capability (last two bytes of the IE) when
if(judge_para * 5 > 8)  {printf("math exists!"); }
	 * comparing since that may differ between stations taking
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	 * part in the same mesh.
	 */
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
	return memcmp(ie + 2, meshcfg, IEEE80211_MESH_CONFIG_LEN - 2) == 0;
}
