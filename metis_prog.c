/*============================----beg-of-source---============================*/

#include   "metis.h"

char      win_title[100]    = "metis_column";


tMY         my;

int         max_disp   = 16;


char        one [20] = "all";
char        urg   = ' ';
char        imp   = ' ';
char        est   = ' ';
char        flg   = ' ';

char        arg_heads  = 0;
char        arg_filter = 1;
char        arg_keys   = ' ';

long        gpu_mem_bef = 0;
long        gpu_mem_aft = 0;

char        debug_top    = 'n';
char        debug_event  = 'n';
char        debug_mem    = 'n';
char        debug_args   = 'n';
char        debug_input  = 'n';
char        debug_graph  = 'n';

/*> char      format       = 't';                                                     <*/


/*====================------------------------------------====================*/
/*===----                           utility                            ----===*/
/*====================------------------------------------====================*/
static void      o___UTILITY_________________o (void) {;}



#define VBO_FREE_MEMORY_ATI          0x87FB
#define TEXTURE_FREE_MEMORY_ATI      0x87FC
#define RENDERBUFFER_FREE_MEMORY_ATI 0x87FD

long
ati_meminfo (void)
{
   GLint param[4];
   glGetIntegerv (TEXTURE_FREE_MEMORY_ATI, param);
   return param[0];
}

long
time_stamp()                      /* PURPOSE : timestamp in microseconds      */
{
   /* second
    * millisecond  ms  0.001 sec
    * microsecond  us  0.000001 sec
    * nanosecond   ns  0.000000001 sec
    * picosecond   ps  0.000000000001 sec
    */
   long             tint = 0;
   struct timeval   tv;
   gettimeofday(&tv, NULL);
   tint += (int) (tv.tv_sec * 1000);
   tint += (int) tv.tv_usec / 1000;
   return tint;
}

char          /*----: keep row and column values in correct ranges -----------*/
range_check        (void)
{
   /*---(format constraints)-------------*/
   if (strchr("tbwpx"  , my.format) != 0)  my.crow = 0;
   if (strchr("cl12"   , my.format) != 0)  my.ccol = 0;
   /*---(range wrapping)-----------------*/
   if (my.ccol >= my.ncols) my.ccol = 0;
   if (my.ccol <  0       ) my.ccol = my.ncols - 1;
   if (my.crow >= my.nrows) my.crow = 0;
   if (my.crow <  0       ) my.crow = my.nrows - 1;
   /*---(output)-------------------------*/
   /*> printf ("ccol %2d of %3d, crow %2d of %3d, ctask %3d of %3d\n",                <* 
    *>       my.ccol, my.ncols, my.crow, my.nrows, ctask, g_ntask);              <*/
   /*---(complete)-----------------------*/
   return 0;
}



/*============================--------------------============================*/
/*===----                             tasks                            ----===*/
/*============================--------------------============================*/
static void      o___TASKS___________________o (void) {;}

char
task_mini          (void)
{
   texture_free();
   draw_init();
   draw_main();
   mask();
   my.ccol = 0;
   my.crow = 0;
   draw_resize(win_w, win_h);
   return 0;
}

char
task_refresh       (void)
{
   /*---(tear down)----------------------*/
   texture_free();
   font_delete();
   yXINIT__gdestroy();
   yXINIT__gsetup();
   /*---(build back up)------------------*/
   DATA_init ();
   DATA_read ();
   font_load();
   draw_init();
   draw_main();
   mask();
   my.ccol = 0;
   my.crow = 0;
   draw_resize(win_w, win_h);
   return 0;
}


/*====================------------------------------------====================*/
/*===----                          polymorphism                        ----===*/
/*====================------------------------------------====================*/
static void      o___POLYMORPH_______________o (void) {;}

char
format_calcs       (void)
{
   /*---(update format)-------------------------*/
   COLUMN {
      tex_h     = 60 * nactive;
      my.ncols  = 1;
      my.nrows  = nactive;
   } else LONG {
      tex_h     = 60 * nactive;
      my.ncols  = 1;
      my.nrows  = nactive;
   } else STREAMER {
      tex_h     = 44 * nactive + 45;
      my.ncols  = 1;
      my.nrows  = nactive;
      my.change =  (1.000 / nactive) * 0.005;
      my.play   =  - my.change;
      my.mspeed =  3.0 * my.change;
   } else TICKER {
      nactive = 11;
      my.ncols  = 10;
      my.nrows  = 1;
      /*> my.play   = -0.0001;                                                        <* 
       *> my.mspeed =  0.0005;                                                        <* 
       *> my.change =  0.0001;                                                        <*/
      my.play   = -0.0001;
      my.mspeed =  0.0005;
      my.change =  0.0001;
   } else BASELINE {
      nactive = 11;
      my.ncols  = 10;
      my.nrows  = 1;
   } else WIDEVIEW {
      my.nrows  = 12;
      my.ncols  = nactive / my.nrows;
      if (nactive % my.nrows > 0) ++my.ncols;
      if (my.ncols < 4) my.ncols = 4;
      if (my.ncols > MAX_COLS) my.ncols = MAX_COLS;
      tex_w = 320 * my.ncols;
   } else PROJECT {
      my.nrows = 12;
      if (my.ncols < 4) my.ncols = 4;
      if (my.ncols > MAX_COLS) my.ncols = MAX_COLS;
      tex_w = 320 * my.ncols;
   } else EXTRA {
      my.nrows = 16;
      my.ncols = nactive / my.nrows;
      if (nactive % my.nrows > 0) ++my.ncols;
      if (my.ncols < 4) my.ncols = 4;
      if (my.ncols > MAX_COLS) my.ncols = MAX_COLS;
      tex_w = 320 * my.ncols;
   }
   /*---(set step)------------------------------*/
   step  = 1.0 / (float) nactive;
   if      (strchr("tb"     , my.format) != 0) step  = 1.0 / 10.0;
   else if (strchr("wpx"    , my.format) != 0) step  = 1.0 / my.ncols;
   my.ccol = 0;
   my.crow = 0;
   DEBUG_I  printf("   format %c so nactive = %d, ncols = %d, nrows = %d\n", my.format, nactive, my.ncols, my.nrows);
   DEBUG_I  printf("      tex_h = %d, tex_w = %d, step = %f\n", tex_h, tex_w, step);
   DEBUG_I  printf("      win_h = %d, win_w = %d\n", win_h, win_w);
   return 0;
}




char
format_streamer    (void)
{
   strcpy(win_title, "metis_streamer");
   win_h  = tex_h  =  (35 * 44) + 45;   /* window height                      */
   win_w  =           300;        /* scancard width                           */
   tex_w  =           300;        /* room for ten tasks                       */
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
format_ticker      (void)
{
   strcpy(win_title, "metis_ticker");
   win_h  = tex_h  =   45;        /* scancard height                          */
   win_w  =          1600;        /* our standard screen size                 */
   tex_w  =          3000;        /* room for ten tasks                       */
   my.format = 't';
   my.nrows  =   1;
   my.ncols  =  10;
   my.wcols  =   4;
   my.wrows  =   1;
   my.action =   1;
   my.incr   = STOP;
   my.move   =   0;
   return 0;
}

char
format_baseline    (void)
{
   strcpy(win_title, "metis_baseline");
   win_h  = tex_h  =   45;        /* scancard height                          */
   win_w  =          1600;        /* our standard screen size                 */
   tex_w  =          3250;        /* room for ten tasks                       */
   my.format = 'b';
   my.nrows  =   1;
   my.ncols  =  10;
   my.wcols  =   4;
   my.wrows  =   1;
   my.action =   0;
   my.incr   = STOP;
   my.move   =   0;
   return 0;
}

char
format_column      (char a_side)
{
   if (a_side == 'r') strcpy(win_title, "metis_column");
   else               strcpy(win_title, "metis_sidebar");
   win_h  = tex_h  =  720 - 15;   /* room for twelve scantasks with gaps      */
   win_w  = tex_w  =  300;        /* scancard width                           */
   my.format = 'c';
   my.ncols  =   1;
   my.nrows  =   0;
   my.wcols  =   1;
   my.wrows  =  12;
   my.action =   0;
   my.incr   = STOP;
   my.move   =   0;
   return 0;
}

char
format_long        (char a_side)
{
   if (a_side == 'r') strcpy(win_title, "metis_column");
   else               strcpy(win_title, "metis_sidebar");
   win_h  = tex_h  =  960 - 15;   /* room for twelve scantasks with gaps      */
   win_w  = tex_w  =  300;        /* scancard width                           */
   my.format = 'l';
   my.ncols  =   1;
   my.nrows  =   0;
   my.wcols  =   1;
   my.wrows  =  16;
   my.action =   0;
   my.incr   = STOP;
   my.move   =   0;
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
   arg_heads = 1;
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
   printf ("format_change (%c)\n", a_which);
   /*---(clear out)-----------------------------*/
   texture_free();
   font_delete();
   /*---(change format)-------------------------*/
   switch (a_which) {
   case 's' : format_streamer ();     break;
   case 't' : format_ticker   ();     break;
   case 'b' : format_baseline ();     break;
   case 'c' : format_column   ('r');  break;
   case '1' : format_column   ('l');  break;
   case 'l' : format_long     ('r');  break;
   case '2' : format_long     ('l');  break;
   case 'w' : format_wideview ();     break;
   case 'p' : format_projects ();     break;
   case 'x' : format_extra    ();     break;
   default  :                         break;
   }
   /*---(get tasks back)------------------------*/
   /*> printf("did the formatting\n");                                                <*/
   /*> DATA_read    ();                                                               <*/
   format_calcs ();
   /*> printf("did the reading\n");                                                   <*/
   /*---(setup window)--------------------------*/
   yX11_end   ();
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
   font_load();
   /*> printf("did the font_load\n");                                                 <*/
   draw_init();
   /*> printf("did the init\n");                                                      <*/
   draw_main();
   /*> printf("did the draw_main\n");                                                 <*/
   mask();
   /*> printf("did the mask\n");                                                      <*/
   my.ccol = 0;
   my.crow = 0;
   draw_resize(win_w, win_h);
   gpu_mem_aft = ati_meminfo();
   /*> if (strchr("ts", my.format) != 0) my.action = 1;                               <*/
   DEBUG_M printf("gpu memory used %ldm of %ldm \n", (gpu_mem_bef - gpu_mem_aft) / 1000, gpu_mem_bef / 1000);
   /*---(complete)------------------------------*/
   return 0;
}


/*====================------------------------------------====================*/
/*===----                            basics                            ----===*/
/*====================------------------------------------====================*/
static void      o___BASICS__________________o (void) {;}

char             /* [------] immediate program initialization ----------------*/
PROG_init          (void)
{
   /*---(log header)---------------------*/
   DEBUG_PROG   yLOG_info     ("purpose" , P_PURPOSE);
   DEBUG_PROG   yLOG_info     ("namesake", P_NAMESAKE);
   DEBUG_PROG   yLOG_info     ("heritage", P_HERITAGE);
   DEBUG_PROG   yLOG_info     ("imagery" , P_IMAGERY);
   DEBUG_PROG   yLOG_info     ("gyges"   , PROG_version    ());
   DEBUG_PROG   yLOG_info     ("yURG"    , yURG_version    ());
   DEBUG_PROG   yLOG_info     ("ySTR"    , ySTR_version    ());
   DEBUG_PROG   yLOG_info     ("yLOG"    , yLOGS_version   ());
   DEBUG_PROG   yLOG_info     ("yRPN"    , yRPN_version    ());
   DEBUG_PROG   yLOG_info     ("yVIKEYS" , yVIKEYS_version ());
   DEBUG_PROG   yLOG_info     ("yCALC"   , yCALC_version   ());
   DEBUG_PROG   yLOG_info     ("yPARSE"  , yPARSE_version  ());
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter    (__FUNCTION__);
   /*---(set globals)--------------------*/
   my.ncols  = 0;
   my.nrows  = 0;
   my.xrow   = 0;
   g_ntask   = 0;
   nactive   = 0;
   ctask     = 0;
   DATA_init ();
   /*---(complete)-----------------------*/
   return 0;
}

char             /* [------] process the command line arguments --------------*/
PROG_args          (int argc, char *argv[])
{
   char     *a         = NULL;         /* current argument                    */
   int       i         = 0;            /* loop iterator -- arguments          */
   int       len       = 0;            /* argument length                     */
   format_column('r');
   for (i = 1; i < argc; ++i) {
      a = argv[i];
      len = strlen(a);
      if      (strncmp(a, "@m",     5) == 0)     debug_top   = debug_mem   = 'y';
      else if (strncmp(a, "@e",     5) == 0)     debug_top   = debug_event = 'y';
      else if (strncmp(a, "@i",     5) == 0)     debug_top   = debug_input = 'y';
      else if (strncmp(a, "@a",     5) == 0)     debug_top   = debug_args  = 'y';
      else if (strncmp(a, "@g",     5) == 0)     debug_top   = debug_graph = 'y';
      else if (strncmp(a, "-u",     2) == 0 && len == 3)       urg         = a[2];
      else if (strncmp(a, "-i",     2) == 0 && len == 3)       imp         = a[2];
      else if (strncmp(a, "-e",     2) == 0 && len == 3)       est         = a[2];
      else if (strncmp(a, "-f",     2) == 0 && len == 3)       flg         = a[2];
      else if (strncmp(a, "-t"     , 7) == 0)     format_ticker();
      else if (strncmp(a, "--tic"  , 7) == 0)     format_ticker();
      else if (strncmp(a, "-b"     , 7) == 0)     format_baseline();
      else if (strncmp(a, "--base" , 7) == 0)     format_baseline();
      else if (strncmp(a, "-c"     , 7) == 0)     format_column('r');
      else if (strncmp(a, "--col"  , 7) == 0)     format_column('r');
      else if (strncmp(a, "-1"     , 7) == 0)     format_column('l');
      else if (strncmp(a, "-l"     , 7) == 0)     format_long('r');
      else if (strncmp(a, "--long" , 7) == 0)     format_long('r');
      else if (strncmp(a, "-2"     , 7) == 0)     format_long  ('l');
      else if (strncmp(a, "-w"     , 7) == 0)     format_wideview();
      else if (strncmp(a, "--wide" , 7) == 0)     format_wideview();
      else if (strncmp(a, "-x"     , 7) == 0)     format_extra();
      else if (strncmp(a, "--extra", 7) == 0)     format_extra();
      else if (strncmp(a, "-s"     , 7) == 0)     format_streamer();
      else if (strncmp(a, "-stream", 7) == 0)     format_streamer();
      else if (strncmp(a, "-h"     , 7) == 0)     arg_heads = 1;
      else if (strncmp(a, "--heads", 7) == 0)     arg_heads = 1;
      else if (strncmp(a, "-"      , 1) == 0)     printf("arg not understood\n");
      else                                        strncpy(one, a, 20);
   }
   /*---(display args)-------------------*/
   DEBUG_T  printf("\nmetis : task management system\n\n");
      /*> printf("PROG_args()\n");                                                    <* 
       *> printf("   sizes\n");                                                       <* 
       *> printf("      win_w   = %d\n", win_w);                                      <* 
       *> printf("      win_h   = %d\n", win_h);                                      <* 
       *> printf("      tex_w   = %d\n", tex_w);                                      <* 
       *> printf("      tex_h   = %d\n", tex_h);                                      <* 
       *> printf("\n");                                                               <*/
   /*---(complete)-----------------------*/
   return 0;
}

char             /* [------] drive program setup activities ------------------*/
PROG_begin         (void)
{
}

char             /* [------] drive program setup activities ------------------*/
PROG_final         (void)
{
   char rc;
   if (debug_top == 'n')  rc = daemon (1, 0);
   if (rc != 0) return rc;
   /*---(load basics)---------------------------*/
   DATA_read ();
   /*---(open window)---------------------------*/
   yX11_start (win_title, win_w, win_h, YX_FOCUSABLE, YX_FIXED, YX_SILENT);
   /*---(create texture)------------------------*/
   gpu_mem_bef = ati_meminfo();
   font_load ();
   draw_init ();
   draw_main();
   mask();
   gpu_mem_aft = ati_meminfo();
   DEBUG_M printf("gpu memory used %ldm of %ldm \n", (gpu_mem_bef - gpu_mem_aft) / 1000, gpu_mem_bef / 1000);
   /*---(ready display)-------------------------*/
   draw_resize(win_w, win_h);
   prog_signals();
   /*---(complete)------------------------------*/
   return 0;
}

char             /* [------] drive the program closure activities ------------*/
PROG_wrap          (void)
{
   font_delete();
   yX11_end();                  /* close window and xwin context            */
   return 0;
}

char             /* [------] drive the program closure activities ------------*/
PROG_end           (void)
{
}


/*====================------------------------------------====================*/
/*===----                           drivers                            ----===*/
/*====================------------------------------------====================*/
static void      o___DRIVERS_________________o (void) {;}

void               /* PURPOSE : handle signals -------------------------------*/
prog_catch         (int a_signal)
{
   switch (a_signal) {
   case  SIGHUP:
      DEBUG_T  printf ("SIGNAL : SIGHUP  means refresh tasks from metis.tasks\n");
      my.sighup  = 1;
      my.update  = 1;
      break;
   case  SIGUSR2:
      DEBUG_T  printf ("SIGNAL : SIGUSR2 means to jumble the font\n");
      my.sigusr2 = 1;
      my.update  = 1;
      break;
   case  SIGTERM:
      DEBUG_T  printf ("SIGNAL : SIGTERM means forced terminate\n");
      exit (-3);
      break;
   case  SIGSEGV:
      DEBUG_T  printf ("SIGNAL : SIGSEGV means program blew up\n");
      exit (-4);
      break;
   }
   prog_signals();    /* must reset signals after use         */
   /*---(complete)------------------------------*/
   return;
}

char               /* PURPOSE : setup signal handling ------------------------*/
prog_signals       (void)
{
   signal(SIGCHLD,  SIG_IGN);        /* ignore children       */
   signal(SIGTSTP,  SIG_IGN);        /* ignore tty signals    */
   signal(SIGTTOU,  SIG_IGN);        /* ignore tty signals    */
   signal(SIGTTIN,  SIG_IGN);        /* ignore tty signals    */
   signal(SIGHUP,   prog_catch);     /* hangup means refresh  */
   signal(SIGTERM,  prog_catch);     /* catch a kill          */
   signal(SIGSEGV,  prog_catch);     /* catch a segfault      */
   signal(SIGUSR2,  prog_catch);     /* means to jumble       */
   return 0;
}

char         /*-> set up programgents/debugging ------[ light  [uz.320.011.05]*/ /*-[00.0000.00#.#]-*/ /*-[--.---.---.--]-*/
PROG__unit_quiet        (void)
{
   int         x_argc      = 1;
   char       *x_args [1]  = { "metis" };
   yURG_logger (1, x_args);
   PROG_init   ();
   yURG_urgs   (1, x_args);
   PROG_args   (1, x_args);
   PROG_begin  ();
   return 0;
}

char         /*-> set up programgents/debugging ------[ light  [uz.320.011.05]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
PROG__unit_loud         (void)
{
   int         x_argc      = 2;
   char       *x_args [20] = { "metis_unit", "@@kitchen"  };
   yURG_logger (x_argc, x_args);
   PROG_init   ();
   yURG_urgs   (x_argc, x_args);
   PROG_args   (x_argc, x_args);
   PROG_begin  ();
   return 0;
}

char         /*-> set up program urgents/debugging ---[ light  [uz.210.001.01]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
PROG__unit_end          (void)
{
   PROG_end       ();
   return 0;
}






/*============================----end-of-source---============================*/
