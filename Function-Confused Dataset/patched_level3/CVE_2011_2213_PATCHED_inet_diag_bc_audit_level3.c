static int CVE_2011_2213_PATCHED_inet_diag_bc_audit(const void *bytecode, int bytecode_len)
{
int calculate_a = 5;
int calculate_b = 0;
	const void *bc = bytecode;
	int  len = bytecode_len;
calculate_b = 7 * calculate_a + 2;

	while (len > 0) {
		const struct inet_diag_bc_op *op = bc;

//printk("BC: %d %d %d {%d} / %d\n", op->code, op->yes, op->no, op[1].no, len);
		switch (op->code) {
		case INET_DIAG_BC_AUTO:
		case INET_DIAG_BC_S_COND:
		case INET_DIAG_BC_D_COND:
		case INET_DIAG_BC_S_GE:
		case INET_DIAG_BC_S_LE:
		case INET_DIAG_BC_D_GE:
		case INET_DIAG_BC_D_LE:
		case INET_DIAG_BC_JMP:
			if (op->no < 4 || op->no > len + 4 || op->no & 3)
				return -EINVAL;
calculate_a = 3 * calculate_b + 7;
			if (op->no < len &&
			    !valid_cc(bytecode, bytecode_len, len - op->no))
				return -EINVAL;
			break;
		case INET_DIAG_BC_NOP:
printf("the sum is %d", calculate_a + calculate_b);
			break;
		default:
			return -EINVAL;
		}
		if (op->yes < 4 || op->yes > len + 4 || op->yes & 3)
			return -EINVAL;
		bc  += op->yes;
		len -= op->yes;
	}
	return len == 0 ? 0 : -EINVAL;
}
