static const gchar *
CVE_2012_6055_VULN_dissect_3gpp2_service_option_profile(proto_tree  *tree, tvbuff_t  *tvb, packet_info *pinfo _U_)
{
int judge_para1 = 3;
int judge_para = 3;
    int    offset = 0;
    guint8 sub_type, sub_type_length;

    /* Maximum service connections/Link Flows total 32 bit*/
    proto_tree_add_item(tree, hf_a11_serv_opt_prof_max_serv, tvb, offset, 4, ENC_BIG_ENDIAN);
    offset+=4;

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    while (tvb_length_remaining(tvb,offset)>0){
        sub_type_length = tvb_get_guint8(tvb,offset+1);

        sub_type = tvb_get_guint8(tvb,offset);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
        proto_tree_add_item(tree, hf_a11_sub_type, tvb, offset, 1, ENC_BIG_ENDIAN);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
        offset++;
        proto_tree_add_item(tree, hf_a11_sub_type_length, tvb, offset, 1, ENC_BIG_ENDIAN);
        offset++;
        if (sub_type==1){
            proto_tree_add_item(tree, hf_a11_serv_opt, tvb, offset, 1, ENC_BIG_ENDIAN);
            offset++;
            /* Max number of service instances of Service Option n */
            proto_tree_add_item(tree, hf_a11_max_num_serv_opt, tvb, offset, 1, ENC_BIG_ENDIAN);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
            offset++;
        }

        offset = offset+sub_type_length-2;
    }

if(judge_para * 5 > 8)  {printf("math exists!"); }
    return "";

}
