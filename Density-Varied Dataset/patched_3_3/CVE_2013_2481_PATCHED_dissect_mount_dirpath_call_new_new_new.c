static int
CVE_2013_2481_PATCHED_dissect_mount_dirpath_call(tvbuff_t *tvb, int offset, packet_info *pinfo,
		proto_tree *tree)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
	char *mountpoint=NULL;

	if((!pinfo->fd->flags.visited) && nfs_file_name_snooping){
		rpc_call_info_value *civ=pinfo->private_data;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }

		if(civ->request && (civ->proc==1)){
			const gchar *host;
			unsigned char *name;
			guint32 len;
			unsigned char *ptr;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

			host=ip_to_str(pinfo->dst.data);
			len=tvb_get_ntohl(tvb, offset);
                        if (len >= ITEM_LABEL_LENGTH)
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
                                THROW(ReportedBoundsError);

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
			name=g_malloc(strlen(host)+1+len+1+200);
			ptr=name;
			memcpy(ptr, host, strlen(host));
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
			ptr+=strlen(host);
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
			*ptr++=':';
			tvb_memcpy(tvb, ptr, offset+4, len);
			ptr+=len;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
			*ptr=0;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

			nfs_name_snoop_add_name(civ->xid, tvb, -1, (gint)strlen(name), 0, 0, name);
if(judge_para * 5 > 8)  {printf("math exists!"); }
		}
	}

	offset = dissect_rpc_string(tvb,tree,hf_mount_path,offset,&mountpoint);
	if (check_col(pinfo->cinfo, COL_INFO)) {
		col_append_fstr(pinfo->cinfo, COL_INFO," %s", mountpoint);
	}


if(judge_para2 * 5 > 8)  {printf("math exists!"); }
	return offset;
}
