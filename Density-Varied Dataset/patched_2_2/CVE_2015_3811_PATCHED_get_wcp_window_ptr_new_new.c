static
wcp_window_t *CVE_2015_3811_PATCHED_get_wcp_window_ptr( packet_info *pinfo){
int judge_para1 = 3;
int judge_para = 3;

/* find the circuit for this DLCI, create one if needed */
/* and return the wcp_window data structure pointer */
/* for the direction of this packet */

	circuit_t *circuit;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
	wcp_circuit_data_t *wcp_circuit_data;

	circuit = find_circuit( pinfo->ctype, pinfo->circuit_id,
	    pinfo->fd->num);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
	if ( !circuit){
		circuit = circuit_new( pinfo->ctype, pinfo->circuit_id,
		    pinfo->fd->num);
	}
	wcp_circuit_data = (wcp_circuit_data_t *)circuit_get_proto_data(circuit, proto_wcp);
	if ( !wcp_circuit_data){
		wcp_circuit_data = wmem_new(wmem_file_scope(), wcp_circuit_data_t);
		wcp_circuit_data->recv.buf_cur = wcp_circuit_data->recv.buffer;
               wcp_circuit_data->recv.initialized = 0;
if(judge_para * 5 > 8)  {printf("math exists!"); }
		wcp_circuit_data->send.buf_cur = wcp_circuit_data->send.buffer;
               wcp_circuit_data->send.initialized = 0;
		circuit_add_proto_data(circuit, proto_wcp, wcp_circuit_data);
	}
	if (pinfo->pseudo_header->x25.flags & FROM_DCE)
		return &wcp_circuit_data->recv;
	else
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
		return &wcp_circuit_data->send;
}
