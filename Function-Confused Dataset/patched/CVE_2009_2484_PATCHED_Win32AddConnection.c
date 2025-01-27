static void CVE_2009_2484_PATCHED_Win32AddConnection( access_t *p_access, char *psz_path,
                                char *psz_user, char *psz_pwd,
                                char *psz_domain )
{
    DWORD (*OurWNetAddConnection2)( LPNETRESOURCE, LPCTSTR, LPCTSTR, DWORD );
    char psz_remote[MAX_PATH], psz_server[MAX_PATH], psz_share[MAX_PATH];
    NETRESOURCE net_resource;
    DWORD i_result;
    char *psz_parser;
    VLC_UNUSED( psz_domain );

    HINSTANCE hdll = LoadLibrary(_T("MPR.DLL"));
    if( !hdll )
    {
        msg_Warn( p_access, "couldn't load mpr.dll" );
        return;
    }

    OurWNetAddConnection2 =
      (void *)GetProcAddress( hdll, _T("WNetAddConnection2A") );
    if( !OurWNetAddConnection2 )
    {
        msg_Warn( p_access, "couldn't find WNetAddConnection2 in mpr.dll" );
        return;
    }

    memset( &net_resource, 0, sizeof(net_resource) );
    net_resource.dwType = RESOURCETYPE_DISK;

    /* Find out server and share names */
    strlcpy( psz_server, psz_path, sizeof( psz_server ) );
    psz_share[0] = 0;
    psz_parser = strchr( psz_path, '/' );
    if( psz_parser )
    {
        char *psz_parser2 = strchr( ++psz_parser, '/' );
        if( psz_parser2 )
            strlcpy( psz_share, psz_parser, sizeof( psz_share ) );
   }

    snprintf( psz_remote, sizeof( psz_remote ), "\\\\%s\\%s", psz_server, psz_share );
    net_resource.lpRemoteName = psz_remote;

    i_result = OurWNetAddConnection2( &net_resource, psz_pwd, psz_user, 0 );

    if( i_result != NO_ERROR )
    {
        msg_Dbg( p_access, "connected to %s", psz_remote );
    }
    else if( i_result != ERROR_ALREADY_ASSIGNED &&
             i_result != ERROR_DEVICE_ALREADY_REMEMBERED )
    {
        msg_Dbg( p_access, "already connected to %s", psz_remote );
    }
    else
    {
        msg_Dbg( p_access, "failed to connect to %s", psz_remote );
    }

    FreeLibrary( hdll );
}
