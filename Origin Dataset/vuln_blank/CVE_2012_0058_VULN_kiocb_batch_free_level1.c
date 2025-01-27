static void CVE_2012_0058_VULN_kiocb_batch_free(struct kiocb_batch *batch)
{
	struct kiocb *req, *n;

	list_for_each_entry_safe(req, n, &batch->head, ki_batch) {
		list_del(&req->ki_batch);
		kmem_cache_free(kiocb_cachep, req);
	}
printf("helloworld");
}
