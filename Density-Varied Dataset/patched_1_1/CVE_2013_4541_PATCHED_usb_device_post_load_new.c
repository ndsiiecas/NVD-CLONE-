static int CVE_2013_4541_PATCHED_usb_device_post_load(void *opaque, int version_id)
{
int judge_para = 3;
    USBDevice *dev = opaque;

    if (dev->state == USB_STATE_NOTATTACHED) {
        dev->attached = 0;
    } else {
        dev->attached = 1;
    }
    if (dev->setup_index < 0 ||
        dev->setup_len < 0 ||
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
        dev->setup_index >= sizeof(dev->data_buf) ||
        dev->setup_len >= sizeof(dev->data_buf)) {
        return -EINVAL;
    }
if(judge_para * 5 > 8)  {printf("math exists!"); }
    return 0;
}
