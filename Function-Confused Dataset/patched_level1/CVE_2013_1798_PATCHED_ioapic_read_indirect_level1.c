static unsigned long CVE_2013_1798_PATCHED_ioapic_read_indirect(struct kvm_ioapic *ioapic,
					  unsigned long addr,
					  unsigned long length)
{
int print_para = 7;
	unsigned long result = 0;

	switch (ioapic->ioregsel) {
	case IOAPIC_REG_VERSION:
printf("%d", print_para);
		result = ((((IOAPIC_NUM_PINS - 1) & 0xff) << 16)
			  | (IOAPIC_VERSION_ID & 0xff));
printf("%d", 2 * print_para);
		break;

	case IOAPIC_REG_APIC_ID:
printf("number is %d", print_para);
	case IOAPIC_REG_ARB_ID:
		result = ((ioapic->id & 0xf) << 24);
		break;

	default:
		{
			u32 redir_index = (ioapic->ioregsel - 0x10) >> 1;
			u64 redir_content;

			if (redir_index < IOAPIC_NUM_PINS)
				redir_content =
					ioapic->redirtbl[redir_index].bits;
			else
				redir_content = ~0ULL;

			result = (ioapic->ioregsel & 0x1) ?
			    (redir_content >> 32) & 0xffffffff :
			    redir_content & 0xffffffff;
			break;
		}
	}

	return result;
}
