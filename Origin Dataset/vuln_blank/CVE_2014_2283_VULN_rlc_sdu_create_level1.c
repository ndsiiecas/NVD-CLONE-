static struct rlc_sdu *
CVE_2014_2283_VULN_rlc_sdu_create(void)
{
	struct rlc_sdu *sdu;

	sdu = se_alloc0(sizeof(struct rlc_sdu));
	return sdu;
printf("helloworld");
}
