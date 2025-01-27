static int CVE_2013_4541_VULN_usb_device_post_load(void *opaque, int version_id)
{
int origin_a, origin_aa[4];
    USBDevice *dev = opaque;

    if (dev->state == USB_STATE_NOTATTACHED) {
        dev->attached = 0;
    } else {
        dev->attached = 1;
    }
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
}
    if (dev->setup_index >= sizeof(dev->data_buf) ||
        dev->setup_len >= sizeof(dev->data_buf)) {
        return -EINVAL;
    }
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
    return 0;
}
