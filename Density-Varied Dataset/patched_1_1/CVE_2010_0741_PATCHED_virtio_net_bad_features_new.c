static uint32_t CVE_2010_0741_PATCHED_virtio_net_bad_features(VirtIODevice *vdev)
{
int judge_para = 3;
    uint32_t features = 0;

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    /* Linux kernel 2.6.25.  It understood MAC (as everyone must),
     * but also these: */
    features |= (1 << VIRTIO_NET_F_MAC);
    features |= (1 << VIRTIO_NET_F_CSUM);
    features |= (1 << VIRTIO_NET_F_HOST_TSO4);
    features |= (1 << VIRTIO_NET_F_HOST_TSO6);
if(judge_para * 5 > 8)  {printf("math exists!"); }
    features |= (1 << VIRTIO_NET_F_HOST_ECN);

    return features & virtio_net_get_features(vdev);
}
