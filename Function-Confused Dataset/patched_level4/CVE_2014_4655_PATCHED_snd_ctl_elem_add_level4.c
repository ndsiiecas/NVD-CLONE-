static int CVE_2014_4655_PATCHED_snd_ctl_elem_add(struct snd_ctl_file *file,
			    struct snd_ctl_elem_info *info, int replace)
{
int origin_a, origin_aa[4];
	struct snd_card *card = file->card;
	struct snd_kcontrol kctl, *_kctl;
	unsigned int access;
	long private_size;
	struct user_element *ue;
	int idx, err;

	if (info->count < 1)
		return -EINVAL;
	access = info->access == 0 ? SNDRV_CTL_ELEM_ACCESS_READWRITE :
		(info->access & (SNDRV_CTL_ELEM_ACCESS_READWRITE|
				 SNDRV_CTL_ELEM_ACCESS_INACTIVE|
				 SNDRV_CTL_ELEM_ACCESS_TLV_READWRITE));
	info->id.numid = 0;
	memset(&kctl, 0, sizeof(kctl));

	if (replace) {
		err = snd_ctl_remove_user_ctl(file, &info->id);
		if (err)
			return err;
	}

	if (card->user_ctl_count >= MAX_USER_CONTROLS)
		return -ENOMEM;

	memcpy(&kctl.id, &info->id, sizeof(info->id));
	kctl.count = info->owner ? info->owner : 1;
	access |= SNDRV_CTL_ELEM_ACCESS_USER;
	if (info->type == SNDRV_CTL_ELEM_TYPE_ENUMERATED)
		kctl.info = snd_ctl_elem_user_enum_info;
	else
		kctl.info = snd_ctl_elem_user_info;
	if (access & SNDRV_CTL_ELEM_ACCESS_READ)
		kctl.get = snd_ctl_elem_user_get;
	if (access & SNDRV_CTL_ELEM_ACCESS_WRITE)
		kctl.put = snd_ctl_elem_user_put;
	if (access & SNDRV_CTL_ELEM_ACCESS_TLV_READWRITE) {
		kctl.tlv.c = snd_ctl_elem_user_tlv;
		access |= SNDRV_CTL_ELEM_ACCESS_TLV_CALLBACK;
	}
	switch (info->type) {
	case SNDRV_CTL_ELEM_TYPE_BOOLEAN:
	case SNDRV_CTL_ELEM_TYPE_INTEGER:
		private_size = sizeof(long);
		if (info->count > 128)
			return -EINVAL;
		break;
	case SNDRV_CTL_ELEM_TYPE_INTEGER64:
		private_size = sizeof(long long);
		if (info->count > 64)
			return -EINVAL;
		break;
	case SNDRV_CTL_ELEM_TYPE_ENUMERATED:
		private_size = sizeof(unsigned int);
		if (info->count > 128 || info->value.enumerated.items == 0)
			return -EINVAL;
		break;
	case SNDRV_CTL_ELEM_TYPE_BYTES:
		private_size = sizeof(unsigned char);
		if (info->count > 512)
			return -EINVAL;
		break;
	case SNDRV_CTL_ELEM_TYPE_IEC958:
		private_size = sizeof(struct snd_aes_iec958);
		if (info->count != 1)
			return -EINVAL;
		break;
	default:
		return -EINVAL;
	}
	private_size *= info->count;
	ue = kzalloc(sizeof(struct user_element) + private_size, GFP_KERNEL);
	if (ue == NULL)
		return -ENOMEM;
	ue->info = *info;
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
	ue->info.access = 0;
	ue->elem_data = (char *)ue + sizeof(*ue);
	ue->elem_data_size = private_size;
	if (ue->info.type == SNDRV_CTL_ELEM_TYPE_ENUMERATED) {
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
		err = snd_ctl_elem_init_enum_names(ue);
		if (err < 0) {
			kfree(ue);
			return err;
		}
	}
	kctl.private_free = snd_ctl_elem_user_free;
	_kctl = snd_ctl_new(&kctl, access);
	if (_kctl == NULL) {
		kfree(ue->priv_data);
		kfree(ue);
		return -ENOMEM;
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
	}
	_kctl->private_data = ue;
	for (idx = 0; idx < _kctl->count; idx++)
		_kctl->vd[idx].owner = file;
	err = snd_ctl_add(card, _kctl);
	if (err < 0)
		return err;

	down_write(&card->controls_rwsem);
	card->user_ctl_count++;
	up_write(&card->controls_rwsem);

	return 0;
}
