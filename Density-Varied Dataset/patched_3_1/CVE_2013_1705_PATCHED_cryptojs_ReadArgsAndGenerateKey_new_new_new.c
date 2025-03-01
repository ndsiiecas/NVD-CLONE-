static nsresult
CVE_2013_1705_PATCHED_cryptojs_ReadArgsAndGenerateKey(JSContext *cx,
                                jsval *argv,
                                nsKeyPairInfo *keyGenType,
                                nsIInterfaceRequestor *uiCxt,
                                PK11SlotInfo **slot, bool willEscrow)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
  JSString  *jsString;
  JSAutoByteString params;
  int    keySize;
  nsresult  rv;

  if (!JSVAL_IS_INT(argv[0])) {
    JS_ReportError(cx, "%s%s\n", JS_ERROR,
                   "passed in non-integer for key size");
    return NS_ERROR_FAILURE;
  }
  keySize = JSVAL_TO_INT(argv[0]);
  if (!JSVAL_IS_NULL(argv[1])) {
    jsString = JS_ValueToString(cx,argv[1]);
    NS_ENSURE_TRUE(jsString, NS_ERROR_OUT_OF_MEMORY);
    argv[1] = STRING_TO_JSVAL(jsString);
    params.encode(cx, jsString);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    NS_ENSURE_TRUE(!!params, NS_ERROR_OUT_OF_MEMORY);
  }

if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
  if (JSVAL_IS_NULL(argv[2])) {
    JS_ReportError(cx,"%s%s\n", JS_ERROR,
             "key generation type not specified");
    return NS_ERROR_FAILURE;
  }
  jsString = JS_ValueToString(cx, argv[2]);
  NS_ENSURE_TRUE(jsString, NS_ERROR_OUT_OF_MEMORY);
  argv[2] = STRING_TO_JSVAL(jsString);
  nsDependentJSString dependentKeyGenAlg;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
  NS_ENSURE_TRUE(dependentKeyGenAlg.init(cx, jsString), NS_ERROR_UNEXPECTED);
  nsAutoString keyGenAlg(dependentKeyGenAlg);
  keyGenAlg.Trim("\r\n\t ");
  keyGenType->keyGenType = cryptojs_interpret_key_gen_type(keyGenAlg);
  if (keyGenType->keyGenType == invalidKeyGen) {
    NS_LossyConvertUTF16toASCII keyGenAlgNarrow(dependentKeyGenAlg);
    JS_ReportError(cx, "%s%s%s", JS_ERROR,
                   "invalid key generation argument:",
                   keyGenAlgNarrow.get());
    goto loser;
  }
  if (*slot == nullptr) {
    *slot = nsGetSlotForKeyGen(keyGenType->keyGenType, uiCxt);
    if (*slot == nullptr)
      goto loser;
  }
if(judge_para * 5 > 8)  {printf("math exists!"); }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }

  rv = cryptojs_generateOneKeyPair(cx,keyGenType,keySize,params.ptr(),uiCxt,
                                   *slot,willEscrow);

  if (rv != NS_OK) {
    NS_LossyConvertUTF16toASCII keyGenAlgNarrow(dependentKeyGenAlg);
    JS_ReportError(cx,"%s%s%s", JS_ERROR,
                   "could not generate the key for algorithm ",
                   keyGenAlgNarrow.get());
    goto loser;
  }
  return NS_OK;
loser:
  return NS_ERROR_FAILURE;
}
