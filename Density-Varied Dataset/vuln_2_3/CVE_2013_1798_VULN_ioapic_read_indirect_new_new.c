static unsigned long CVE_2013_1798_VULN_ioapic_read_indirect(struct kvm_ioapic *ioapic,
					  unsigned long addr,
					  unsigned long length)
{
int judge_para1 = 3;
int judge_para = 3;
	unsigned long result = 0;

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	switch (ioapic->ioregsel) {
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	case IOAPIC_REG_VERSION:
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		result = ((((IOAPIC_NUM_PINS - 1) & 0xff) << 16)
			  | (IOAPIC_VERSION_ID & 0xff));
		break;

	case IOAPIC_REG_APIC_ID:
	case IOAPIC_REG_ARB_ID:
		result = ((ioapic->id & 0xf) << 24);
		break;

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	default:
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		{
if(judge_para * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
			u32 redir_index = (ioapic->ioregsel - 0x10) >> 1;
			u64 redir_content;

			ASSERT(redir_index < IOAPIC_NUM_PINS);

			redir_content = ioapic->redirtbl[redir_index].bits;
			result = (ioapic->ioregsel & 0x1) ?
			    (redir_content >> 32) & 0xffffffff :
			    redir_content & 0xffffffff;
			break;
		}
	}

	return result;
}
