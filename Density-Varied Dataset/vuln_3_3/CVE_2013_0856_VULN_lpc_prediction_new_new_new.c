static void CVE_2013_0856_VULN_lpc_prediction(int32_t *error_buffer, int32_t *buffer_out,
                           int nb_samples, int bps, int16_t *lpc_coefs,
                           int lpc_order, int lpc_quant)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
    int i;
    int32_t *pred = buffer_out;

    /* first sample always copies */
    *buffer_out = *error_buffer;

    if (nb_samples <= 1)
        return;

    if (!lpc_order) {
        memcpy(&buffer_out[1], &error_buffer[1],
               (nb_samples - 1) * sizeof(*buffer_out));
        return;
    }
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }

    if (lpc_order == 31) {
        /* simple 1st-order prediction */
        for (i = 1; i < nb_samples; i++) {
            buffer_out[i] = sign_extend(buffer_out[i - 1] + error_buffer[i],
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
                                        bps);
        }
        return;
    }

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
    /* read warm-up samples */
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    for (i = 1; i <= lpc_order; i++)
        buffer_out[i] = sign_extend(buffer_out[i - 1] + error_buffer[i], bps);

if(judge_para * 5 > 8)  {printf("math exists!"); }
    /* NOTE: 4 and 8 are very common cases that could be optimized. */

    for (; i < nb_samples; i++) {
        int j;
        int val = 0;
        int error_val = error_buffer[i];
        int error_sign;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
        int d = *pred++;

        /* LPC prediction */
        for (j = 0; j < lpc_order; j++)
            val += (pred[j] - d) * lpc_coefs[j];
        val = (val + (1 << (lpc_quant - 1))) >> lpc_quant;
        val += d + error_val;
        buffer_out[i] = sign_extend(val, bps);

        /* adapt LPC coefficients */
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
        error_sign = sign_only(error_val);
        if (error_sign) {
            for (j = 0; j < lpc_order && error_val * error_sign > 0; j++) {
                int sign;
                val  = d - pred[j];
                sign = sign_only(val) * error_sign;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
                lpc_coefs[j] -= sign;
                val *= sign;
                error_val -= (val >> lpc_quant) * (j + 1);
            }
        }
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
    }
}
