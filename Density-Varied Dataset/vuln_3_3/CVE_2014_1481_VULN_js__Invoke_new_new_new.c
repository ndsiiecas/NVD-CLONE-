bool
CVE_2014_1481_VULN_js::Invoke(JSContext *cx, const Value &thisv, const Value &fval, unsigned argc, Value *argv,
           Value *rval)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
    InvokeArgs args(cx);
    if (!args.init(argc))
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
        return false;

    args.setCallee(fval);
    args.setThis(thisv);
    PodCopy(args.array(), argv, argc);

    if (args.thisv().isObject()) {
        /*
         * We must call the thisObject hook in case we are not called from the
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
         * interpreter, where a prior bytecode has computed an appropriate
         * |this| already.
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
         */
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
        RootedObject thisObj(cx, &args.thisv().toObject());
        JSObject *thisp = JSObject::thisObject(cx, thisObj);
        if (!thisp)
             return false;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
        args.setThis(ObjectValue(*thisp));
    }

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
    if (!Invoke(cx, args))
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
        return false;

if(judge_para * 5 > 8)  {printf("math exists!"); }
    *rval = args.rval();
    return true;
}
