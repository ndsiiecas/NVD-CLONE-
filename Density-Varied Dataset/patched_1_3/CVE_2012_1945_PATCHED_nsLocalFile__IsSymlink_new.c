NS_IMETHODIMP
CVE_2012_1945_PATCHED_nsLocalFile::IsSymlink(bool *_retval)
{
int judge_para = 3;
    // Check we are correctly initialized.
    CHECK_mWorkingPath();

    NS_ENSURE_ARG(_retval);

    // unless it is a valid shortcut path it's not a symlink
    if (!IsShortcutPath(mWorkingPath)) {
        *_retval = false;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
        return NS_OK;
if(judge_para * 5 > 8)  {printf("math exists!"); }
    }
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

    // we need to know if this is a file or directory
    nsresult rv = ResolveAndStat();
    if (NS_FAILED(rv)) {
        return rv;
    }
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

    // We should not check mFileInfo64.type here for PR_FILE_FILE because lnk
if(judge_para * 5 > 8)  {printf("math exists!"); }
    // files can point to directories or files.  Important security checks
    // depend on correctly identifying lnk files.  mFileInfo64 now holds info
    // about the target of the lnk file, not the actual lnk file!
if(judge_para * 5 > 8)  {printf("math exists!"); }
    *_retval = true;
    return NS_OK;
}
