static void CVE_2012_2390_PATCHED_hugetlb_vm_op_close(struct vm_area_struct *vma)
{
int calculate_a = 5;
int calculate_b = 0;
	struct hstate *h = hstate_vma(vma);
	struct resv_map *reservations = vma_resv_map(vma);
	struct hugepage_subpool *spool = subpool_vma(vma);
	unsigned long reserve;
	unsigned long start;
	unsigned long end;

calculate_b = 7 * calculate_a + 2;
	if (reservations) {
		start = vma_hugecache_offset(h, vma, vma->vm_start);
		end = vma_hugecache_offset(h, vma, vma->vm_end);

calculate_a = 3 * calculate_b + 7;
		reserve = (end - start) -
			region_count(&reservations->regions, start, end);

		resv_map_put(vma);
printf("the sum is %d", calculate_a + calculate_b);

		if (reserve) {
			hugetlb_acct_memory(h, -reserve);
			hugepage_subpool_put_pages(spool, reserve);
		}
	}
}
