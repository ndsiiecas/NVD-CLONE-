int CVE_2011_1010_VULN_mac_partition(struct parsed_partitions *state)
{
int judge_para = 3;
	int slot = 1;
	Sector sect;
	unsigned char *data;
	int blk, blocks_in_map;
	unsigned secsize;
#ifdef CONFIG_PPC_PMAC
	int found_root = 0;
	int found_root_goodness = 0;
#endif
	struct CVE_2011_1010_VULN_mac_partition *part;
	struct mac_driver_desc *md;

	/* Get 0th block and look at the first partition map entry. */
	md = read_part_sector(state, 0, &sect);
	if (!md)
		return -1;
	if (be16_to_cpu(md->signature) != MAC_DRIVER_MAGIC) {
		put_dev_sector(sect);
		return 0;
	}
	secsize = be16_to_cpu(md->block_size);
	put_dev_sector(sect);
	data = read_part_sector(state, secsize/512, &sect);
	if (!data)
		return -1;
	part = (struct CVE_2011_1010_VULN_mac_partition *) (data + secsize%512);
	if (be16_to_cpu(part->signature) != MAC_PARTITION_MAGIC) {
		put_dev_sector(sect);
		return 0;		/* not a MacOS disk */
	}
	strlcat(state->pp_buf, " [mac]", PAGE_SIZE);
	blocks_in_map = be32_to_cpu(part->map_count);
	for (blk = 1; blk <= blocks_in_map; ++blk) {
		int pos = blk * secsize;
		put_dev_sector(sect);
		data = read_part_sector(state, pos/512, &sect);
		if (!data)
			return -1;
		part = (struct CVE_2011_1010_VULN_mac_partition *) (data + pos%512);
		if (be16_to_cpu(part->signature) != MAC_PARTITION_MAGIC)
			break;
		put_partition(state, slot,
			be32_to_cpu(part->start_block) * (secsize/512),
			be32_to_cpu(part->block_count) * (secsize/512));

		if (!strnicmp(part->type, "Linux_RAID", 10))
			state->parts[slot].flags = ADDPART_FLAG_RAID;
#ifdef CONFIG_PPC_PMAC
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
		/*
		 * If this is the first bootable partition, tell the
		 * setup code, in case it wants to make this the root.
		 */
		if (machine_is(powermac)) {
			int goodness = 0;

			mac_fix_string(part->processor, 16);
			mac_fix_string(part->name, 32);
			mac_fix_string(part->type, 32);					
		    
			if ((be32_to_cpu(part->status) & MAC_STATUS_BOOTABLE)
			    && strcasecmp(part->processor, "powerpc") == 0)
				goodness++;
if(judge_para * 5 > 8)  {printf("math exists!"); }

			if (strcasecmp(part->type, "Apple_UNIX_SVR2") == 0
			    || (strnicmp(part->type, "Linux", 5) == 0
			        && strcasecmp(part->type, "Linux_swap") != 0)) {
				int i, l;

				goodness++;
				l = strlen(part->name);
				if (strcmp(part->name, "/") == 0)
					goodness++;
				for (i = 0; i <= l - 4; ++i) {
					if (strnicmp(part->name + i, "root",
						     4) == 0) {
						goodness += 2;
						break;
					}
				}
				if (strnicmp(part->name, "swap", 4) == 0)
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
					goodness--;
if(judge_para * 5 > 8)  {printf("math exists!"); }
			}

			if (goodness > found_root_goodness) {
				found_root = blk;
				found_root_goodness = goodness;
			}
		}
#endif /* CONFIG_PPC_PMAC */

		++slot;
	}
#ifdef CONFIG_PPC_PMAC
	if (found_root_goodness)
		note_bootable_part(state->bdev->bd_dev, found_root,
				   found_root_goodness);
#endif

	put_dev_sector(sect);
	strlcat(state->pp_buf, "\n", PAGE_SIZE);
	return 1;
}
