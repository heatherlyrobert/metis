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
format_streamer    (void)
{
   strcpy(win_title, "metis_streamer");
   win_h  = tex_h  =  (24 * 44) + 45;   /* window height                      */
   win_w  =           300;        /* scancard width                           */
   tex_w  =           300;
   my.format = 's';
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
   if (strchr ("t", a_type) != NULL) {
      strcpy (win_title, "metis_ticker");
      my.format = 't';
      x_wide = 300;
   } else {
      strcpy (win_title, "metis_baseline");
      my.format = 'b';
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
   win_h     = my.wrows *  45.0;
   tex_h     = my.trows *  45.0;
   win_w     = my.wcols * x_wide;
   tex_w     = my.tcols * x_wide;
   /*---(playing)------------------------*/
   my.action =   0;
   my.incr   = STOP;
   my.move   =   0;
   /*---(complete)-----------------------*/
   DEBUG_USER   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
format_column      (char a_side)
{
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_enter    (__FUNCTION__);
   /*---(format)-------------------------*/
   if (strchr ("Rr", a_side) != NULL) {
      strcpy (win_title, "metis_column");
      my.format = 'c';
   } else {
      strcpy (win_title, "metis_sidebar");
      my.format = 'l';
   }
   /*---(cols)---------------------------*/
   my.wcols  =   1;
   my.tcols  =   1;
   my.ncols  =   1;
   /*---(rows)---------------------------*/
   if (strchr ("LR", a_side) == NULL)  my.wrows  =  12;
   else                                my.wrows  =  25;
   my.trows  =  60;
   if (my.nact <= my.trows)  my.nrows = my.nact;
   else                      my.nrows = my.trows;
   /*---(win/tex)------------------------*/
   win_h     = my.wrows *  60.0;
   tex_h     = my.trows *  60.0;
   win_w     = my.wcols * 300.0;
   tex_w     = my.tcols * 300.0;
   /*---(playing)------------------------*/
   my.action =   0;
   my.incr   = STOP;
   my.move   =   0;
   /*---(complete)-----------------------*/
   DEBUG_USER   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
format_wideview    (void)
{
   strcpy(win_title, "metis_wideview");
   win_h  = tex_h  =  720 - 15;   /* room for twelve scantasks with gaps      */
   win_w  = tex_w  = 1260;        /* scancard width times four                */
   my.format = 'w';
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
   strcpy(win_title, "metis_projects");
   win_h  = tex_h  =  720 - 15;   /* room for twelve scantasks with gaps      */
   win_w  = tex_w  = 1260;        /* scancard width times four                */
   my.format = 'p';
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
   strcpy(win_title, "metis_extra");
   win_h  = tex_h  =  960 - 15;   /* room for twelve scantasks with gaps      */
   win_w  = tex_w  = 1260;        /* scancard width times four                */
   my.format = 'x';
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
format_change      (char a_which)
{
   /*---(header)-------------------------*/
   DEBUG_USER   yLOG_enter    (__FUNCTION__);
   /*---(clear out)-----------------------------*/
   texture_free ();
   font_delete  ();
   /*---(update the data)-----------------------*/
   DATA_refresh   ();
   SORT_refresh   ();
   FILTER_refresh ();
   /*---(change format)-------------------------*/
   DEBUG_USER   yLOG_char     ("format"    , a_which);
   switch (a_which) {
   case 't' : format_ticker   ('t');  break;
   case 'b' : format_ticker   ('b');  break;
   case 'c' : format_column   ('r');  break;
   case 'l' : format_column   ('R');  break;
   case '1' : format_column   ('l');  break;
   case '2' : format_column   ('L');  break;
   case 'w' : format_wideview ();     break;
   case 'p' : format_projects ();     break;
   case 'x' : format_extra    ();     break;
   default  :                         break;
   }
   my.ccol = my.pcol = 0;
   my.crow = my.prow = 0;
   /*---(get tasks back)------------------------*/
   DEBUG_USER   yLOG_note     ("format calcs");
   /*> printf("did the formatting\n");                                                <*/
   /*> format_calcs ();                                                               <*/
   /*> printf("did the reading\n");                                                   <*/
   /*---(setup window)--------------------------*/
   DEBUG_USER   yLOG_note     ("yX11_end");
   yX11_end   ();
   DEBUG_USER   yLOG_note     ("yX11_start");
   yX11_start (win_title, win_w, win_h, YX_FOCUSABLE, YX_FIXED, YX_SILENT);
   /*> yXINIT__gdestroy();                                                                                                                                             <* 
    *> XUnmapWindow(DISP, BASE);                                                                                                                                       <* 
    *> XDestroyWindow(DISP, BASE);                                                                                                                                     <* 
    *> if (a_which != '-') {                                                                                                                                           <* 
    *>    XSetWindowAttributes   attr;                                                                                                                                 <* 
    *>    XColor         xc1, xc2;                                                                                                                                     <* 
    *>    attr.colormap  = DefaultColormap(DISP, SCRN);                                                                                                                <* 
    *>    attr.background_pixel = XAllocNamedColor(DISP, CMAP, "black"      ,  &xc1, &xc2);                                                                            <* 
    *>    BASE = XCreateWindow(DISP, ROOT,                                                                                                                             <* 
    *>          0, 0, win_w, win_h, 0,                                                                                                                                 <* 
    *>          CopyFromParent, InputOutput, CopyFromParent,                                                                                                           <* 
    *>          CWBackPixel|CWColormap, &attr);                                                                                                                        <* 
    *>    XStoreName(DISP, BASE, win_title);                                                                                                                           <* 
    *>    XSelectInput(DISP, BASE, KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonMotionMask|ButtonReleaseMask|ExposureMask|StructureNotifyMask|FocusChangeMask);   <* 
    *>    XMapWindow(DISP, BASE);                                                                                                                                      <* 
    *>    XFlush(DISP);                                                                                                                                                <* 
    *>    yXINIT__gsetup();                                                                                                                                            <* 
    *> }                                                                                                                                                               <*/
   /*> printf("did the gsetup\n");                                                    <*/
   /*---(populate the window)-------------------*/
   DEBUG_USER   yLOG_note     ("font load");
   font_load();
   DEBUG_USER   yLOG_note     ("draw init");
   draw_init();
   DEBUG_USER   yLOG_note     ("draw main");
   draw_main();
   DEBUG_USER   yLOG_note     ("draw mask");
   mask();
   DEBUG_USER   yLOG_note     ("defaults");
   my.ccol = 0;
   my.crow = 0;
   draw_resize(win_w, win_h);
   /*> if (strchr("ts", my.format) != 0) my.action = 1;                               <*/
   /*---(complete)------------------------------*/
   DEBUG_USER   yLOG_exit     (__FUNCTION__);
   return 0;
}
