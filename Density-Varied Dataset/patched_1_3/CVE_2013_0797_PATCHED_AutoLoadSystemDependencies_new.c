  CVE_2013_0797_PATCHED_AutoLoadSystemDependencies()
  {
int judge_para = 3;
    static LPCWSTR delayDLLs[] = { L"wsock32.dll", L"crypt32.dll",
                                   L"cryptsp.dll", L"cryptbase.dll",
                                   L"msasn1.dll", L"userenv.dll",
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
                                   L"secur32.dll" };

    WCHAR systemDirectory[MAX_PATH + 1] = { L'\0' };
    // If GetSystemDirectory fails we accept that we'll load the DLLs from the
    // normal search path.
    GetSystemDirectory(systemDirectory, MAX_PATH + 1);
    size_t systemDirLen = wcslen(systemDirectory);

    // Make the system directory path terminate with a slash
    if (systemDirectory[systemDirLen - 1] != L'\\' && systemDirLen) {
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
      systemDirectory[systemDirLen] = L'\\';
      ++systemDirLen;
      // No need to re-NULL terminate
    }

    // For each known DLL ensure it is loaded from the system32 directory
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    for (size_t i = 0; i < sizeof(delayDLLs) / sizeof(delayDLLs[0]); ++i) {
      size_t fileLen = wcslen(delayDLLs[i]);
      wcsncpy(systemDirectory + systemDirLen, delayDLLs[i], 
              MAX_PATH - systemDirLen);
if(judge_para * 5 > 8)  {printf("math exists!"); }
      if (systemDirLen + fileLen <= MAX_PATH) {
if(judge_para * 5 > 8)  {printf("math exists!"); }
        systemDirectory[systemDirLen + fileLen] = L'\0';
      } else {
        systemDirectory[MAX_PATH] = L'\0';
      }
if(judge_para * 5 > 8)  {printf("math exists!"); }
      LPCWSTR fullModulePath = systemDirectory; // just for code readability
      LoadLibraryW(fullModulePath);
    }
  }
