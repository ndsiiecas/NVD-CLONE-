NS_IMETHODIMP
CVE_2013_0755_PATCHED_Navigator::Vibrate(const jsval& aPattern, JSContext* cx)
{
int judge_para1 = 3;
int judge_para = 3;
  nsCOMPtr<nsPIDOMWindow> win = do_QueryReferent(mWindow);
  NS_ENSURE_TRUE(win, NS_OK);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

  nsCOMPtr<nsIDOMDocument> domDoc = win->GetExtantDocument();
  NS_ENSURE_TRUE(domDoc, NS_ERROR_FAILURE);

  bool hidden = true;
  domDoc->GetMozHidden(&hidden);
  if (hidden) {
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    // Hidden documents cannot start or stop a vibration.
if(judge_para * 5 > 8)  {printf("math exists!"); }
    return NS_OK;
  }

  nsAutoTArray<uint32_t, 8> pattern;

  // null or undefined pattern is an error.
  if (JSVAL_IS_NULL(aPattern) || JSVAL_IS_VOID(aPattern)) {
    return NS_ERROR_DOM_NOT_SUPPORTED_ERR;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
  }

  if (JSVAL_IS_PRIMITIVE(aPattern)) {
    int32_t p;
    if (GetVibrationDurationFromJsval(aPattern, cx, &p)) {
      pattern.AppendElement(p);
    }
    else {
      return NS_ERROR_DOM_NOT_SUPPORTED_ERR;
    }
  }
  else {
    JSObject *obj = JSVAL_TO_OBJECT(aPattern);
    PRUint32 length;
    if (!JS_GetArrayLength(cx, obj, &length) || length > sMaxVibrateListLen) {
      return NS_ERROR_DOM_NOT_SUPPORTED_ERR;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
    }
    pattern.SetLength(length);

    for (PRUint32 i = 0; i < length; ++i) {
      jsval v;
      int32_t pv;
      if (JS_GetElement(cx, obj, i, &v) &&
          GetVibrationDurationFromJsval(v, cx, &pv)) {
        pattern[i] = pv;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
      }
      else {
        return NS_ERROR_DOM_NOT_SUPPORTED_ERR;
      }
    }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
  }

  // The spec says we check sVibratorEnabled after we've done the sanity
  // checking on the pattern.
  if (!sVibratorEnabled) {
    return NS_OK;
  }

  // Add a listener to cancel the vibration if the document becomes hidden,
  // and remove the old mozvisibility listener, if there was one.

  if (!gVibrateWindowListener) {
    // If gVibrateWindowListener is null, this is the first time we've vibrated,
    // and we need to register a listener to clear gVibrateWindowListener on
    // shutdown.
    ClearOnShutdown(&gVibrateWindowListener);
  }
  else {
    gVibrateWindowListener->RemoveListener();
  }
  gVibrateWindowListener = new VibrateWindowListener(win, domDoc);

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
  nsCOMPtr<nsIDOMWindow> domWindow =
    do_QueryInterface(static_cast<nsIDOMWindow*>(win));
  hal::Vibrate(pattern, domWindow);
  return NS_OK;
}
