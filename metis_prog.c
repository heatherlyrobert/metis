/*============================----beg-of-source---============================*/
#include   "metis.h"

/*
 * metis  dw2··  make task file name command line configurable
 */


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
char        g_major   = ' ';
char        g_minor   = ' ';

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
   /*---(header)-------------------------*/
   DEBUG_LOOP   yLOG_enter    (__FUNCTION__);
   DEBUG_LOOP   yLOG_char     ("my.format" , my.format);
   DEBUG_LOOP   yLOG_value    ("g_ntask"   , g_ntask);
   DEBUG_LOOP   yLOG_value    ("my.wcols"  , my.wcols);
   DEBUG_LOOP   yLOG_value    ("my.wrows"  , my.wrows);
   DEBUG_LOOP   yLOG_value    ("my.ncols"  , my.ncols);
   DEBUG_LOOP   yLOG_value    ("my.nrows"  , my.nrows);
   DEBUG_LOOP   yLOG_value    ("my.ccol"   , my.ccol);
   DEBUG_LOOP   yLOG_value    ("my.crow"   , my.crow);
   /*---(format constraints)-------------*/
   switch (my.format) {
   case 'c' : case 'l' : case '1' : case '2' :  /* verticals */
      my.ccol = 0;
      if (my.crow >= g_ntask) my.crow = 0;
      if (my.crow <  0      ) my.crow = g_ntask - 1;
      break;
   case 't' : case 'b' : case 'w' : case 'p' : case 'x' :  /* horizontals */
      my.crow = 0;
      if (my.ccol >= g_ntask) my.ccol = 0;
      if (my.ccol <  0      ) my.ccol = g_ntask - 1;
      break;
   }
   /*---(output)-------------------------*/
   DEBUG_LOOP   yLOG_value    ("my.ccol"   , my.ccol);
   DEBUG_LOOP   yLOG_value    ("my.crow"   , my.crow);
   /*---(complete)-----------------------*/
   DEBUG_LOOP   yLOG_exit     (__FUNCTION__);
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
/*===----                            basics                            ----===*/
/*====================------------------------------------====================*/
static void      o___BASICS__________________o (void) {;}

char      verstring    [500];

char*        /*-> return library versio --------------[ leaf   [gs.420.012.00]*/ /*-[00.0000.012.!]-*/ /*-[--.---.---.--]-*/
PROG_version       (void)
{
   char    t [20] = "";
#if    __TINYC__ > 0
   strncpy (t, "[tcc built  ]", 15);
#elif  __GNUC__  > 0
   strncpy (t, "[gnu gcc    ]", 15);
#elif  __CBANG__  > 0
   strncpy (t, "[cbang      ]", 15);
#else
   strncpy (t, "[unknown    ]", 15);
#endif
   snprintf (verstring, 100, "%s   %s : %s", t, P_VERNUM, P_VERTXT);
   return verstring;
}

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
   DEBUG_PROG   yLOG_exit     (__FUNCTION__);
   return 0;
}

char             /* [------] process the command line arguments --------------*/
PROG_args          (int argc, char *argv[])
{
   char     *a         = NULL;         /* current argument                    */
   int       i         = 0;            /* loop iterator -- arguments          */
   int       len       = 0;            /* argument length                     */
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter    (__FUNCTION__);
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
   DEBUG_PROG   yLOG_exit     (__FUNCTION__);
   return 0;
}

char             /* [------] drive program setup activities ------------------*/
PROG_begin         (void)
{
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter    (__FUNCTION__);
   DEBUG_PROG   yLOG_exit     (__FUNCTION__);
   return 0;
}

char             /* [------] drive program setup activities ------------------*/
PROG_final         (void)
{
   char rc;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter    (__FUNCTION__);
   DATA_sources ();
   /*> task_list ();                                                                  <*/
   if (debug_top == 'n')  rc = daemon (1, 0);
   if (rc != 0) return rc;
   /*---(load basics)---------------------------*/
   /*> DATA_read ();                                                                  <*/
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
   DEBUG_PROG   yLOG_exit     (__FUNCTION__);
   return 0;
}

char             /* [------] drive the program closure activities ------------*/
PROG_wrap          (void)
{
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter    (__FUNCTION__);
   font_delete();
   yX11_end();                  /* close window and xwin context            */
   DEBUG_PROG   yLOG_exit     (__FUNCTION__);
   return 0;
}

char             /* [------] drive the program closure activities ------------*/
PROG_end           (void)
{
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter    (__FUNCTION__);
   DEBUG_PROG   yLOG_exit     (__FUNCTION__);
   return 0;
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
