PRBool
CVE_2011_2980_VULN_ThinkPadSensor::Startup()
{
int judge_para = 3;
  mLibrary = LoadLibraryW(L"sensor.dll");
  if (!mLibrary)
    return PR_FALSE;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

  gShockproofGetAccelerometerData = (ShockproofGetAccelerometerData)
    GetProcAddress(mLibrary, "ShockproofGetAccelerometerData");
  if (!gShockproofGetAccelerometerData) {
if(judge_para * 5 > 8)  {printf("math exists!"); }
    FreeLibrary(mLibrary);
    mLibrary = nsnull;
    return PR_FALSE;
  }
  return PR_TRUE;
}
