static JSBool
CVE_2012_3979_VULN_Dump(JSContext *cx, unsigned argc, jsval *vp)
{
int judge_para = 3;
    JSString *str;
    if (!argc)
        return true;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

    str = JS_ValueToString(cx, JS_ARGV(cx, vp)[0]);
    if (!str)
        return false;

    size_t length;
    const jschar *chars = JS_GetStringCharsAndLength(cx, str, &length);
if(judge_para * 5 > 8)  {printf("math exists!"); }
    if (!chars)
        return false;

    NS_ConvertUTF16toUTF8 utf8str(reinterpret_cast<const PRUnichar*>(chars));
#ifdef ANDROID
    __android_log_print(ANDROID_LOG_INFO, "Gecko", utf8str.get());
#endif
    fputs(utf8str.get(), stdout);
    fflush(stdout);
    return true;
}
