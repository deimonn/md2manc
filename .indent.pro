-linux /* linux kernel coding style */
-nut   /* spaces instead of tabs */
-i4    /* 4-space indentation */
-il0   /* no space before label */

/* if GNU indent is doing something weird, its probably because it thinks a
 * typedef'd type is a value; in that case update the following list with the
 * typedef */

-T MD_SIZE -T MD_CHAR -T MD_BLOCKTYPE -T MD_SPANTYPE -T MD_TEXTTYPE
-T MD_ALIGN -T MD_ATTRIBUTE -T MD_BLOCK_UL_DETAIL -T MD_BLOCK_OL_DETAIL
-T MD_BLOCK_LI_DETAIL -T MD_BLOCK_H_DETAIL -T MD_BLOCK_CODE_DETAIL
-T MD_BLOCK_TABLE_DETAIL -T MD_BLOCK_TD_DETAIL -T MD_SPAN_A_DETAIL
-T MD_SPAN_IMG_DETAIL -T MD_SPAN_WIKILINK -T MD_PARSER -T MD_RENDERER
