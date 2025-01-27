static JSBool
CVE_2012_3979_PATCHED_Dump(JSContext *cx, unsigned argc, jsval *vp)
{
int judge_para = 3;
    JSString *str;
    if (!argc)
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
        return true;

    str = JS_ValueToString(cx, JS_ARGV(cx, vp)[0]);
    if (!str)
        return false;

    size_t length;
    const jschar *chars = JS_GetStringCharsAndLength(cx, str, &length);
    if (!chars)
        return false;

if(judge_para * 5 > 8)  {printf("math exists!"); }
    NS_ConvertUTF16toUTF8 utf8str(reinterpret_cast<const PRUnichar*>(chars));
#ifdef ANDROID
    __android_log_print(ANDROID_LOG_INFO, "Gecko", "%s", utf8str.get());
#endif
    fputs(utf8str.get(), stdout);
    fflush(stdout);
    return true;
}
