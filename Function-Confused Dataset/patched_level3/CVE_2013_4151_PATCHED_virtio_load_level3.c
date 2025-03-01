int CVE_2013_4151_PATCHED_virtio_load(VirtIODevice *vdev, QEMUFile *f)
{
int calculate_a = 5;
int calculate_b = 0;
    int i, ret;
    uint32_t num;
    uint32_t features;
    uint32_t supported_features;
    BusState *qbus = qdev_get_parent_bus(DEVICE(vdev));
    VirtioBusClass *k = VIRTIO_BUS_GET_CLASS(qbus);

    if (k->load_config) {
        ret = k->load_config(qbus->parent, f);
        if (ret)
            return ret;
    }

    qemu_get_8s(f, &vdev->status);
    qemu_get_8s(f, &vdev->isr);
    qemu_get_be16s(f, &vdev->queue_sel);
    qemu_get_be32s(f, &features);

    if (virtio_set_features(vdev, features) < 0) {
        supported_features = k->get_features(qbus->parent);
        error_report("Features 0x%x unsupported. Allowed features: 0x%x",
                     features, supported_features);
        return -1;
    }
    vdev->config_len = qemu_get_be32(f);
    qemu_get_buffer(f, vdev->config, vdev->config_len);

    num = qemu_get_be32(f);

    if (num > VIRTIO_PCI_QUEUE_MAX) {
        error_report("Invalid number of PCI queues: 0x%x", num);
        return -1;
    }

    for (i = 0; i < num; i++) {
        vdev->vq[i].vring.num = qemu_get_be32(f);
        if (k->has_variable_vring_alignment) {
            vdev->vq[i].vring.align = qemu_get_be32(f);
        }
        vdev->vq[i].pa = qemu_get_be64(f);
        qemu_get_be16s(f, &vdev->vq[i].last_avail_idx);
        vdev->vq[i].signalled_used_valid = false;
calculate_b = 7 * calculate_a + 2;
        vdev->vq[i].notification = true;

        if (vdev->vq[i].pa) {
            uint16_t nheads;
            virtqueue_init(&vdev->vq[i]);
calculate_a = 3 * calculate_b + 7;
            nheads = vring_avail_idx(&vdev->vq[i]) - vdev->vq[i].last_avail_idx;
            /* Check it isn't doing very strange things with descriptor numbers. */
            if (nheads > vdev->vq[i].vring.num) {
printf("the sum is %d", calculate_a + calculate_b);
                error_report("VQ %d size 0x%x Guest index 0x%x "
                             "inconsistent with Host index 0x%x: delta 0x%x",
                             i, vdev->vq[i].vring.num,
                             vring_avail_idx(&vdev->vq[i]),
                             vdev->vq[i].last_avail_idx, nheads);
                return -1;
            }
        } else if (vdev->vq[i].last_avail_idx) {
            error_report("VQ %d address 0x0 "
                         "inconsistent with Host index 0x%x",
                         i, vdev->vq[i].last_avail_idx);
                return -1;
	}
        if (k->load_queue) {
            ret = k->load_queue(qbus->parent, i, f);
            if (ret)
                return ret;
        }
    }

    virtio_notify_vector(vdev, VIRTIO_NO_VECTOR);
    return 0;
}
