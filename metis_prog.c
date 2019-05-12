/*============================----beg-of-source---============================*/
#include   "metis.h"

/*
 * metis  dw2#·  make task file name command line configurable
 */


char      win_title[100]    = "metis_column";


tMY         my;

int         max_disp   = 16;


char        one [20] = "all";

char        g_major   = ' ';
char        g_minor   = ' ';



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
      if (my.crow >= my.nrows) my.crow = 0;
      if (my.crow <  0       ) my.crow = my.nrows - 1;
      break;
   case 't' : case 'b' : case 'w' : case 'p' : case 'x' :  /* horizontals */
      my.crow = 0;
      if (my.ccol >= my.ncols) my.ccol = 0;
      if (my.ccol <  0       ) my.ccol = my.ncols - 1;
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
   DATA_refresh ();
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
   FILTER_clear ();
   my.sort   = '-';
   my.ncols  = 0;
   my.nrows  = 0;
   DATA_init ();
   my.source = DATA_PIPE;
   my.format = FORMAT_COLUMN;
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
      if (a[0] == '@')  continue;
      len = strlen(a);
      /*---(statistics filtering)--------*/
      if      (strncmp (a, "-u"          ,  2) == 0 && len == 3)  my.curg  = a[2];
      else if (strncmp (a, "-i"          ,  2) == 0 && len == 3)  my.cimp  = a[2];
      else if (strncmp (a, "-e"          ,  2) == 0 && len == 3)  my.cest  = a[2];
      else if (strncmp (a, "-f"          ,  2) == 0 && len == 3)  my.cflg  = a[2];
      /*---(initial format)--------------*/
      else if (strcmp  (a, "--ticker"        ) == 0)  format_ticker ('t');
      else if (strcmp  (a, "--baseline"      ) == 0)  format_ticker ('b');
      else if (strcmp  (a, "--column"        ) == 0)  format_column ('r');
      else if (strcmp  (a, "-1"              ) == 0)  format_column ('l');
      else if (strcmp  (a, "--long"          ) == 0)  format_column ('R');
      else if (strcmp  (a, "-2"              ) == 0)  format_column ('L');
      else if (strcmp  (a, "--wide"          ) == 0)  format_wideview();
      else if (strcmp  (a, "--extra"         ) == 0)  format_extra();
      /*---(task/data source)------------*/
      else if (strcmp  (a, "--master"        ) == 0)  my.source = DATA_MASTER;
      else if (strcmp  (a, "--source"        ) == 0)  my.source = DATA_SOURCES;
      else if (strcmp  (a, "--code"          ) == 0)  my.source = DATA_SOURCES;
      else if (strncmp (a, "-"           ,  1) != 0)  {
         strncpy (my.file, a, LEN_RECD);
         my.source = DATA_CUSTOM;
      }
      /*---(unknown)---------------------*/
      else   printf("arg <%s> not understood\n", a);
   }
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
   rc = DATA_refresh ();
   if (g_ntask <= 0) {
      printf ("no data found\n");
      return -1;
   }
   /*> task_list ();                                                                  <*/
   rc = daemon (1, 0);
   if (rc != 0) return rc;
   /*---(open window)---------------------------*/
   yX11_start (win_title, win_w, win_h, YX_FOCUSABLE, YX_FIXED, YX_SILENT);
   /*---(create texture)------------------------*/
   font_load ();
   draw_init ();
   draw_main();
   mask();
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
      /*> DEBUG_T  printf ("SIGNAL : SIGHUP  means refresh tasks from metis.tasks\n");   <*/
      my.sighup  = 1;
      my.update  = 1;
      break;
   case  SIGUSR2:
      /*> DEBUG_T  printf ("SIGNAL : SIGUSR2 means to jumble the font\n");            <*/
      my.sigusr2 = 1;
      my.update  = 1;
      break;
   case  SIGTERM:
      /*> DEBUG_T  printf ("SIGNAL : SIGTERM means forced terminate\n");              <*/
      exit (-3);
      break;
   case  SIGSEGV:
      /*> DEBUG_T  printf ("SIGNAL : SIGSEGV means program blew up\n");               <*/
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
   int         x_argc      = 3;
   char       *x_args [20] = { "metis_unit", "@@kitchen", "@@sort"  };
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
