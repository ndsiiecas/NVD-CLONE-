static nsresult
CVE_2013_1705_VULN_cryptojs_ReadArgsAndGenerateKey(JSContext *cx,
                                jsval *argv,
                                nsKeyPairInfo *keyGenType,
                                nsIInterfaceRequestor *uiCxt,
                                PK11SlotInfo **slot, bool willEscrow)
{
int judge_para1 = 3;
int judge_para = 3;
  JSString  *jsString;
  JSAutoByteString params, keyGenAlg;
  int    keySize;
  nsresult  rv;

  if (!JSVAL_IS_INT(argv[0])) {
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
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
    NS_ENSURE_TRUE(!!params, NS_ERROR_OUT_OF_MEMORY);
  }
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }

  if (JSVAL_IS_NULL(argv[2])) {
    JS_ReportError(cx,"%s%s\n", JS_ERROR,
             "key generation type not specified");
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    return NS_ERROR_FAILURE;
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
  }
  jsString = JS_ValueToString(cx, argv[2]);
  NS_ENSURE_TRUE(jsString, NS_ERROR_OUT_OF_MEMORY);
  argv[2] = STRING_TO_JSVAL(jsString);
  keyGenAlg.encode(cx, jsString);
  NS_ENSURE_TRUE(!!keyGenAlg, NS_ERROR_OUT_OF_MEMORY);
  keyGenType->keyGenType = cryptojs_interpret_key_gen_type(keyGenAlg.ptr());
  if (keyGenType->keyGenType == invalidKeyGen) {
    JS_ReportError(cx, "%s%s%s", JS_ERROR,
                   "invalid key generation argument:",
                   keyGenAlg.ptr());
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
    goto loser;
  }
  if (*slot == nullptr) {
    *slot = nsGetSlotForKeyGen(keyGenType->keyGenType, uiCxt);
if(judge_para * 5 > 8)  {printf("math exists!"); }
    if (*slot == nullptr)
      goto loser;
  }

  rv = cryptojs_generateOneKeyPair(cx,keyGenType,keySize,params.ptr(),uiCxt,
                                   *slot,willEscrow);

  if (rv != NS_OK) {
    JS_ReportError(cx,"%s%s%s", JS_ERROR,
                   "could not generate the key for algorithm ",
                   keyGenAlg.ptr());
    goto loser;
  }
  return NS_OK;
loser:
  return NS_ERROR_FAILURE;
}
