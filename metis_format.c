#include   "metis.h"


/*
 * metis § ww4-· § add --lstreamer (two screen) vs -streamer (one screen)                 § M1FDil §
 * metis § ww4-· § bring back menu items and commands for formatting                      § M1FDim §
 * metis § dn2·· § data refresh needs to update card count and format                     § M1GLZh §
 *
 * metis § mn*·· § format that has four indenpendently chosen and scrolled columns        § M1K2CY §
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
   yCMD_add (YCMD_M_VIEW   , "window"      , ""    , "s"    , api_yvikeys_window  , "size and placement of window"    );
   /*> yVIKEYS_menu_add ("µvwr", "col/rig"   , ":window col_rig¦");                   <* 
    *> yVIKEYS_menu_add ("µvwR", "long/rig"  , ":window long_rig¦");                  <* 
    *> yVIKEYS_menu_add ("µvwl", "col/lef"   , ":window col_lef¦");                   <* 
    *> yVIKEYS_menu_add ("µvwL", "long/lef"  , ":window long_lef¦");                  <* 
    *> yVIKEYS_menu_add ("µvwS", "streamer"  , ":window streamer¦");                  <* 
    *> yVIKEYS_menu_add ("µvwt", "ticker"    , ":window ticker¦");                    <* 
    *> yVIKEYS_menu_add ("µvwb", "baseline"  , ":window baseline¦");                  <* 
    *> yVIKEYS_menu_add ("µvwp", "project"   , ":window project¦");                   <* 
    *> yVIKEYS_menu_add ("µvww", "wideview"  , ":window wide¦");                      <* 
    *> yVIKEYS_menu_add ("µvwx", "extra"     , ":window extra¦");                     <* 
    *> yVIKEYS_menu_add ("µvws", "sticky"    , ":window sticky¦");                    <* 
    *> yVIKEYS_menu_add ("µvwn", "nonstick"  , ":window nonstick¦");                  <*/
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
   NCOLS = NROWS = 0;
   return 0;
}

int
format__assign     (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         x_col, x_row;
   int         n           =    0;
   int         c           =    0;
   tTASK      *x_task      = NULL;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter    (__FUNCTION__);
   /*---(prepare)------------------------*/
   format__clear ();
   n = metis_task_count ();
   if (n < 0) {
      DEBUG_DATA   yLOG_note     ("no data available");
      DEBUG_DATA   yLOG_exit     (__FUNCTION__);
      return 0;
   }
   /*---(walk tasks)---------------------*/
   metis_task_by_cursor (YDLST_HEAD, &x_task);
   for (x_col = 1; x_col <= TCOLS; ++x_col) {
      for (x_row = 1; x_row <= TROWS; ++x_row) {
         /*---(process)--------*/
         ++c;
         s_map [x_col][x_row] = n;
         s_map [x_col][0    ] = x_row;
         /*---(check maxes)----*/
         if (NCOLS < x_col)  NCOLS = x_col;
         if (NROWS < x_row)  NROWS = x_row;
         /*---(output)---------*/
         DEBUG_DATA   yLOG_complex  ("place"     , "%2dx ,%2dy, %2dc, %2d ncols, %2d nrows", x_col, x_row, c, NCOLS, NROWS);
         /*---(next)-----------*/
         metis_task_by_cursor (YDLST_NEXT, &x_task);
         if (x_task == NULL) {
            DEBUG_DATA   yLOG_note     ("done with existing data");
            DEBUG_DATA   yLOG_exit     (__FUNCTION__);
            return c;
         }
         /*---(done)-----------*/
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_exit     (__FUNCTION__);
   return c;
}

int
format__show       (void)
{
   int         x_col, x_row;
   printf ("\n");
   printf ("col  tot ", x_col);
   for (x_row = 1; x_row <= TROWS; ++x_row) {
      printf (" %3d", x_row);
   }
   printf ("\n");
   for (x_col = 1; x_col <= TCOLS; ++x_col) {
      if (s_map [x_col][0] <= 0) break;
      printf ("%3d (%3d)", x_col, s_map [x_col][0]);
      for (x_row = 1; x_row <= TROWS; ++x_row) {
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
   my.w_wide   = (WCOLS + my.c_over) * my.c_offset;
   my.w_tall   = (WROWS + my.r_over) * my.r_offset + my.m_offset;
   my.w_ftall  = my.w_tall + 15.0;
   /*---(texture)------------------------*/
   my.t_wide   = TCOLS * my.c_offset;
   my.t_tall   = TROWS * my.r_offset;
   /*---(playing)------------------------*/
   my.action   =   0;
   my.incr     = STOP;
   my.move     =   0;
   /*---(complete)-----------------------*/
   return 0;
}

char
format__fulldebug  (void)
{
   /*---(prework)------------------------*/
   DEBUG_USER   yLOG_snote    ("cr");
   DEBUG_USER   yLOG_sint     (WCOLS);
   DEBUG_USER   yLOG_sint     (WROWS);
   DEBUG_USER   yLOG_snote    ("win");
   DEBUG_USER   yLOG_sint     (my.w_wide);
   DEBUG_USER   yLOG_sint     (my.w_tall);
   DEBUG_USER   yLOG_snote    ("tex");
   DEBUG_USER   yLOG_sint     (my.t_wide);
   DEBUG_USER   yLOG_sint     (my.t_tall);
   DEBUG_USER   yLOG_snote    ("xy");
   DEBUG_USER   yLOG_sint     (my.w_left);
   DEBUG_USER   yLOG_sint     (my.w_topp);
   return 0;
}


char
format_streamer    (void)
{
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_senter   (__FUNCTION__);
   /*---(columns)------------------------*/
   my.c_offset = 300;
   WCOLS    =   1;
   TCOLS    =   1;
   /*---(rows)---------------------------*/
   my.r_offset =  44;
   WROWS    =  my.s_tall / my.r_offset;
   TROWS    =  60;
   /*---(menu spacing)-------------------*/
   my.m_offset =   0;
   /*---(win/tex)------------------------*/
   my.w_left  = my.s_wide - my.c_offset;
   format__wintex ();
   my.w_topp  = (my.s_tall - my.w_ftall) / 2;
   format__fulldebug ();
   /*---(complete)-----------------------*/
   DEBUG_USER   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
format_ticker           (void)
{
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_senter   (__FUNCTION__);
   /*---(rows)---------------------------*/
   my.r_offset =  45;
   WROWS    =   1;
   TROWS    =   1;
   /*---(columns)------------------------*/
   my.c_offset = 300;
   if (my.format != FORMAT_TICKER)  my.c_offset = 325;
   WCOLS    =   4;
   my.c_over   =   1;
   TCOLS    =  12;
   /*---(menu spacing)-------------------*/
   my.m_offset = 195;
   /*---(win/tex)------------------------*/
   my.w_left   =    0;
   format__wintex ();
   my.w_topp   = (my.s_tall / 2.0) - my.r_offset - my.m_offset;
   format__fulldebug ();
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
      WROWS  =  25;
   else
      WROWS  =  12;
   TROWS    =  60;
   /*---(columns)------------------------*/
   my.c_offset = 300;
   WCOLS    =   1;
   TCOLS    =   1;
   /*---(menu spacing)-------------------*/
   my.m_offset =   0;
   /*---(win/tex)------------------------*/
   if (strchr (FORMAT_RIGHTS, my.format) != NULL)
      my.w_left  = my.s_wide - my.c_offset - 15;
   else
      my.w_left  =   15;
   format__wintex ();
   if (strchr (FORMAT_LONGS , my.format) != NULL)
      my.w_topp  = (my.s_tall - my.w_ftall) / 2;
   else
      my.w_topp  = ((my.s_tall / 2) - my.w_ftall) / 2;
   format__fulldebug ();
   /*---(complete)-----------------------*/
   DEBUG_USER   yLOG_sexit    (__FUNCTION__);
   return 0;
}

char
format_wideview    (void)
{
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_senter   (__FUNCTION__);
   /*---(columns)------------------------*/
   my.c_offset = 320;
   WCOLS    =   4;
   TCOLS    =  12;
   /*---(rows)---------------------------*/
   my.r_offset =  60;
   WROWS    =  12;
   TROWS    =  12;
   /*---(menu spacing)-------------------*/
   my.m_offset =   0;
   /*---(win/tex)------------------------*/
   my.w_left   =  (my.s_wide - (4 * my.c_wide) - (3 * (my.c_offset - my.c_wide))) / 2;
   format__wintex ();
   my.w_topp  = ((my.s_tall / 2) - my.w_ftall) / 2;
   format__fulldebug ();
   /*---(complete)-----------------------*/
   DEBUG_USER   yLOG_sexit    (__FUNCTION__);
   return 0;
}

char
format_projects    (void)
{
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_senter   (__FUNCTION__);
   /*---(columns)------------------------*/
   my.c_offset = 320;
   WCOLS    =   4;
   TCOLS    =  12;
   /*---(rows)---------------------------*/
   my.r_offset =  60;
   WROWS    =  25;
   TROWS    =  25;
   /*---(menu spacing)-------------------*/
   my.m_offset =   0;
   /*---(win/tex)------------------------*/
   my.w_left   =  (my.s_wide - (4 * my.c_wide) - (3 * (my.c_offset - my.c_wide))) / 2;
   format__wintex ();
   my.w_topp  = (my.s_tall - my.w_ftall) / 2;
   format__fulldebug ();
   /*---(complete)-----------------------*/
   DEBUG_USER   yLOG_sexit    (__FUNCTION__);
   return 0;
}

char
format_extra       (void)
{
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_senter   (__FUNCTION__);
   /*---(columns)------------------------*/
   my.c_offset = 320;
   WCOLS    =   4;
   TCOLS    =  12;
   /*---(rows)---------------------------*/
   my.r_offset =  44;
   WROWS    =  my.s_tall / my.r_offset;
   TROWS    =  WROWS + my.r_over;
   /*---(menu spacing)-------------------*/
   my.m_offset =   0;
   /*---(win/tex)------------------------*/
   my.w_left   =  (my.s_wide - (4 * my.c_wide) - (3 * (my.c_offset - my.c_wide))) / 2;
   format__wintex ();
   my.w_topp  = (my.s_tall - my.w_ftall) / 2;
   format__fulldebug ();
   /*---(complete)-----------------------*/
   DEBUG_USER   yLOG_sexit    (__FUNCTION__);
   return 0;
}

char
FORMAT_refresh          (void)
{
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter    (__FUNCTION__);
   /*---(globals)------------------------*/
   my.c_wide  =  300;
   my.c_over  =    0;
   my.r_tall  =   45;
   my.r_over  =    0;
   /*---(specifics)----------------------*/
   switch (my.format) {
   case FORMAT_TICKER  : case FORMAT_BASELINE :
      DEBUG_DATA   yLOG_note     ("ticker/baseline type");
      rc = format_ticker   ();
      break;
   case FORMAT_RSHORT  : case FORMAT_RLONG  : case FORMAT_LSHORT : case FORMAT_LLONG  :
      DEBUG_DATA   yLOG_note     ("column type");
      rc = format_column   ();
      break;
   case FORMAT_STREAMER:
      DEBUG_DATA   yLOG_note     ("streamer type");
      rc = format_streamer ();
      break;
   case FORMAT_WIDE    :
      DEBUG_DATA   yLOG_note     ("wide type");
      rc = format_wideview ();
      break;
   case FORMAT_PROJECT :
      DEBUG_DATA   yLOG_note     ("project type");
      rc = format_projects ();
      break;
   case FORMAT_EXTRA   :
      DEBUG_DATA   yLOG_note     ("extra type");
      rc = format_extra    ();
      break;
   default  :
      DEBUG_DATA   yLOG_note     ("defaulting column type");
      my.format = FORMAT_RSHORT;
      rc = format_column   ();
      break;
   }
   DEBUG_DATA   yLOG_value    ("WCOLS"     , WCOLS);
   yMAP_axis_avail (YMAP_XAXIS, WCOLS);
   DEBUG_DATA   yLOG_value    ("WROWS"     , WROWS);
   yMAP_axis_avail (YMAP_YAXIS, WROWS);
   DEBUG_DATA   yLOG_value    ("format"    , rc);
   rc = format__clear  ();
   DEBUG_DATA   yLOG_value    ("clear"     , rc);
   rc = format__assign ();
   DEBUG_DATA   yLOG_value    ("assign"    , rc);
   /*> format__show   ();                                                             <*/
   /*---(complete)------------------------------*/
   DEBUG_DATA   yLOG_exit     (__FUNCTION__);
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
      snprintf (unit_answer, LEN_FULL, "FORMAT unit  (%c) : wide %4dt %4dw %4do %4dn     - tall %4dt %4dw %4do %4dn     -", my.format, TCOLS, WCOLS, my.c_over, NCOLS, TROWS, WROWS, my.r_over, NROWS);
   }
   else if (strcmp (a_question, "window"        ) == 0) {
      snprintf (unit_answer, LEN_FULL, "FORMAT win   (%c) : wide %4ds %4dc %4do %4dl %4dw tall %4ds %4dr %4do %4dt %4dt %4df", my.format, my.s_wide, my.c_wide, my.c_offset, my.w_left, my.w_wide, my.s_tall, my.r_tall, my.r_offset, my.w_topp, my.w_tall, my.w_ftall);
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


