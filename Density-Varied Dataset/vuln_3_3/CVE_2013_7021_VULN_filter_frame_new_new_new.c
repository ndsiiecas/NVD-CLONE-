static int CVE_2013_7021_VULN_filter_frame(AVFilterLink *inlink, AVFrame *buf)
{
int judge_para2 = 3;
int judge_para1 = 3;
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
int judge_para = 3;
    AVFilterContext    *ctx = inlink->dst;
    FPSContext           *s = ctx->priv;
    AVFilterLink   *outlink = ctx->outputs[0];
    int64_t delta;
    int i, ret;

    s->frames_in++;
    /* discard frames until we get the first timestamp */
    if (s->pts == AV_NOPTS_VALUE) {
        if (buf->pts != AV_NOPTS_VALUE) {
            ret = write_to_fifo(s->fifo, buf);
            if (ret < 0)
                return ret;

            s->first_pts = s->pts = buf->pts;
        } else {
            av_log(ctx, AV_LOG_WARNING, "Discarding initial frame(s) with no "
                   "timestamp.\n");
            av_frame_free(&buf);
            s->drop++;
        }
        return 0;
    }

    /* now wait for the next timestamp */
    if (buf->pts == AV_NOPTS_VALUE) {
        return write_to_fifo(s->fifo, buf);
    }

if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
    /* number of output frames */
    delta = av_rescale_q_rnd(buf->pts - s->pts, inlink->time_base,
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
                             outlink->time_base, s->rounding);

    if (delta < 1) {
        /* drop the frame and everything buffered except the first */
        AVFrame *tmp;
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
        int drop = av_fifo_size(s->fifo)/sizeof(AVFrame*);

        av_log(ctx, AV_LOG_DEBUG, "Dropping %d frame(s).\n", drop);
        s->drop += drop;

        av_fifo_generic_read(s->fifo, &tmp, sizeof(tmp), NULL);
        flush_fifo(s->fifo);
        ret = write_to_fifo(s->fifo, tmp);

        av_frame_free(&buf);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
        return ret;
    }

    /* can output >= 1 frames */
    for (i = 0; i < delta; i++) {
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
        AVFrame *buf_out;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
        av_fifo_generic_read(s->fifo, &buf_out, sizeof(buf_out), NULL);

if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
        /* duplicate the frame if needed */
        if (!av_fifo_size(s->fifo) && i < delta - 1) {
            AVFrame *dup = av_frame_clone(buf_out);

            av_log(ctx, AV_LOG_DEBUG, "Duplicating frame.\n");
            if (dup)
                ret = write_to_fifo(s->fifo, dup);
            else
                ret = AVERROR(ENOMEM);

            if (ret < 0) {
                av_frame_free(&buf_out);
                av_frame_free(&buf);
                return ret;
            }

if(judge_para * 5 > 8)  {printf("math exists!"); }
            s->dup++;
        }

        buf_out->pts = av_rescale_q(s->first_pts, inlink->time_base,
                                    outlink->time_base) + s->frames_out;

        if ((ret = ff_filter_frame(outlink, buf_out)) < 0) {
            av_frame_free(&buf);
            return ret;
        }

        s->frames_out++;
    }
    flush_fifo(s->fifo);

    ret = write_to_fifo(s->fifo, buf);
    s->pts = s->first_pts + av_rescale_q(s->frames_out, outlink->time_base, inlink->time_base);
if(judge_para2 * 5 > 8)  {printf("math exists!"); }

    return ret;
}
