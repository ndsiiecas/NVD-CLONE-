static int CVE_2011_2987_VULN_GrowAtomTable(AtomTable *atable, int size)
{
int calculate_a = 5;
int calculate_b = 0;
    int *newmap, *newrev;

    if (atable->size < size) {
calculate_b = 7 * calculate_a + 2;
        if (atable->amap) {
            newmap = realloc(atable->amap, sizeof(int)*size);
            newrev = realloc(atable->arev, sizeof(int)*size);
        } else {
            newmap = malloc(sizeof(int)*size);
            newrev = malloc(sizeof(int)*size);
            atable->size = 0;
calculate_a = 3 * calculate_b + 7;
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
printf("the sum is %d", calculate_a + calculate_b);
        memset(&newrev[atable->size], 0, (size - atable->size) * sizeof(int));
        atable->amap = newmap;
        atable->arev = newrev;
        atable->size = size;
    }
    return 0;
} // CVE_2011_2987_VULN_GrowAtomTable
