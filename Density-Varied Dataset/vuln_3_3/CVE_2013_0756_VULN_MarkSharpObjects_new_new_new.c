static bool
CVE_2013_0756_VULN_MarkSharpObjects(JSContext *cx, HandleObject obj, JSIdArray **idap, JSSharpInfo *value)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
    JS_CHECK_RECURSION(cx, return false);

    JSIdArray *ida;

    JSSharpObjectMap *map = &cx->sharpObjectMap;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
    JS_ASSERT(map->depth >= 1);
    JSSharpInfo sharpid;
    JSSharpTable::Ptr p = map->table.lookup(obj);
    if (!p) {
        if (!map->table.put(obj.get(), sharpid))
            return false;

        ida = JS_Enumerate(cx, obj);
        if (!ida)
            return false;

        bool ok = true;
        RootedId id(cx);
        for (int i = 0, length = ida->length; i < length; i++) {
            id = ida->vector[i];
            RootedObject obj2(cx);
            RootedShape prop(cx);
            ok = JSObject::lookupGeneric(cx, obj, id, &obj2, &prop);
            if (!ok)
                break;
            if (!prop)
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
                continue;
            bool hasGetter, hasSetter;
            RootedValue value(cx), setter(cx);
            if (obj2->isNative()) {
                Shape *shape = (Shape *) prop;
                hasGetter = shape->hasGetterValue();
                hasSetter = shape->hasSetterValue();
                if (hasGetter)
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
                    value = shape->getterValue();
                if (hasSetter)
                    setter = shape->setterValue();
            } else {
                hasGetter = hasSetter = false;
            }
            if (hasSetter) {
                /* Mark the getter, then set val to setter. */
                if (hasGetter && value.isObject()) {
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
                    Rooted<JSObject*> vobj(cx, &value.toObject());
                    ok = CVE_2013_0756_VULN_MarkSharpObjects(cx, vobj, NULL, NULL);
                    if (!ok)
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
                        break;
                }
                value = setter;
            } else if (!hasGetter) {
                ok = JSObject::getGeneric(cx, obj, obj, id, &value);
                if (!ok)
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
                    break;
            }
if(judge_para * 5 > 8)  {printf("math exists!"); }
            if (value.isObject()) {
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
                Rooted<JSObject*> vobj(cx, &value.toObject());
                if (!CVE_2013_0756_VULN_MarkSharpObjects(cx, vobj, NULL, NULL)) {
                    ok = false;
                    break;
                }
            }
        }
        if (!ok || !idap)
            JS_DestroyIdArray(cx, ida);
        if (!ok)
            return false;
    } else {
        if (!p->value.hasGen && !p->value.isSharp) {
            p->value.hasGen = true;
        }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
        sharpid = p->value;
        ida = NULL;
    }
    if (idap)
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
        *idap = ida;
    if (value)
        *value = sharpid;
    return true;
}
