/*============================----beg-of-source---============================*/
/*===[[ HEADER ]]=============================================================*

 *   focus         : (MH) mind_hacking
 *   niche         : (ta) task_mgmt
 *   application   : metis       (titan goddess of cunning and wise counsel)
 *   purpose       : simple, light, clean, and powerful task management system
 *
 *   module        : metis_filter
 *   purpose       : segregate data filtering from other program elements
 * 
 */
/*============================================================================*/

/*===[[ METIS BACKLOG ]]======================================================*
 *  metis  -----  tbd
 *
 */

#include   "metis.h"

char
FILTER_init             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        i           =    0;
   char        t           [LEN_LABEL];
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter    (__FUNCTION__);
   /*---(commands)-----------------------*/
   yVIKEYS_cmds_add (YVIKEYS_M_DATASET, "sort"        , ""    , "s"    , api_yvikeys_sort    , "sort metis data"    );
   yVIKEYS_menu_add ("�dsc", "clear"     , ":sort clear�");
   yVIKEYS_menu_add ("�dsu", "by urg"    , ":sort urg�");
   yVIKEYS_menu_add ("�dsi", "by imp"    , ":sort imp�");
   yVIKEYS_menu_add ("�dse", "by est"    , ":sort est�");
   yVIKEYS_menu_add ("�dsf", "by flg"    , ":sort flg�");
   yVIKEYS_menu_add ("�dsn", "by names"  , ":sort names�");
   yVIKEYS_menu_add ("�dsa", "ascend"    , ":sort ascend�");
   yVIKEYS_menu_add ("�dsd", "descend"   , ":sort descend�");
   yVIKEYS_cmds_add (YVIKEYS_M_DATASET, "filter"      , ""    , "ss"   , api_yvikeys_filter  , "filter metis data"    );
   yVIKEYS_menu_add ("�dfc", "clear"     , ":filter clear�");
   yVIKEYS_menu_add ("�dfu", "by urg"    , ":filter urg ");
   yVIKEYS_menu_add ("�dfi", "by imp"    , ":filter imp ");
   yVIKEYS_menu_add ("�dfe", "by est"    , ":filter est ");
   yVIKEYS_menu_add ("�dff", "by flg"    , ":filter flg ");
   yVIKEYS_menu_add ("�df1", "by one"    , ":filter one ");
   yVIKEYS_menu_add ("�df2", "by two"    , ":filter two ");
   yVIKEYS_menu_add ("�dft", "by txt"    , ":filter txt ");
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
FILTER_clear            (void)
{
   my.curg   = my.cimp = my.cest = my.cflg = ' ';
   my.cone [0] = my.ctwo [0] = my.ctxt [0] = '\0';
   return 0;
}

char
FILTER_refresh          (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter    (__FUNCTION__);
   DEBUG_DATA   yLOG_char     ("my.curg"    , my.curg);
   DEBUG_DATA   yLOG_char     ("my.cimp"    , my.cimp);
   DEBUG_DATA   yLOG_char     ("my.cest"    , my.cest);
   DEBUG_DATA   yLOG_char     ("my.cflg"    , my.cflg);
   /*---(prepare)------------------------*/
   my.nact = 0;
   g_tasks [g_ntask].act = '-';
   /*---(display)------------------------*/
   for (i = 0; i < g_ntask; ++i) {
      DEBUG_DATA   yLOG_complex  ("review"    , "%3d#, %cu, %ci, %ce, %cf", i, g_tasks [i].urg, g_tasks [i].imp, g_tasks [i].est, g_tasks [i].flg);
      /*---(default)---------------------*/
      g_tasks [i].act  = '-';
      /*> if (my.nact > my.nrows)  continue;                                         <*/
      /*---(urgency)---------------------*/
      if ((my.curg != ' ' && g_tasks [i].urg != my.curg)) {
         DEBUG_DATA   yLOG_note     ("skip as urgent does not match filter");
         continue;
      }
      /*---(importance)------------------*/
      if ((my.cimp != ' ' && g_tasks [i].imp != my.cimp)) {
         DEBUG_DATA   yLOG_note     ("skip as importance does not match filter");
         continue;
      }
      /*---(estimate)--------------------*/
      if ((my.cest != ' ' && g_tasks [i].est != my.cest)) {
         DEBUG_DATA   yLOG_note     ("skip as estimate does not match filter");
         continue;
      }
      /*---(status)----------------------*/
      if ((my.cflg != ' ' && g_tasks [i].flg != my.cflg)) {
         DEBUG_DATA   yLOG_note     ("skip as flag/status does not match filter");
         continue;
      }
      /*---(one)-------------------------*/
      if (my.cone [0] != '\0' && strstr (g_tasks [i].one, my.cone) == NULL) {
         DEBUG_DATA   yLOG_note     ("skip as cat one does not contain filter");
         continue;
      }
      /*---(two)-------------------------*/
      if (my.ctwo [0] != '\0' && strstr (g_tasks [i].two, my.ctwo) == NULL) {
         DEBUG_DATA   yLOG_note     ("skip as cat two does not contain filter");
         continue;
      }
      /*---(text)------------------------*/
      if (my.ctxt [0] != '\0' && strstr (g_tasks [i].txt, my.ctxt) == NULL) {
         DEBUG_DATA   yLOG_note     ("skip as txt does not contain filter");
         continue;
      }
      /*---(increment)-------------------*/
      g_tasks [i].act  = 'y';
      ++my.nact;
      /*---(done)------------------------*/
   }
   DEBUG_DATA   yLOG_value    ("my.nact"  , my.nact);
   if (my.nact == 0) {
      DEBUG_DATA   yLOG_note     ("add placeholder");
      g_tasks [g_ntask].act = 'y';
      ++my.nact;
   }
   DEBUG_DATA   yLOG_value    ("my.nact"  , my.nact);
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_exit     (__FUNCTION__);
   return 0;
}


static  s_swaps      = 0;
static  s_notes      = 0;
static  s_comps      = 0;
static  s_teles      = 0;

char
SORT__show              (tCARD *a_cur)
{
}

char
SORT__swap              (tCARD *a_one, tCARD *a_two)
{
   /*---(locals)-----------+-----+-----+-*/
   tCARD       x_temp;
   /*---(one to temp)--------------------*/
   strlcpy (x_temp.one, a_one->one, LEN_LABEL);
   strlcpy (x_temp.two, a_one->two, LEN_LABEL);
   x_temp.urg        = a_one->urg;
   x_temp.imp        = a_one->imp;
   x_temp.est        = a_one->est;
   x_temp.flg        = a_one->flg;
   strlcpy (x_temp.txt, a_one->txt, LEN_HUND);
   x_temp.seq        = a_one->seq;
   x_temp.line       = a_one->line;
   x_temp.act        = a_one->act;
   strlcpy (x_temp.key, a_one->key, LEN_HUND);
   /*---(two to one)---------------------*/
   strlcpy (a_one->one, a_two->one, LEN_LABEL);
   strlcpy (a_one->two, a_two->two, LEN_LABEL);
   a_one->urg        = a_two->urg;
   a_one->imp        = a_two->imp;
   a_one->est        = a_two->est;
   a_one->flg        = a_two->flg;
   strlcpy (a_one->txt, a_two->txt, LEN_HUND);
   a_one->seq        = a_two->seq;
   a_one->line       = a_two->line;
   a_one->act        = a_two->act;
   strlcpy (a_one->key, a_two->key, LEN_HUND);
   /*---(temp to two)--------------------*/
   strlcpy (a_two->one, x_temp.one, LEN_LABEL);
   strlcpy (a_two->two, x_temp.two, LEN_LABEL);
   a_two->urg        = x_temp.urg;
   a_two->imp        = x_temp.imp;
   a_two->est        = x_temp.est;
   a_two->flg        = x_temp.flg;
   strlcpy (a_two->txt, x_temp.txt, LEN_HUND);
   a_two->seq        = x_temp.seq;
   a_two->line       = x_temp.line;
   a_two->act        = x_temp.act;
   strlcpy (a_two->key, x_temp.key, LEN_HUND);
   /*---(complete)-----------------------*/
   return 0;
}

char
SORT__gnome             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   int         i           =    0;          /* current index                  */
   int         t           =    0;          /* teleport index                 */
   int         x_tmp;
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   /*---(gnome)--------------------------*/
   i = 0;
   while (i < g_ntask) {
      if (i > t)  t = i;
      /*---(beginning)-------------------*/
      if (i <= 0) {
         DEBUG_SORT   yLOG_note    ("bounce off beginning");
         ++s_teles;
         i = t + 1;
         continue;
      }
      /*---(compare)---------------------*/
      ++s_comps;
      rc = strcmp (g_tasks [i - 1].key, g_tasks [i].key);
      if (my.order == 'd')   rc *= -1;
      if (rc <= 0) {
         DEBUG_SORT   yLOG_complex ("correct"   , "%3d %-20.20s v %3d %-20.20s   %c %4d   %5d %5d %5d", i - 1, g_tasks [i - 1].key, i, g_tasks [i].key, (rc <= 0) ? 'y' : '-', rc, s_comps, s_swaps, s_teles);
         ++s_teles;
         i = t + 1;
         continue;
      }
      /*---(swap)------------------------*/
      ++s_notes;
      DEBUG_SORT   yLOG_complex ("swapped"   , "%3d %-20.20s # %3d %-20.20s   %c %4d   %5d %5d %5d", i - 1, g_tasks [i - 1].key, i, g_tasks [i].key, (rc <= 0) ? 'y' : '-', rc, s_comps, s_swaps, s_teles);
      SORT__swap (&(g_tasks [i - 1]), &(g_tasks [i]));
      /*---(next)------------------------*/
      --i;
   }
   /*---(display)------------------------*/
   DEBUG_SORT   yLOG_value   ("size"       , g_ntask);
   DEBUG_SORT   yLOG_value   ("compares"   , s_comps);
   DEBUG_SORT   yLOG_value   ("notes"      , s_notes);
   DEBUG_SORT   yLOG_value   ("swaps"      , s_swaps);
   DEBUG_SORT   yLOG_value   ("teleports"  , s_teles);
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
SORT_stats              (char a_type)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   char        x_urg       =    0;
   char        x_imp       =    0;
   char        x_est       =    0;
   char        x_flg       =    0;
   char        t           [LEN_LABEL];
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   /*---(prepare keys)-------------------*/
   for (i = 0; i < g_ntask; ++i) {
      /*---(convert to scale)------------*/
      x_urg = strchr (my.urgs, g_tasks [i].urg) - my.urgs + 'a';
      x_imp = strchr (my.imps, g_tasks [i].imp) - my.imps + 'a';
      x_est = strchr (my.ests, g_tasks [i].est) - my.ests + 'a';
      x_flg = strchr (my.flgs, g_tasks [i].flg) - my.flgs + 'a';
      /*---(combine)---------------------*/
      switch (a_type) {
      case 'i' :  sprintf (t, "%c%c%c%c", x_imp, x_urg, x_est, x_flg);  break;
      case 'e' :  sprintf (t, "%c%c%c%c", x_est, x_urg, x_imp, x_flg);  break;
      case 'f' :  sprintf (t, "%c%c%c%c", x_flg, x_urg, x_imp, x_est);  break;
      default  :
      case 'u' :  sprintf (t, "%c%c%c%c", x_urg, x_imp, x_est, x_flg);  break;
      }
      /*---(put in key)------------------*/
      sprintf (g_tasks [i].key, "%-4.4s %-20.20s %-20.20s %s", t,
            g_tasks [i].one, g_tasks [i].two, g_tasks [i].txt);
      DEBUG_SORT   yLOG_bullet  (i            , g_tasks [i].key);
   }
   /*---(sort)---------------------------*/
   SORT__gnome ();
   /*---(save sort type)-----------------*/
   my.sort = a_type;
   /*---(complete)-----------------------*/
   return 0;
}

char
SORT_names              (void)
{
   char        rc          =    0;
   int         i           =    0;
   char        x_urg       =    0;
   char        x_imp       =    0;
   char        x_est       =    0;
   char        x_flg       =    0;
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   /*---(prepare key)--------------------*/
   for (i = 0; i < g_ntask; ++i) {
      x_urg = strchr (my.urgs, g_tasks [i].urg) - my.urgs + 'a';
      x_imp = strchr (my.imps, g_tasks [i].imp) - my.imps + 'a';
      x_est = strchr (my.ests, g_tasks [i].est) - my.ests + 'a';
      x_flg = strchr (my.flgs, g_tasks [i].flg) - my.flgs + 'a';
      sprintf (g_tasks [i].key, "%-20.20s %-20.20s %c%c%c%c %s",
            g_tasks [i].one, g_tasks [i].two,
            x_urg, x_imp, x_est, x_flg,
            g_tasks [i].txt);
      DEBUG_SORT   yLOG_bullet  (i            , g_tasks [i].key);
   }
   /*---(sort)---------------------------*/
   rc = SORT__gnome ();
   /*---(save sort type)-----------------*/
   my.sort = 'n';
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
SORT_unsort             (void)
{
   char        rc          =    0;
   int         i           =    0;
   char        x_urg       =    0;
   char        x_imp       =    0;
   char        x_est       =    0;
   char        x_flg       =    0;
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   /*---(prepare key)--------------------*/
   for (i = 0; i < g_ntask; ++i) {
      sprintf (g_tasks [i].key, "%04d", g_tasks [i].seq);
      DEBUG_SORT   yLOG_bullet  (i            , g_tasks [i].key);
   }
   /*---(sort)---------------------------*/
   rc = SORT__gnome ();
   /*---(save sort type)-----------------*/
   my.sort = 'o';
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
SORT_refresh            (void)
{
   switch (my.sort) {
   case 'u'  : SORT_stats  ('u');  break;
   case 'i'  : SORT_stats  ('i');  break;
   case 'e'  : SORT_stats  ('e');  break;
   case 'f'  : SORT_stats  ('f');  break;
   case 'n'  : SORT_names  ();     break;
   case 'o'  : SORT_unsort ();     break;
   }
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      unit testing                            ----===*/
/*====================------------------------------------====================*/
static void  o___UNITTEST________o () { return; }

char*        /*-> unit test accessor -----------------[ light  [us.B60.2A3.F2]*/ /*-[01.0000.00#.#]-*/ /*-[--.---.---.--]-*/
FILTER__unit       (char *a_question, int a_num)
{
   /*---(locals)-------------------------*/
   char        rc          =    0;
   char        s           [LEN_HUND]   = "[]";
   char        t           [LEN_HUND]   = "[]";
   char        u           [LEN_HUND]   = "[]";
   /*---(overall)------------------------*/
   strcpy (unit_answer, "FILTER           : question not understood");
   if      (strcmp (a_question, "count"         ) == 0) {
      snprintf (unit_answer, LEN_FULL, "FILTER count     : %d", my.nact);
   }
   else if (strcmp (a_question, "detail"        ) == 0) {
      snprintf (unit_answer, LEN_FULL, "FILTER detl (%2d) : %c   [%.10s]", a_num, g_tasks [a_num].act, g_tasks [a_num].txt);
   }
   else if (strcmp (a_question, "sorted"        ) == 0) {
      sprintf (s, "[%.10s]", g_tasks [a_num].one);
      sprintf (t, "[%.10s]", g_tasks [a_num].two);
      sprintf (u, "[%.10s]", g_tasks [a_num].txt);
      snprintf (unit_answer, LEN_FULL, "FILTER sort (%2d) : %c%c%c%c %c %-12.12s %-12.12s %-12.12s %3d", a_num,
            g_tasks [a_num].urg, g_tasks [a_num].imp, g_tasks [a_num].est, g_tasks [a_num].flg, 
            g_tasks [a_num].act, s, t, u, g_tasks [a_num].seq);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


/*============================----end-of-source---============================*/
