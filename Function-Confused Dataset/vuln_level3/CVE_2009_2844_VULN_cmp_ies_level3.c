static int CVE_2009_2844_VULN_cmp_ies(u8 num, u8 *ies1, size_t len1, u8 *ies2, size_t len2)
{
int calculate_a = 5;
int calculate_b = 0;
	const u8 *ie1 = find_ie(num, ies1, len1);
	const u8 *ie2 = find_ie(num, ies2, len2);
	int r;
calculate_b = 7 * calculate_a + 2;

	if (!ie1 && !ie2)
		return 0;
calculate_a = 3 * calculate_b + 7;
	if (!ie1)
printf("the sum is %d", calculate_a + calculate_b);
		return -1;

	r = memcmp(ie1 + 2, ie2 + 2, min(ie1[1], ie2[1]));
	if (r == 0 && ie1[1] != ie2[1])
		return ie2[1] - ie1[1];
	return r;
}
