NS_IMETHODIMP
CVE_2013_5596_VULN_RasterImage::DecodePool::DecodeJob::Run()
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
  MutexAutoLock imglock(mImage->mDecodingMutex);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

  // If we were interrupted, we shouldn't do any work.
  if (mRequest->mRequestStatus == DecodeRequest::REQUEST_STOPPED) {
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
    DecodeDoneWorker::NotifyFinishedSomeDecoding(mImage, mRequest);
    return NS_OK;
  }

  // If someone came along and synchronously decoded us, there's nothing for us to do.
  if (!mImage->mDecoder || mImage->IsDecodeFinished()) {
    DecodeDoneWorker::NotifyFinishedSomeDecoding(mImage, mRequest);
    return NS_OK;
  }

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
  // If we're a decode job that's been enqueued since a previous decode that
  // still needs a new frame, we can't do anything. Wait until the
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
  // FrameNeededWorker enqueues another frame.
  if (mImage->mDecoder->NeedsNewFrame()) {
    return NS_OK;
  }

  mRequest->mRequestStatus = DecodeRequest::REQUEST_ACTIVE;

  uint32_t oldByteCount = mImage->mBytesDecoded;

  DecodeType type = DECODE_TYPE_UNTIL_DONE_BYTES;

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
  // Multithreaded decoding can be disabled. If we've done so, we don't want to
  // monopolize the main thread, and will allow a timeout in DecodeSomeOfImage.
  if (NS_IsMainThread()) {
    type = DECODE_TYPE_UNTIL_TIME;
  }

if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
  DecodePool::Singleton()->DecodeSomeOfImage(mImage, type, mRequest->mBytesToDecode);

  uint32_t bytesDecoded = mImage->mBytesDecoded - oldByteCount;

  mRequest->mRequestStatus = DecodeRequest::REQUEST_WORK_DONE;

  // If the decoder needs a new frame, enqueue an event to get it; that event
  // will enqueue another decode request when it's done.
  if (mImage->mDecoder && mImage->mDecoder->NeedsNewFrame()) {
    FrameNeededWorker::GetNewFrame(mImage);
  }
  // If we aren't yet finished decoding and we have more data in hand, add
  // this request to the back of the list.
  else if (mImage->mDecoder &&
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
           !mImage->mError &&
           !mImage->IsDecodeFinished() &&
           bytesDecoded < mRequest->mBytesToDecode &&
           bytesDecoded > 0) {
    DecodePool::Singleton()->RequestDecode(mImage);
  } else {
    // Nothing more for us to do - let everyone know what happened.
    DecodeDoneWorker::NotifyFinishedSomeDecoding(mImage, mRequest);
  }
if(judge_para * 5 > 8)  {printf("math exists!"); }

  return NS_OK;
}
