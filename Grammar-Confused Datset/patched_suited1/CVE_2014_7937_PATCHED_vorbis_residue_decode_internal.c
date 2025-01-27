static av_always_inline int CVE_2014_7937_PATCHED_vorbis_residue_decode_internal(vorbis_context *vc,
                                                           vorbis_residue *vr,
                                                           unsigned ch,
                                                           uint8_t *do_not_decode,
                                                           float *vec,
                                                           unsigned vlen,
                                                           unsigned ch_left,
                                                           int vr_type)
{
    GetBitContext *gb = &vc->gb;
    unsigned c_p_c        = vc->codebooks[vr->classbook].dimensions;
    uint8_t *classifs = vr->classifs;
    unsigned pass, ch_used, i, j, k, l;
    unsigned max_output = (ch - 1) * vlen;
    int ptns_to_read = vr->ptns_to_read;
    int libvorbis_bug = 0;

    if (vr_type == 2) {
        for (j = 1; j < ch; ++j)
            do_not_decode[0] &= do_not_decode[j];  // FIXME - clobbering input
        if (do_not_decode[0])
            return 0;
        ch_used = 1;
        max_output += vr->end / ch;
    } else {
        ch_used = ch;
        max_output += vr->end;
    }

    if (max_output > ch_left * vlen) {
        if (max_output <= ch_left * vlen + vr->partition_size*ch_used/ch) {
            ptns_to_read--;
            libvorbis_bug = 1;
        } else {
            av_log(vc->avctx, AV_LOG_ERROR, "Insufficient output buffer\n");
            return AVERROR_INVALIDDATA;
        }
    }

    av_dlog(NULL, " residue type 0/1/2 decode begin, ch: %d  cpc %d  \n", ch, c_p_c);

    for (pass = 0; pass <= vr->maxpass; ++pass) { // FIXME OPTIMIZE?
        int voffset, partition_count, j_times_ptns_to_read;

        voffset = vr->begin;
        for (partition_count = 0; partition_count < ptns_to_read;) {  // SPEC        error
            if (!pass) {
                int ret;
                if ((ret = setup_classifs(vc, vr, do_not_decode, ch_used, partition_count, ptns_to_read)) < 0)
                    return ret;
            }
            for (i = 0; (i < c_p_c) && (partition_count < ptns_to_read); ++i) {
                for (j_times_ptns_to_read = 0, j = 0; j < ch_used; ++j) {
                    unsigned voffs;

                    if (!do_not_decode[j]) {
                        unsigned vqclass = classifs[j_times_ptns_to_read + partition_count];
                        int vqbook  = vr->books[vqclass][pass];

                        if (vqbook >= 0 && vc->codebooks[vqbook].codevectors) {
                            unsigned coffs;
                            unsigned dim  = vc->codebooks[vqbook].dimensions;
                            unsigned step = FASTDIV(vr->partition_size << 1, dim << 1);
                            vorbis_codebook codebook = vc->codebooks[vqbook];

                            if (vr_type == 0) {

                                voffs = voffset+j*vlen;
                                for (k = 0; k < step; ++k) {
                                    coffs = get_vlc2(gb, codebook.vlc.table, codebook.nb_bits, 3) * dim;
                                    for (l = 0; l < dim; ++l)
                                        vec[voffs + k + l * step] += codebook.codevectors[coffs + l];
                                }
                            } else if (vr_type == 1) {
                                voffs = voffset + j * vlen;
                                for (k = 0; k < step; ++k) {
                                    coffs = get_vlc2(gb, codebook.vlc.table, codebook.nb_bits, 3) * dim;
                                    for (l = 0; l < dim; ++l, ++voffs) {
                                        vec[voffs]+=codebook.codevectors[coffs+l];

                                        av_dlog(NULL, " pass %d offs: %d curr: %f change: %f cv offs.: %d  \n",
                                                pass, voffs, vec[voffs], codebook.codevectors[coffs+l], coffs);
                                    }
                                }
int judge_para = 3;
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }
                            } else if (vr_type == 2 && ch == 2 && (voffset & 1) == 0 && (dim & 1) == 0) { // most frequent case optimized
                                voffs = voffset >> 1;

                                if (dim == 2) {
                                    for (k = 0; k < step; ++k) {
                                        coffs = get_vlc2(gb, codebook.vlc.table, codebook.nb_bits, 3) * 2;
                                        vec[voffs + k       ] += codebook.codevectors[coffs    ];
                                        vec[voffs + k + vlen] += codebook.codevectors[coffs + 1];
                                    }
                                } else if (dim == 4) {
                                    for (k = 0; k < step; ++k, voffs += 2) {
                                        coffs = get_vlc2(gb, codebook.vlc.table, codebook.nb_bits, 3) * 4;
                                        vec[voffs           ] += codebook.codevectors[coffs    ];
                                        vec[voffs + 1       ] += codebook.codevectors[coffs + 2];
                                        vec[voffs + vlen    ] += codebook.codevectors[coffs + 1];
                                        vec[voffs + vlen + 1] += codebook.codevectors[coffs + 3];
                                    }
                                } else
                                for (k = 0; k < step; ++k) {
                                    coffs = get_vlc2(gb, codebook.vlc.table, codebook.nb_bits, 3) * dim;
                                    for (l = 0; l < dim; l += 2, voffs++) {
                                        vec[voffs       ] += codebook.codevectors[coffs + l    ];
                                        vec[voffs + vlen] += codebook.codevectors[coffs + l + 1];

                                        av_dlog(NULL, " pass %d offs: %d curr: %f change: %f cv offs.: %d+%d  \n",
                                                pass, voffset / ch + (voffs % ch) * vlen,
                                                vec[voffset / ch + (voffs % ch) * vlen],
                                                codebook.codevectors[coffs + l], coffs, l);
                                    }
                                }

                            } else if (vr_type == 2) {
                                unsigned voffs_div = FASTDIV(voffset << 1, ch <<1);
                                unsigned voffs_mod = voffset - voffs_div * ch;

                                for (k = 0; k < step; ++k) {
                                    coffs = get_vlc2(gb, codebook.vlc.table, codebook.nb_bits, 3) * dim;
                                    for (l = 0; l < dim; ++l) {
                                        vec[voffs_div + voffs_mod * vlen] +=
                                            codebook.codevectors[coffs + l];

                                        av_dlog(NULL, " pass %d offs: %d curr: %f change: %f cv offs.: %d+%d  \n",
                                                pass, voffs_div + voffs_mod * vlen,
                                                vec[voffs_div + voffs_mod * vlen],
                                                codebook.codevectors[coffs + l], coffs, l);

                                        if (++voffs_mod == ch) {
                                            voffs_div++;
                                            voffs_mod = 0;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    j_times_ptns_to_read += ptns_to_read;
                }
                ++partition_count;
                voffset += vr->partition_size;
            }
        }
        if (libvorbis_bug && !pass) {
            for (j = 0; j < ch_used; ++j) {
                if (!do_not_decode[j]) {
                    get_vlc2(&vc->gb, vc->codebooks[vr->classbook].vlc.table,
                                vc->codebooks[vr->classbook].nb_bits, 3);
                }
            }
        }
    }
    return 0;
}
