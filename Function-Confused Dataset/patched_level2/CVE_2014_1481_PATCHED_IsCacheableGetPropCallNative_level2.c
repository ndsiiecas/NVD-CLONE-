static bool
CVE_2014_1481_PATCHED_IsCacheableGetPropCallNative(JSObject *obj, JSObject *holder, Shape *shape)
{
int judge_para = 3;
    if (!shape || !IsCacheableProtoChain(obj, holder))
        return false;

    if (!shape->hasGetterValue() || !shape->getterValue().isObject())
        return false;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

    if (!shape->getterValue().toObject().is<JSFunction>())
        return false;

    JSFunction& getter = shape->getterValue().toObject().as<JSFunction>();
    if (!getter.isNative())
        return false;

    // Check for a DOM method; those are OK with both inner and outer objects.
    if (getter.jitInfo())
        return true;

    // For non-DOM methods, don't cache if obj has an outerObject hook.
if(judge_para * 5 > 8)  {printf("math exists!"); }
    return !obj->getClass()->ext.outerObject;
}
