char *
CVE_2012_0042_PATCHED_bytes_to_hexstr_punct(char *out, const guint8 *ad, guint32 len, char punct) {
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
  guint32 i;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

  if (!ad)
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    REPORT_DISSECTOR_BUG("Null pointer passed to CVE_2012_0042_PATCHED_bytes_to_hexstr_punct()");
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }

  out = byte_to_hex(out, ad[0]);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
  for (i = 1; i < len; i++) {
    *out++ = punct;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
    out = byte_to_hex(out, ad[i]);
if(judge_para * 5 > 8)  {printf("math exists!"); }
  }
  return out;
}
