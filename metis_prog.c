/*============================----beg-of-source---============================*/
#include   "metis.h"

/*===[[ METIS BACKLOG ]]======================================================*
 *
 */


char      win_title[100]    = "metis_tasklist";


tMY         my;

int         max_disp   = 16;


char        one [20] = "all";

uchar        g_mode    = ' ';
uchar        g_major   = ' ';
uchar        g_minor   = ' ';



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
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(log header)---------------------*/
   DEBUG_PROG   yLOG_info     ("purpose" , P_PURPOSE);
   DEBUG_PROG   yLOG_info     ("namesake", P_NAMESAKE);
   DEBUG_PROG   yLOG_info     ("heritage", P_HERITAGE);
   DEBUG_PROG   yLOG_info     ("imagery" , P_IMAGERY);
   DEBUG_PROG   yLOG_info     ("gyges"   , PROG_version    ());
   DEBUG_PROG   yLOG_info     ("yURG"    , yURG_version    ());
   DEBUG_PROG   yLOG_info     ("ySTR"    , ySTR_version    ());
   DEBUG_PROG   yLOG_info     ("yLOG"    , yLOGS_version   ());
   DEBUG_PROG   yLOG_info     ("yVIKEYS" , yVIKEYS_version ());
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter    (__FUNCTION__);
   /*---(set globals)--------------------*/
   FILTER_clear ();
   my.sort    = '-';
   my.order   = 'a';
   my.ncols   = 0;
   my.nrows   = 0;
   my.source  = DATA_PIPE;
   my.format  = FORMAT_COLUMN;
   my.quit    = '-';
   my.trouble = '-';
   /*---(yvikeys config)-----------------*/
   rc = yVIKEYS_init   ();
   rc = yVIKEYS_whoami ("metis", "tasks", P_VERNUM, P_VERTXT, "/usr/local/bin/metis", "task consolitation, visualization, and navigation");
   DATA_init ();
   FILTER_init ();
   FORMAT_init ();
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
   /*---(overall)------------------------*/
   DEBUG_PROG   yLOG_exit     (__FUNCTION__);
   return 0;
}

char             /* [------] drive program setup activities ------------------*/
PROG_final         (void)
{
   char rc;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter    (__FUNCTION__);
   DATA_refresh   ();
   SORT_refresh   ();
   FILTER_refresh ();
   if (g_ntask <= 0) {
      printf ("no data found\n");
      return -1;
   }
   /*> task_list ();                                                                  <*/
   rc = daemon (1, 0);
   if (rc != 0) return rc;
   /*---(open window)---------------------------*/
   yVIKEYS_view_config   ("metis_tasklist", P_VERNUM, YVIKEYS_OPENGL, 300, 60 * 12, 0);
   yVIKEYS_view_setup    (YVIKEYS_MAIN , YVIKEYS_FLAT, YVIKEYS_TOPLEF,   0, 300, -60 * 12, 60 * 12, 0, 0, 0, OPENGL_show);
   yVIKEYS_view_setup    (YVIKEYS_FLOAT, YVIKEYS_FLAT, YVIKEYS_BOTLEF,  10, 280, -35     , 20     , 0, 0, 0, NULL);
   yVIKEYS_view_setup    (YVIKEYS_MENUS, YVIKEYS_FLAT, YVIKEYS_BOTLEF,  10, 280, -270    , 200    , 0, 0, 0, NULL);
   yVIKEYS_cmds_direct   (":layout min");
   yVIKEYS_cmds_direct   (":title disable");
   yVIKEYS_cmds_direct   (":version disable");
   yVIKEYS_cmds_direct   (":buffer disable");
   yVIKEYS_cmds_direct   (":formula disable");
   yVIKEYS_cmds_direct   (":nav disable");
   yVIKEYS_cmds_direct   (":alt disable");
   yVIKEYS_cmds_direct   (":progress disable");
   yVIKEYS_cmds_direct   (":status disable");
   yVIKEYS_cmds_direct   (":keys disable");
   yVIKEYS_cmds_direct   (":command disable");
   yVIKEYS_cmds_direct   (":details disable");
   yVIKEYS_cmds_direct   (":ribbon disable");
   yVIKEYS_cmds_direct   (":grid disable");
   yVIKEYS_cmds_direct   (":cursor disable");
   yVIKEYS_cmds_direct   (":layers disable");
   yVIKEYS_cmds_direct   (":xaxis disable");
   yVIKEYS_cmds_direct   (":yaxis disable");
   yVIKEYS_view_font     (my.fixed);
   yVIKEYS_map_config    (YVIKEYS_OFFICE, api_yvikeys_mapper, api_yvikeys_locator, api_yvikeys_addressor);
   /*---(create texture)------------------------*/
   font_load ();
   OPENGL_init  ();
   OPENGL_draw  ();
   OPENGL_mask  ();
   /*---(ready display)-------------------------*/
   OPENGL_resize (win_w, win_h);
   prog_signals();
   yVIKEYS_map_refresh ();
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
   /*> yX11_end();                  /+ close window and xwin context            +/    <*/
   yVIKEYS_wrap ();
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
