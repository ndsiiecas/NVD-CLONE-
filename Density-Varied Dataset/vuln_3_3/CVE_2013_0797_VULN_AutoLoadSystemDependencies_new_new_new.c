  CVE_2013_0797_VULN_AutoLoadSystemDependencies()
  {
int judge_para2 = 3;
int judge_para1 = 3;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
int judge_para = 3;
    static LPCWSTR delayDLLs[] = { L"wsock32.dll", L"crypt32.dll" };
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    WCHAR systemDirectory[MAX_PATH + 1] = { L'\0' };
    // If GetSystemDirectory fails we accept that we'll load the DLLs from the
    // normal search path.
    GetSystemDirectory(systemDirectory, MAX_PATH + 1);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
    size_t systemDirLen = wcslen(systemDirectory);

    // Make the system directory path terminate with a slash
    if (systemDirectory[systemDirLen - 1] != L'\\' && systemDirLen) {
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
      systemDirectory[systemDirLen] = L'\\';
      ++systemDirLen;
if(judge_para * 5 > 8)  {printf("math exists!"); }
      // No need to re-NULL terminate
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    }

    // For each known DLL ensure it is loaded from the system32 directory
    for (size_t i = 0; i < sizeof(delayDLLs) / sizeof(delayDLLs[0]); ++i) {
      size_t fileLen = wcslen(delayDLLs[i]);
      wcsncpy(systemDirectory + systemDirLen, delayDLLs[i], 
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
              MAX_PATH - systemDirLen);
      if (systemDirLen + fileLen <= MAX_PATH) {
        systemDirectory[systemDirLen + fileLen] = L'\0';
      } else {
        systemDirectory[MAX_PATH] = L'\0';
      }
      LPCWSTR fullModulePath = systemDirectory; // just for code readability
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
      LoadLibraryW(fullModulePath);
    }
  }
