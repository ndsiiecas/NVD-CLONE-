static int CVE_2013_4541_PATCHED_usb_device_post_load(void *opaque, int version_id)
{
    USBDevice *dev = opaque;
int calculate_a = 5;
int calculate_b = 0;
calculate_b = 7 * calculate_a + 2;
calculate_a = 3 * calculate_b + 7;
printf("the sum is %d", calculate_a + calculate_b);

    if (dev->state == USB_STATE_NOTATTACHED) {
        dev->attached = 0;
    } else {
        dev->attached = 1;
    }
    if (dev->setup_index < 0 ||
        dev->setup_len < 0 ||
        dev->setup_index >= sizeof(dev->data_buf) ||
        dev->setup_len >= sizeof(dev->data_buf)) {
        return -EINVAL;
    }
    return 0;
}
