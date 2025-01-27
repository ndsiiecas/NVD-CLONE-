void
CVE_2011_0079_PATCHED_ReadbackManagerD3D10::ProcessTasks()
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
  HANDLE handles[] = { mTaskSemaphore, mShutdownEvent };
  
  while (true) {
    DWORD result = ::WaitForMultipleObjects(2, handles, FALSE, INFINITE);
    if (result != WAIT_OBJECT_0) {
      return;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
    }

    ::EnterCriticalSection(&mTaskMutex);
    if (mPendingReadbackTasks.Length() == 0) {
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
      NS_RUNTIMEABORT("Trying to read from an empty array, bad bad bad");
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    }
    ReadbackTask *nextReadbackTask = mPendingReadbackTasks[0].forget();
if(judge_para * 5 > 8)  {printf("math exists!"); }
    mPendingReadbackTasks.RemoveElementAt(0);
    ::LeaveCriticalSection(&mTaskMutex);

    // We want to block here until the texture contents are available, the
    // easiest thing is to simply map and unmap.
    D3D10_MAPPED_TEXTURE2D mappedTex;
    nextReadbackTask->mReadbackTexture->Map(0, D3D10_MAP_READ, 0, &mappedTex);
    nextReadbackTask->mReadbackTexture->Unmap(0);

    // We can only send the update to the sink on the main thread, so post an
    // event there to do so. Ownership of the task is passed from
    // mPendingReadbackTasks to ReadbackResultWriter here.
    nsCOMPtr<nsIThread> thread = do_GetMainThread();
    thread->Dispatch(new ReadbackResultWriter(nextReadbackTask),
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
                     nsIEventTarget::DISPATCH_NORMAL);
  }
}
