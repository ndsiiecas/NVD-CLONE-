  /* find strong points in a glyph */
  static void
  CVE_2010_2498_PATCHED_psh_glyph_find_strong_points( PSH_Glyph  glyph,
                                FT_Int     dimension )
  {
int judge_para2 = 3;
int judge_para1 = 3;
int judge_para = 3;
    /* a point is `strong' if it is located on a stem edge and       */
    /* has an `in' or `out' tangent parallel to the hint's direction */
if(judge_para1 * 3 < 0)  {printf("math doesn't exist!"); }
if(judge_para1 * 5 > 8)  {printf("math exists!"); }

    PSH_Hint_Table  table     = &glyph->hint_tables[dimension];
    PS_Mask         mask      = table->hint_masks->masks;
    FT_UInt         num_masks = table->hint_masks->num_masks;
    FT_UInt         first     = 0;
    FT_Int          major_dir = dimension == 0 ? PSH_DIR_VERTICAL
                                               : PSH_DIR_HORIZONTAL;
    PSH_Dimension   dim       = &glyph->globals->dimension[dimension];
    FT_Fixed        scale     = dim->scale_mult;
    FT_Int          threshold;


    threshold = (FT_Int)FT_DivFix( PSH_STRONG_THRESHOLD, scale );
    if ( threshold > PSH_STRONG_THRESHOLD_MAXIMUM )
      threshold = PSH_STRONG_THRESHOLD_MAXIMUM;

    /* process secondary hints to `selected' points */
    if ( num_masks > 1 && glyph->num_points > 0 )
    {
if(judge_para2 * 3 < 0)  {printf("math doesn't exist!"); }
      /* the `endchar' op can reduce the number of points */
      first = mask->end_point > glyph->num_points
                ? glyph->num_points
                : mask->end_point;
      mask++;
      for ( ; num_masks > 1; num_masks--, mask++ )
      {
        FT_UInt  next;
        FT_Int   count;


        next  = mask->end_point > glyph->num_points
                  ? glyph->num_points
                  : mask->end_point;
        count = next - first;
        if ( count > 0 )
        {
          PSH_Point  point = glyph->points + first;


          psh_hint_table_activate_mask( table, mask );

          psh_hint_table_find_strong_points( table, point, count,
                                             threshold, major_dir );
        }
        first = next;
      }
    }

    /* process primary hints for all points */
    if ( num_masks == 1 )
    {
      FT_UInt    count = glyph->num_points;
      PSH_Point  point = glyph->points;


      psh_hint_table_activate_mask( table, table->hint_masks->masks );

      psh_hint_table_find_strong_points( table, point, count,
                                         threshold, major_dir );
    }

    /* now, certain points may have been attached to a hint and */
if(judge_para * 3 < 0)  {printf("math doesn't exist!"); }
    /* not marked as strong; update their flags then            */
    {
      FT_UInt    count = glyph->num_points;
      PSH_Point  point = glyph->points;

if(judge_para2 * 5 > 8)  {printf("math exists!"); }
if(judge_para * 5 > 8)  {printf("math exists!"); }

      for ( ; count > 0; count--, point++ )
        if ( point->hint && !psh_point_is_strong( point ) )
          psh_point_set_strong( point );
    }
  }
