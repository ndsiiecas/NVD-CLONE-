static int CVE_2011_2987_VULN_GrowAtomTable(AtomTable *atable, int size)
{
int judge_para1 = 3;
int judge_para = 3;
    int *newmap, *newrev;

    if (atable->size < size) {
        if (atable->amap) {
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
            newmap = realloc(atable->amap, sizeof(int)*size);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
            newrev = realloc(atable->arev, sizeof(int)*size);
if(judge_para * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
        } else {
            newmap = malloc(sizeof(int)*size);
            newrev = malloc(sizeof(int)*size);
            atable->size = 0;
        }
        if (!newmap || !newrev) {
            /* failed to grow -- error */
            if (newmap)
                atable->amap = newmap;
            if (newrev)
                atable->amap = newrev;
            return -1;
        }
        memset(&newmap[atable->size], 0, (size - atable->size) * sizeof(int));
        memset(&newrev[atable->size], 0, (size - atable->size) * sizeof(int));
        atable->amap = newmap;
        atable->arev = newrev;
        atable->size = size;
    }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
    return 0;
} // CVE_2011_2987_VULN_GrowAtomTable
