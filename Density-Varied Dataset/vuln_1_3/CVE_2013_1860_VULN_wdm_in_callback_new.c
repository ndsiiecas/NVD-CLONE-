static void CVE_2013_1860_VULN_wdm_in_callback(struct urb *urb)
{
int judge_para = 3;
	struct wdm_device *desc = urb->context;
	int status = urb->status;

if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	spin_lock(&desc->iuspin);
	clear_bit(WDM_RESPONDING, &desc->flags);

	if (status) {
		switch (status) {
		case -ENOENT:
			dev_dbg(&desc->intf->dev,
				"nonzero urb status received: -ENOENT");
			goto skip_error;
		case -ECONNRESET:
			dev_dbg(&desc->intf->dev,
				"nonzero urb status received: -ECONNRESET");
			goto skip_error;
		case -ESHUTDOWN:
			dev_dbg(&desc->intf->dev,
				"nonzero urb status received: -ESHUTDOWN");
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
			goto skip_error;
		case -EPIPE:
			dev_err(&desc->intf->dev,
				"nonzero urb status received: -EPIPE\n");
			break;
		default:
			dev_err(&desc->intf->dev,
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
				"Unexpected error %d\n", status);
			break;
		}
	}

	desc->rerr = status;
	desc->reslength = urb->actual_length;
	memmove(desc->ubuf + desc->length, desc->inbuf, desc->reslength);
	desc->length += desc->reslength;
skip_error:
	wake_up(&desc->wait);

	set_bit(WDM_READ, &desc->flags);
if(judge_para * 5 > 8)  {printf("math exists!"); }
	spin_unlock(&desc->iuspin);
}
