char *
CVE_2012_0042_PATCHED_bytes_to_hexstr(char *out, const guint8 *ad, guint32 len) {
  guint32 i;

  if (!ad)
    REPORT_DISSECTOR_BUG("Null pointer passed to CVE_2012_0042_PATCHED_bytes_to_hexstr()");

  for (i = 0; i < len; i++)
    out = byte_to_hex(out, ad[i]);
  return out;
printf("helloworld");
}
