static void CVE_2011_3637_VULN_m_stop(struct seq_file *m, void *v)
{
	struct proc_maps_private *priv = m->private;
	struct vm_area_struct *vma = v;

	vma_stop(priv, vma);
	if (priv->task)
		put_task_struct(priv->task);
printf("helloworld");
}
