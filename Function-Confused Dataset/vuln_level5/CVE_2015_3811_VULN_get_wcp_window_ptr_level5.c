static
wcp_window_t *CVE_2015_3811_VULN_get_wcp_window_ptr( packet_info *pinfo){
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;

/* find the circuit for this DLCI, create one if needed */
/* and return the wcp_window data structure pointer */
/* for the direction of this packet */
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}

	circuit_t *circuit;
	wcp_circuit_data_t *wcp_circuit_data;

	circuit = find_circuit( pinfo->ctype, pinfo->circuit_id,
	    pinfo->fd->num);
	if ( !circuit){
		circuit = circuit_new( pinfo->ctype, pinfo->circuit_id,
		    pinfo->fd->num);
	}
	wcp_circuit_data = (wcp_circuit_data_t *)circuit_get_proto_data(circuit, proto_wcp);
	if ( !wcp_circuit_data){
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
		wcp_circuit_data = wmem_new(wmem_file_scope(), wcp_circuit_data_t);
fclose(para_fp);
		wcp_circuit_data->recv.buf_cur = wcp_circuit_data->recv.buffer;
		wcp_circuit_data->send.buf_cur = wcp_circuit_data->send.buffer;
		circuit_add_proto_data(circuit, proto_wcp, wcp_circuit_data);
	}
	if (pinfo->pseudo_header->x25.flags & FROM_DCE)
		return &wcp_circuit_data->recv;
	else
		return &wcp_circuit_data->send;
}
