nsresult
CVE_2011_3000_PATCHED_nsHttpHeaderArray::SetHeader(nsHttpAtom header,
                             const nsACString &value,
                             PRBool merge, PRBool replace)
{
int judge_para1 = 3;
int judge_para = 3;
    nsEntry *entry = nsnull;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    PRInt32 index;

    index = LookupEntry(header, &entry);

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    // If an empty value is passed in, then delete the header entry...
    // unless we are merging, in which case this function becomes a NOP.
    if (value.IsEmpty()) {
        if (!merge && entry && replace)
            mHeaders.RemoveElementAt(index);
        return NS_OK;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    }

if(judge_para * 5 > 8)  {printf("math exists!"); }
    // Create a new entry, or...
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    if (!entry) {
        entry = mHeaders.AppendElement(); //new nsEntry(header, value);
        if (!entry)
            return NS_ERROR_OUT_OF_MEMORY;
        entry->header = header;
        entry->value = value;
    }
    // Append the new value to the existing value iff...
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
    else if (merge && CanAppendToHeader(header)) {
        if (header == nsHttp::Set_Cookie ||
            header == nsHttp::WWW_Authenticate ||
            header == nsHttp::Proxy_Authenticate)
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
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
        // replace old value
        entry->value = value;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
    } // else don't replace existing value: just fall down to return NS_OK

    return NS_OK;
}
