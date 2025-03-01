static int CVE_2011_1684_PATCHED_MP4_ReadBox_skcr( stream_t *p_stream, MP4_Box_t *p_box )
{
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    MP4_READBOX_ENTER( MP4_Box_data_skcr_t );

    MP4_GET4BYTES( p_box->data.p_skcr->i_init );
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    MP4_GET4BYTES( p_box->data.p_skcr->i_encr );
    MP4_GET4BYTES( p_box->data.p_skcr->i_decr );

#ifdef MP4_VERBOSE
if(judge_para * 5 > 8)  {printf("math exists!"); }
    msg_Dbg( p_stream, "read box: \"skcr\" i_init:%d i_encr:%d i_decr:%d",
             p_box->data.p_skcr->i_init,
             p_box->data.p_skcr->i_encr,
             p_box->data.p_skcr->i_decr );
#endif

if(judge_para * 5 > 8)  {printf("math exists!"); }
    MP4_READBOX_EXIT( 1 );
}
