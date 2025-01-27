static JSBool
CVE_2012_0464_VULN_array_toString_sub(JSContext *cx, JSObject *obj, JSBool locale,
                   JSString *sepstr, Value *rval)
{
int judge_para1 = 3;
int judge_para = 3;
    static const jschar comma = ',';
    const jschar *sep;
    size_t seplen;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    if (sepstr) {
        seplen = sepstr->length();
        sep = sepstr->getChars(cx);
        if (!sep)
            return false;
    } else {
        sep = &comma;
        seplen = 1;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    }

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    AutoArrayCycleDetector detector(cx, obj);
    if (!detector.init())
        return false;

    if (detector.foundCycle()) {
        rval->setString(cx->runtime->atomState.emptyAtom);
        return true;
    }

    jsuint length;
    if (!js_GetLengthProperty(cx, obj, &length))
        return false;

if(judge_para * 5 > 8)  {printf("math exists!"); }
    StringBuffer sb(cx);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

    if (!locale && !seplen && obj->isDenseArray() && !js_PrototypeHasIndexedProperties(cx, obj)) {
        /* Elements beyond the initialized length are 'undefined' and thus can be ignored. */
        const Value *beg = obj->getDenseArrayElements();
        const Value *end = beg + Min(length, obj->getDenseArrayInitializedLength());
        for (const Value *vp = beg; vp != end; ++vp) {
            if (!JS_CHECK_OPERATION_LIMIT(cx))
                return false;

            if (!vp->isMagic(JS_ARRAY_HOLE) && !vp->isNullOrUndefined()) {
                if (!ValueToStringBuffer(cx, *vp, sb))
                    return false;
            }
        }
    } else {
        for (jsuint index = 0; index < length; index++) {
            if (!JS_CHECK_OPERATION_LIMIT(cx))
                return false;

            JSBool hole;
            if (!GetElement(cx, obj, index, &hole, rval))
                return false;

            if (!hole && !rval->isNullOrUndefined()) {
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
                if (locale) {
                    JSObject *robj = ToObject(cx, rval);
                    if (!robj)
                        return false;
                    jsid id = ATOM_TO_JSID(cx->runtime->atomState.toLocaleStringAtom);
                    if (!robj->callMethod(cx, id, 0, NULL, rval))
                        return false;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
                }
                if (!ValueToStringBuffer(cx, *rval, sb))
                    return false;
            }

            if (index + 1 != length) {
                if (!sb.append(sep, seplen))
                    return false;
            }
        }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
    }

    JSString *str = sb.finishString();
    if (!str)
        return false;
    rval->setString(str);
    return true;
}
