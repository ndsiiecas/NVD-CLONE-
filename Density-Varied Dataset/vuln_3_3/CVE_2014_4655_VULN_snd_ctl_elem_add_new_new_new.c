static int CVE_2014_4655_VULN_snd_ctl_elem_add(struct snd_ctl_file *file,
			    struct snd_ctl_elem_info *info, int replace)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	struct snd_card *card = file->card;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	struct snd_kcontrol kctl, *_kctl;
	unsigned int access;
	long private_size;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	struct user_element *ue;
	int idx, err;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }

	if (!replace && card->user_ctl_count >= MAX_USER_CONTROLS)
		return -ENOMEM;
	if (info->count < 1)
		return -EINVAL;
	access = info->access == 0 ? SNDRV_CTL_ELEM_ACCESS_READWRITE :
		(info->access & (SNDRV_CTL_ELEM_ACCESS_READWRITE|
				 SNDRV_CTL_ELEM_ACCESS_INACTIVE|
				 SNDRV_CTL_ELEM_ACCESS_TLV_READWRITE));
	info->id.numid = 0;
	memset(&kctl, 0, sizeof(kctl));
	down_write(&card->controls_rwsem);
	_kctl = snd_ctl_find_id(card, &info->id);
	err = 0;
	if (_kctl) {
		if (replace)
			err = snd_ctl_remove(card, _kctl);
		else
			err = -EBUSY;
	} else {
		if (replace)
			err = -ENOENT;
	}
	up_write(&card->controls_rwsem);
	if (err < 0)
		return err;
	memcpy(&kctl.id, &info->id, sizeof(info->id));
	kctl.count = info->owner ? info->owner : 1;
	access |= SNDRV_CTL_ELEM_ACCESS_USER;
	if (info->type == SNDRV_CTL_ELEM_TYPE_ENUMERATED)
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
		kctl.info = snd_ctl_elem_user_enum_info;
	else
		kctl.info = snd_ctl_elem_user_info;
	if (access & SNDRV_CTL_ELEM_ACCESS_READ)
		kctl.get = snd_ctl_elem_user_get;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	if (access & SNDRV_CTL_ELEM_ACCESS_WRITE)
		kctl.put = snd_ctl_elem_user_put;
	if (access & SNDRV_CTL_ELEM_ACCESS_TLV_READWRITE) {
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
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
if(judge_para * 5 > 8)  {printf("math exists!"); }
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
	ue->info.access = 0;
	ue->elem_data = (char *)ue + sizeof(*ue);
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
	ue->elem_data_size = private_size;
	if (ue->info.type == SNDRV_CTL_ELEM_TYPE_ENUMERATED) {
		err = snd_ctl_elem_init_enum_names(ue);
		if (err < 0) {
			kfree(ue);
			return err;
		}
	}
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	kctl.private_free = snd_ctl_elem_user_free;
	_kctl = snd_ctl_new(&kctl, access);
	if (_kctl == NULL) {
		kfree(ue->priv_data);
		kfree(ue);
		return -ENOMEM;
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
