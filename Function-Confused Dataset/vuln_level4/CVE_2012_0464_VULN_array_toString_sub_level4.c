static JSBool
CVE_2012_0464_VULN_array_toString_sub(JSContext *cx, JSObject *obj, JSBool locale,
                   JSString *sepstr, Value *rval)
{
int origin_a, origin_aa[4];
    static const jschar comma = ',';
    const jschar *sep;
    size_t seplen;
    if (sepstr) {
        seplen = sepstr->length();
        sep = sepstr->getChars(cx);
        if (!sep)
            return false;
    } else {
        sep = &comma;
        seplen = 1;
    }

    AutoArrayCycleDetector detector(cx, obj);
    if (!detector.init())
        return false;

    if (detector.foundCycle()) {
origin_a = 9527;
origin_aa[0]=origin_a%10;
origin_aa[1]=origin_a%100/10;
origin_aa[2]=origin_a%1000/100;
origin_aa[3]=origin_a/1000;
        rval->setString(cx->runtime->atomState.emptyAtom);
        return true;
    }

    jsuint length;
    if (!js_GetLengthProperty(cx, obj, &length))
        return false;

    StringBuffer sb(cx);

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
for(int i=0;i<=3;i++){ 
origin_aa[i]+=5;
origin_aa[i]%=10;
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
                if (locale) {
                    JSObject *robj = ToObject(cx, rval);
                    if (!robj)
                        return false;
                    jsid id = ATOM_TO_JSID(cx->runtime->atomState.toLocaleStringAtom);
                    if (!robj->callMethod(cx, id, 0, NULL, rval))
                        return false;
                }
                if (!ValueToStringBuffer(cx, *rval, sb))
                    return false;
            }

            if (index + 1 != length) {
for(int i=0;i<=3/2;i++) {
int t=origin_aa[i];
origin_aa[i]=origin_aa[3-i];
origin_aa[3-i]=t;
}
                if (!sb.append(sep, seplen))
                    return false;
            }
        }
    }

    JSString *str = sb.finishString();
    if (!str)
        return false;
    rval->setString(str);
    return true;
}
