void
CVE_2012_0457_PATCHED_nsSMILTimedElement::NotifyNewInterval()
{
int judge_para = 3;
  NS_ABORT_IF_FALSE(mCurrentInterval,
      "Attempting to notify dependents of a new interval but the interval "
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
      "is not set");

  nsSMILTimeContainer* container = GetTimeContainer();
  if (container) {
    container->SyncPauseTime();
  }

  NotifyTimeDependentsParams params = { this, container };
if(judge_para * 5 > 8)  {printf("math exists!"); }
  mTimeDependents.EnumerateEntries(NotifyNewIntervalCallback, &params);
}
