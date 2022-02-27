/*============================----beg-of-source---============================*/
#include   "metis.h"

/*
 * metis § tn4#³ § metis is not terminating properly leaving zombie processes             § M1FDih §
 * metis § tn2r³ § put back data refresh feature to avoid kill/init cycle                 § M1FDij §
 * metis § tn2#· § bring the opengl command bar in as a float                             § M1GLUb §
 * metis § tv2#· § update the opengl menu and get it to work                              § M1GLUc §
 *
 *
 */



tMY         my;
char        g_print     [LEN_RECD] = "";

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



/*====================------------------------------------====================*/
/*===----                       pre-initialization                     ----===*/
/*====================------------------------------------====================*/
static void      o___PREINIT_________________o (void) {;}


char
PROG_urgents            (int a_argc, char *a_argv [])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(initialize)---------------------*/
   yURG_all_mute ();
   rc = yURG_logger  (a_argc, a_argv);
   rc = yURG_urgs    (a_argc, a_argv);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        program startup                       ----===*/
/*====================------------------------------------====================*/
static void      o___STARTUP_________________o (void) {;}

char
PROG_reset_yjobs   (void)
{
   my.run_as   = IAM_METIS;
   my.run_mode = ACT_NONE;
   strcpy (my.run_file, "");
   return 0;
}

char             /* [------] immediate program initialization ----------------*/
PROG__init              (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter    (__FUNCTION__);
   /*---(log header)---------------------*/
   DEBUG_PROG   yLOG_info     ("purpose" , P_PURPOSE);
   DEBUG_PROG   yLOG_info     ("namesake", P_NAMESAKE);
   DEBUG_PROG   yLOG_info     ("heritage", P_HERITAGE);
   DEBUG_PROG   yLOG_info     ("imagery" , P_IMAGERY);
   DEBUG_PROG   yLOG_info     ("gyges"   , PROG_version    ());
   DEBUG_PROG   yLOG_info     ("yURG"    , yURG_version    ());
   DEBUG_PROG   yLOG_info     ("ySTR"    , ySTR_version    ());
   DEBUG_PROG   yLOG_info     ("yLOG"    , yLOGS_version   ());
   DEBUG_PROG   yLOG_info     ("yVIOPEN" , yVIOPENGL_version ());
   DEBUG_PROG   yLOG_info     ("yJOBS"   , yJOBS_version   ());
   /*---(yJOB config)--------------------*/
   PROG_reset_yjobs ();
   my.run_uid     = getuid ();
   my.runtime     = time (NULL);
   DEBUG_TOPS  yLOG_char    ("run_as"    , my.run_as);
   /*---(set globals)--------------------*/
   metis_filter_clear ();
   my.daemon  = 'y';
   my.quick   = '-';
   my.lines   = '-';
   my.sort    = METIS_ORIG;
   NCOLS      = 0;
   NROWS      = 0;
   my.s_wide  = 1366;  /* temp default for my iconia */
   my.s_tall  = 1536;  /* temp default for my iconia */
   my.c_wide  =  300;
   my.r_tall  =   45;
   my.source  = DATA_NONE;
   my.format  = FORMAT_RSHORT;
   format_column ();
   my.quit    = '-';
   my.trouble = '-';
   strlcpy (my.win_title, "metis_tasklist", LEN_DESC);
   my.png     = '-';
   /*---(elements)-----------------------*/
   metis_major_init  ();
   metis_minor_init  ();
   metis_task_init   ();
   metis_source_init ();
   /*---(yvikeys config)-----------------*/
   metis_data_init ();
   metis_filter_init ();
   metis_format_init ();
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit     (__FUNCTION__);
   return 0;
}

char             /* [------] process the command line arguments --------------*/
PROG__args              (int a_argc, char *a_argv [])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char       *a           = NULL;         /* current argument                    */
   char       *b           = NULL;          /* next argument                  */
   int         i           = 0;            /* loop iterator -- arguments          */
   int         l           = 0;            /* argument length                     */
   int         x_total     = 0;
   int         x_args      = 0;
   char        x_mongo     [LEN_TERSE] = "";
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter    (__FUNCTION__);
   if (a_argv > 0)  yJOBS_runas (&(my.run_as), a_argv [0]);
   DEBUG_TOPS  yLOG_char    ("run_as"    , my.run_as);
   for (i = 1; i < a_argc; ++i) {
      /*---(get arg)---------------------*/
      a = a_argv [i];
      if (a == NULL) {
         yURG_err ('f', "arg %d is NULL", i);
         DEBUG_TOPS   yLOG_note    ("FATAL, found a null argument, really bad news");
         DEBUG_TOPS   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      ++x_total;
      /*---(screen urgents)--------------*/
      if (a[0] == '@')  continue;
      /*---(prepare)---------------------*/
      DEBUG_ARGS  yLOG_info    ("cli arg", a);
      ++x_args;
      if (i < a_argc - 1)  b = a_argv [i + 1];
      else                 b = NULL;
      l = strlen(a);
      /*---(statistics filtering)--------*/
      if      (strncmp (a, "-u"          ,  2) == 0 && l == 3)  my.curg  = a[2];
      else if (strncmp (a, "-i"          ,  2) == 0 && l == 3)  my.cimp  = a[2];
      else if (strncmp (a, "-e"          ,  2) == 0 && l == 3)  my.cest  = a[2];
      else if (strncmp (a, "-f"          ,  2) == 0 && l == 3)  my.cprg  = a[2];
      else if (strncmp (a, "-s"          ,  2) == 0 && l == 3)  my.cshr  = a[2];
      /*---(source lines)----------------*/
      else if (strcmp  (a, "--code"          ) == 0)  my.quick  = 'c';
      else if (strcmp  (a, "--unit"          ) == 0)  my.quick  = 'u';
      else if (strcmp  (a, "--date"          ) == 0) {
         if (b == NULL) {
            DEBUG_TOPS  yLOG_note  ("no epoch value included with data");
            DEBUG_TOPS  yLOG_exitr (__FUNCTION__, rce);
            return rce;
         }
         str2mongo (atoi (b), x_mongo);
         printf ("%s\n", x_mongo);
         DEBUG_TOPS  yLOG_exitr (__FUNCTION__, rce);
         return rce;
      }
      /*---(initial format)--------------*/
      else if (strcmp  (a, "--ticker"        ) == 0)  my.format = FORMAT_TICKER;
      else if (strcmp  (a, "--baseline"      ) == 0)  my.format = FORMAT_BASELINE;
      else if (strcmp  (a, "--short"         ) == 0)  my.format = FORMAT_RSHORT;
      else if (strcmp  (a, "--lshort"        ) == 0)  my.format = FORMAT_LSHORT;
      else if (strcmp  (a, "--long"          ) == 0)  my.format = FORMAT_RLONG;
      else if (strcmp  (a, "--llong"         ) == 0)  my.format = FORMAT_LLONG;
      else if (strcmp  (a, "--streamer"      ) == 0)  my.format = FORMAT_STREAMER;
      else if (strcmp  (a, "--wide"          ) == 0)  my.format = FORMAT_WIDE;
      else if (strcmp  (a, "--project"       ) == 0)  my.format = FORMAT_PROJECT;
      else if (strcmp  (a, "--extra"         ) == 0)  my.format = FORMAT_EXTRA;
      /*---(task/data source)------------*/
      else if (strcmp  (a, "--master"        ) == 0)  my.source = DATA_MASTER;
      else if (strcmp  (a, "--sources"       ) == 0)  my.source = DATA_SOURCES;
      /*---(control)---------------------*/
      else if (strcmp  (a, "--pngonly"       ) == 0)  my.png    = 'y';
      else if (strcmp  (a, "--pngalso"       ) == 0)  my.png    = '+';
      else if (strcmp  (a, "--lines"         ) == 0)  my.lines  = 'y';
      else if (strcmp  (a, "--foreground"    ) == 0)  my.daemon = '-';
      /*---(file)------------------------*/
      else if (strncmp (a, "-"           ,  1) != 0)  {
         strncpy (my.file, a, LEN_RECD);
         my.source = DATA_CUSTOM;
      }
      /*---(unknown)---------------------*/
      else {
         rc = yJOBS_args_handle (&(my.run_as), &(my.run_mode), my.run_file, &i, a, b);
         if (rc < 0) {
            DEBUG_TOPS  yLOG_note  ("argument not understood");
            DEBUG_TOPS  yLOG_exitr (__FUNCTION__, rc);
            return rc;
         }
      }
   }
   FORMAT_refresh ();
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit     (__FUNCTION__);
   return 0;
}

char             /* [------] drive program setup activities ------------------*/
PROG__begin             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   long        x_now       =    0;
   char        x_mongo     [LEN_TERSE] = "";
   long        x_back      =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter    (__FUNCTION__);
   /*---(quick line creation)------------*/
   --rce;  if (my.quick != '-') {
      x_now = time (NULL);
      str2mongo (x_now, x_mongo);
      if      (my.quick == 'u')  printf ("#> ");
      else                       printf (" * ");
      /*> str4mongo (x_mongo, &x_back);                                               <*/
      printf ("metis § ····· § tbd                                                                    § %6s §\n", x_mongo);
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(process data)-------------------*/
   /*> rc = metis_data_refresh   ();                                                  <*/
   DEBUG_PROG   yLOG_value    ("data"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*> rc = SORT_refresh   ();                                                        <*/
   DEBUG_PROG   yLOG_value    ("sort"      , rc);
   /*> rc = metis_filter_set ();                                                        <*/
   DEBUG_PROG   yLOG_value    ("filter"    , rc);
   /*> --rce;  if (g_ntask <= 0) {                                                    <* 
    *>    DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);                             <* 
    *>    return rce;                                                                 <* 
    *> }                                                                              <*/
   /*> rc = FORMAT_refresh ();                                                        <*/
   DEBUG_PROG   yLOG_value    ("format"    , rc);
   /*---(overall)------------------------*/
   DEBUG_PROG   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
PROG_startup            (int a_argc, char *a_argv [])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   yURG_stage_check (YURG_BEG);
   DEBUG_TOPS  yLOG_enter   (__FUNCTION__);
   /*---(initialize)---------------------*/
   if (rc >= 0) {
      rc = PROG__init    ();
      DEBUG_TOPS  yLOG_value   ("init"      , rc);
   }
   /*---(arguments)----------------------*/
   if (rc >= 0) {
      rc = PROG__args    (a_argc, a_argv);
      DEBUG_TOPS  yLOG_value   ("args"      , rc);
   }
   /*---(begin)--------------------------*/
   if (rc >= 0) {
      rc = PROG__begin   ();
      DEBUG_TOPS  yLOG_value   ("args"      , rc);
   }
   /*---(complete)-----------------------*/
   DEBUG_TOPS  yLOG_exit  (__FUNCTION__);
   yURG_stage_check (YURG_MID);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                        program execution                     ----===*/
/*====================------------------------------------====================*/
static void      o___EXECUTION_______________o (void) {;}

char             /* [------] drive program setup activities ------------------*/
PROG_dawn          (void)
{
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter    (__FUNCTION__);
   rc = yVIOPENGL_init ("metis-okeanides", P_VERNUM, MODE_MAP, my.w_wide, my.w_tall);
   DEBUG_PROG   yLOG_value    ("yVIOPENGL" , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*> rc = FILE_init      ();                                                        <*/
   /*> rc = yFILE_whoami   (P_FULLPATH, P_VERNUM, P_VERTXT, P_ONELINE, P_SUFFIX, P_CONTENT, NULL, NULL, NULL);   <*/
   /*> rc = yVIKEYS_whoami ("metis", "tasks", P_VERNUM, P_VERTXT, "/usr/local/bin/metis", "task consolitation, visualization, and navigation");   <*/
   if (my.daemon == 'y') {
      rc = daemon (1, 0);
      if (rc != 0) return rc;
   }
   /*---(open window)---------------------------*/
   api_yvikeys_init      ();
   /*---(create texture)------------------------*/
   metis_opengl_font_load ();
   rc = yVIEW_full (YVIEW_MAIN , YVIEW_FLAT, YVIEW_TOPLEF,  1, 0, metis_opengl_show);
   DEBUG_PROG   yLOG_value    ("MAIN"      , rc);
   yCMD_direct (":layout  min");
   yCMD_direct (":title   disable");
   yCMD_direct (":version disable");
   yCMD_direct (":formula disable");
   yCMD_direct (":nav     disable");
   yCMD_direct (":details disable");
   yCMD_direct (":ribbon  disable");
   yCMD_direct (":keys    disable");
   yCMD_direct (":menus   k");
   yCMD_direct (":float   t");
   metis_opengl_init  ();
   metis_opengl_draw  ();
   metis_opengl_show  ();
   metis_opengl_mask  ();
   /*> yVIKEYS_cmds_direct   (":window col_rig");                                     <* 
    *> yVIKEYS_menu_add ("µv?u", "urgent"    , ":help u¦");                           <* 
    *> yVIKEYS_menu_add ("µv?i", "important" , ":help i¦");                           <* 
    *> yVIKEYS_menu_add ("µv?e", "estimate"  , ":help e¦");                           <* 
    *> yVIKEYS_menu_add ("µv?p", "progress"  , ":help p¦");                           <* 
    *> yVIKEYS_menu_add ("µv?f", "full"      , ":help f¦");                           <* 
    *> yVIKEYS_map_refresh ();                                                        <*/
   /*---(load commands/menus)-------------------*/
   metis_data_vikeys   ();
   metis_filter_vikeys ();
   metis_format_vikeys ();
   /*---(ready display)-------------------------*/
   /*> OPENGL_resize (my.w_wide, my.w_tall);                                            <*/
   prog_signals();
   api_yvikeys_refresh ();
   api_yvikeys__resize ('-');
   yVIEW_debug_list ();
   /*---(complete)------------------------------*/
   DEBUG_PROG   yLOG_exit     (__FUNCTION__);
   return 0;
}

char             /* [------] drive the program closure activities ------------*/
PROG_dusk               (void)
{
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter    (__FUNCTION__);
   metis_opengl_font_close ();
   /*> yX11_end();                  /+ close window and xwin context            +/    <*/
   yVIOPENGL_wrap ();
   DEBUG_PROG   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
PROG__verify            (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG    yLOG_enter   (__FUNCTION__);
   /*---(title)--------------------------*/
   yURG_msg ('>', "  option --vverify, check current project suitability for usage");
   yURG_msg (' ', "");
   /*---(call action)--------------------*/
   rc = metis_data_refresh ();
   /*---(failure)------------------------*/
   if (rc < 0) {
      yURG_msg (' ', "");
      if (my.run_mode == ACT_CVERIFY )   yURG_msg_live ();
      if (my.run_mode == ACT_CVERIFY )   yURG_msg ('>', "FAILURE, source contains hard failures, run --vverify to identify reasons");
      if (my.run_mode == ACT_VVERIFY )   yURG_msg ('>', "FAILURE, source contains hard failures, the reasons are shown above");
      if (my.run_mode == ACT_CVERIFY )   yURG_msg_mute ();
      DEBUG_PROG    yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(warning)------------------------*/
   if (rc > 0) {
      yURG_msg (' ', "");
      if (my.run_mode == ACT_CVERIFY )   yURG_msg_live ();
      if (my.run_mode == ACT_CVERIFY )   yURG_msg ('>', "WARNING, source can be used with minor issues, run --vverify to identify warnings");
      if (my.run_mode == ACT_VVERIFY )   yURG_msg ('>', "WARNING, source can be used with minor issues, the warnings are shown above");
      if (my.run_mode == ACT_CVERIFY )   yURG_msg_mute ();
      DEBUG_PROG    yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(success)------------------------*/
   yURG_msg (' ', "");
   IF_CONFIRM  yURG_msg_live ();
   yURG_msg ('>', "SUCCESS, source is suitable for inclusion in database");
   /*---(complete)-----------------------*/
   DEBUG_PROG    yLOG_exit    (__FUNCTION__);
   return rc;
}

char
PROG_dispatch           (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG    yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   yJOBS_final (my.run_uid);
   /*---(title)--------------------------*/
   IF_VERBOSE   yURG_msg ('>', "%s", P_ONELINE);
   /*---(route action)-------------------*/
   --rce;  switch (my.run_mode) {
      /*---(basic)-----------------------*/
   case ACT_STATS       :
      /*> rc = PROG__stats    ();                                                     <*/
      break;
      /*---(incomming)-------------------*/
   case CASE_VERIFY     :
      rc = PROG__verify   ();
      break;
   case CASE_REGISTER   :
      /*> rc = PROG__register ();                                                     <*/
      break;
   case CASE_UPDATE     :
      /*> rc = PROG__update   ();                                                     <*/
      break;
   case CASE_INSTALL    :
      /*> rc = PROG__install  ();                                                     <*/
      break;
      /*---(outgoing)--------------------*/
   case CASE_WITHDRAW   :
      /*> rc = PROG__withdraw ();                                                     <*/
      break;
   case CASE_CLEAR      :
      /*> rc = PROG__clear    ();                                                     <*/
      break;
   case CASE_REMOVE     :
      /*> rc = PROG__remove   ();                                                     <*/
      break;
      /*---(central)---------------------*/
   case CASE_REPORT     :
      rc = metis_reporter   ();
      break;
   case CASE_AUDIT      :
      /*> rc = PROG__audit    ();                                                     <*/
      break;
      /*---(execute)---------------------*/
   case CASE_GATHER     :
      /*> rc = PROG__system   ();                                                     <*/
      break;
   case CASE_NORMAL     : case CASE_STRICT     :
      rc = PROG_dawn    ();
      DEBUG_PROG    yLOG_value   ("dawn"      , rc);
      if (rc < 0)  break;
      if (my.png != 'y')   rc = yVIOPENGL_main  ("keys", "every", NULL);
      DEBUG_PROG    yLOG_value   ("main"      , rc);
      rc = PROG_dusk   ();
      DEBUG_PROG    yLOG_value   ("dusk"      , rc);
      break;
      /*---(trouble)---------------------*/
   default              :
      rc = rce;
      break;
   }
   /*---(cut-off)------------------------*/
   yURG_all_mute ();
   /*---(complete)-----------------------*/
   DEBUG_PROG    yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                        program shutdown                      ----===*/
/*====================------------------------------------====================*/
static void      o___SHUTDOWN________________o (void) {;}

char             /* [------] drive the program closure activities ------------*/
PROG__end               (void)
{
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter    (__FUNCTION__);
   metis_data_purge_all ();
   DEBUG_PROG   yLOG_exit     (__FUNCTION__);
   return 0;
}

char             /* [------] drive the program closure activities ------------*/
PROG_shutdown           (void)
{
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter    (__FUNCTION__);
   PROG__end ();
   DEBUG_PROG   yLOG_exit     (__FUNCTION__);
   DEBUG_TOPS   yLOGS_end    ();
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
   PROG_urgents  (x_argc, x_args);
   PROG_startup  (x_argc, x_args);
   return 0;
}

char         /*-> set up programgents/debugging ------[ light  [uz.320.011.05]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
PROG__unit_loud         (void)
{
   int         x_argc      = 3;
   char       *x_args [20] = { "metis_unit", "@@kitchen", "@@sort"  };
   PROG_urgents  (x_argc, x_args);
   PROG_startup  (x_argc, x_args);
   return 0;
}

char         /*-> set up program urgents/debugging ---[ light  [uz.210.001.01]*/ /*-[00.0000.00#.!]-*/ /*-[--.---.---.--]-*/
PROG__unit_end          (void)
{
   PROG_shutdown ();
   return 0;
}






/*============================----end-of-source---============================*/
