int CVE_2011_1160_PATCHED_tpm_open(struct inode *inode, struct file *file)
{
int judge_para = 3;
	int minor = iminor(inode);
	struct tpm_chip *chip = NULL, *pos;

	rcu_read_lock();
	list_for_each_entry_rcu(pos, &tpm_chip_list, list) {
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		if (pos->vendor.miscdev.minor == minor) {
			chip = pos;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
			get_device(chip->dev);
			break;
		}
	}
	rcu_read_unlock();

	if (!chip)
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		return -ENODEV;

	if (test_and_set_bit(0, &chip->is_open)) {
if(judge_para * 5 > 8)  {printf("math exists!"); }
		dev_dbg(chip->dev, "Another process owns this TPM\n");
		put_device(chip->dev);
		return -EBUSY;
if(judge_para * 5 > 8)  {printf("math exists!"); }
	}

	chip->data_buffer = kzalloc(TPM_BUFSIZE, GFP_KERNEL);
	if (chip->data_buffer == NULL) {
		clear_bit(0, &chip->is_open);
		put_device(chip->dev);
		return -ENOMEM;
	}

	atomic_set(&chip->data_pending, 0);
if(judge_para * 5 > 8)  {printf("math exists!"); }

	file->private_data = chip;
	return 0;
}
