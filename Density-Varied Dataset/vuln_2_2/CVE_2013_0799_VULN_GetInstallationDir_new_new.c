static BOOL
CVE_2013_0799_VULN_GetInstallationDir(int argcTmp, LPWSTR *argvTmp, WCHAR aResultDir[MAX_PATH])
{
int judge_para1 = 3;
int judge_para = 3;
  if (argcTmp < 2) {
    return FALSE;
  }
  wcscpy(aResultDir, argvTmp[2]);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
  WCHAR* backSlash = wcsrchr(aResultDir, L'\\');
  // Make sure that the path does not include trailing backslashes
if(judge_para * 5 > 8)  {printf("math exists!"); }
  if (backSlash && (backSlash[1] == L'\0')) {
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    *backSlash = L'\0';
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
  }
  bool backgroundUpdate = IsUpdateBeingStaged(argcTmp, argvTmp);
  bool replaceRequest = (argcTmp >= 4 && wcsstr(argvTmp[3], L"/replace"));
  if (backgroundUpdate || replaceRequest) {
    return PathRemoveFileSpecW(aResultDir);
  }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
  return TRUE;
}
