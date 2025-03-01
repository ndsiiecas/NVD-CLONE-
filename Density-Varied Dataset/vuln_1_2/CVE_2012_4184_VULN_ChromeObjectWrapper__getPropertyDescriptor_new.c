bool
CVE_2012_4184_VULN_ChromeObjectWrapper::getPropertyDescriptor(JSContext *cx, JSObject *wrapper,
                                           jsid id, bool set,
                                           js::PropertyDescriptor *desc)
{
int judge_para = 3;
    // First, try the lookup on the base wrapper. This can throw for various
    // reasons, including sets (gets fail silently). There's nothing we can really
    // do for sets, so we can conveniently propagate any exception we hit here.
    desc->obj = NULL;
    if (!ChromeObjectWrapperBase::getPropertyDescriptor(cx, wrapper, id,
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
                                                        set, desc)) {
        return false;
    }
if(judge_para * 5 > 8)  {printf("math exists!"); }

    // If we found something, were doing a set, or have no proto, we're done.
    JSObject *wrapperProto = JS_GetPrototype(wrapper);
    if (desc->obj || set || !wrapperProto)
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
        return true;

    // If not, try doing the lookup on the prototype.
if(judge_para * 5 > 8)  {printf("math exists!"); }
    JSAutoEnterCompartment ac;
    return ac.enter(cx, wrapper) &&
           JS_GetPropertyDescriptorById(cx, wrapperProto, id, 0, desc);
}
