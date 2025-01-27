static int CVE_2010_4648_PATCHED_orinoco_ioctl_set_auth(struct net_device *dev,
				  struct iw_request_info *info,
				  union iwreq_data *wrqu, char *extra)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	struct orinoco_private *priv = ndev_priv(dev);
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	hermes_t *hw = &priv->hw;
	struct iw_param *param = &wrqu->param;
	unsigned long flags;
	int ret = -EINPROGRESS;

	if (orinoco_lock(priv, &flags) != 0)
		return -EBUSY;

	switch (param->flags & IW_AUTH_INDEX) {
	case IW_AUTH_WPA_VERSION:
	case IW_AUTH_CIPHER_PAIRWISE:
	case IW_AUTH_CIPHER_GROUP:
	case IW_AUTH_RX_UNENCRYPTED_EAPOL:
	case IW_AUTH_PRIVACY_INVOKED:
	case IW_AUTH_DROP_UNENCRYPTED:
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
		/*
		 * orinoco does not use these parameters
		 */
		break;

	case IW_AUTH_KEY_MGMT:
		/* wl_lkm implies value 2 == PSK for Hermes I
		 * which ties in with WEXT
		 * no other hints tho :(
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		 */
		priv->key_mgmt = param->value;
		break;

	case IW_AUTH_TKIP_COUNTERMEASURES:
		/* When countermeasures are enabled, shut down the
		 * card; when disabled, re-enable the card. This must
		 * take effect immediately.
		 *
		 * TODO: Make sure that the EAPOL message is getting
		 *       out before card disabled
		 */
		if (param->value) {
			priv->tkip_cm_active = 1;
			ret = hermes_disable_port(hw, 0);
		} else {
			priv->tkip_cm_active = 0;
			ret = hermes_enable_port(hw, 0);
		}
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		break;

	case IW_AUTH_80211_AUTH_ALG:
		if (param->value & IW_AUTH_ALG_SHARED_KEY)
			priv->wep_restrict = 1;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		else if (param->value & IW_AUTH_ALG_OPEN_SYSTEM)
			priv->wep_restrict = 0;
		else
			ret = -EINVAL;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		break;

	case IW_AUTH_WPA_ENABLED:
		if (priv->has_wpa) {
			priv->wpa_enabled = param->value ? 1 : 0;
		} else {
			if (param->value)
				ret = -EOPNOTSUPP;
			/* else silently accept disable of WPA */
			priv->wpa_enabled = 0;
		}
if(judge_para * 5 > 8)  {printf("math exists!"); }
		break;

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	default:
		ret = -EOPNOTSUPP;
	}
if(judge_para2 * 5 > 8)  {printf("math exists!"); }

	orinoco_unlock(priv, &flags);
	return ret;
}
