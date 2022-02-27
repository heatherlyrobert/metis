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
 *
 * metis § !n2-· § update yMAP with active tasks, not all (NROW vs nact)                  § M1Q5aR §
 *
 */

#include   "metis.h"

char
metis_filter_init       (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        i           =    0;
   char        t           [LEN_LABEL];
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter    (__FUNCTION__);
   /*---(defaults)-----------------------*/
   my.sort = METIS_ORIG;
   metis_filter_clear ();
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
metis_filter_vikeys     (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        i           =    0;
   char        t           [LEN_LABEL];
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter    (__FUNCTION__);
   /*---(commands)-----------------------*/
   yCMD_add (YCMD_M_DATASET, "sort"        , ""    , "s"    , api_yvikeys_sort    , "sort metis data"    );
   yCMD_menu_add ("µdsc", "clear"     , ":sort clear¦");
   yCMD_menu_add ("µdso", "original"  , ":sort orig¦");
   yCMD_menu_add ("µdsu", "by urg"    , ":sort urg¦");
   yCMD_menu_add ("µdsi", "by imp"    , ":sort imp¦");
   yCMD_menu_add ("µdse", "by est"    , ":sort est¦");
   yCMD_menu_add ("µdsp", "by prg"    , ":sort prg¦");
   yCMD_menu_add ("µdss", "by shr"    , ":sort shr¦");
   yCMD_menu_add ("µdsd", "by date"   , ":sort date¦");
   yCMD_add (YCMD_M_DATASET, "filter"      , ""    , "ss"   , api_yvikeys_filter  , "filter metis data"    );
   yCMD_menu_add ("µdfc", "clear"     , ":filter clear¦");
   yCMD_menu_add ("µdfu", "by urg"    , ":filter urg ");
   yCMD_menu_add ("µdfi", "by imp"    , ":filter imp ");
   yCMD_menu_add ("µdfe", "by est"    , ":filter est ");
   yCMD_menu_add ("µdfp", "by prg"    , ":filter prg ");
   yCMD_menu_add ("µdfs", "by shr"    , ":filter shr ");
   yCMD_menu_add ("µdft", "by txt"    , ":filter txt ");
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
metis_filter_clear      (void)
{
   my.curg   = my.cimp = my.cest = my.cprg = my.cshr = ' ';
   my.ctxt [0] = '\0';
   return 0;
}

char
metis_filter_set        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         c           =    0;
   char        x_show      =  'y';
   tTASK      *x_task      = NULL;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter    (__FUNCTION__);
   DEBUG_DATA   yLOG_char     ("my.curg"    , my.curg);
   DEBUG_DATA   yLOG_char     ("my.cimp"    , my.cimp);
   DEBUG_DATA   yLOG_char     ("my.cest"    , my.cest);
   DEBUG_DATA   yLOG_char     ("my.cprg"    , my.cprg);
   DEBUG_DATA   yLOG_char     ("my.cshr"    , my.cshr);
   /*---(prepare)------------------------*/
   my.nact = 0;
   if (my.ctxt [0] != '\0') {
      rc = yREGEX_comp (my.ctxt);
      DEBUG_DATA   yLOG_value   ("comp"      , rc);
      --rce;  if (rc < 0) {
         DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(display)------------------------*/
   metis_task_by_cursor (YDLST_HEAD, &x_task);
   while (x_task != NULL) {
      DEBUG_DATA   yLOG_complex  ("review"    , "%3d#, %cu, %ci, %ce, %cf", c, x_task->urg, x_task->imp, x_task->est, x_task->prg);
      /*---(default)---------------------*/
      x_task->show  = 'y';
      x_task->note  = '-';
      ++c;
      /*---(urgency)---------------------*/
      if      ((my.curg != ' ' && x_task->urg != my.curg)) {
         DEBUG_DATA   yLOG_note     ("skip as urgent does not match filter");
         x_task->show = '-';
      }
      /*---(importance)------------------*/
      else if ((my.cimp != ' ' && x_task->imp != my.cimp)) {
         DEBUG_DATA   yLOG_note     ("skip as importance does not match filter");
         x_task->show = '-';
      }
      /*---(estimate)--------------------*/
      else if ((my.cest != ' ' && x_task->est != my.cest)) {
         DEBUG_DATA   yLOG_note     ("skip as estimate does not match filter");
         x_task->show = '-';
      }
      /*---(progress)--------------------*/
      else if ((my.cprg != ' ' && x_task->prg != my.cprg)) {
         DEBUG_DATA   yLOG_note     ("skip as progress does not match filter");
         x_task->show = '-';
      }
      /*---(sharing)---------------------*/
      else if ((my.cshr != ' ' && x_task->shr != my.cshr)) {
         DEBUG_DATA   yLOG_note     ("skip as sharing does not match filter");
         x_task->show = '-';
      }
      /*---(text)------------------------*/
      else if (my.ctxt [0] != '\0' && yREGEX_filter (x_task->txt) <= 0) {
         DEBUG_DATA   yLOG_note     ("skip as txt does not contain filter");
         x_task->show = '-';
      }
      /*---(increment)-------------------*/
      if (x_task->show == 'y')  ++my.nact;
      metis_task_by_cursor (YDLST_NEXT, &x_task);
      /*---(done)------------------------*/
   }
   DEBUG_DATA   yLOG_value    ("my.nact"  , my.nact);
   /*> if (my.nact == 0) {                                                            <* 
    *>    DEBUG_DATA   yLOG_note     ("add placeholder");                             <* 
    *>    g_tasks [g_ntask].act = 'y';                                                <* 
    *>    ++my.nact;                                                                  <* 
    *> }                                                                              <*/
   DEBUG_DATA   yLOG_value    ("my.nact"  , my.nact);
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
metis_filter_search     (char *a_search)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         c           =    0;
   char        x_show      =  'y';
   tTASK      *x_task      = NULL;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter    (__FUNCTION__);
   /*---(prepare)------------------------*/
   rc = yREGEX_comp (a_search);
   DEBUG_DATA   yLOG_value   ("comp"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(display)------------------------*/
   metis_task_by_cursor (YDLST_HEAD, &x_task);
   while (x_task != NULL) {
      DEBUG_DATA   yLOG_complex  ("review"    , "%3d#, %cu, %ci, %ce, %cf", c, x_task->urg, x_task->imp, x_task->est, x_task->prg);
      /*---(default)---------------------*/
      x_task->show  = 'y';
      x_task->note  = '-';
      ++c;
      /*---(text)------------------------*/
      rc = yREGEX_filter (x_task->srch);
      if (rc > 0) {
         DEBUG_DATA   yLOG_note     ("skip as txt does not contain filter");
         x_task->note = 'r';
      }
      /*---(increment)-------------------*/
      if (x_task->show == 'y')  ++my.nact;
      metis_task_by_cursor (YDLST_NEXT, &x_task);
      /*---(done)------------------------*/
   }
   DEBUG_DATA   yLOG_value    ("my.nact"  , my.nact);
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_exit     (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      sort ordering                           ----===*/
/*====================------------------------------------====================*/
static void  o___ORDER___________o () { return; }

char
metis_filter_key        (tTASK *a_task)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_urg       =    0;
   char        x_imp       =    0;
   char        x_est       =    0;
   char        x_prg       =    0;
   char        x_shr       =    0;
   char        t           [LEN_LABEL];
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   DEBUG_DATA   yLOG_char    ("my.sort"   , my.sort);
   /*---(defense)------------------------*/
   DEBUG_DATA   yLOG_point   ("a_task"    , a_task);
   --rce;  if (a_task == NULL) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(original order)--------------*/
   if (my.sort == METIS_ORIG) {
      DEBUG_DATA   yLOG_note    ("original/natural sort order");
      snprintf (a_task->key, LEN_HUND, "%-20s %-20s %3d", a_task->minor->major->name, a_task->minor->name, a_task->seq);
   }
   /*---(statistic sorts)-------------*/
   else if (strchr (METIS_STATS, my.sort) != NULL) {
      DEBUG_DATA   yLOG_note    ("statistical sort order");
      /*---(convert to scale)------------*/
      x_urg = strchr (METIS_URGS, a_task->urg);
      if (x_urg == NULL)  x_urg = '0';
      else                x_urg = x_urg - (int) METIS_URGS + '1';
      x_imp = strchr (METIS_IMPS, a_task->imp);
      if (x_imp == NULL)  x_imp = '0';
      else                x_imp = x_imp - (int) METIS_IMPS + '1';
      x_est = strchr (METIS_ESTS, a_task->est);
      if (x_est == NULL)  x_est = '0';
      else                x_est = x_est - (int) METIS_ESTS + '1';
      x_prg = strchr (METIS_PRGS, a_task->prg);
      if (x_prg == NULL)  x_prg = '0';
      else                x_prg = x_prg - (int) METIS_PRGS + '1';
      x_shr = strchr (METIS_SHRS, a_task->shr);
      if (x_shr == NULL)  x_shr = '0';
      else                x_shr = x_shr - (int) METIS_SHRS + '1';
      DEBUG_DATA   yLOG_complex ("scales"    , "%cÖ%c %cÖ%c %cÖ%c %cÖ%c %cÖ%c", a_task->urg, x_urg, a_task->imp, x_imp, a_task->est, x_est, a_task->prg, x_prg, a_task->shr, x_shr);
      /*---(create prefix)---------------*/
      switch (my.sort) {
      case METIS_URG :
         DEBUG_DATA   yLOG_note    ("urgency sort");
         sprintf (t, "%c", x_urg);
         break;
      case METIS_IMP :
         DEBUG_DATA   yLOG_note    ("importance sort");
         sprintf (t, "%c", x_imp);
         break;
      case METIS_EST :
         DEBUG_DATA   yLOG_note    ("estimate sort");
         sprintf (t, "%c", x_est);
         break;
      case METIS_PRG :
         DEBUG_DATA   yLOG_note    ("progress sort");
         sprintf (t, "%c", x_prg);
         break;
      case METIS_SHR :
         DEBUG_DATA   yLOG_note    ("sharing sort");
         sprintf (t, "%c", x_shr);
         break;
      }
      /*---(put in key)------------------*/
      snprintf (a_task->key, LEN_HUND, "%s %-20.20s %-20.20s %3d",
            t, a_task->minor->major->name, a_task->minor->name, a_task->seq);
      DEBUG_DATA   yLOG_info    ("key"       , a_task->key);
      /*---(done)------------------------*/
   }
   /*---(date order)------------------*/
   else if (my.sort == METIS_DATE) {
      DEBUG_DATA   yLOG_note    ("epoch/data order");
      snprintf (a_task->key, LEN_HUND, "%-6.6s", a_task->epoch);
   }
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
metis_filter_sort       (void)
{  /*---(design notes)-------------------*/
   /*
    *  always start in original order to keep sort stable and predictable
    */
   /*---(locals)-----------+-----+-----+-*/
   tTASK      *x_task      = NULL;
   char        x_sort      =  '-';
   /*---(header)-------------------------*/
   DEBUG_SORT   yLOG_enter   (__FUNCTION__);
   /*---(sort to original)---------------*/
   x_sort = my.sort;
   /*---(sort to original)---------------*/
   my.sort = METIS_ORIG;
   metis_task_by_cursor (YDLST_HEAD, &x_task);
   while (x_task != NULL) {
      metis_filter_key (x_task);
      metis_task_by_cursor (YDLST_NEXT, &x_task);
   }
   ySORT_prepare  (B_TASK);
   /*---(sort to new order)--------------*/
   if (x_sort != my.sort) {
      my.sort = x_sort;
      metis_task_by_cursor (YDLST_HEAD, &x_task);
      while (x_task != NULL) {
         metis_filter_key (x_task);
         metis_task_by_cursor (YDLST_NEXT, &x_task);
      }
      ySORT_prepare  (B_TASK);
   }
   /*---(debugging)----------------------*/
   ySORT_list     (B_TASK);
   ySORT_treeform (B_TASK);
   /*---(complete)-----------------------*/
   DEBUG_SORT   yLOG_exit    (__FUNCTION__);
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
            g_tasks [a_num].urg, g_tasks [a_num].imp, g_tasks [a_num].est, g_tasks [a_num].prg, 
            g_tasks [a_num].act, s, t, u, g_tasks [a_num].seq);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


/*============================----end-of-source---============================*/
