#include   "metis.h"


/*
 *
 *
 */






/*====================------------------------------------====================*/
/*===----                          polymorphism                        ----===*/
/*====================------------------------------------====================*/
static void      o___POLYMORPH_______________o (void) {;}

char
metis_format_init        (void)
{
   my.breaks = '-';
   my.x_skip =  -1;
   my.y_skip =  -1;
   return 0;
}

char
metis_format_vikeys      (void)
{
   yCMD_add (YVIHUB_M_VIEW   , "window"      , ""    , "s"    , api_yvikeys_window  , "size and placement of window"    );
   yCMD_menu_add ("µvwr", "col/rig"   , ":window col_rig¦");
   yCMD_menu_add ("µvwR", "long/rig"  , ":window long_rig¦");
   yCMD_menu_add ("µvwl", "col/lef"   , ":window col_lef¦");
   yCMD_menu_add ("µvwL", "long/lef"  , ":window long_lef¦");
   yCMD_menu_add ("µvwS", "streamer"  , ":window streamer¦");
   yCMD_menu_add ("µvwt", "ticker"    , ":window ticker¦");
   yCMD_menu_add ("µvwb", "baseline"  , ":window baseline¦");
   yCMD_menu_add ("µvwp", "project"   , ":window project¦");
   yCMD_menu_add ("µvww", "wideview"  , ":window wide¦");
   yCMD_menu_add ("µvwx", "extra"     , ":window extra¦");
   yCMD_menu_add ("µvws", "sticky"    , ":window sticky¦");
   yCMD_menu_add ("µvwn", "nonstick"  , ":window nonstick¦");
   return 0;
}

char
format__wintex     (void)
{
   /*---(window)-------------------------*/
   my.w_wide   = (WCOLS + my.c_over) * my.c_offset;
   if (my.w_wide > my.s_wide)  my.w_wide = my.s_wide;
   my.w_tall   = (WROWS + my.r_over) * my.r_offset + my.m_offset;
   if (my.w_tall > my.s_tall)  my.w_tall = my.s_tall;
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
   TROWS    =  82;
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
   TROWS    =   5;
   /*---(columns)------------------------*/
   my.c_offset = 300;
   if (my.format != FORMAT_TICKER)  my.c_offset = 325;
   WCOLS    =   4;
   my.c_over   =   1;
   TCOLS    =  12;
   /*---(menu spacing)-------------------*/
   my.m_offset = 180 - my.r_offset;
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
   TROWS    =  60;
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
   TROWS    =  60;
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
   TROWS    =  82;
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
metis_format_refresh    (void)
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
   /*> rc = metis_place_assign ();                                                    <*/
   /*> DEBUG_DATA   yLOG_value    ("assign"    , rc);                                 <*/
   /*> metis_place_show   ();                                                             <*/
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
   /*> if      (strcmp (a_question, "count"         ) == 0) {                         <* 
    *>    snprintf (unit_answer, LEN_FULL, "DATA count       : %d", g_ntask);         <* 
    *> }                                                                              <*/
   if      (strcmp (a_question, "units"         ) == 0) {
      snprintf (unit_answer, LEN_FULL, "FORMAT unit  (%c) : wide  ···- %4dt %4dw  ···- %4do %4dn § tall  ···- %4dt %4dw  ···- %4do %4dn  ···-", my.format,
            TCOLS, WCOLS, my.c_over, NCOLS,
            TROWS, WROWS, my.r_over, NROWS);
   }
   else if (strcmp (a_question, "window"        ) == 0) {
      snprintf (unit_answer, LEN_FULL, "FORMAT win   (%c) : wide %4ds %4dt %4dw %4dc %4do %4dl § tall %4ds %4dt %4dw %4dr %4do %4dt %4df", my.format,
            my.s_wide, my.t_wide, my.w_wide, my.c_wide, my.c_offset, my.w_left,
            my.s_tall, my.t_tall, my.w_tall, my.r_tall, my.r_offset, my.w_topp, my.w_ftall);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


