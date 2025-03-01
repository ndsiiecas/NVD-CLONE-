static void
CVE_2015_0833_PATCHED_UpdateThreadFunc(void *param)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
  // open ZIP archive and process...
  int rv;
  if (sReplaceRequest) {
    rv = ProcessReplaceRequest();
  } else {
    NS_tchar dataFile[MAXPATHLEN];
    rv = GetUpdateFileName(dataFile, sizeof(dataFile)/sizeof(dataFile[0]));
    if (rv == OK) {
      rv = gArchiveReader.Open(dataFile);
    }

#ifdef MOZ_VERIFY_MAR_SIGNATURE
    if (rv == OK) {
#ifdef XP_WIN
      HKEY baseKey = nullptr;
      wchar_t valueName[] = L"Image Path";
      wchar_t rasenh[] = L"rsaenh.dll";
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
      bool reset = false;
      if (RegOpenKeyExW(HKEY_LOCAL_MACHINE,
                        L"SOFTWARE\\Microsoft\\Cryptography\\Defaults\\Provider\\Microsoft Enhanced Cryptographic Provider v1.0",
                        0, KEY_READ | KEY_WRITE,
                        &baseKey) == ERROR_SUCCESS) {
        wchar_t path[MAX_PATH + 1];
        DWORD size = sizeof(path);
        DWORD type;
        if (RegQueryValueExW(baseKey, valueName, 0, &type,
                             (LPBYTE)path, &size) == ERROR_SUCCESS) {
          if (type == REG_SZ && wcscmp(path, rasenh) == 0) {
            wchar_t rasenhFullPath[] = L"%SystemRoot%\\System32\\rsaenh.dll";
            if (RegSetValueExW(baseKey, valueName, 0, REG_SZ,
                               (const BYTE*)rasenhFullPath,
                               sizeof(rasenhFullPath)) == ERROR_SUCCESS) {
              reset = true;
            }
          }
        }
      }
#endif
      rv = gArchiveReader.VerifySignature();
#ifdef XP_WIN
      if (baseKey) {
        if (reset) {
          RegSetValueExW(baseKey, valueName, 0, REG_SZ,
                         (const BYTE*)rasenh,
                         sizeof(rasenh));
        }
        RegCloseKey(baseKey);
      }
#endif
    }

    if (rv == OK) {
      if (rv == OK) {
        NS_tchar updateSettingsPath[MAX_TEXT_LEN];
        NS_tsnprintf(updateSettingsPath,
                     sizeof(updateSettingsPath) / sizeof(updateSettingsPath[0]),
                     NS_T("%s/update-settings.ini"), gWorkingDirPath);
        MARChannelStringTable MARStrings;
        if (ReadMARChannelIDs(updateSettingsPath, &MARStrings) != OK) {
          // If we can't read from update-settings.ini then we shouldn't impose
          // a MAR restriction.  Some installations won't even include this file.
          MARStrings.MARChannelID[0] = '\0';
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
        }

        rv = gArchiveReader.VerifyProductInformation(MARStrings.MARChannelID,
                                                     MOZ_APP_VERSION);
      }
    }
#endif

    if (rv == OK && sStagedUpdate && !sIsOSUpdate) {
      rv = CopyInstallDirToDestDir();
    }

    if (rv == OK) {
      rv = DoUpdate();
      gArchiveReader.Close();
      NS_tchar updatingDir[MAXPATHLEN];
      NS_tsnprintf(updatingDir, sizeof(updatingDir)/sizeof(updatingDir[0]),
                   NS_T("%s/updating"), gWorkingDirPath);
      ensure_remove_recursive(updatingDir);
    }
  }

  bool reportRealResults = true;
  if (sReplaceRequest && rv && !getenv("MOZ_NO_REPLACE_FALLBACK")) {
    // When attempting to replace the application, we should fall back
    // to non-staged updates in case of a failure.  We do this by
    // setting the status to pending, exiting the updater, and
    // launching the callback application.  The callback application's
    // startup path will see the pending status, and will start the
    // updater application again in order to apply the update without
    // staging.
    // The MOZ_NO_REPLACE_FALLBACK environment variable is used to
    // bypass this fallback, and is used in the updater tests.
    // The only special thing which we should do here is to remove the
    // staged directory as it won't be useful any more.
    ensure_remove_recursive(gWorkingDirPath);
    WriteStatusFile(sUsingService ? "pending-service" : "pending");
    putenv(const_cast<char*>("MOZ_PROCESS_UPDATES=")); // We need to use -process-updates again in the tests
    reportRealResults = false; // pretend success
  }

  if (reportRealResults) {
fclose(para_fp);
    if (rv) {
      LOG(("failed: %d", rv));
    }
    else {
#ifdef XP_MACOSX
      // If the update was successful we need to update the timestamp on the
      // top-level Mac OS X bundle directory so that Mac OS X's Launch Services
      // picks up any major changes when the bundle is updated.
      if (!sStagedUpdate && utimes(gInstallDirPath, nullptr) != 0) {
        LOG(("Couldn't set access/modification time on application bundle."));
      }
#endif

      LOG(("succeeded"));
    }
    WriteStatusFile(rv);
  }

  LOG(("calling QuitProgressUI"));
  QuitProgressUI();
}
