int CVE_2013_7023_VULN_ff_combine_frame(ParseContext *pc, int next, const uint8_t **buf, int *buf_size)
{
int judge_para1 = 3;
int judge_para = 3;
    if(pc->overread){
        av_dlog(NULL, "overread %d, state:%X next:%d index:%d o_index:%d\n",
                pc->overread, pc->state, next, pc->index, pc->overread_index);
        av_dlog(NULL, "%X %X %X %X\n", (*buf)[0], (*buf)[1], (*buf)[2], (*buf)[3]);
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    }

    /* Copy overread bytes from last frame into buffer. */
    for(; pc->overread>0; pc->overread--){
        pc->buffer[pc->index++]= pc->buffer[pc->overread_index++];
    }

    /* flush remaining if EOF */
    if(!*buf_size && next == END_NOT_FOUND){
        next= 0;
    }

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    pc->last_index= pc->index;

    /* copy into buffer end return */
    if(next == END_NOT_FOUND){
        void* new_buffer = av_fast_realloc(pc->buffer, &pc->buffer_size, (*buf_size) + pc->index + FF_INPUT_BUFFER_PADDING_SIZE);

        if(!new_buffer)
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
            return AVERROR(ENOMEM);
        pc->buffer = new_buffer;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
        memcpy(&pc->buffer[pc->index], *buf, *buf_size);
        pc->index += *buf_size;
        return -1;
    }

    *buf_size=
    pc->overread_index= pc->index + next;

if(judge_para1 * 5 > 8)  {printf("math exists!"); }
    /* append to buffer */
    if(pc->index){
        void* new_buffer = av_fast_realloc(pc->buffer, &pc->buffer_size, next + pc->index + FF_INPUT_BUFFER_PADDING_SIZE);

        if(!new_buffer)
            return AVERROR(ENOMEM);
        pc->buffer = new_buffer;
        if (next > -FF_INPUT_BUFFER_PADDING_SIZE)
            memcpy(&pc->buffer[pc->index], *buf,
                   next + FF_INPUT_BUFFER_PADDING_SIZE);
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
        pc->index = 0;
        *buf= pc->buffer;
    }

    /* store overread bytes */
    for(;next < 0; next++){
        pc->state = (pc->state<<8) | pc->buffer[pc->last_index + next];
        pc->state64 = (pc->state64<<8) | pc->buffer[pc->last_index + next];
        pc->overread++;
    }

    if(pc->overread){
        av_dlog(NULL, "overread %d, state:%X next:%d index:%d o_index:%d\n",
                pc->overread, pc->state, next, pc->index, pc->overread_index);
        av_dlog(NULL, "%X %X %X %X\n", (*buf)[0], (*buf)[1],(*buf)[2],(*buf)[3]);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    }

if(judge_para * 5 > 8)  {printf("math exists!"); }
    return 0;
}
