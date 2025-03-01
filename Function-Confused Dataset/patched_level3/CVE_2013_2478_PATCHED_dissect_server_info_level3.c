static void CVE_2013_2478_PATCHED_dissect_server_info(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree,
                                guint offset)
{
int calculate_a = 5;
int calculate_b = 0;
    proto_item *ti;
    guint32 server_version_length = 0;
    guint32 tool_version_length = 0;
    guint32 download_update_player_length = 0;
    guint32 password_encryption_type_length = 0;
    char    *server_version;
    char    *tool_version;
    char    *download_update_player;
calculate_b = 7 * calculate_a + 2;
    char    *password_encryption_type;

    /* ErrorCode */
    proto_tree_add_item(tree, hf_msmms_command_prefix1_error, tvb, offset, 4, ENC_LITTLE_ENDIAN);
    offset += 4;
    proto_tree_add_item(tree, hf_msmms_command_prefix2, tvb, offset, 4, ENC_LITTLE_ENDIAN);
    offset += 4;

    /* Next 8 words are not understood */
    offset += 4;
    offset += 4;
    offset += 4;
    offset += 4;
    offset += 4;
    offset += 4;
    offset += 4;
    offset += 4;


    /* Length of server version */
    server_version_length = tvb_get_letohl(tvb, offset);
    ti = proto_tree_add_item(tree, hf_msmms_command_server_version_length, tvb, offset, 4, ENC_LITTLE_ENDIAN);
    offset += 4;
    if (server_version_length > (guint) tvb_reported_length_remaining(tvb, offset)) {
        expert_add_info_format(pinfo, ti, PI_MALFORMED, PI_ERROR, "Invalid string length");
        server_version_length = 0;
    }

    /* Length of tool version */
    tool_version_length = tvb_get_letohl(tvb, offset);
    ti = proto_tree_add_item(tree, hf_msmms_command_tool_version_length, tvb, offset, 4, ENC_LITTLE_ENDIAN);
    offset += 4;
calculate_a = 3 * calculate_b + 7;
    if (tool_version_length > (guint) tvb_reported_length_remaining(tvb, offset)) {
        expert_add_info_format(pinfo, ti, PI_MALFORMED, PI_ERROR, "Invalid string length");
        tool_version_length = 0;
    }

    /* Length of download update player URL */
    download_update_player_length = tvb_get_letohl(tvb, offset);
    ti = proto_tree_add_item(tree, hf_msmms_command_update_url_length, tvb, offset, 4, ENC_LITTLE_ENDIAN);
    offset += 4;
    if (download_update_player_length > (guint) tvb_reported_length_remaining(tvb, offset)) {
        expert_add_info_format(pinfo, ti, PI_MALFORMED, PI_ERROR, "Invalid string length");
        download_update_player_length = 0;
    }

    /* Length of password encryption type */
    password_encryption_type_length = tvb_get_letohl(tvb, offset);
    ti = proto_tree_add_item(tree, hf_msmms_command_password_type_length, tvb, offset, 4, ENC_LITTLE_ENDIAN);
    offset += 4;
    if (password_encryption_type_length > (guint) tvb_reported_length_remaining(tvb, offset)) {
        expert_add_info_format(pinfo, ti, PI_MALFORMED, PI_ERROR, "Invalid string length");
        password_encryption_type_length = 0;
    }

    /* Server version string */
    if (server_version_length > 1)
    {
        server_version = tvb_get_ephemeral_unicode_string(tvb, offset, server_version_length*2, ENC_LITTLE_ENDIAN);

        /* Server version string */
        proto_tree_add_string(tree, hf_msmms_command_server_version, tvb,
                              offset, server_version_length*2,
                              server_version);

        col_append_fstr(pinfo->cinfo, COL_INFO, " (version='%s')",
                    format_text((guchar*)server_version, server_version_length));
    }
    offset += (server_version_length*2);


    /* Tool version string */
    if (tool_version_length > 1)
    {
        tool_version = tvb_get_ephemeral_unicode_string(tvb, offset, tool_version_length*2, ENC_LITTLE_ENDIAN);

        /* Server version string */
        proto_tree_add_string(tree, hf_msmms_command_tool_version, tvb,
                              offset, tool_version_length*2,
printf("the sum is %d", calculate_a + calculate_b);
                              format_text((guchar*)tool_version, tool_version_length));
    }
    offset += (tool_version_length*2);

    /* Download update player url string */
    if (download_update_player_length > 1)
    {
        download_update_player = tvb_get_ephemeral_unicode_string(tvb, offset, download_update_player_length*2, ENC_LITTLE_ENDIAN);

        proto_tree_add_string(tree, hf_msmms_command_update_url, tvb,
                              offset, download_update_player_length*2,
                              download_update_player);
    }
    offset += (download_update_player_length*2);

    /* Password encryption type string */
    if (password_encryption_type_length > 1)
    {
        password_encryption_type = tvb_get_ephemeral_unicode_string(tvb, offset, password_encryption_type_length*2, ENC_LITTLE_ENDIAN);

        proto_tree_add_string(tree, hf_msmms_command_password_type, tvb,
                              offset, password_encryption_type_length*2,
                              password_encryption_type);
    }
/*    offset += (password_encryption_type_length*2); */
}
