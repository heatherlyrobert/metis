#include   "metis.h"


/*===[[ METIS BACKLOG ]]======================================================*
 *  metis  -----  tbd
 *
 */


/*====================------------------------------------====================*/
/*===----                          polymorphism                        ----===*/
/*====================------------------------------------====================*/
static void      o___POLYMORPH_______________o (void) {;}

char
FORMAT_init        (void)
{
   yVIKEYS_cmds_add (YVIKEYS_M_VIEW   , "window"      , ""    , "s"    , api_yvikeys_window  , "size and placement of window"    );
   yVIKEYS_menu_add ("µvwr", "col/rig"   , ":window col_rig¦");
   yVIKEYS_menu_add ("µvwR", "long/rig"  , ":window long_rig¦");
   yVIKEYS_menu_add ("µvwl", "col/lef"   , ":window col_lef¦");
   yVIKEYS_menu_add ("µvwL", "long/lef"  , ":window long_lef¦");
   yVIKEYS_menu_add ("µvwt", "ticker"    , ":window ticker¦");
   yVIKEYS_menu_add ("µvwb", "baseline"  , ":window baseline¦");
   yVIKEYS_menu_add ("µvwp", "project"   , ":window project¦");
   yVIKEYS_menu_add ("µvww", "wideview"  , ":window wide¦");
   yVIKEYS_menu_add ("µvwx", "extra"     , ":window extra¦");
   yVIKEYS_menu_add ("µvws", "sticky"    , ":window sticky¦");
   yVIKEYS_menu_add ("µvwn", "nonstick"  , ":window nonstick¦");
}

char
format_streamer    (void)
{
   my.win_h  = my.tex_h  =  (24 * 44) + 45;   /* window height                      */
   my.win_w  = 300;        /* scancard width                           */
   my.tex_w  = 300;
   my.format = FORMAT_STREAMER;
   my.nrows  =   0;
   my.ncols  =   0;
   my.wcols  =   0;
   my.wrows  =  24;
   my.action =   1;
   my.incr   = STOP;
   my.move   =   0;
   return 0;
}

char
format_ticker      (char a_type)
{
   int         x_wide      =    0;
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_enter    (__FUNCTION__);
   /*---(format)-------------------------*/
   if (a_type == FORMAT_TICKER) {
      my.format = FORMAT_TICKER;
      x_wide = 300;
   } else {
      my.format = FORMAT_BASELINE;
      x_wide = 325;
   }
   /*---(rows)---------------------------*/
   my.wrows  =   1;
   my.trows  =   1;
   my.nrows  =   1;
   /*---(rows)---------------------------*/
   my.wcols  =   6;
   my.tcols  =  12;
   if (my.nact <= my.tcols)  my.ncols = my.nact;
   else                      my.ncols = my.tcols;
   /*---(win/tex)------------------------*/
   my.win_x  =    0;
   /*> my.win_y  =  723;                                                              <*/
   my.win_y  =  528;
   my.win_h  = my.wrows *  45.0 + 195.0;
   my.tex_h  = my.trows *  45.0;
   my.win_w  = my.wcols * x_wide;
   my.tex_w  = my.tcols * x_wide;
   /*---(playing)------------------------*/
   my.action =   0;
   my.incr   = STOP;
   my.move   =   0;
   /*---(complete)-----------------------*/
   DEBUG_USER   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
format_column      (void)
{
   /*---(locals)-------------------------*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_senter   (__FUNCTION__);
   /*---(format)-------------------------*/
   DEBUG_USER   yLOG_schar    (my.format);
   --rce;  if (strchr (FORMAT_COLUMNS, my.format) == NULL) {
      DEBUG_USER   yLOG_sexitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(columns)------------------------*/
   my.wcols  =   1;
   my.tcols  =   1;
   my.ncols  =   1;
   DEBUG_USER   yLOG_sint     (my.wcols);
   /*---(rows)---------------------------*/
   if (strchr (FORMAT_LONGS , my.format) != NULL)  my.wrows  =  25;
   else                                            my.wrows  =  12;
   DEBUG_USER   yLOG_sint     (my.wrows);
   my.trows  =  60;
   if (my.nact <= my.trows)  my.nrows = my.nact;
   else                      my.nrows = my.trows;
   /*---(win/tex)------------------------*/
   if (strchr (FORMAT_RIGHTS, my.format) != NULL)  my.win_x  = 1055;
   else                                            my.win_x  =   15;
   my.win_y  = 25;
   DEBUG_USER   yLOG_sint     (my.win_x);
   DEBUG_USER   yLOG_sint     (my.win_y);
   my.win_h  = my.wrows *  60.0;
   my.tex_h  = my.trows *  60.0;
   my.win_w  = my.wcols * 300.0;
   my.tex_w  = my.tcols * 300.0;
   DEBUG_USER   yLOG_sint     (my.win_w);
   DEBUG_USER   yLOG_sint     (my.win_h);
   /*---(playing)------------------------*/
   my.action =   0;
   my.incr   = STOP;
   my.move   =   0;
   /*---(complete)-----------------------*/
   DEBUG_USER   yLOG_sexit    (__FUNCTION__);
   return 0;
}

char
format_wideview    (void)
{
   my.win_h  = my.tex_h  =  720 - 15;   /* room for twelve scantasks with gaps      */
   my.win_w  = my.tex_w  = 1260;        /* scancard width times four                */
   my.format = FORMAT_WIDE;
   my.ncols  =   0;
   my.nrows  =  12;
   my.wcols  =   4;
   my.wrows  =  12;
   my.action =   0;
   my.incr   = STOP;
   my.move   =   0;
   return 0;
}

char
format_projects    (void)
{
   my.win_h  = my.tex_h  =  720 - 15;   /* room for twelve scantasks with gaps      */
   my.win_w  = my.tex_w  = 1260;        /* scancard width times four                */
   my.format = FORMAT_PROJECT;
   my.ncols  =   0;
   my.nrows  =  12;
   my.wcols  =   4;
   my.wrows  =  12;
   my.action =   0;
   my.incr   = STOP;
   my.move   =   0;
   return 0;
}

char
format_extra       (void)
{
   my.win_h  = my.tex_h  =  960 - 15;   /* room for twelve scantasks with gaps      */
   my.win_w  = my.tex_w  = 1260;        /* scancard width times four                */
   my.format = FORMAT_EXTRA;
   my.ncols  =   0;
   my.nrows  =  16;
   my.wcols  =   4;
   my.wrows  =  16;
   my.action =   0;
   my.incr   = STOP;
   my.move   =   0;
   return 0;
}

char
FORMAT_refresh          (void)
{
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_enter    (__FUNCTION__);
   switch (my.format) {
   case FORMAT_TICKER  : case FORMAT_BASELINE :
      format_ticker   (my.format);
      break;
   case FORMAT_RSHORT  : case FORMAT_RLONG  : case FORMAT_LSHORT : case FORMAT_LLONG  :
      format_column   ();
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
   }
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
   char        t           [LEN_HUND]   = "[]";
   char        s           [LEN_HUND]   = "[]";
   /*---(overall)------------------------*/
   strcpy (unit_answer, "FORMAT           : question not understood");
   if      (strcmp (a_question, "count"         ) == 0) {
      snprintf (unit_answer, LEN_FULL, "DATA count       : %d", g_ntask);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


