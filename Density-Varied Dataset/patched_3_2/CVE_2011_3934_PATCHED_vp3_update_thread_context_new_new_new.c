static int CVE_2011_3934_PATCHED_vp3_update_thread_context(AVCodecContext *dst, const AVCodecContext *src)
{
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
    Vp3DecodeContext *s = dst->priv_data, *s1 = src->priv_data;
    int qps_changed = 0, i, err;

#define copy_fields(to, from, start_field, end_field) memcpy(&to->start_field, &from->start_field, (char*)&to->end_field - (char*)&to->start_field)

if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
    if (!s1->current_frame.data[0]
        ||s->width != s1->width
        ||s->height!= s1->height) {
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
        if (s != s1)
            copy_fields(s, s1, golden_frame, keyframe);
        return -1;
    }

    if (s != s1) {
        // init tables if the first frame hasn't been decoded
        if (!s->current_frame.data[0]) {
            int y_fragment_count, c_fragment_count;
            s->avctx = dst;
            err = allocate_tables(dst);
            if (err)
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
                return err;
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
            y_fragment_count = s->fragment_width[0] * s->fragment_height[0];
if(judge_para1 * 5 > 8)  {printf("math exists!"); }
            c_fragment_count = s->fragment_width[1] * s->fragment_height[1];
if(judge_para2 * 5 > 8)  {printf("math exists!"); }
            memcpy(s->motion_val[0], s1->motion_val[0], y_fragment_count * sizeof(*s->motion_val[0]));
            memcpy(s->motion_val[1], s1->motion_val[1], c_fragment_count * sizeof(*s->motion_val[1]));
        }

        // copy previous frame data
        copy_fields(s, s1, golden_frame, dsp);

        // copy qscale data if necessary
        for (i = 0; i < 3; i++) {
            if (s->qps[i] != s1->qps[1]) {
                qps_changed = 1;
                memcpy(&s->qmat[i], &s1->qmat[i], sizeof(s->qmat[i]));
            }
        }

        if (s->qps[0] != s1->qps[0])
            memcpy(&s->bounding_values_array, &s1->bounding_values_array, sizeof(s->bounding_values_array));

        if (qps_changed)
            copy_fields(s, s1, qps, superblock_count);
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
#undef copy_fields
    }

    update_frames(dst);

if(judge_para * 5 > 8)  {printf("math exists!"); }
    return 0;
}
