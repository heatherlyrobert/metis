/*============================----beg-of-source---============================*/
#include   "metis.h"

/*===[[ METIS BACKLOG ]]======================================================*
 * metis  tn1#-  add menu mode to main loop and global flag
 *
 */


char      win_title[100]    = "metis_tasklist";


tMY         my;

int         max_disp   = 16;


char        one [20] = "all";

uchar        g_mode    = ' ';
uchar        g_major   = ' ';
uchar        g_minor   = ' ';



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
   yGLTEX_free (&g_tex, &g_fbo, &g_dep);
   OPENGL_init ();
   OPENGL_draw ();
   OPENGL_mask ();
   my.ccol = 0;
   my.crow = 0;
   OPENGL_resize (win_w, win_h);
   return 0;
}

char
task_refresh       (void)
{
   /*---(tear down)----------------------*/
   yGLTEX_free (&g_tex, &g_fbo, &g_dep);
   font_delete();
   yXINIT__gdestroy();
   yXINIT__gsetup();
   /*---(build back up)------------------*/
   DATA_refresh   ();
   SORT_refresh   ();
   FILTER_refresh ();
   font_load ();
   OPENGL_init  ();
   OPENGL_draw  ();
   OPENGL_mask  ();
   my.ccol = 0;
   my.crow = 0;
   OPENGL_resize (win_w, win_h);
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

uchar
PROG_main_layout        (uchar a_major, uchar a_minor)
{
   /*---(format changes)-----*/
   switch (a_minor) {
   case 'C' : format_change ('c');                break;
   case 'L' : format_change ('l');                break;
   case 'T' : format_change ('t');                break;
   case 'B' : format_change ('b');                break;
   case '1' : format_change ('1');                break;
   case '2' : format_change ('2');                break;
   case 'W' : format_change ('w');                break;
   case 'X' : format_change ('x');                break;
   case 'P' : format_change ('p');                break;
   }
   return 0;
}

uchar
PROG_main_moves         (uchar a_major, uchar a_minor)
{
   switch (g_minor) {
   case '_' : my.crow = 0;       break;
   case 'k' : --my.crow;         break;
   case 'j' : ++my.crow;         break;
   case '%' : break;
   case '0' : my.ccol = 0;       break;
   case 'h' : --my.ccol;         break;
   case 'l' : ++my.ccol;         break;
   case '$' : break;
   }
   return 0;
}

uchar
PROG_main_modes         (uchar a_major, uchar a_minor)
{
   char        rc          =    0;
   switch (a_minor) {
   case '\\': rc = 'µ';     OPENGL_menu_start ();  break;
   case '/' : rc = 's'; my.search[0] = '\0';        break;
   case 'u' : rc = a_minor;                        break;
   case 'i' : rc = a_minor;                        break;
   case 'e' : rc = a_minor;                        break;
   case 'f' : rc = a_minor;                        break;
   case 'a' : FILTER_clear(); return my.format;  break;
   }
   return rc;
}

uchar
PROG_main_handle        (uchar a_key)
{
   /*---(locals)-----------+-----+-----+-*/
   uchar       rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_LOOP   yLOG_enter   (__FUNCTION__);
   DEBUG_LOOP   yLOG_value   ("a_key"     , a_key);
   /*---(defense)------------------------*/
   if (a_key == 0) {
      DEBUG_LOOP   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(prepare)------------------------*/
   my.trouble   = '-';
   g_minor = chrworking (a_key);
   DEBUG_LOOP   yLOG_value   ("g_major"   , g_major);
   DEBUG_LOOP   yLOG_value   ("g_minor"   , g_minor);
   /*---(handle keystroke)---------------*/
   switch (g_major) {
   case ' ' :
      if      (a_key == 'Q') { my.quit = 'y';  return 1; }
      else if (strchr ("CL12TBWXP", g_minor) != NULL) rc = PROG_main_layout (g_major, g_minor);
      else if (strchr ("_jk%0hl$" , g_minor) != NULL) rc = PROG_main_moves  (g_major, g_minor);
      else if (strchr ("\\/uiefa" , g_minor) != NULL) rc = PROG_main_modes  (g_major, g_minor);
   }
   /*---(setup for next keystroke)-------*/
   if      (rc == 0)    g_major = G_KEY_SPACE;
   else if (rc >  0)    g_major = rc;
   else               { g_major = G_KEY_SPACE;  my.trouble = 'y'; }
   /*---(complete)-----------------------*/
   DEBUG_LOOP   yLOG_exit    (__FUNCTION__);
   return 0;
}

uchar        /*-> process input string in main loop --[ ------ [ge.C74.153.42]*/ /*-[02.0000.00#.D]-*/ /*-[--.---.---.--]-*/
PROG_main_string        (uchar *a_keys)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         =  -10;     /* return code for errors              */
   uchar       rc          =    0;
   int         i           =    0;
   int         x_len       =    0;
   uchar       x_ch        =  ' ';     /* current keystroke                   */
   char        x_keys      [LEN_RECD];
   /*---(header)-------------------------*/
   DEBUG_LOOP   yLOG_enter   (__FUNCTION__);
   DEBUG_LOOP   yLOG_point   ("a_keys"    , a_keys);
   --rce;  if (a_keys == NULL) {
      DEBUG_LOOP   yLOG_note    ("a_keys is null");
      DEBUG_LOOP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   strlcpy    (x_keys, a_keys  , LEN_RECD);
   strlencode (x_keys, ySTR_MAX, LEN_RECD);
   DEBUG_LOOP   yLOG_info    ("x_keys"    , x_keys);
   x_len = strlen (a_keys);
   DEBUG_LOOP   yLOG_value   ("x_len"     , x_len);
   --rce;
   for (i = 0; i < x_len; ++i) {
      DEBUG_LOOP   yLOG_value   ("LOOP"      , i);
      /*---(get next char)---------------*/
      DEBUG_LOOP   yLOG_value   ("a_keys[i]" , a_keys[i]);
      DEBUG_LOOP   yLOG_char    ("a_keys[i]" , chrvisible (a_keys[i]));
      x_ch = chrworking (a_keys [i]);
      DEBUG_LOOP   yLOG_value   ("x_ch"      , x_ch);
      /*---(handle keystroke)------------*/
      rc = PROG_main_handle (x_ch);
      DEBUG_LOOP   yLOG_value   ("rc"        , rc);
      /*---(done)------------------------*/
   }
   DEBUG_LOOP   yLOG_note    ("main loop done");
   /*---(complete)-----------------------*/
   DEBUG_LOOP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char        /* PURPOSE : process the xwindows event stream                    */
PROG_event    (void)
{
   /*---(locals)--------------------------------*/
   XKeyEvent  *key_event;
   char        x_keys      [LEN_TERSE];
   int         x_bytes     =    0;
   my.update = 0;
   int        loop = 0;
   /*---(header)-------------------------*/
   DEBUG_LOOP   yLOG_enter    (__FUNCTION__);
   while (1) {
      XNextEvent(DISP, &EVNT);
      my.update = 1;
      my.pcol = my.ccol;
      my.prow = my.crow;
      /*---(start)----------------------------*/
      switch(EVNT.type) {
      case KeyPress:
         key_event  = (XKeyEvent *) &EVNT;
         x_bytes = XLookupString((XKeyEvent *) &EVNT, x_keys, 5, NULL, NULL);
         if (x_bytes < 1) break;
         g_minor = x_keys [0];
         DEBUG_LOOP   yLOG_char     ("g_major"   , g_major);
         DEBUG_LOOP   yLOG_char     ("g_minor"   , g_minor);
         PROG_main_handle (x_keys [0]);
         if (g_major == ' ') {
            if (g_minor == 'Q') {
               my.quit = 'y';
               return 1;
            }
            if (strchr ("CL12TBWXXP", g_minor) != NULL) {
               PROG_main_layout (g_major, g_minor);
               continue;
            }
            switch (g_minor) {
            case '\\' : g_major = g_minor; OPENGL_menu_start ();  break;
            }
            /*---(format changes)-----*/
            switch (g_minor) {
            case 'Q'  : my.quit = 'y';  return 1;           break;
            case 'C'  : format_change ('c');                break;
            case 'L'  : format_change ('l');                break;
            case 'T'  : format_change ('t');                break;
            case 'B'  : format_change ('b');                break;
            case '1'  : format_change ('1');                break;
            case '2'  : format_change ('2');                break;
            case 'W'  : format_change ('w');                break;
            case 'X'  : format_change ('x');                break;
            case 'P'  : format_change ('p');                break;
            }
            /*---(mode changes)-------*/
            switch (g_minor) {
            case 'u' : g_alpha = 0.8; g_major = 'u';           break;
            case 'i' : g_alpha = 0.8; g_major = 'i';           break;
            case 'e' : g_alpha = 0.8; g_major = 'e';           break;
            case 'f' : g_alpha = 0.8; g_major = 'f';           break;
            case 'a' : FILTER_clear(); return my.format;     break;
            case '/' : g_alpha = 0.8; g_major = 's'; my.search[0] = '\0'; break;
            }
            /*---(filters and tasks)--*/
            switch (g_minor) {
            case '[':
            case '?': my.incr  = STOP;   my.move   = 0.0;   break;
            case '_': my.incr  = STOP;   my.move   = 0.0;  my.ccol = my.crow = 0; my.move = 0.0;  break;
            case 'l': my.incr  = STOP;   ++my.ccol;      break;
            case 'j': my.incr  = STOP;   ++my.crow;      break;
            case ',': my.incr  = my.play;                break;
            case '+': my.incr += my.change;              break;
            case '-': my.incr -= my.change;              break;
            case '.': my.incr  = STOP;                   break;
            case 'h': my.incr  = STOP;   --my.ccol;      break;
            case 'k': my.incr  = STOP;   --my.crow;      break;
                      /*> case ']':                                                             <*/
                      /*> case '~': my.incr  = STOP;   my.move   = 0.0; my.crow = my.nrows - 1; break;   <*/
            }
            /*---(task related)-------*/
            switch (g_minor) {
            case 'p': task_list();                       break;
            case 'r': task_refresh();                    break;
                      /*> case 'J': font_change(); task_refresh();                    break;    <*/
            }
            /*---(format changes)-----*/
            switch (g_minor) {
            case '0' : my.curg = my.cimp = my.cest = ' '; strcpy(one, "all"); my.cflg = '<'; return 't'; break;
            case 'S' : my.sort = 's'; return my.format;  break;
            case 'U' : my.sort = 'u'; return my.format;  break;
            }
            /*---(verify currents)----*/
            DEBUG_GRAF   yLOG_value    ("my.ccol"   , my.ccol);
            DEBUG_GRAF   yLOG_value    ("my.crow"   , my.crow);
            range_check();

         } else {
            if (g_major == '\\') {
               if (g_minor == G_KEY_ESCAPE) {
                  g_major = ' ';
                  OPENGL_mask ();
               } else {
                  OPENGL_menu_cont  ();
               }
            } else if (g_major == 's' && g_minor != 13) {
               if (g_minor == G_KEY_ESCAPE) {
                  g_major = ' ';

               } else {
                  char   ch  = g_minor;
                  /*> printf("ch = %3d\n", ch);                                       <*/
                  if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || ch == '_' || ch == 8) {
                     char   temp[10];
                     sprintf (temp, "%c", g_minor);
                     strcat  (my.ctxt, temp);
                     /*> printf("search = <<%s>>\n", my.search);                       <*/
                  }
               }
            } else {
               switch (g_major) {
               case 'u': if (strchr (my.urgs, g_minor) != 0) my.curg  = g_minor;  else my.curg  = ' '; break;
               case 'i': if (strchr (my.imps, g_minor) != 0) my.cimp  = g_minor;  else my.cimp  = ' '; break;
               case 'e': if (strchr (my.ests, g_minor) != 0) my.cest  = g_minor;  else my.cest  = ' '; break;
               case 'f': if (strchr (my.flgs, g_minor) != 0) my.cflg  = g_minor;  else my.cflg  = ' '; break;
               case 's': if (my.search[0] == '\0') strlcpy (one, "all",    LEN_LABEL);
                            else                  strlcpy (one, my.search, LEN_LABEL);
                            break;
               }
               g_major = ' ';
               g_alpha    = 0.0;
               /*> task_refresh();                                                 <*/
               return my.format;
            }
         }
         DEBUG_LOOP   yLOG_char     ("g_major"   , g_major);
         DEBUG_LOOP   yLOG_char     ("g_minor"   , g_minor);
         break;

      default:
         break;
      }
      DEBUG_LOOP   yLOG_value    ("my.ccol"   , my.ccol);
      my.update = 0;
      if (g_major == '\\')  OPENGL_mask ();
      OPENGL_show  ();
   }
   /*---(complete)------------------------------*/
   DEBUG_LOOP   yLOG_exit     (__FUNCTION__);
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
