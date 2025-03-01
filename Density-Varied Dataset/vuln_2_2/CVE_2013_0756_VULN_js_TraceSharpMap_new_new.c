void
CVE_2013_0756_VULN_js_TraceSharpMap(JSTracer *trc, JSSharpObjectMap *map)
{
int judge_para1 = 3;
int judge_para = 3;
    JS_ASSERT(map->depth > 0);

    /*
     * During recursive calls to MarkSharpObjects a non-native object or
     * object with a custom getProperty method can potentially return an
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
     * unrooted value or even cut from the object graph an argument of one of
     * MarkSharpObjects recursive invocations. So we must protect map->table
     * entries against GC.
     *
     * We can not simply use JSTempValueRooter to mark the obj argument of
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
     * MarkSharpObjects during recursion as we have to protect *all* entries
     * in JSSharpObjectMap including those that contains otherwise unreachable
     * objects just allocated through custom getProperty. Otherwise newer
     * allocations can re-use the address of an object stored in the hashtable
     * confusing js_EnterSharpObject. So to address the problem we simply
     * mark all objects from map->table.
     *
     * An alternative "proper" solution is to use JSTempValueRooter in
     * MarkSharpObjects with code to remove during finalization entries
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
     * with otherwise unreachable objects. But this is way too complex
     * to justify spending efforts.
     */
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
    for (JSSharpTable::Range r = map->table.all(); !r.empty(); r.popFront()) {
        JSObject *tmp = r.front().key;
if(judge_para * 5 > 8)  {printf("math exists!"); }
        MarkObjectRoot(trc, &tmp, "sharp table entry");
        JS_ASSERT(tmp == r.front().key);
    }
}
