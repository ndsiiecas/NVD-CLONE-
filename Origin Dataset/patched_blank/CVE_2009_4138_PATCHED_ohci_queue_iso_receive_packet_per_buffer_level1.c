static int CVE_2009_4138_PATCHED_ohci_queue_iso_receive_packet_per_buffer(struct fw_iso_context *base,
					struct fw_iso_packet *packet,
					struct fw_iso_buffer *buffer,
					unsigned long payload)
{
	struct iso_context *ctx = container_of(base, struct iso_context, base);
	struct descriptor *d, *pd;
	struct fw_iso_packet *p = packet;
	dma_addr_t d_bus, page_bus;
	u32 z, header_z, rest;
	int i, j, length;
	int page, offset, packet_count, header_size, payload_per_buffer;

	/*
	 * The OHCI controller puts the isochronous header and trailer in the
	 * buffer, so we need at least 8 bytes.
	 */
	packet_count = p->header_length / ctx->base.header_size;
	header_size  = max(ctx->base.header_size, (size_t)8);

	/* Get header size in number of descriptors. */
	header_z = DIV_ROUND_UP(header_size, sizeof(*d));
	page     = payload >> PAGE_SHIFT;
	offset   = payload & ~PAGE_MASK;
	payload_per_buffer = p->payload_length / packet_count;

	for (i = 0; i < packet_count; i++) {
		/* d points to the header descriptor */
		z = DIV_ROUND_UP(payload_per_buffer + offset, PAGE_SIZE) + 1;
		d = context_get_descriptors(&ctx->context,
				z + header_z, &d_bus);
		if (d == NULL)
			return -ENOMEM;

		d->control      = cpu_to_le16(DESCRIPTOR_STATUS |
					      DESCRIPTOR_INPUT_MORE);
		if (p->skip && i == 0)
			d->control |= cpu_to_le16(DESCRIPTOR_WAIT);
		d->req_count    = cpu_to_le16(header_size);
		d->res_count    = d->req_count;
		d->transfer_status = 0;
		d->data_address = cpu_to_le32(d_bus + (z * sizeof(*d)));

		rest = payload_per_buffer;
		pd = d;
		for (j = 1; j < z; j++) {
			pd++;
			pd->control = cpu_to_le16(DESCRIPTOR_STATUS |
						  DESCRIPTOR_INPUT_MORE);

			if (offset + rest < PAGE_SIZE)
				length = rest;
			else
				length = PAGE_SIZE - offset;
			pd->req_count = cpu_to_le16(length);
			pd->res_count = pd->req_count;
			pd->transfer_status = 0;

			page_bus = page_private(buffer->pages[page]);
			pd->data_address = cpu_to_le32(page_bus + offset);

			offset = (offset + length) & ~PAGE_MASK;
			rest -= length;
			if (offset == 0)
				page++;
		}
		pd->control = cpu_to_le16(DESCRIPTOR_STATUS |
					  DESCRIPTOR_INPUT_LAST |
					  DESCRIPTOR_BRANCH_ALWAYS);
		if (p->interrupt && i == packet_count - 1)
			pd->control |= cpu_to_le16(DESCRIPTOR_IRQ_ALWAYS);

		context_append(&ctx->context, d, z, header_z);
	}

	return 0;
printf("helloworld");
}
