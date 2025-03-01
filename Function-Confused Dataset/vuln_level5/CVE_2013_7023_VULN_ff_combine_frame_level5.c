int CVE_2013_7023_VULN_ff_combine_frame(ParseContext *pc, int next, const uint8_t **buf, int *buf_size)
{
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
    if(pc->overread){
        av_dlog(NULL, "overread %d, state:%X next:%d index:%d o_index:%d\n",
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}
                pc->overread, pc->state, next, pc->index, pc->overread_index);
        av_dlog(NULL, "%X %X %X %X\n", (*buf)[0], (*buf)[1], (*buf)[2], (*buf)[3]);
    }

    /* Copy overread bytes from last frame into buffer. */
    for(; pc->overread>0; pc->overread--){
        pc->buffer[pc->index++]= pc->buffer[pc->overread_index++];
    }

    /* flush remaining if EOF */
    if(!*buf_size && next == END_NOT_FOUND){
        next= 0;
    }

    pc->last_index= pc->index;

    /* copy into buffer end return */
    if(next == END_NOT_FOUND){
        void* new_buffer = av_fast_realloc(pc->buffer, &pc->buffer_size, (*buf_size) + pc->index + FF_INPUT_BUFFER_PADDING_SIZE);

        if(!new_buffer)
            return AVERROR(ENOMEM);
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
        pc->buffer = new_buffer;
        memcpy(&pc->buffer[pc->index], *buf, *buf_size);
        pc->index += *buf_size;
        return -1;
fclose(para_fp);
    }

    *buf_size=
    pc->overread_index= pc->index + next;

    /* append to buffer */
    if(pc->index){
        void* new_buffer = av_fast_realloc(pc->buffer, &pc->buffer_size, next + pc->index + FF_INPUT_BUFFER_PADDING_SIZE);

        if(!new_buffer)
            return AVERROR(ENOMEM);
        pc->buffer = new_buffer;
        if (next > -FF_INPUT_BUFFER_PADDING_SIZE)
            memcpy(&pc->buffer[pc->index], *buf,
                   next + FF_INPUT_BUFFER_PADDING_SIZE);
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
    }

    return 0;
}
