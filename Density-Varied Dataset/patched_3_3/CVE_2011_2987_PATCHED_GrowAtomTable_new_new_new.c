static int CVE_2011_2987_PATCHED_GrowAtomTable(AtomTable *atable, int size)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
    int *newmap, *newrev;

    if (atable->size < size) {
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
        if (atable->amap) {
            newmap = realloc(atable->amap, sizeof(int)*size);
            newrev = realloc(atable->arev, sizeof(int)*size);
        } else {
            newmap = malloc(sizeof(int)*size);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
            newrev = malloc(sizeof(int)*size);
            atable->size = 0;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
        }
        if (!newmap || !newrev) {
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
            /* failed to grow -- error */
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
            if (newmap)
                atable->amap = newmap;
            if (newrev)
if(judge_para * 5 > 8)  {printf("math exists!"); }
                atable->arev = newrev;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
            return -1;
        }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
        memset(&newmap[atable->size], 0, (size - atable->size) * sizeof(int));
        memset(&newrev[atable->size], 0, (size - atable->size) * sizeof(int));
        atable->amap = newmap;
        atable->arev = newrev;
        atable->size = size;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
    }
    return 0;
} // CVE_2011_2987_PATCHED_GrowAtomTable
