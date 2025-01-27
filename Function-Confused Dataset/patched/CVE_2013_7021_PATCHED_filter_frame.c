static int CVE_2013_7021_PATCHED_filter_frame(AVFilterLink *inlink, AVFrame *buf)
{
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
    if (buf->pts == AV_NOPTS_VALUE || av_fifo_size(s->fifo) <= 0) {
        return write_to_fifo(s->fifo, buf);
    }

    /* number of output frames */
    delta = av_rescale_q_rnd(buf->pts - s->pts, inlink->time_base,
                             outlink->time_base, s->rounding);

    if (delta < 1) {
        /* drop the frame and everything buffered except the first */
        AVFrame *tmp;
        int drop = av_fifo_size(s->fifo)/sizeof(AVFrame*);

        av_log(ctx, AV_LOG_DEBUG, "Dropping %d frame(s).\n", drop);
        s->drop += drop;

        av_fifo_generic_read(s->fifo, &tmp, sizeof(tmp), NULL);
        flush_fifo(s->fifo);
        ret = write_to_fifo(s->fifo, tmp);

        av_frame_free(&buf);
        return ret;
    }

    /* can output >= 1 frames */
    for (i = 0; i < delta; i++) {
        AVFrame *buf_out;
        av_fifo_generic_read(s->fifo, &buf_out, sizeof(buf_out), NULL);

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

    return ret;
}
