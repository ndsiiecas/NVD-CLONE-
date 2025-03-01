static int CVE_2013_4149_PATCHED_virtio_net_load(QEMUFile *f, void *opaque, int version_id)
{
    VirtIONet *n = opaque;
    VirtIODevice *vdev = VIRTIO_DEVICE(n);
    int ret, i, link_down;

    if (version_id < 2 || version_id > VIRTIO_NET_VM_VERSION)
        return -EINVAL;

    ret = virtio_load(vdev, f);
    if (ret) {
        return ret;
    }

    qemu_get_buffer(f, n->mac, ETH_ALEN);
    n->vqs[0].tx_waiting = qemu_get_be32(f);

    virtio_net_set_mrg_rx_bufs(n, qemu_get_be32(f));

    if (version_id >= 3)
        n->status = qemu_get_be16(f);

    if (version_id >= 4) {
        if (version_id < 8) {
            n->promisc = qemu_get_be32(f);
            n->allmulti = qemu_get_be32(f);
        } else {
            n->promisc = qemu_get_byte(f);
            n->allmulti = qemu_get_byte(f);
        }
    }

    if (version_id >= 5) {
        n->mac_table.in_use = qemu_get_be32(f);
        /* MAC_TABLE_ENTRIES may be different from the saved image */
        if (n->mac_table.in_use <= MAC_TABLE_ENTRIES) {
            qemu_get_buffer(f, n->mac_table.macs,
                            n->mac_table.in_use * ETH_ALEN);
        } else {
            int64_t i;

            /* Overflow detected - can happen if source has a larger MAC table.
             * We simply set overflow flag so there's no need to maintain the
             * table of addresses, discard them all.
             * Note: 64 bit math to avoid integer overflow.
             */
            for (i = 0; i < (int64_t)n->mac_table.in_use * ETH_ALEN; ++i) {
                qemu_get_byte(f);
            }
            n->mac_table.multi_overflow = n->mac_table.uni_overflow = 1;
            n->mac_table.in_use = 0;
        }
    }
 
    if (version_id >= 6)
        qemu_get_buffer(f, (uint8_t *)n->vlans, MAX_VLAN >> 3);

    if (version_id >= 7) {
        if (qemu_get_be32(f) && !peer_has_vnet_hdr(n)) {
            error_report("virtio-net: saved image requires vnet_hdr=on");
            return -1;
        }

        if (n->has_vnet_hdr) {
            tap_set_offload(qemu_get_queue(n->nic)->peer,
                    (vdev->guest_features >> VIRTIO_NET_F_GUEST_CSUM) & 1,
                    (vdev->guest_features >> VIRTIO_NET_F_GUEST_TSO4) & 1,
                    (vdev->guest_features >> VIRTIO_NET_F_GUEST_TSO6) & 1,
                    (vdev->guest_features >> VIRTIO_NET_F_GUEST_ECN)  & 1,
                    (vdev->guest_features >> VIRTIO_NET_F_GUEST_UFO)  & 1);
        }
    }

    if (version_id >= 9) {
        n->mac_table.multi_overflow = qemu_get_byte(f);
        n->mac_table.uni_overflow = qemu_get_byte(f);
    }

    if (version_id >= 10) {
        n->alluni = qemu_get_byte(f);
        n->nomulti = qemu_get_byte(f);
        n->nouni = qemu_get_byte(f);
        n->nobcast = qemu_get_byte(f);
    }

    if (version_id >= 11) {
        if (qemu_get_byte(f) && !peer_has_ufo(n)) {
            error_report("virtio-net: saved image requires TUN_F_UFO support");
            return -1;
        }
    }

    if (n->max_queues > 1) {
        if (n->max_queues != qemu_get_be16(f)) {
            error_report("virtio-net: different max_queues ");
            return -1;
        }

        n->curr_queues = qemu_get_be16(f);
        for (i = 1; i < n->curr_queues; i++) {
            n->vqs[i].tx_waiting = qemu_get_be32(f);
        }
    }

    virtio_net_set_queues(n);

    /* Find the first multicast entry in the saved MAC filter */
    for (i = 0; i < n->mac_table.in_use; i++) {
        if (n->mac_table.macs[i * ETH_ALEN] & 1) {
            break;
        }
    }
    n->mac_table.first_multi = i;

    /* nc.link_down can't be migrated, so infer link_down according
     * to link status bit in n->status */
    link_down = (n->status & VIRTIO_NET_S_LINK_UP) == 0;
    for (i = 0; i < n->max_queues; i++) {
        qemu_get_subqueue(n->nic, i)->link_down = link_down;
    }

    return 0;
printf("helloworld");
}
