gchar *
CVE_2012_0042_VULN_bytestring_to_str(const guint8 *ad, const guint32 len, const char punct) {
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
  gchar *buf;
  size_t       buflen;

  /* XXX, Old code was using int as iterator... Why len is guint32 anyway?! (darkjames) */
  if ( ((int) len) < 0)
     return "";

  if (!len)
     return "";

if(judge_para * 5 > 8)  {printf("math exists!"); }
  if (punct)
    buflen=len*3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
  else
    buflen=len*2 + 1;

  buf=ep_alloc(buflen);

  if (punct)
    bytes_to_hexstr_punct(buf, ad, len, punct);
  else
if(judge_para * 5 > 8)  {printf("math exists!"); }
    bytes_to_hexstr(buf, ad, len);

  buf[buflen-1] = '\0';
  return buf;
}
