#include   "metis.h"


/*===[[ METIS BACKLOG ]]======================================================*
 *  metis  -----  tbd
 *
 */



static int  s_map [MAX_COLS][MAX_ROWS];



/*====================------------------------------------====================*/
/*===----                          polymorphism                        ----===*/
/*====================------------------------------------====================*/
static void      o___POLYMORPH_______________o (void) {;}

char
FORMAT_init        (void)
{
   yVIKEYS_cmds_add (YVIKEYS_M_VIEW   , "window"      , ""    , "s"    , api_yvikeys_window  , "size and placement of window"    );
   yVIKEYS_menu_add ("�vwr", "col/rig"   , ":window col_rig�");
   yVIKEYS_menu_add ("�vwR", "long/rig"  , ":window long_rig�");
   yVIKEYS_menu_add ("�vwl", "col/lef"   , ":window col_lef�");
   yVIKEYS_menu_add ("�vwL", "long/lef"  , ":window long_lef�");
   yVIKEYS_menu_add ("�vwS", "streamer"  , ":window streamer�");
   yVIKEYS_menu_add ("�vwt", "ticker"    , ":window ticker�");
   yVIKEYS_menu_add ("�vwb", "baseline"  , ":window baseline�");
   yVIKEYS_menu_add ("�vwp", "project"   , ":window project�");
   yVIKEYS_menu_add ("�vww", "wideview"  , ":window wide�");
   yVIKEYS_menu_add ("�vwx", "extra"     , ":window extra�");
   yVIKEYS_menu_add ("�vws", "sticky"    , ":window sticky�");
   yVIKEYS_menu_add ("�vwn", "nonstick"  , ":window nonstick�");
}

char
format__clear      (void)
{
   int         x_col, x_row;
   for (x_col = 0; x_col < MAX_COLS; ++x_col) {
      for (x_row = 0; x_row < MAX_ROWS; ++x_row) {
         s_map [x_col][x_row] = -1;
      }
   }
   my.ncols = my.nrows = 0;
   return 0;
}

int
format__assign     (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         x_col, x_row;
   int         n           =    0;
   int         c           =    0;
   /*---(prepare)------------------------*/
   format__clear ();
   n = DATA_cursor ('[');
   if (n < 0)  return c;
   /*---(walk tasks)---------------------*/
   for (x_col = 1; x_col <= my.tcols; ++x_col) {
      for (x_row = 1; x_row <= my.trows; ++x_row) {
         /*---(process)--------*/
         ++c;
         s_map [x_col][x_row] = n;
         s_map [x_col][0    ] = x_row;
         /*---(check maxes)----*/
         if (my.ncols < x_col)  my.ncols = x_col;
         if (my.nrows < x_row)  my.nrows = x_row;
         /*---(next)-----------*/
         n = DATA_cursor ('>');
         if (n < 0)     return c;
         /*---(done)-----------*/
      }
   }
   /*---(complete)-----------------------*/
   return c;
}

int
format__show       (void)
{
   int         x_col, x_row;
   printf ("\n");
   printf ("col  tot ", x_col);
   for (x_row = 1; x_row <= my.trows; ++x_row) {
      printf (" %3d", x_row);
   }
   printf ("\n");
   for (x_col = 1; x_col <= my.tcols; ++x_col) {
      if (s_map [x_col][0] <= 0) break;
      printf ("%3d (%3d)", x_col, s_map [x_col][0]);
      for (x_row = 1; x_row <= my.trows; ++x_row) {
         if (s_map [x_col][x_row] <= 0)  break;
         printf (" %3d", s_map [x_col][x_row]);
      }
      printf ("\n");
   }
}

int
format_check            (int a_col, int a_row)
{
   return s_map [a_col][a_row];
}

char
format__wintex     (void)
{
   /*---(window)-------------------------*/
   DEBUG_USER   yLOG_sint     (my.w_left);
   DEBUG_USER   yLOG_sint     (my.w_topp);
   my.w_wide   = (my.wcols + my.c_over) * my.c_offset;
   my.w_tall   = (my.wrows + my.r_over) * my.r_offset + my.m_offset;
   DEBUG_USER   yLOG_sint     (my.w_wide);
   DEBUG_USER   yLOG_sint     (my.w_tall);
   /*---(texture)------------------------*/
   my.t_wide   = my.tcols * my.c_offset;
   my.t_tall   = my.trows * my.r_offset;
   /*---(playing)------------------------*/
   my.action   =   0;
   my.incr     = STOP;
   my.move     =   0;
   /*---(complete)-----------------------*/
   return 0;
}

char
format_streamer    (void)
{
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_enter    (__FUNCTION__);
   /*---(columns)------------------------*/
   my.c_offset = 300;
   my.wcols    =   1;
   my.tcols    =   1;
   DEBUG_USER   yLOG_sint     (my.wcols);
   /*---(rows)---------------------------*/
   my.r_offset =  44;
   my.wrows    =  my.s_tall / my.r_offset;
   if (my.wrows * my.r_offset < my.w_tall)   my.r_over = 1;
   DEBUG_USER   yLOG_sint     (my.wrows);
   my.trows    =  60;
   /*> if (my.nact <= my.trows)  my.nrows = my.nact;                                  <* 
    *> else                      my.nrows = my.trows;                                 <*/
   /*---(menu spacing)-------------------*/
   my.m_offset =   0;
   /*---(win/tex)------------------------*/
   my.w_left  = my.s_wide - my.c_offset;
   my.w_topp  = 0;
   format__wintex ();
   /*---(complete)-----------------------*/
   DEBUG_USER   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
format_ticker           (void)
{
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_enter    (__FUNCTION__);
   /*---(rows)---------------------------*/
   my.r_offset =  45;
   my.wrows    =   1;
   my.trows    =   1;
   /*> my.nrows    =   1;                                                             <*/
   /*---(columns)------------------------*/
   my.c_offset = 300;
   if (my.format != FORMAT_TICKER)  my.c_offset = 325;
   my.wcols    =   4;
   my.c_over   =   1;
   my.tcols    =  12;
   /*> if (my.nact <= my.tcols)  my.ncols = my.nact;                                  <* 
    *> else                      my.ncols = my.tcols;                                 <*/
   /*---(menu spacing)-------------------*/
   my.m_offset = 195;
   /*---(win/tex)------------------------*/
   my.w_left   =    0;
   my.w_topp   = (my.s_tall / 2.0) - my.r_offset - my.m_offset;
   format__wintex ();
   /*---(complete)-----------------------*/
   DEBUG_USER   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
format_column      (void)
{
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_senter   (__FUNCTION__);
   /*---(rows)---------------------------*/
   my.r_offset =  60;
   if (strchr (FORMAT_LONGS , my.format) != NULL)
      my.wrows  =  25;
   else
      my.wrows  =  12;
   DEBUG_USER   yLOG_sint     (my.wrows);
   my.trows    =  60;
   /*> if (my.nact <= my.trows)  my.nrows = my.nact;                                  <* 
    *> else                      my.nrows = my.trows;                                 <*/
   /*---(columns)------------------------*/
   my.c_offset = 300;
   my.wcols    =   1;
   my.tcols    =   1;
   /*> my.ncols    =   1;                                                             <*/
   DEBUG_USER   yLOG_sint     (my.wcols);
   /*---(menu spacing)-------------------*/
   my.m_offset =   0;
   /*---(win/tex)------------------------*/
   if (strchr (FORMAT_RIGHTS, my.format) != NULL)
      my.w_left  = my.s_wide - my.c_offset - 15;
   else
      my.w_left  =   15;
   my.w_topp   = 25;
   format__wintex ();
   /*---(complete)-----------------------*/
   DEBUG_USER   yLOG_sexit    (__FUNCTION__);
   return 0;
}

char
format_wideview    (void)
{
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_enter    (__FUNCTION__);
   /*---(columns)------------------------*/
   my.c_offset = 320;
   my.wcols    =   4;
   my.tcols    =  12;
   /*> my.ncols    =  12;                                                             <*/
   DEBUG_USER   yLOG_sint     (my.wcols);
   /*---(rows)---------------------------*/
   my.r_offset =  60;
   my.wrows    =  12;
   my.trows    =  12;
   /*> my.nrows    =  12;                                                             <*/
   DEBUG_USER   yLOG_sint     (my.wrows);
   /*---(menu spacing)-------------------*/
   my.m_offset =   0;
   /*---(win/tex)------------------------*/
   my.w_left   =  (my.s_wide - (4 * my.c_wide) - (3 * (my.c_offset - my.c_wide))) / 2;
   my.w_topp   =  25;
   format__wintex ();
   /*---(complete)-----------------------*/
   DEBUG_USER   yLOG_sexit    (__FUNCTION__);
   return 0;
}

char
format_projects    (void)
{
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_enter    (__FUNCTION__);
   /*---(columns)------------------------*/
   my.c_offset = 320;
   my.wcols    =   4;
   my.tcols    =  12;
   /*> my.ncols    =  12;                                                             <*/
   DEBUG_USER   yLOG_sint     (my.wcols);
   /*---(rows)---------------------------*/
   my.r_offset =  60;
   my.wrows    =  25;
   my.trows    =  25;
   /*> my.nrows    =  25;                                                             <*/
   DEBUG_USER   yLOG_sint     (my.wrows);
   /*---(menu spacing)-------------------*/
   my.m_offset =   0;
   /*---(win/tex)------------------------*/
   my.w_left   =  (my.s_wide - (4 * my.c_wide) - (3 * (my.c_offset - my.c_wide))) / 2;
   my.w_topp   =  25;
   format__wintex ();
   /*---(complete)-----------------------*/
   DEBUG_USER   yLOG_sexit    (__FUNCTION__);
   return 0;
}

char
format_extra       (void)
{
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_enter    (__FUNCTION__);
   /*---(columns)------------------------*/
   my.c_offset = 320;
   my.wcols    =   4;
   my.tcols    =  12;
   /*> my.ncols    =  12;                                                             <*/
   DEBUG_USER   yLOG_sint     (my.wcols);
   /*---(rows)---------------------------*/
   my.r_offset =  44;
   my.wrows    =  my.s_tall / my.r_offset;
   if (my.wrows * my.r_offset < my.w_tall)   my.r_over = 1;
   my.trows    =  my.wrows + my.r_over;
   DEBUG_USER   yLOG_sint     (my.wrows);
   /*---(menu spacing)-------------------*/
   my.m_offset =   0;
   /*---(win/tex)------------------------*/
   my.w_left   =  (my.s_wide - (4 * my.c_wide) - (3 * (my.c_offset - my.c_wide))) / 2;
   my.w_topp   =   0;
   format__wintex ();
   /*---(complete)-----------------------*/
   DEBUG_USER   yLOG_sexit    (__FUNCTION__);
   return 0;
}

char
FORMAT_refresh          (void)
{
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_enter    (__FUNCTION__);
   /*---(globals)------------------------*/
   my.c_wide  =  300;
   my.c_over  =    0;
   my.r_tall  =   45;
   my.r_over  =    0;
   /*---(specifics)----------------------*/
   switch (my.format) {
   case FORMAT_TICKER  : case FORMAT_BASELINE :
      format_ticker   ();
      break;
   case FORMAT_RSHORT  : case FORMAT_RLONG  : case FORMAT_LSHORT : case FORMAT_LLONG  :
      format_column   ();
      break;
   case FORMAT_STREAMER:
      format_streamer ();
      break;
   case FORMAT_WIDE    :
      format_wideview ();
      break;
   case FORMAT_PROJECT :
      format_projects ();
      break;
   case FORMAT_EXTRA   :
      format_extra    ();
      break;
   default  :
      my.format = FORMAT_RSHORT;
      format_column   ();
      break;
   }
   format__clear  ();
   format__assign ();
   /*> format__show   ();                                                             <*/
   /*---(complete)------------------------------*/
   DEBUG_USER   yLOG_exit     (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      unit testing                            ----===*/
/*====================------------------------------------====================*/
static void  o___UNITTEST________o () { return; }

char*        /*-> unit test accessor -----------------[ light  [us.B60.2A3.F2]*/ /*-[01.0000.00#.#]-*/ /*-[--.---.---.--]-*/
FORMAT__unit       (char *a_question, int a_num)
{
   /*---(locals)-------------------------*/
   char        rc          =    0;
   int         i           =    0;
   char        t           [LEN_HUND]   = "";
   char        s           [LEN_LABEL]  = "";
   /*---(overall)------------------------*/
   strcpy (unit_answer, "FORMAT           : question not understood");
   if      (strcmp (a_question, "count"         ) == 0) {
      snprintf (unit_answer, LEN_FULL, "DATA count       : %d", g_ntask);
   }
   else if (strcmp (a_question, "units"         ) == 0) {
      snprintf (unit_answer, LEN_FULL, "FORMAT unit  (%c) : wide %4dt %4dw %4do %4dn     - tall %4dt %4dw %4do %4dn     -", my.format, my.tcols, my.wcols, my.c_over, my.ncols, my.trows, my.wrows, my.r_over, my.nrows);
   }
   else if (strcmp (a_question, "window"        ) == 0) {
      snprintf (unit_answer, LEN_FULL, "FORMAT win   (%c) : wide %4ds %4dc %4do %4dl %4dw tall %4ds %4dr %4do %4dt %4dt", my.format, my.s_wide, my.c_wide, my.c_offset, my.w_left, my.w_wide, my.s_tall, my.r_tall, my.r_offset, my.w_topp, my.w_tall);
   }
   else if (strcmp (a_question, "map"           ) == 0) {
      sprintf (t, "%2d   ", s_map [a_num][0]);
      for (i = 1; i <= 20; ++i) {
         if (s_map [a_num][i] > -1)  sprintf (s, " %2d", s_map [a_num][i]);
         else                        sprintf (s, "  -");
         strlcat (t, s, LEN_HUND);
      }
      if (s_map [a_num][0] > 20)  strlcat (t, " >", LEN_HUND);
      snprintf (unit_answer, LEN_FULL, "FORMAT map  (%2d) : %s", a_num, t);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


