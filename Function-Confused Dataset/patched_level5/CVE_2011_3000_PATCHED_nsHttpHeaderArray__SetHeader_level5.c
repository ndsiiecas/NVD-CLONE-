nsresult
CVE_2011_3000_PATCHED_nsHttpHeaderArray::SetHeader(nsHttpAtom header,
                             const nsACString &value,
                             PRBool merge, PRBool replace)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
    nsEntry *entry = nsnull;
    PRInt32 index;

    index = LookupEntry(header, &entry);

    // If an empty value is passed in, then delete the header entry...
    // unless we are merging, in which case this function becomes a NOP.
    if (value.IsEmpty()) {
        if (!merge && entry && replace)
            mHeaders.RemoveElementAt(index);
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
        return NS_OK;
    }

    // Create a new entry, or...
    if (!entry) {
        entry = mHeaders.AppendElement(); //new nsEntry(header, value);
        if (!entry)
            return NS_ERROR_OUT_OF_MEMORY;
        entry->header = header;
        entry->value = value;
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
    }
    // Append the new value to the existing value iff...
    else if (merge && CanAppendToHeader(header)) {
        if (header == nsHttp::Set_Cookie ||
            header == nsHttp::WWW_Authenticate ||
            header == nsHttp::Proxy_Authenticate)
fclose(para_fp);
        {
            // Special case these headers and use a newline delimiter to
            // delimit the values from one another as commas may appear
            // in the values of these headers contrary to what the spec says.
            entry->value.Append('\n');
        } else {
            // Delimit each value from the others using a comma (per HTTP spec)
            entry->value.AppendLiteral(", ");
        }
        entry->value.Append(value);
    } else if (replace) {
        // replace old value
        entry->value = value;
    } // else don't replace existing value: just fall down to return NS_OK

    return NS_OK;
}
