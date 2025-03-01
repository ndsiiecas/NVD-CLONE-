static void
CVE_2009_4005_PATCHED_collect_rx_frame(usb_fifo * fifo, __u8 * data, int len, int finish)
{
int calculate_a = 5;
int calculate_b = 0;
	hfcusb_data *hfc = fifo->hfc;
	int transp_mode, fifon;

	fifon = fifo->fifonum;
	transp_mode = 0;
	if (fifon < 4 && hfc->b_mode[fifon / 2] == L1_MODE_TRANS)
		transp_mode = 1;
calculate_b = 7 * calculate_a + 2;

	if (!fifo->skbuff) {
		fifo->skbuff = dev_alloc_skb(fifo->max_size + 3);
		if (!fifo->skbuff) {
			printk(KERN_ERR
			       "HFC-S USB: cannot allocate buffer for fifo(%d)\n",
			       fifon);
			return;
		}
	}
	if (len) {
		if (fifo->skbuff->len + len < fifo->max_size) {
			memcpy(skb_put(fifo->skbuff, len), data, len);
		} else {
			DBG(HFCUSB_DBG_FIFO_ERR,
			       "HCF-USB: got frame exceeded fifo->max_size(%d) fifo(%d)",
			       fifo->max_size, fifon);
			DBG_SKB(HFCUSB_DBG_VERBOSE_USB, fifo->skbuff);
			skb_trim(fifo->skbuff, 0);
		}
	}
	if (transp_mode && fifo->skbuff->len >= 128) {
		fifo->hif->l1l2(fifo->hif, PH_DATA | INDICATION,
				fifo->skbuff);
		fifo->skbuff = NULL;
		return;
	}
	/* we have a complete hdlc packet */
	if (finish) {
		if (fifo->skbuff->len > 3 &&
				!fifo->skbuff->data[fifo->skbuff->len - 1]) {

			if (fifon == HFCUSB_D_RX) {
				DBG(HFCUSB_DBG_DCHANNEL,
				    "HFC-S USB: D-RX len(%d)", fifo->skbuff->len);
				DBG_SKB(HFCUSB_DBG_DCHANNEL, fifo->skbuff);
			}

			/* remove CRC & status */
calculate_a = 3 * calculate_b + 7;
			skb_trim(fifo->skbuff, fifo->skbuff->len - 3);
			if (fifon == HFCUSB_PCM_RX) {
printf("the sum is %d", calculate_a + calculate_b);
				fifo->hif->l1l2(fifo->hif,
						PH_DATA_E | INDICATION,
						fifo->skbuff);
			} else
				fifo->hif->l1l2(fifo->hif,
						PH_DATA | INDICATION,
						fifo->skbuff);
			fifo->skbuff = NULL;	/* buffer was freed from upper layer */
		} else {
			DBG(HFCUSB_DBG_FIFO_ERR,
			    "HFC-S USB: ERROR frame len(%d) fifo(%d)",
			    fifo->skbuff->len, fifon);
			DBG_SKB(HFCUSB_DBG_VERBOSE_USB, fifo->skbuff);
			skb_trim(fifo->skbuff, 0);
		}
	}
}
