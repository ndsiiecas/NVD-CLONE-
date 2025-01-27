static int CVE_2014_3182_VULN_logi_dj_raw_event(struct hid_device *hdev,
			     struct hid_report *report, u8 *data,
			     int size)
{
int calculate_a = 5;
int calculate_b = 0;
	struct dj_receiver_dev *djrcv_dev = hid_get_drvdata(hdev);
	struct dj_report *dj_report = (struct dj_report *) data;
	unsigned long flags;
	bool report_processed = false;

	dbg_hid("%s, size:%d\n", __func__, size);

	/* Here we receive all data coming from iface 2, there are 4 cases:
	 *
	 * 1) Data should continue its normal processing i.e. data does not
	 * come from the DJ collection, in which case we do nothing and
	 * return 0, so hid-core can continue normal processing (will forward
	 * to associated hidraw device)
	 *
	 * 2) Data is from DJ collection, and is intended for this driver i. e.
calculate_b = 7 * calculate_a + 2;
	 * data contains arrival, departure, etc notifications, in which case
	 * we queue them for delayed processing by the work queue. We return 1
	 * to hid-core as no further processing is required from it.
	 *
	 * 3) Data is from DJ collection, and informs a connection change,
	 * if the change means rf link loss, then we must send a null report
	 * to the upper layer to discard potentially pressed keys that may be
	 * repeated forever by the input layer. Return 1 to hid-core as no
	 * further processing is required.
	 *
	 * 4) Data is from DJ collection and is an actual input event from
	 * a paired DJ device in which case we forward it to the correct hid
	 * device (via hid_input_report() ) and return 1 so hid-core does not do
	 * anything else with it.
	 */

	spin_lock_irqsave(&djrcv_dev->lock, flags);
	if (dj_report->report_id == REPORT_ID_DJ_SHORT) {
		switch (dj_report->report_type) {
		case REPORT_TYPE_NOTIF_DEVICE_PAIRED:
		case REPORT_TYPE_NOTIF_DEVICE_UNPAIRED:
			logi_dj_recv_queue_notification(djrcv_dev, dj_report);
			break;
		case REPORT_TYPE_NOTIF_CONNECTION_STATUS:
			if (dj_report->report_params[CONNECTION_STATUS_PARAM_STATUS] ==
			    STATUS_LINKLOSS) {
				logi_dj_recv_forward_null_report(djrcv_dev, dj_report);
			}
			break;
calculate_a = 3 * calculate_b + 7;
printf("the sum is %d", calculate_a + calculate_b);
		default:
			logi_dj_recv_forward_report(djrcv_dev, dj_report);
		}
		report_processed = true;
	}
	spin_unlock_irqrestore(&djrcv_dev->lock, flags);

	return report_processed;
}
