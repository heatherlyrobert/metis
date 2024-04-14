/*============================----beg-of-source---============================*/
#include   "metis.h"

/*
 *
 */

/*
 *
 *
 * urgency is NOT how fast i would like to do it or plan to do it !!!!
 * urgency IS how fast i must do it, a lot of things should be months or years
 *
 *                    urgency
 *
 *         ∑   y   m   w   d   s   t   !
 *       ÉÄÄÄÄÄÄÄâÄÄÄÄÄÄÄâÄÄÄÄÄÄÄâÄÄÄÄÄÄÄÇ
 *     a Å       å       Å       å       Å a  absolute      all dies without
 *       Å focus å       Å       å drive Å
 *     n Å       å       Å       å       Å n  need          functions missing/fail
 *  i    á∑∑∑∑∑∑∑ä∑∑∑∑∑∑∑ä∑∑∑∑∑∑∑ä∑∑∑∑∑∑∑Ü
 *  m  v Å       å       Å       å       Å v  value         clear business case
 *  p    Å       å       Å       å       Å
 *  o  c Å       å       Å       å       Å c  crave         really, really wanna
 *  r    áÄÄÄÄÄÄÄäÄÄÄÄÄÄÄäÄÄÄÄÄÄÄäÄÄÄÄÄÄÄÜ
 *  t  w Å       å       Å       å       Å g  good          can see its use
 *  a    Å       å       Å       å       Å
 *  n  l Å       å       Å       å       Å l  like          kinda cool
 *  c    á∑∑∑∑∑∑∑ä∑∑∑∑∑∑∑ä∑∑∑∑∑∑∑ä∑∑∑∑∑∑∑Ü
 *  e  m Å       å       Å       å       Å i  idea          pondering
 *       Å waste å       Å       å weeds Å
 *     ∑ Å       å       Å       å       Å ∑  backlog
 *       ÑÄÄÄÄÄÄÄàÄÄÄÄÄÄÄàÄÄÄÄÄÄÄàÄÄÄÄÄÄÄÖ
 *         ∑   y   m   w   d   s   t   !
 *
 *         b   y   m   w   d   s   t   n
 *         a   e   o   e   a   o   o   o
 *         c   a   n   e   y   o   d   w
 *         k   r   t   k   s   n   a   ∑
 *         l   s   h   s       e   y   n
 *         o       s           s       o
 *         g                   t       w
 *
 */




#define  MAX_DECODE      50
typedef  struct  cDECODE  tDECODE;
struct cDECODE {
   char        cat;
   char        sub;
   char       *label;
   char       *desc;
};
const tDECODE g_decode   [] = {
   /*---(urgency)------------------------*/
   { METIS_URG, '!', "now-now"     , "drop everything and get it done before anything else"         },
   { METIS_URG, 't', "today"       , "need it by end-of-day, or worst before work starts tomorrow"  },
   { METIS_URG, 's', "soonest"     , "do your best to get it done soon, don't get distracted"       },
   { METIS_URG, 'd', "days"        , "complete in a couple of days, or at least this week"          },
   { METIS_URG, 'w', "weeks"       , "complete in a couple weeks, or at least this month"           },
   { METIS_URG, 'm', "months"      , "complete in a couple months, or at least under a year"        },
   { METIS_URG, 'y', "years"       , "this task is long-term and is expected to be over a year"     },
   { METIS_URG, '-', "backlog"     , "not been assigned an urgency"                                 },
   /*---(importance)---------------------*/
   { METIS_IMP, 'a', "absolute"    , "true work/life or fail/death for project, app, or me"         },
   { METIS_IMP, 'r', "require"     , "no choice, it is required to meet objectives"                 },
   { METIS_IMP, 'v', "value"       , "adds solid, quantifiable value to the objective"              },
   { METIS_IMP, 'c', "crave"       , "very strong want, true belief that this is necessary"         },
   { METIS_IMP, 'g', "good"        , "good idea, but not absolutely needed, in the final product"   },
   { METIS_IMP, 'l', "like"        , "nice to have, but only if it comes unforced/naturally"        },
   { METIS_IMP, 'i', "idea"        , "plausable, could be done, but there is no real push"          },
   { METIS_IMP, '-', "backlog"     , "not been assigned an importance"                              },
   /*---(estimate)-----------------------*/
   { METIS_EST, '+', "longer"      , "longer than a full day of work"                               },
   { METIS_EST, '8', "8hrs"        , "full day of work, or possibly until start of the next day"    },
   { METIS_EST, '4', "4hrs"        , "half day of work, which means serious focus and dedication"   },
   { METIS_EST, '2', "2hrs"        , "couple hours, meaning dedicated focus and continuous time"    },
   { METIS_EST, '1', "1hr"         , "full hour of work is a moderate task that needs focus"        },
   { METIS_EST, 'h', "30m"         , "half hour of work is usually smaller, but needs attention"    },
   { METIS_EST, 'q', "quick"       , "quick task that likely takes a little thought and prep"       },
   { METIS_EST, '-', "backlog"     , "not been assigned an estimate yet"                            },
   /*---(progress)-----------------------*/
   { METIS_PRG, '-', "backlog"     , "work that has not been prepared or acted upon yet"            },
   { METIS_PRG, '<', "planned"     , "task is set for immediate focus and starting"                 },
   { METIS_PRG, '=', "active"      , "selected for today and/or working it right now"               },
   { METIS_PRG, ',', "paused"      , "waiting on change, something is holding this task up"         },
   { METIS_PRG, '>', "verify"      , "task awaiting some kind of verification or final check"       },
   { METIS_PRG, '#', "done"        , "successfully completed and any checking done"                 },
   { METIS_PRG, 'x', "cancelled"   , "detiremened this effort is no longer necessary"               },
   { METIS_PRG, '/', "reduntant"   , "covered by another task, but maybe additional detail"         },
   /*---(share)--------------------------*/
   { METIS_SHR, '∞', "focus"       , "keep on focus/primary task list"                              },
   { METIS_SHR, '*', "public"      , "allow to be picked up by shared database"                     },
   { METIS_SHR, '-', "normal"      , "locally shared, not important enough for public"              },
   { METIS_SHR, '†', "private"     , "do not allow into shared database"                            },
   { METIS_SHR, '¥', "kept"        , "kept in archive for some historical reason"                   },
   /*---(done)---------------------------*/
   {  0       ,  0 , ""            , ""                                                             },
};

static   s_total   =  0;
static   s_tries   =  0;
static   s_good    =  0;
static   s_warn    =  0;
static   s_badd    =  0;



/*====================------------------------------------====================*/
/*===----                           utility                            ----===*/
/*====================------------------------------------====================*/
static void      o___UTILITY_________________o (void) {;}

char
metis_data_purge_all    (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter    (__FUNCTION__);
   rc = metis_task_purge_all ();
   DEBUG_INPT   yLOG_value    ("tasks"     , rc);
   rc = metis_source_cleanse ();
   DEBUG_INPT   yLOG_value    ("sources"   , rc);
   rc = metis_minor_cleanse  ();
   DEBUG_INPT   yLOG_value    ("minors"    , rc);
   rc = metis_major_cleanse  ();
   DEBUG_INPT   yLOG_value    ("majors"    , rc);
   /*---(feedback)-----------------------*/
   DEBUG_INPT   yLOG_value    ("majors"    , metis_major_count  ());
   DEBUG_INPT   yLOG_value    ("minors"    , metis_minor_count  ());
   DEBUG_INPT   yLOG_value    ("tasks"     , metis_task_count   ());
   DEBUG_INPT   yLOG_value    ("sources"   , metis_source_count ());
   DEBUG_INPT   yLOG_value    ("unique"    , metis_epoch_count  ());
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit     (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                           utility                            ----===*/
/*====================------------------------------------====================*/
static void      o___PROGRAM_________________o (void) {;}

char
metis_data_init         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        i           =    0;
   char        t           [LEN_LABEL];
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter    (__FUNCTION__);
   /*---(purge tasks)--------------------*/
   metis_data_purge_all ();
   /*---(create validation strings)------*/
   ystrlcpy (METIS_URGS, "", LEN_LABEL);
   ystrlcpy (METIS_IMPS, "", LEN_LABEL);
   ystrlcpy (METIS_ESTS, "", LEN_LABEL);
   ystrlcpy (METIS_PRGS, "", LEN_LABEL);
   ystrlcpy (METIS_SHRS, "", LEN_LABEL);
   for (i = 0; i < MAX_DECODE; ++i) {
      if (g_decode [i].cat == 0)                   break;
      sprintf (t, "%c", g_decode [i].sub);
      switch (g_decode [i].cat) {
      case METIS_URG : ystrlcat (METIS_URGS, t, LEN_LABEL);   break;
      case METIS_IMP : ystrlcat (METIS_IMPS, t, LEN_LABEL);   break;
      case METIS_EST : ystrlcat (METIS_ESTS, t, LEN_LABEL);   break;
      case METIS_PRG : ystrlcat (METIS_PRGS, t, LEN_LABEL);   break;
      case METIS_SHR : ystrlcat (METIS_SHRS, t, LEN_LABEL);   break;
      }
   }
   DEBUG_INPT   yLOG_info     ("my.urgs"   , METIS_URGS);
   DEBUG_INPT   yLOG_info     ("my.imps"   , METIS_IMPS);
   DEBUG_INPT   yLOG_info     ("my.ests"   , METIS_ESTS);
   DEBUG_INPT   yLOG_info     ("my.prgs"   , METIS_PRGS);
   DEBUG_INPT   yLOG_info     ("my.shrs"   , METIS_SHRS);
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
metis_data_vikeys       (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        i           =    0;
   char        t           [LEN_LABEL];
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter    (__FUNCTION__);
   /*---(add commands and menus)---------*/
   yCMD_add (YVIHUB_M_FILE   , "refresh"     , "r"   , ""     , api_yvikeys_refresh , ""                   );
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit     (__FUNCTION__);
   return 0;
}


/*============================--------------------============================*/
/*===----                        detailed parsing                      ----===*/
/*============================--------------------============================*/
static void      o___PARSE___________________o (void) {;}

#define   MAXLINE  1000
char      g_recd [LEN_RECD] = "";      /* record from stdin                   */
char*     p         = NULL;         /* strtok() parsing pointer            */
char     *q         = "";         /* strtok() delimiters                 */


char
metis_data_catinfo      (char a_cat, char a_sub, char *a_clabel, char *a_slabel, char *a_desc)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   /*---(defaults)-----------------------*/
   if (a_clabel != NULL)  ystrlcpy (a_clabel, "unknown", LEN_LABEL);
   if (a_slabel != NULL)  ystrlcpy (a_slabel, "unknown", LEN_LABEL);
   if (a_desc   != NULL)  ystrlcpy (a_desc  , "unknown", LEN_HUND );
   /*---(take care of cat)---------------*/
   if (a_clabel != NULL) {
      switch (a_cat) {
      case METIS_URG : ystrlcpy (a_clabel, "urgency"     , LEN_LABEL); break;
      case METIS_IMP : ystrlcpy (a_clabel, "importance"  , LEN_LABEL); break;
      case METIS_EST : ystrlcpy (a_clabel, "estimate"    , LEN_LABEL); break;
      case METIS_PRG : ystrlcpy (a_clabel, "progress"    , LEN_LABEL); break;
      case METIS_SHR : ystrlcpy (a_clabel, "sharing"     , LEN_LABEL); break;
      }
   }
   /*---(walk thru subs)-----------------*/
   for (i = 0; i < MAX_DECODE; ++i) {
      if (g_decode [i].cat == 0)       break;
      if (g_decode [i].cat != a_cat)  continue;
      if (g_decode [i].sub != a_sub)  continue;
      if (a_slabel != NULL)  ystrlcpy (a_slabel, g_decode [i].label, LEN_LABEL);
      if (a_desc   != NULL)  ystrlcpy (a_desc  , g_decode [i].desc , LEN_HUND );
      return 0;
   }
   /*---(complete)-----------------------*/
   return -1;
}

char
metis_data_stats   (tTASK *a_task, char *a_stats)
{
   /*------------------------------------*/
   /*  return negative if catastrophic for the task
    *  return positive if warning about illegal value (force default)
    */
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         x_len       =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter    (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point    ("a_task"    , a_task);
   --rce;  if (a_task == NULL) {
      DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   DEBUG_INPT   yLOG_point    ("a_stats"   , a_stats);
   if (a_stats == NULL)   x_len = 0;
   else                   x_len = strlen (a_stats);
   DEBUG_INPT   yLOG_value    ("x_len"     , x_len);
   /*---(urgency)-------------------------*/
   if      (x_len < 1)                              a_task->urg = '-';
   else if (a_stats [0] == '∑')                     a_task->urg = '-';
   else if (strchr (METIS_URGS, a_stats [0]) != NULL)  a_task->urg = a_stats [0];
   else {
      yURG_err ('w', "urgency (%c) not in Â%sÊ, therefore replaced with ?", a_stats [0], METIS_URGS);
      a_task->urg = '?';
   }
   DEBUG_INPT   yLOG_char     ("urg"       , a_task->urg);
   /*---(importance)----------------------*/
   if      (x_len < 2)                              a_task->imp = '-';
   else if (a_stats [1] == '∑')                     a_task->imp = '-';
   else if (strchr (METIS_IMPS, a_stats [1]) != NULL)  a_task->imp = a_stats [1];
   else  {
      yURG_err ('w', "importance (%c) not in Â%sÊ, therefore replaced with ?", a_stats [1], METIS_IMPS);
      a_task->imp = '?';
   }
   DEBUG_INPT   yLOG_char     ("imp"       , a_task->imp);
   /*---(estimate)------------------------*/
   if      (x_len < 3)                              a_task->est = '-';
   else if (a_stats [2] == '∑')                     a_task->est = '-';
   else if (strchr (METIS_ESTS, a_stats [2]) != NULL)  a_task->est = a_stats [2];
   else {
      yURG_err ('w', "estimate (%c) not in Â%sÊ, therefore replaced with ?", a_stats [2], METIS_ESTS);
      a_task->est = '?';
   }
   DEBUG_INPT   yLOG_char     ("est"       , a_task->est);
   /*---(progress)------------------------*/
   if      (x_len < 4)                              a_task->prg = '-';
   else if (a_stats [3] == '∑')                     a_task->prg = '-';
   else if (strchr (METIS_PRGS, a_stats [3]) != NULL)  a_task->prg = a_stats [3];
   else  {
      yURG_err ('w', "progress (%c) not in Â%sÊ, therefore replaced with ?", a_stats [3], METIS_PRGS);
      a_task->prg = '?';
   }
   DEBUG_INPT   yLOG_char     ("prg"       , a_task->prg);
   /*---(shares)--------------------------*/
   if      (x_len < 5)                              a_task->shr = '-';
   else if (a_stats [4] == '∑')                     a_task->shr = '-';
   else if (strchr (METIS_SHRS, a_stats [4]) != NULL)  a_task->shr = a_stats [4];
   else  {
      yURG_err ('w', "shared (%c) not in Â%sÊ, therefore replaced with ?", a_stats [4], METIS_SHRS);
      a_task->shr = '?';
   }
   DEBUG_INPT   yLOG_char     ("shr"       , a_task->shr);
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
metis_data_header       (char *a_recd, tMAJOR **r_major, tMINOR **r_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_recd      [LEN_RECD];
   char       *p           = NULL;
   char       *q           = "ß";
   int         l           =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter    (__FUNCTION__);
   /*---(defenses)----------------------*/
   DEBUG_INPT   yLOG_point   ("r_major"   , r_major);
   --rce;  if (r_major == NULL)  {
      DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
      return  rce;
   }
   *r_major = NULL;
   DEBUG_INPT   yLOG_point   ("r_minor"   , r_minor);
   --rce;  if (r_minor == NULL)  {
      DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
      return  rce;
   }
   *r_minor = NULL;
   DEBUG_INPT   yLOG_point   ("a_recd"    , a_recd);
   --rce;  if (a_recd == NULL)  {
      DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
      return  rce;
   }
   ystrlcpy (x_recd, a_recd, LEN_RECD);
   /*---(create major)-------------------*/
   p = strtok  (x_recd, q);
   DEBUG_INPT   yLOG_point   ("p"         , p);
   --rce;  if (p == NULL) {
      DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   ystrltrim (p, ySTR_BOTH, LEN_LABEL);
   rc = metis_major_new (p, '-', r_major);
   DEBUG_INPT   yLOG_value   ("new"       , rc);
   DEBUG_INPT   yLOG_point   ("*r_major"  , *r_major);
   --rce;  if (*r_major == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("->name"    , (*r_major)->name);
   /*---(create minor)-------------------*/
   p = strtok  (NULL, q);
   DEBUG_INPT   yLOG_point   ("p"         , p);
   --rce;  if (p == NULL) {
      DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   ystrltrim (p, ySTR_BOTH, LEN_LABEL);
   rc = metis_minor_new (*r_major, p, '-', r_minor);
   DEBUG_INPT   yLOG_value   ("new"       , rc);
   DEBUG_INPT   yLOG_point   ("*r_minor"  , *r_minor);
   --rce;  if (*r_minor == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("->name"    , (*r_minor)->name);
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit     (__FUNCTION__);
   return 0;
}

char             /* [p-----] parse task detail -------------------------------*/
metis_data_parsing      (tMINOR *a_minor, tSOURCE *a_source, int a_line, char *a_recd)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_recd      [LEN_RECD];
   int         x_len       =    0;
   char       *p           = NULL;
   char       *q           = "ß";
   tTASK      *x_task      = NULL;
   tTASK      *x_exist     = NULL;
   char        x_mongo     [LEN_TERSE] = "";
   long        x_date      =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter    (__FUNCTION__);
   /*---(defenses)----------------------*/
   DEBUG_INPT   yLOG_point    ("a_minor"   , a_minor);
   --rce;  if (a_minor == NULL) {
      DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
      return  rce;
   }
   DEBUG_INPT   yLOG_point    ("a_source"  , a_source);
   --rce;  if (a_source == NULL) {
      DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
      return  rce;
   }
   DEBUG_INPT   yLOG_point    ("a_recd"    , a_recd);
   --rce;  if (a_recd  == NULL) {
      DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
      return  rce;
   }
   DEBUG_INPT   yLOG_info     ("a_recd"    , a_recd);
   ystrlcpy (x_recd, a_recd, LEN_RECD);
   x_len = strlen (x_recd);
   DEBUG_INPT   yLOG_value    ("x_len"     , x_len);
   --rce;  if (x_len <  10) {
      DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
      return  rce;
   }
   yURG_msg ('-', "line %3d parsing %dÂ%sÊ ", a_line, x_len, x_recd);
   /*---(cut front off)-----------------*/
   p = strtok  (x_recd, q);
   --rce;  if (p == NULL) {
      DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
      return  rce;
   }
   /*---(create task)--------------------*/
   rc = metis_task_new (a_minor, '-', &x_task);
   DEBUG_INPT   yLOG_value   ("new"       , rc);
   DEBUG_INPT   yLOG_point   ("x_task"    , x_task);
   --rce;  if (x_task == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(statistics)--------------------*/
   p = strtok  (NULL, q);
   --rce;  if (p == NULL) {
      metis_task_free (&x_task);
      DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
      return  rce;
   }
   ystrltrim (p, ySTR_BOTH, LEN_LABEL);
   rc = metis_data_stats (x_task, p);
   --rce;  if (rc < 0) {
      metis_task_free (&x_task);
      DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
      return  rce;
   }
   /*---(text)--------------------------*/
   p = strtok  (NULL, q);
   if (p == NULL) {
      yURG_err ('f', "record contains no text for task");
      metis_task_free (&x_task);
      DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
      return  rce;
   }
   ystrltrim (p, ySTR_BOTH, LEN_HUND);
   if (strlen (p) <= 0) {
      yURG_err ('f', "record contains no text for task");
      metis_task_free (&x_task);
      DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
      return  rce;
   }
   ystrlcpy  (x_task->txt, p, LEN_HUND);
   /*---(unique/start)------------------*/
   p = strtok  (NULL, q);
   if (p == NULL) {
      yURG_err ('f', "record contains no unique epoch/date");
      metis_task_free (&x_task);
      DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
      return  rce;
   }
   ystrltrim (p, ySTR_BOTH, LEN_LABEL);
   x_len = strlen (p);
   DEBUG_INPT   yLOG_value    ("x_len"     , x_len);
   --rce; if (x_len ==  0) {
      yURG_err ('f', "record contains no unique epoch/date");
      metis_task_free (&x_task);
      DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
      return  rce;
   }
   --rce;  if (x_len ==  6) {
      DEBUG_INPT   yLOG_note     ("date/epoch in mongo form");
      ystr4mongo (p, &x_date);
      if (x_date > time (NULL) + 86400 * 3) {
         DEBUG_INPT   yLOG_note     ("date in future more than 3 days, illegal");
         yURG_err ('f', "time code Â%sÊÂ%dÊ more than three days in future", p, x_date);
         metis_task_free (&x_task);
         DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
         return  rce;
      }
      /*> if (x_date < time (NULL) - 86400 * 100) {                                        <* 
       *>    DEBUG_INPT   yLOG_note     ("date older than 100 days, illegal");             <* 
       *>    yURG_err ('f', "time code Â%sÊÂ%dÊ more than 100 days in past", p, x_date);   <* 
       *>    metis_task_free (&x_task);                                                    <* 
       *>    DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);                               <* 
       *>    return  rce;                                                                  <* 
       *> }                                                                                <*/
      ystrlcpy (x_task->epoch, p, LEN_TERSE);
   } else if   (x_len == 10) {
      DEBUG_INPT   yLOG_note     ("date/epoch in epoch form");
      if (atoi (p) > time (NULL) + 86400 * 3) {
         DEBUG_INPT   yLOG_note     ("date in future more than 3 days, illegal");
         yURG_err ('f', "time code (epoch) Â%sÊÂ%sÊ more than three days in future", p, x_mongo);
         metis_task_free (&x_task);
         DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
         return  rce;
      }
      /*> if (atoi (p) < time (NULL) - 86400 * 100) {                                               <* 
       *>    DEBUG_INPT   yLOG_note     ("date older than 100 days");                               <* 
       *>    yURG_err ('f', "time code (epoch) Â%sÊÂ%sÊ more than 100 days in past", p, x_mongo);   <* 
       *>    metis_task_free (&x_task);                                                             <* 
       *>    DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);                                        <* 
       *>    return  rce;                                                                           <* 
       *> }                                                                                         <*/
      ystr2mongo (atoi (p), x_mongo);
      ystrlcpy (x_task->epoch, x_mongo, LEN_TERSE);
   } else {
      yURG_err ('f', "unique epoch/date is neither an epoch or mongo date");
      metis_task_free (&x_task);
      DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
      return  rce;
   }
   metis_epoch_by_name (x_task->epoch, &x_exist);
   DEBUG_INPT   yLOG_point   ("x_exist"   , x_exist);
   --rce;  if (x_exist != NULL) {
      yURG_err ('f', "time code Â%sÊ matches another task (illegal)", x_task->epoch);
      metis_task_free (&x_task);
      DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
      return  rce;
   }
   /*---(hook to unique)-----------------*/
   rc = ySORT_hook (B_UNIQUE, x_task, x_task->epoch, &(x_task->unique));
   DEBUG_INPT   yLOG_value   ("hook"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = ySORT_prepare (B_UNIQUE);
   /*---(hook to source)-----------------*/
   rc = metis_source_hook (a_source, x_task);
   DEBUG_INPT   yLOG_value   ("hook"      , rc);
   --rce;  if (rc < 0) {
      metis_task_free (&x_task);
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check days)--------------------*/
   p = strtok  (NULL, q);
   if (p != NULL) {
      ystrltrim (p, ySTR_BOTH, LEN_LABEL);
      x_task->days = atoi (p);
   }
   /*---(source)------------------------*/
   x_task->line = a_line;
   /*---(update sort)-------------------*/
   metis_filter_key (x_task);
   snprintf (x_task->srch, LEN_FULL, "%c%c%c%c%c %-20.20s %-30.30s %s",
         x_task->urg, x_task->imp, x_task->est, x_task->prg, x_task->shr,
         x_task->minor->major->name, x_task->minor->name, x_task->txt);
   /*---(stats)--------------------------*/
   DEBUG_INPT   yLOG_value    ("majors"    , metis_major_count ());
   DEBUG_INPT   yLOG_value    ("minors"    , metis_minor_count ());
   DEBUG_INPT   yLOG_value    ("tasks"     , metis_task_count ());
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit     (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         source files                         ----===*/
/*====================------------------------------------====================*/
static void      o___SOURCE__________________o (void) {;}

char
metis_data_file         (tMINOR *a_minor, tSOURCE *a_source, char a_type)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char       *p           = NULL;
   char        x_proj      [LEN_LABEL];
   char        x_recd      [LEN_RECD];
   int         x_len       =    0;
   int         x_line      =    0;
   int         x_try       =    0;
   int         x_good      =    0;
   int         x_warn      =    0;
   int         x_badd      =    0;
   int         x_off       =    0;
   FILE       *f           = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter    (__FUNCTION__);
   /*---(open)---------------------------*/
   f = fopen (a_source->path, "r");
   --rce;  if (f == NULL) {
      DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   yURG_msg ('>', "%s", a_source->path);
   /*---(walk the entries)---------------*/
   --rce;  while (1) {
      /*---(read)------------------------*/
      fgets (x_recd, LEN_RECD, f);
      ++x_line;
      DEBUG_INPT   yLOG_value    ("x_line"    , x_line);
      if (feof (f))    break;
      /*---(filter)----------------------*/
      ystrldchg (x_recd, '', 'ß', LEN_RECD);
      ystrltrim (x_recd, ySTR_SINGLE, LEN_RECD);
      x_len = strlen (x_recd);
      DEBUG_INPT   yLOG_value    ("x_len"     , x_len);
      if (x_len < 10)    continue;
      /*---(remove newline)--------------*/
      if (x_recd [x_len - 1] == '\n')  x_recd [--x_len] = '\0';
      DEBUG_INPT   yLOG_info     ("x_recd"    , x_recd);
      /*---(check prefix)----------------*/
      if        (strchr ("ch", a_type) != NULL && (strncmp (x_recd, "/* metis "   ,  9) == 0 || strncmp (x_recd, "/* METIS "   ,  9) == 0)) {
         if (strncmp (x_recd + 9, "ß ", 2) == 0) {
            DEBUG_INPT   yLOG_note     ("FOUND single-line or open comment (1) version");
         } else {
            DEBUG_INPT   yLOG_note     ("prefixed as single-line or open comment, but no field separator");
            continue;
         }
      } else if (strchr ("ch", a_type) != NULL && (strncmp (x_recd, "* metis "    ,  8) == 0 ||strncmp (x_recd, "* METIS "    ,  8) == 0)) {
         if (strncmp (x_recd + 8, "ß ", 2) == 0) {
            DEBUG_INPT   yLOG_note     ("FOUND continuing comment (2) version");
         } else {
            DEBUG_INPT   yLOG_note     ("prefixed as continuing comment, but no field separator");
            continue;
         }
      } else if (a_type == 'u'                 && (strncmp (x_recd, "#> metis "   ,  9) == 0 || strncmp (x_recd, "#> METIS "   ,  9) == 0)) {
         if (strncmp (x_recd + 9, "ß ", 2) == 0) {
            DEBUG_INPT   yLOG_note     ("FOUND traditional non-code comment (3) version");
         } else {
            DEBUG_INPT   yLOG_note     ("prefixed as traditional non-code comment, but no field separator");
            continue;
         }
      } else continue;
      /*---(check metis tag)-------------*/
      rc = metis_data_parsing (a_minor, a_source, x_line, x_recd);
      ++x_try;
      if (rc >  0) {
         DEBUG_INPT   yLOG_note     ("WARNING on parsing");
         ++x_warn;
      } else if (rc == 0) {
         DEBUG_INPT   yLOG_note     ("success on parsing");
         ++x_good;
      } else {
         DEBUG_INPT   yLOG_note     ("FAILURE on parsing");
         ++x_badd;
      }
   }
   fclose(f);
   if      (x_try  == 0)  yURG_msg ('-', "%d lines checked, no metis entries found", x_line);
   else if (x_badd >  0)  yURG_msg ('-', "failure, %d lines checked, %d parsing attempted, %d failed, %d warned, %d passed", x_line, x_try, x_badd, x_warn, x_good);
   else if (x_warn >  0)  yURG_msg ('-', "warning, %d lines checked, %d parsing attempted, %d warned, %d passed", x_line, x_try, x_warn, x_good);
   else                   yURG_msg ('-', "success, %d lines checked, %d parsing attempted, %d passed", x_line, x_try, x_good);
   yURG_msg (' ', "");
   /*---(stats)--------------------------*/
   s_total += x_line;
   s_tries += x_try;
   s_good  += x_good;
   s_warn  += x_warn;
   s_badd  += x_badd;
   /*---(stats)--------------------------*/
   DEBUG_INPT   yLOG_value    ("majors"    , metis_major_count ());
   DEBUG_INPT   yLOG_value    ("minors"    , metis_minor_count ());
   DEBUG_INPT   yLOG_value    ("tasks"     , metis_task_count ());
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit     (__FUNCTION__);
   return 0;
}

char         /*--> make a list of input files --------------------------------*/
metis_data_directory    (tMAJOR *a_major, char *a_home)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;          /* return code for errors         */
   int         rc          =    0;          /* generic return code            */
   int         i           =    0;
   int         x_suflen    =    0;
   DIR        *x_dir       = NULL;          /* directory pointer              */
   tDIRENT    *x_file      = NULL;          /* directory entry pointer        */
   char        x_name      [LEN_TITLE];      /* file name                      */
   int         x_len       =    0;
   int         x_read      =    0;          /* count of entries reviewed      */
   int         x_good      =    0;          /* count of entries processed     */
   tMINOR     *x_minor     = NULL;
   char        x_full      [LEN_PATH]  = "";
   tSOURCE    *x_source    = NULL;
   char        x_pass      =  '-';
   int         l           =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(open dir)-----------------------*/
   x_dir = opendir(".");
   DEBUG_INPT   yLOG_point   ("x_dir"      , x_dir);
   --rce;  if (x_dir == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return  rce;
   }
   DEBUG_INPT   yLOG_note    ("openned successfully");
   while (1) {
      DEBUG_INPT   yLOG_note    ("processing entries");
      /*---(read a directory entry)------*/
      x_file = readdir (x_dir);
      DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
      if (x_file == NULL)  break;
      ++x_read;
      /*---(filter by name)--------------*/
      ystrlcpy (x_name, x_file->d_name, LEN_TITLE);
      DEBUG_INPT   yLOG_info    ("x_name"    , x_name);
      if (x_name [0] == '.')  {
         DEBUG_INPT   yLOG_note    ("hidden, SKIP");
         continue;
      }
      /*---(check suffixes)--------------*/
      x_pass = '-';
      x_len = strlen (x_name);
      DEBUG_INPT   yLOG_value   ("x_len"     , x_len);
      if (x_len >= 3 && strncmp (x_name + x_len - 2, ".c"     , 2) == 0) {
         if (x_len >= 8 && strncmp (x_name + x_len - 7, "_unit.c", 7) == 0) {
            x_pass = '-';
         } else {
            DEBUG_INPT   yLOG_note    ("normal c source file");
            x_pass = 'c';
         }
      }
      if (x_len >= 3 && strncmp (x_name + x_len - 2, ".h"     , 2) == 0) {
         DEBUG_INPT   yLOG_note    ("normal c header file");
         x_pass = 'h';
      }
      if (x_len >= 6 && strncmp (x_name + x_len - 5, ".unit"  , 5) == 0) {
         DEBUG_INPT   yLOG_note    ("koios unit testing file");
         x_pass = 'u';
      }
      if (x_pass == '-') {
         DEBUG_INPT   yLOG_note    ("not an acceptible metis line source");
         continue;
      }
      /*---(create file/minor)--------------*/
      l = strlen (a_major);
      if (strncmp (x_name, a_major, l) != 0)  l = 0;
      rc = metis_minor_new (a_major, x_name + l, 'y', &x_minor);
      DEBUG_INPT   yLOG_value   ("new"       , rc);
      DEBUG_INPT   yLOG_point   ("x_minor"   , x_minor);
      --rce;  if (x_minor == NULL) {
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_INPT   yLOG_info    ("->name"    , x_minor->name);
      /*---(create source)------------------*/
      sprintf (x_full, "%s/%s", a_home, x_name);
      rc = metis_source_new (x_full, DATA_SOURCES, 'y', &x_source);
      DEBUG_INPT   yLOG_value   ("new"       , rc);
      DEBUG_INPT   yLOG_point   ("x_source"  , x_source);
      --rce;  if (x_source == NULL) {
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_INPT   yLOG_info    ("->path"    , x_source->path);
      /*---(save)------------------------*/
      metis_data_file (x_minor, x_source, x_pass);
      ++x_good;
      DEBUG_INPT   yLOG_note    ("added to inventory");
      /*---(done)------------------------*/
   }
   DEBUG_INPT   yLOG_value   ("x_read"    , x_read);
   DEBUG_INPT   yLOG_value   ("x_good"    , x_good);
   /*---(close dir)----------------------*/
   DEBUG_INPT   yLOG_note    ("closing directory");
   rc = closedir (x_dir);
   DEBUG_INPT   yLOG_value   ("close_rc"  , rc);
   /*> printf ("   end-of-files\n\n\n");                                              <*/
   /*---(complete)------------------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
metis_data_project      (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_home      [LEN_HUND]  = "";
   char        x_name      [LEN_LABEL] = "";
   tMAJOR     *x_major     = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   rc = ystrlhere (x_home, x_name);
   DEBUG_INPT   yLOG_value   ("ystrlhere"  , rc);
   if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("x_home"    , x_home);
   DEBUG_INPT   yLOG_info    ("x_name"    , x_name);
   /*---(create project/major)-----------*/
   rc = metis_major_new (x_name, '-', &x_major);
   DEBUG_INPT   yLOG_value   ("new"       , rc);
   DEBUG_INPT   yLOG_point   ("x_major"   , x_major);
   --rce;  if (x_major == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("->name"    , x_major->name);
   /*---(run files)----------------------*/
   s_total = s_tries = s_good  = s_warn  = s_badd  = 0;
   rc = metis_data_directory (x_major, x_home);
   /*---(purge empties)------------------*/
   metis_source_cleanse ();
   metis_minor_cleanse  ();
   metis_major_cleanse  ();
   /*---(stats)--------------------------*/
   DEBUG_INPT   yLOG_value    ("majors"    , metis_major_count ());
   DEBUG_INPT   yLOG_value    ("minors"    , metis_minor_count ());
   DEBUG_INPT   yLOG_value    ("tasks"     , metis_task_count ());
   /*---(complete)------------------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   if      (s_badd > 0)  return -1;
   else if (s_warn > 0)  return  1;
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       other sources                          ----===*/
/*====================------------------------------------====================*/
static void      o___OTHER___________________o (void) {;}

char             /* [G-----] read all tasks from the file --------------------*/
DATA__stdin        (void)
{
   return 0;
}

char             /* [G-----] read all tasks from the file --------------------*/
metis_data_read         (char *a_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tMAJOR     *x_major     = NULL;
   tMINOR     *x_minor     = NULL;
   tSOURCE    *x_source    = NULL;
   int         x_line      =    0;
   char        x_prefix    [LEN_LABEL]  = "";
   char        x_full      [LEN_RECD]   = "";
   FILE       *f           = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("a_file"    , a_file);
   --rce;  if (a_file == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_file"    , a_file);
   /*---(open)---------------------------*/
   f = fopen (a_file, "r");
   DEBUG_INPT   yLOG_point   ("f"         , f);
   --rce;  if (f == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(create source)------------------*/
   rc = metis_source_new (a_file, DATA_FILE, 'y', &x_source);
   DEBUG_INPT   yLOG_point   ("x_source"  , x_source);
   --rce;  if (x_source == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("->path"    , x_source->path);
   /*---(walk file)----------------------*/
   while (1) {
      /*---(get a line)------------------*/
      fgets (g_recd, MAXLINE, f);
      if (feof(f))   break;
      /*---(get rid of the newline)------*/
      p = strchr (g_recd, '\n');           /* look for a newline              */
      if (p != NULL)   strcpy (p, "\0");   /* if found, copy '\0' over it     */
      /*---(defenses)--------------------*/
      if (strncmp (g_recd, "# end_of_visible", 16) == 0)   break;
      ++x_line;
      if (g_recd [0] == '#')   continue;
      if (g_recd [0] == '\0')  continue;
      /*---(handle real lines)-----------*/
      if (g_recd [0] == ' ') {
         ystrlcpy  (x_prefix, g_recd, LEN_LABEL);
         ystrltrim (x_prefix, ySTR_BOTH, LEN_LABEL);
         if (strncmp ("metis ", x_prefix, 6) == 0) {
            rc = metis_data_parsing (x_minor, x_source, x_line, g_recd);
         } else {
            snprintf (x_full, LEN_RECD, "metis ß %s", g_recd);
            rc = metis_data_parsing (x_minor, x_source, x_line, x_full);
         }
      } else {
         rc = metis_data_header  (g_recd, &x_major, &x_minor);
      }
   }
   /*---(close)--------------------------*/
   fclose (f);
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       main driver                            ----===*/
/*====================------------------------------------====================*/
static void      o___DRIVER__________________o (void) {;}

char
metis_data_refresh      (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   static int  c           =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_info    ("all"       , DATA_ALL);
   DEBUG_INPT   yLOG_char    ("source"    , my.source);
   --rce;  if (my.source == 0 || my.source == DATA_NONE) {
      yURG_err ('F', "data source set to (%c) NONE; must set with command-line option", my.source);
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (strchr (DATA_ALL, my.source) == NULL) {
      yURG_err ('F', "data source set to (%c) UNKNOWN; must set with command-line option", my.source);
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(initialize)---------------------*/
   DEBUG_INPT   yLOG_value   ("c"         , c);
   if (my.source == DATA_PIPE && c > 0) {
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*> metis_data_purge_all ();                                                       <*/
   /*---(update)-------------------------*/
   --rce;  switch (my.source) {
   case DATA_DATABASE :
      rc = metis_db_read      ();
      break;
   case DATA_SOURCES  :
      rc = metis_data_project ();
      break;
   case DATA_FILE     :
      rc = metis_data_read    (my.file);
      break;
   case DATA_PIPE     :
      rc = DATA__stdin        ();
      break;
   }
   ++c;
   /*---(refresh others)-----------------*/
   CROW = CCOL = 0;
   yMARK_execute ("/");
   /*---(complete)------------------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                      unit testing                            ----===*/
/*====================------------------------------------====================*/
static void  o___UNITTEST________o () { return; }

char          unit_answer [LEN_FULL];

char*        /*-> unit test accessor -----------------[ light  [us.B60.2A3.F2]*/ /*-[01.0000.00#.#]-*/ /*-[--.---.---.--]-*/
DATA__unit         (char *a_question, int a_num)
{
   /*---(locals)-------------------------*/
   char        rc          =    0;
   char        t           [LEN_HUND]   = "[]";
   char        s           [LEN_HUND]   = "[]";
   /*---(overall)------------------------*/
   strcpy (unit_answer, "COL              : question not understood");
   /*> if      (strcmp (a_question, "count"         ) == 0) {                         <* 
    *>    snprintf (unit_answer, LEN_FULL, "DATA count       : %d", g_ntask);         <* 
    *> }                                                                              <*/
   /*> else if (strcmp (a_question, "stats"         ) == 0) {                                                                                                                                                                  <* 
    *>    if (a_num < g_ntask) {                                                                                                                                                                                               <* 
    *>       snprintf (unit_answer, LEN_FULL, "DATA stats  (%2d) : urg %c, imp %c, est %c, prog %c   %3d", a_num, g_tasks [a_num].urg, g_tasks [a_num].imp, g_tasks [a_num].est, g_tasks [a_num].prg, g_tasks [a_num].line);   <* 
    *>    } else {                                                                                                                                                                                                             <* 
    *>       snprintf (unit_answer, LEN_FULL, "DATA stats  (%2d) : urg -, imp -, est -, prog -     -", a_num);                                                                                                                 <* 
    *>    }                                                                                                                                                                                                                    <* 
    *> }                                                                                                                                                                                                                       <*/
   /*> else if (strcmp (a_question, "header"        ) == 0) {                                                               <* 
    *>    sprintf  (s, "[%.20s]", s_one);                                                                                   <* 
    *>    sprintf  (t, "[%.20s]", s_two);                                                                                   <* 
    *>    snprintf (unit_answer, LEN_FULL, "DATA header      : %2d%-22.22s %2d%s", strlen (s) - 2, s, strlen (t) - 2, t);   <* 
    *> }                                                                                                                    <*/
   /*> else if (strcmp (a_question, "cats"          ) == 0) {                                                                       <* 
    *>    if (a_num < g_ntask) {                                                                                                    <* 
    *>       sprintf  (s, "[%.20s]", g_tasks [a_num].one);                                                                          <* 
    *>       sprintf  (t, "[%.20s]", g_tasks [a_num].two);                                                                          <* 
    *>    }                                                                                                                         <* 
    *>    snprintf (unit_answer, LEN_FULL, "DATA cats   (%2d) : %2d%-22.22s %2d%s", a_num, strlen (s) - 2, s, strlen (t) - 2, t);   <* 
    *> }                                                                                                                            <*/
   /*> else if (strcmp (a_question, "text"          ) == 0) {                                        <* 
    *>    if (a_num < g_ntask) {                                                                     <* 
    *>       sprintf  (s, "[%.35s]", g_tasks [a_num].txt);                                           <* 
    *>    }                                                                                          <* 
    *>    snprintf (unit_answer, LEN_FULL, "DATA text   (%2d) : %2d%s", a_num, strlen (s) - 2, s);   <* 
    *> }                                                                                             <*/
   /*---(complete)-----------------------*/
   return unit_answer;
}



/*============================----end-of-source---============================*/
