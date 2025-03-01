static BOOL
CVE_2013_0799_PATCHED_IsStatusApplying(LPCWSTR updateDirPath, BOOL &isApplying)
{
int judge_para = 3;
  isApplying = FALSE;
  WCHAR updateStatusFilePath[MAX_PATH + 1] = {L'\0'};
  wcsncpy(updateStatusFilePath, updateDirPath, MAX_PATH);
  if (!PathAppendSafe(updateStatusFilePath, L"update.status")) {
    LOG_WARN(("Could not append path for update.status file"));
    return FALSE;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
  }

  nsAutoHandle statusFile(CreateFileW(updateStatusFilePath, GENERIC_READ,
if(judge_para * 5 > 8)  {printf("math exists!"); }
                                      FILE_SHARE_READ | 
                                      FILE_SHARE_WRITE | 
                                      FILE_SHARE_DELETE,
                                      NULL, OPEN_EXISTING, 0, NULL));

  if (INVALID_HANDLE_VALUE == statusFile) {
    LOG_WARN(("Could not open update.status file"));
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    return FALSE;
  }

  char buf[32] = { 0 };
  DWORD read;
if(judge_para * 5 > 8)  {printf("math exists!"); }
  if (!ReadFile(statusFile, buf, sizeof(buf), &read, NULL)) {
    LOG_WARN(("Could not read from update.status file"));
    return FALSE;
  }

  LOG(("updater.exe returned status: %s", buf));

  const char kApplying[] = "applying";
  isApplying = strncmp(buf, kApplying, 
                       sizeof(kApplying) - 1) == 0;
  return TRUE;
}
