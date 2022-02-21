/*============================----beg-of-source---============================*/
#include   "metis.h"



/*
 * 12345 � 12345 � 12345678901-12345678901-12345678901-12345678901-12345678901-12345678901- � ---beg---- � ---end---- �
 *
 * metis � dw2#� � add data refresh command and check                                       � 1645047879 � 1645055000 �
 * metis � dw2�� � add data refresh to menus                                                � 1645047880 � ���������� �
 * metis � ww4-� � add mark to tasks so that they can be selected to a short list           � 1645047881 � ���������� �
 * metis � ww4-� � allow forced voids for appearance, like row 18 or col 2 or 2x/4y         � 1645047882 � ���������� �
 * metis � ww4-� � add sharing flag to control database usage and marking                   � 1645047883 � ���������� �
 * metis � wl4-� � switch beg and end dates to pseudo-mongo (6 chars)                       � 1645162236 � ���������� �
 *
 */

/*
 *                    urgency
 *
 *         -   y   m   w   d   s   t   !
 *       ���������������������������������
 *     a � focus �       �       � drive � a  absolute
 *       �       �       �       �       �
 *     n �       �       �       �       � n  need
 *  i    ���������������������������������
 *  m  v �       �       �       �       � v  value
 *  p    �       �       �       �       �
 *  o  c �       �       �       �       � c  crave
 *  r    ���������������������������������
 *  t  w �       �       �       �       � w  want
 *  a    �       �       �       �       �
 *  n  l �       �       �       �       � l  like
 *  c    ���������������������������������
 *  e  m �       �       �       �       � m  might
 *       �       �       �       �       �
 *     - � dump  �       �       � weeds � -  backlog
 *       ���������������������������������
 *         -   y   m   w   d   s   t   !
 *
 *         b   y   m   w   d   s   t   n
 *         a   e   o   e   a   o   o   o
 *         c   a   n   e   y   o   d   w
 *         k   r   t   k   s   n   a   �
 *         l   s   h   s       e   y   n
 *         o       s           s       o
 *         g                   t       w
 *
 */

tCARD       g_tasks [MAX_CARDS];
int         g_ntask       =  0;



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
   { 'u', '!', "now-now"     , "drop everything and get it done before anything else"         },
   { 'u', 't', "today"       , "need it by end-of-day, or worst before work starts tomorrow"  },
   { 'u', 's', "soonest"     , "do your best to get it done soon, don't get distracted"       },
   { 'u', 'd', "days"        , "complete in a couple of days, or at least this week"          },
   { 'u', 'w', "weeks"       , "complete in a couple weeks, or at least this month"           },
   { 'u', 'm', "months"      , "complete in a couple months, or at least under a year"        },
   { 'u', 'y', "years"       , "this task is long-term and is expected to by over a year"     },
   { 'u', '-', "backlog"     , "not been assigned an urgency"                                 },
   /*---(importance)---------------------*/
   { 'i', 'a', "absolute"    , "this is a true life or death for project, app, or me"         },
   { 'i', 'n', "need"        , "must be completed, fact it is required to meeet objective"    },
   { 'i', 'v', "value"       , "adds solid, logic additional value to the objective"          },
   { 'i', 'c', "crave"       , "very strong want, true belief that this is necessary"         },
   { 'i', 'w', "want"        , "desired, but not absolutely needed, in the final product"     },
   { 'i', 'l', "like"        , "nice to have, but only if it comes unforced/naturally"        },
   { 'i', 'm', "might"       , "plausable, could be done, but there is no real push"          },
   { 'i', '-', "backlog"     , "not been assigned an importance"                              },
   /*---(estimate)-----------------------*/
   { 'e', '*', "huge"        , "longer than a full day of work"                               },
   { 'e', '8', "480m"        , "full day of work, or possibly until start of the next day"    },
   { 'e', '4', "240m"        , "half day of work, which means serious focus and dedication"   },
   { 'e', '2', "120m"        , "couple hours, meaning dedicated focus and continuous time"    },
   { 'e', '1', "60m"         , "full hour of work is a moderate task that needs focus"        },
   { 'e', 'h', "30m"         , "half hour of work is usually smaller, but needs attention"    },
   { 'e', 'q', "15m"         , "quick task that likely takes a little thought and prep"       },
   { 'e', '-', "backlog"     , "not been assigned an estimate yet"                            },
   /*---(progress)-----------------------*/
   { 'p', '-', "backlog"     , "work that has not been prepared or acted upon yet"            },
   { 'p', '�', "waiting"     , "waiting on change, something is holding this task up"         },
   { 'p', '<', "starting"    , "on longer tasks, indicates pre-work complete and task ready"  },
   { 'p', 'o', "active"      , "selected for today and/or working it right now"               },
   { 'p', '>', "checking"    , "on longer tasks, indicates post-work confirmation required"   },
   { 'p', '#', "done"        , "successfully completed and any checking done"                 },
   { 'p', 'x', "cancelled"   , "detiremened this effort is no longer necessary"               },
   { 'p', 'd', "duplicate"   , "covered by another task, but maybe additional detail"         },
   /*---(share)--------------------------*/
   { 's', '!', "primary"     , "keep on primary task list"                                    },
   { 's', 'p', "private"     , "do not allow into shared database"                            },
   { 's', '-', "shared"      , "allow to be picked up by shared database"                     },
   { 's', '�', "archived"    , "final check before deleting in source"                        },
   /*---(done)---------------------------*/
   {  0 ,  0 , ""            , ""                                                             },
};


static char      s_one [LEN_LABEL] = "blank";    /* group one  (save)                   */
static char      s_two [LEN_LABEL] = "blank";    /* group two  (save)                   */



/*====================------------------------------------====================*/
/*===----                           utility                            ----===*/
/*====================------------------------------------====================*/
static void      o___UTILITY_________________o (void) {;}



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
   /*---(create validation strings)------*/
   strlcpy (my.urgs, "", LEN_LABEL);
   strlcpy (my.imps, "", LEN_LABEL);
   strlcpy (my.ests, "", LEN_LABEL);
   strlcpy (my.prgs, "", LEN_LABEL);
   strlcpy (my.shrs, "", LEN_LABEL);
   for (i = 0; i < MAX_DECODE; ++i) {
      if (g_decode [i].cat == 0)                   break;
      sprintf (t, "%c", g_decode [i].sub);
      switch (g_decode [i].cat) {
      case 'u' : strlcat (my.urgs, t, LEN_LABEL);   break;
      case 'i' : strlcat (my.imps, t, LEN_LABEL);   break;
      case 'e' : strlcat (my.ests, t, LEN_LABEL);   break;
      case 'p' : strlcat (my.prgs, t, LEN_LABEL);   break;
      case 's' : strlcat (my.shrs, t, LEN_LABEL);   break;
      }
   }
   DEBUG_INPT   yLOG_info     ("my.urgs"   , my.urgs);
   DEBUG_INPT   yLOG_info     ("my.imps"   , my.imps);
   DEBUG_INPT   yLOG_info     ("my.ests"   , my.ests);
   DEBUG_INPT   yLOG_info     ("my.prgs"   , my.prgs);
   DEBUG_INPT   yLOG_info     ("my.shrs"   , my.shrs);
   /*---(add commands and menus)---------*/
   yCMD_add (YCMD_M_FILE   , "refresh"     , "r"   , ""     , api_yvikeys_refresh , ""                   );
   /*> yVIKEYS_cmds_add (YVIKEYS_M_DATASET, "dump"        , ""    , ""     , task_dump           , ""                   );   <*/
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit     (__FUNCTION__);
   return 0;
}


/*============================--------------------============================*/
/*===----                        detailed parsing                      ----===*/
/*============================--------------------============================*/
static void      o___PARSE___________________o (void) {;}

#define   MAXLINE  1000
FILE     *f         = NULL;         /* input file                          */
char      g_recd [LEN_RECD] = "";      /* record from stdin                   */
char*     p         = NULL;         /* strtok() parsing pointer            */
char     *q         = "";         /* strtok() delimiters                 */


char             /* [p-----] initialize a single task ------------------------*/
DATA__clear        (int a_num)
{
   /*> /+---(master data)-----------------+/                                                    <* 
    *> g_tasks [a_num].one [0]  = '\0';                                                         <* 
    *> g_tasks [a_num].two [0]  = '\0';                                                         <* 
    *> g_tasks [a_num].urg      = '-';                                                          <* 
    *> g_tasks [a_num].imp      = '-';                                                          <* 
    *> g_tasks [a_num].est      = '-';                                                          <* 
    *> g_tasks [a_num].prg      = '-';                                                          <* 
    *> g_tasks [a_num].shr      = '-';                                                          <* 
    *> g_tasks [a_num].txt [0]  = '\0';                                                         <* 
    *> /+> g_tasks [a_num].beg      =   0;                                                <+/   <* 
    *> /+> g_tasks [a_num].end      =   0;                                                <+/   <* 
    *> /+---(source data)-----------------+/                                                    <* 
    *> g_tasks [a_num].line     =  -1;                                                          <* 
    *> g_tasks [a_num].seq      =  -1;                                                          <* 
    *> /+---(filtering)-------------------+/                                                    <* 
    *> g_tasks [a_num].act      = '-';                                                          <* 
    *> g_tasks [a_num].key [0]  = '\0';                                                         <* 
    *> /+---(visualization)---------------+/                                                    <* 
    *> g_tasks [a_num].pos      =  0;                                                           <* 
    *> g_tasks [a_num].col      = -1;                                                           <* 
    *> g_tasks [a_num].row      = -1;                                                           <* 
    *> /+---(complete)-----------------------+/                                                 <* 
    *> return 0;                                                                                <*/
}

char
metis_data_catinfo      (char a_cat, char a_sub, char *a_clabel, char *a_slabel, char *a_desc)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   /*---(defaults)-----------------------*/
   if (a_clabel != NULL)  strlcpy (a_clabel, "unknown", LEN_LABEL);
   if (a_slabel != NULL)  strlcpy (a_slabel, "unknown", LEN_LABEL);
   if (a_desc   != NULL)  strlcpy (a_desc  , "unknown", LEN_HUND );
   /*---(take care of cat)---------------*/
   if (a_clabel != NULL) {
      switch (a_cat) {
      case 'u' : strlcpy (a_clabel, "urgency"     , LEN_LABEL); break;
      case 'i' : strlcpy (a_clabel, "importance"  , LEN_LABEL); break;
      case 'e' : strlcpy (a_clabel, "estimate"    , LEN_LABEL); break;
      case 'p' : strlcpy (a_clabel, "progress"    , LEN_LABEL); break;
      case 's' : strlcpy (a_clabel, "sharing"     , LEN_LABEL); break;
      }
   }
   /*---(walk thru subs)-----------------*/
   for (i = 0; i < MAX_DECODE; ++i) {
      if (g_decode [i].cat == 0)       break;
      if (g_decode [i].cat != a_cat)  continue;
      if (g_decode [i].sub != a_sub)  continue;
      if (a_slabel != NULL)  strlcpy (a_slabel, g_decode [i].label, LEN_LABEL);
      if (a_desc   != NULL)  strlcpy (a_desc  , g_decode [i].desc , LEN_HUND );
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
   else if (a_stats [0] == '�')                     a_task->urg = '-';
   else if (strchr (my.urgs, a_stats [0]) != NULL)  a_task->urg = a_stats [0];
   else                                             a_task->urg = '?';
   DEBUG_INPT   yLOG_char     ("urg"       , a_task->urg);
   /*---(importance)----------------------*/
   if      (x_len < 2)                              a_task->imp = '-';
   else if (a_stats [1] == '�')                     a_task->imp = '-';
   else if (strchr (my.imps, a_stats [1]) != NULL)  a_task->imp = a_stats [1];
   else                                             a_task->imp = '?';
   DEBUG_INPT   yLOG_char     ("imp"       , a_task->imp);
   /*---(progress)------------------------*/
   if      (x_len < 3)                              a_task->est = '-';
   else if (a_stats [2] == '�')                     a_task->est = '-';
   else if (strchr (my.ests, a_stats [2]) != NULL)  a_task->est = a_stats [2];
   else                                             a_task->est = '?';
   DEBUG_INPT   yLOG_char     ("est"       , a_task->est);
   /*---(tick/flag)-----------------------*/
   if      (x_len < 4)                              a_task->prg = '-';
   else if (a_stats [3] == '�')                     a_task->prg = '-';
   else if (strchr (my.prgs, a_stats [3]) != NULL)  a_task->prg = a_stats [3];
   else                                             a_task->prg = '?';
   DEBUG_INPT   yLOG_char     ("prg"       , a_task->prg);
   /*---(shares)--------------------------*/
   if      (x_len < 5)                              a_task->shr = '-';
   else if (a_stats [4] == '�')                     a_task->shr = '-';
   else if (strchr (my.shrs, a_stats [4]) != NULL)  a_task->shr = a_stats [4];
   else                                             a_task->shr = '?';
   DEBUG_INPT   yLOG_char     ("shr"       , a_task->shr);
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
metis_data_header       (char *a_recd)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_recd      [LEN_RECD];
   int         x_len       =    0;
   char       *p           = NULL;
   char       *q           = "";
   /*---(prepare)-----------------------*/
   strlcpy  (s_one, "blank", LEN_LABEL);
   strlcpy  (s_two, "blank", LEN_LABEL);
   /*---(defenses)----------------------*/
   --rce;  if (a_recd     == NULL)          return  rce;
   strlcpy (x_recd, a_recd, LEN_RECD);
   x_len = strlen (x_recd);
   --rce;  if (x_len <  10)                 return  rce;
   /*---(category one)------------------*/
   p = strtok  (x_recd, q);
   --rce;  if (p == NULL)                   return rce;
   strltrim (p, ySTR_BOTH, LEN_LABEL);
   strlcpy  (s_one, p, LEN_LABEL);
   /*---(category two)------------------*/
   p = strtok  (NULL, q);
   --rce;  if (p == NULL)                   return rce;
   strltrim (p, ySTR_BOTH, LEN_LABEL);
   strlcpy  (s_two, p, LEN_LABEL);
   /*---(complete)-----------------------*/
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
   char       *q           = "�";
   tTASK      *x_task      = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter    (__FUNCTION__);
   DEBUG_INPT   yLOG_value    ("g_ntask"   , g_ntask);
   DEBUG_INPT   yLOG_info     ("a_recd"    , a_recd);
   /*---(defenses)----------------------*/
   --rce;  if (a_recd     == NULL) {
      DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
      return  rce;
   }
   strlcpy (x_recd, a_recd, LEN_RECD);
   x_len = strlen (x_recd);
   DEBUG_INPT   yLOG_value    ("x_len"     , x_len);
   --rce;  if (x_len <  10) {
      DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
      return  rce;
   }
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
   /*---(create task)--------------------*/
   rc = metis_source_hook (a_source, x_task);
   DEBUG_INPT   yLOG_value   ("hook"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(statistics)--------------------*/
   p = strtok  (NULL, q);
   --rce;  if (p == NULL) {
      DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
      return  rce;
   }
   strltrim (p, ySTR_BOTH, LEN_LABEL);
   rc = metis_data_stats (x_task, p);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
      return  rce;
   }
   /*---(text)--------------------------*/
   p = strtok  (NULL, q);
   if (p == NULL) {
      DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
      return  rce;
   }
   strltrim (p, ySTR_BOTH, LEN_HUND);
   if (strlen (p) <= 0) {
      DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
      return  rce;
   }
   strlcpy  (x_task->txt, p, LEN_HUND);
   /*---(unique/start)------------------*/
   p = strtok  (NULL, q);
   if (p != NULL) {
      strltrim (p, ySTR_BOTH, LEN_LABEL);
      x_task->beg = atoi (p);
   }
   /*---(unique/end)--------------------*/
   if (p != NULL) {
      p = strtok  (NULL, q);
      if (p != NULL) {
         strltrim (p, ySTR_BOTH, LEN_LABEL);
         x_task->end = atoi (p);
      }
   }
   /*---(source)------------------------*/
   x_task->line = a_line;
   /*---(categories)--------------------*/
   /*> strlcpy  (g_tasks [g_ntask].one, s_one, LEN_LABEL);                            <* 
    *> strlcpy  (g_tasks [g_ntask].two, s_two, LEN_LABEL);                            <* 
    *> strlcpy  (g_tasks [g_ntask].source, a_source, LEN_LABEL);                      <* 
    *> g_tasks [g_ntask].line  = a_line;                                              <* 
    *> g_tasks [g_ntask].seq   = g_ntask;                                             <* 
    *> ++g_ntask;                                                                     <*/
   /*> DEBUG_INPT   yLOG_value    ("g_ntask"   , g_ntask);                            <*/
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
metis_data_file         (tMINOR *a_minor, tSOURCE *a_source, char *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char       *p           = NULL;
   char        x_proj      [LEN_LABEL];
   char        x_recd      [LEN_RECD];
   int         x_len       =    0;
   int         a           =    0;
   int         x_off       =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter    (__FUNCTION__);
   /*---(open)---------------------------*/
   f = fopen (a_name, "r");
   --rce;  if (f == NULL) {
      DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   strlcpy (x_proj, a_name, LEN_LABEL);
   p = strchr (x_proj, '_');
   if (p != NULL)  p [0] = '\0';
   sprintf (x_recd, "%s  %s ", x_proj, a_name);
   metis_data_header (x_recd);
   --rce;  while (1) {
      /*---(read)------------------------*/
      fgets (x_recd, LEN_RECD, f);
      ++a;
      DEBUG_INPT   yLOG_value    ("a"         , a);
      if (feof (f))  {
         DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
         return rce;
      }
      /*---(filter)----------------------*/
      strldchg (x_recd, '', '�', LEN_RECD);
      strltrim (x_recd, ySTR_SINGLE, LEN_RECD);
      x_len = strlen (x_recd);
      DEBUG_INPT   yLOG_value    ("x_len"     , x_len);
      if (x_len < 10)    continue;
      /*---(remove newline)--------------*/
      if (x_recd [x_len - 1] == '\n')  x_recd [--x_len] = '\0';
      DEBUG_INPT   yLOG_info     ("x_recd"    , x_recd);
      /*---(check prefix)----------------*/
      if        (strncmp (x_recd, "/* metis "   ,  9) == 0) {
         if (strncmp (x_recd + 9, "� ", 2) == 0) {
            DEBUG_INPT   yLOG_note     ("FOUND single-line or open comment (1) version");
         } else {
            DEBUG_INPT   yLOG_note     ("prefixed as single-line or open comment, but no field separator");
            continue;
         }
      } else if (strncmp (x_recd, "* metis "    ,  8) == 0) {
         if (strncmp (x_recd + 8, "� ", 2) == 0) {
            DEBUG_INPT   yLOG_note     ("FOUND continuing comment (2) version");
         } else {
            DEBUG_INPT   yLOG_note     ("prefixed as continuing comment, but no field separator");
            continue;
         }
      } else if (strncmp (x_recd, "#> metis "   ,  9) == 0) {
         if (strncmp (x_recd + 9, "� ", 2) == 0) {
            DEBUG_INPT   yLOG_note     ("FOUND traditional non-code comment (3) version");
         } else {
            DEBUG_INPT   yLOG_note     ("prefixed as traditional non-code comment, but no field separator");
            continue;
         }
      } else continue;
      /*---(check metis tag)-------------*/
      metis_data_parsing (a_minor, a_source, a, x_recd);
   }
   fclose(f);
   /*---(stats)--------------------------*/
   DEBUG_INPT   yLOG_value    ("majors"    , metis_major_count ());
   DEBUG_INPT   yLOG_value    ("minors"    , metis_minor_count ());
   DEBUG_INPT   yLOG_value    ("tasks"     , metis_task_count ());
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit     (__FUNCTION__);
   return 0;
}

char         /*--> make a list of input files --------------------------------*/
metis_data_directory    (tMAJOR *a_major, char *a_home, char *a_suffix)
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
   char        x_type      =  '-';
   int         x_read      =    0;          /* count of entries reviewed      */
   int         x_good      =    0;          /* count of entries processed     */
   tMINOR     *x_minor     = NULL;
   char        x_full      [LEN_PATH]  = "";
   tSOURCE    *x_source    = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("a_suffix"  , a_suffix);
   --rce;  if (a_suffix == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return  rce;
   }
   --rce;  if (a_suffix [0] != '.') {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return  rce;
   }
   /*---(process entries)----------------*/
   x_suflen = strlen (a_suffix);
   DEBUG_INPT   yLOG_info    ("SUFFIX"    , a_suffix);
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
      strlcpy (x_name, x_file->d_name, LEN_TITLE);
      DEBUG_INPT   yLOG_info    ("x_name"    , x_name);
      if (x_name [0] == '.')  {
         DEBUG_INPT   yLOG_note    ("hidden, SKIP");
         continue;
      }
      x_len = strlen (x_name);
      DEBUG_INPT   yLOG_value   ("x_len"     , x_len);
      if (x_len < x_suflen + 2) {
         DEBUG_INPT   yLOG_note    ("name too short with suffix, SKIP");
         continue;
      }
      /*---(cut on suffix len)-----------*/
      DEBUG_INPT   yLOG_info    ("potential" , x_name + x_len - x_suflen);
      if (strncmp (x_name + x_len - x_suflen, a_suffix, x_suflen) != 0) {
         DEBUG_INPT   yLOG_note    ("suffix does not match, SKIP");
         continue;
      }
      /*---(filter unit test)------------*/
      if (x_len > 7 && strcmp ("_unit.c", x_name + x_len - 7) == 0) {
         DEBUG_INPT   yLOG_note    ("cut the unit testing code files, SKIP");
         continue;
      }
      /*---(create file/minor)--------------*/
      rc = metis_minor_new (a_major, x_name, 'y', &x_minor);
      DEBUG_INPT   yLOG_value   ("new"       , rc);
      DEBUG_INPT   yLOG_point   ("x_minor"   , x_minor);
      --rce;  if (x_minor == NULL) {
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_INPT   yLOG_info    ("->name"    , x_minor->name);
      /*---(create source)------------------*/
      sprintf (x_full, "%s/%s", a_home, x_name);
      rc = metis_source_new (x_full, 'y', &x_source);
      DEBUG_INPT   yLOG_value   ("new"       , rc);
      DEBUG_INPT   yLOG_point   ("x_source"  , x_source);
      --rce;  if (x_source == NULL) {
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      DEBUG_INPT   yLOG_info    ("->path"    , x_source->path);
      /*---(save)------------------------*/
      metis_data_file (x_minor, x_source, x_name);
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
   rc = strlhere (x_home, x_name);
   DEBUG_INPT   yLOG_value   ("strlhere"  , rc);
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
   rc = metis_data_directory (x_major, x_home, ".h");
   rc = metis_data_directory (x_major, x_home, ".c");
   rc = metis_data_directory (x_major, x_home, ".unit");
   /*---(stats)--------------------------*/
   DEBUG_INPT   yLOG_value    ("majors"    , metis_major_count ());
   DEBUG_INPT   yLOG_value    ("minors"    , metis_minor_count ());
   DEBUG_INPT   yLOG_value    ("tasks"     , metis_task_count ());
   /*---(purge empties)------------------*/
   metis_minor_wrap ();
   metis_major_wrap ();
   /*---(stats)--------------------------*/
   DEBUG_INPT   yLOG_value    ("majors"    , metis_major_count ());
   DEBUG_INPT   yLOG_value    ("minors"    , metis_minor_count ());
   DEBUG_INPT   yLOG_value    ("tasks"     , metis_task_count ());
   /*---(complete)------------------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       other sources                          ----===*/
/*====================------------------------------------====================*/
static void      o___OTHER___________________o (void) {;}

char             /* [G-----] read all tasks from the file --------------------*/
DATA__stdin        (void)
{
   /*> /+---(locals)--------------------------------+/                                                <* 
    *> char    rc = 0;                                                                                <* 
    *> char    msg[100];                                                                              <* 
    *> /+> char      card_file[200] = "";                                                 <+/         <* 
    *> int         a            =    0;                                                               <* 
    *> /+---(header)-------------------------+/                                                       <* 
    *> DEBUG_INPT   yLOG_enter   (__FUNCTION__);                                                      <* 
    *> /+---(identify file)------------------+/                                                       <* 
    *> /+> snprintf(card_file, 190, "/home/member/g_hlosdo/metis_new.tasks", getenv("HOME"));   <+/   <* 
    *> /+---(open)---------------------------+/                                                       <* 
    *> while (1) {                                                                                    <* 
    *>    DEBUG_INPT   yLOG_value   ("a"         , a);                                                <* 
    *>    fgets (g_recd, MAXLINE, stdin);                                                             <* 
    *>    if (feof(stdin))   break;                                                                   <* 
    *>    /+---(get rid of the newline)-------------+/                                                <* 
    *>    p = strchr(g_recd, '\n');              /+ look for a newline              +/                <* 
    *>    if (p != NULL)   strcpy(p, "\0");    /+ if found, copy '\0' over it     +/                  <* 
    *>    /+---(defenses)---------------------------+/                                                <* 
    *>    if (strncmp(g_recd, "# end_of_visible", 16) == 0)   break;                                  <* 
    *>    ++a;                                                                                        <* 
    *>    if (g_recd [0] == '#')   continue;                                                          <* 
    *>    if (g_recd [0] == '\0')  continue;                                                          <* 
    *>    if (strncmp (g_recd, "  ", 2) == 0) rc = metis_data_parsing ("stdin", a, g_recd);           <* 
    *>    else                                rc = metis_data_header (g_recd);                        <* 
    *>    /+> rc = metis_data_parsing ();                                                       <+/   <* 
    *>    /+> if (rc < 0)                         return 0;                               <*          <* 
    *>     *> if (g_ntask >=  MAX_CARDS - 2)    break;                                     <+/        <* 
    *> }                                                                                              <* 
    *> /+---(complete)------------------------------+/                                                <* 
    *> DEBUG_INPT   yLOG_exit    (__FUNCTION__);                                                      <* 
    *> return 0;                                                                                      <*/
}

char             /* [G-----] read all tasks from the file --------------------*/
DATA__read         (char *a_filename)
{
   /*> /+---(locals)--------------------------------+/                                                <* 
    *> char    rc = 0;                                                                                <* 
    *> char    msg[100];                                                                              <* 
    *> /+> char      card_file[200] = "";                                                 <+/         <* 
    *> int         a            =    0;                                                               <* 
    *> /+---(header)-------------------------+/                                                       <* 
    *> DEBUG_INPT   yLOG_enter   (__FUNCTION__);                                                      <* 
    *> /+---(identify file)------------------+/                                                       <* 
    *> /+> snprintf(card_file, 190, "/home/member/g_hlosdo/metis_new.tasks", getenv("HOME"));   <+/   <* 
    *> /+---(open)---------------------------+/                                                       <* 
    *> f = fopen(a_filename, "r");                                                                    <* 
    *> if (f == NULL) {                                                                               <* 
    *>    printf("FATAL : can not open card file <<%s>>\n", a_filename);                              <* 
    *>    exit (-2);                                                                                  <* 
    *> }                                                                                              <* 
    *> while (1) {                                                                                    <* 
    *>    fgets (g_recd, MAXLINE, f);                                                                 <* 
    *>    if (feof(f))   break;                                                                       <* 
    *>    /+---(get rid of the newline)-------------+/                                                <* 
    *>    p = strchr(g_recd, '\n');              /+ look for a newline              +/                <* 
    *>    if (p != NULL)   strcpy(p, "\0");    /+ if found, copy '\0' over it     +/                  <* 
    *>    /+---(defenses)---------------------------+/                                                <* 
    *>    if (strncmp(g_recd, "# end_of_visible", 16) == 0)   break;                                  <* 
    *>    ++a;                                                                                        <* 
    *>    if (g_recd [0] == '#')   continue;                                                          <* 
    *>    if (g_recd [0] == '\0')  continue;                                                          <* 
    *>    if (strncmp (g_recd, "  ", 2) == 0) rc = metis_data_parsing (a_filename, a, g_recd);        <* 
    *>    else                                rc = metis_data_header (g_recd);                        <* 
    *>    /+> rc = metis_data_parsing ();                                                       <+/   <* 
    *>    /+> if (rc < 0)                         return 0;                               <*          <* 
    *>     *> if (g_ntask >=  MAX_CARDS - 2)    break;                                     <+/        <* 
    *> }                                                                                              <* 
    *> if (f == NULL) return 0;                                                                       <* 
    *> fclose(f);                                                                                     <* 
    *> /+---(complete)------------------------------+/                                                <* 
    *> DEBUG_INPT   yLOG_exit    (__FUNCTION__);                                                      <* 
    *> return 0;                                                                                      <*/
}

char DATA__master        (void) { return DATA__read    (FILE_MASTER); }
char DATA__custom        (void) { return DATA__read    (my.file);     }



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
   metis_data_init ();
   /*---(update)-------------------------*/
   --rce;  if (my.source == DATA_SOURCES) {
   }
   /*---(update)-------------------------*/
   --rce;  switch (my.source) {
   case DATA_SOURCES :
      rc = metis_data_project ();
      break;
   case DATA_MASTER  :
      rc = DATA__master  (); 
      break;
   case DATA_CUSTOM  :
      rc = DATA__custom  ();
      break;
   case DATA_PIPE    :
      rc = DATA__stdin   ();
      break;
   }
   ++c;
   /*---(add placeholder)----------------*/
   DATA__blankcard ();
   /*---(complete)------------------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
DATA__blankcard    (void)
{
   DEBUG_INPT   yLOG_bullet  (g_ntask     , "adding blank card");
   DATA__clear (g_ntask);
   strlcpy (g_tasks [g_ntask].one, "empty", LEN_LABEL);
   strlcpy (g_tasks [g_ntask].two, "empty", LEN_LABEL);
   strlcpy (g_tasks [g_ntask].txt, "this card intentionally left blank", LEN_HUND);
   g_tasks [g_ntask].act = '-';
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        task cursor                           ----===*/
/*====================------------------------------------====================*/
static void      o___CURSOR__________________o (void) {;}

static int s_cursor  = -1;

int          /*--> find the nth active task ---------[------ [---------------]*/
DATA_cursor             (char a_type)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   int         x_beg       =    0;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter    (__FUNCTION__);
   DEBUG_DATA   yLOG_value    ("g_ntask"   , g_ntask);
   /*---(adjust max)---------------------*/
   switch (a_type) {
   case YDLST_HEAD :  x_beg = 0;              break;
   case YDLST_NEXT :  x_beg = s_cursor + 1;   break;
   }
   /*---(findst max)---------------------*/
   for (i = x_beg; i <= g_ntask; ++i) {
      /*---(filter)-------------------*/
      DEBUG_DATA   yLOG_complex  ("card"      , "%2d, %c %s", i, g_tasks [i].act, g_tasks [i].txt);
      if (g_tasks [i].act != 'y')  continue;
      /*---(found)--------------------*/
      s_cursor = i;
      DEBUG_DATA   yLOG_value    ("FOUND"     , s_cursor);
      DEBUG_DATA   yLOG_exit    (__FUNCTION__);
      return s_cursor;
      /*---(done)---------------------*/
   }
   s_cursor = -1;
   /*---(complete)------------------------------*/
   DEBUG_DATA   yLOG_exitr   (__FUNCTION__, -1);
   return -1;
}



/*====================------------------------------------====================*/
/*===----                          reporting                           ----===*/
/*====================------------------------------------====================*/
static void      o___REPORTING_______________o (void) {;}

char             /* [G-----] output a formatted report of tasks --------------*/
task_list          (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;
   /*---(display)------------------------*/
   printf (" num     u i e f     ------one------     ------two------     ----------------------------------text--------------------------------     a seq     --pos-- -col- -row-\n");
   for (i = 0; i < g_ntask; ++i) {
      if ((i % 3) == 0)  printf ("\n");
      printf (" %3d  "              , i);
      if (g_tasks [i].prg == 'h') {
         printf ("   %c-%c-%c-%c  ", g_tasks [i].urg, g_tasks [i].imp, g_tasks [i].est, g_tasks [i].prg);
      } else {
         printf ("   %c %c %c %c  ", g_tasks [i].urg, g_tasks [i].imp, g_tasks [i].est, g_tasks [i].prg);
      }
      printf ("   %-15s     %-15s  ", g_tasks [i].one, g_tasks [i].two);
      printf ("   %-70.70s  "       , g_tasks [i].txt);
      printf ("   %c  "             , g_tasks [i].act);
      printf ("   %7d %5d %5d   \n" , g_tasks [i].pos, g_tasks [i].col, g_tasks [i].row);
   }
   printf ("\n");
   printf (" num     u i e f     ------one------     ------two------     ----------------------------------text--------------------------------     a seq     --pos-- -col- -row-\n");
   printf ("g_ntask = %3d, max_disp %3d, my.nact = %3d\n", g_ntask, max_disp, my.nact);
   /*---(complete)-----------------------*/
   return 0;
}

char             /* [G-----] output a formatted structure of tasks -----------*/
task_structure     (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;
   char        s_one       [15] = "";  /* save/prev major category            */
   char        s_two       [15] = "";  /* save/prev minor category            */
   int         n           = 0;
   /*---(display)------------------------*/
   printf ("\n");
   printf (" num     ------one------     ------two------\n");
   for (i = 0; i < g_ntask; ++i) {
      if (strcmp (g_tasks [i].one, s_one) != 0) {
         ++n;
         printf (" %3d  |  %-15s  |  %-15s  \n", i, g_tasks [i].one, g_tasks [i].two);
         strcpy (s_one, g_tasks [i].one);
         strcpy (s_two, g_tasks [i].two);
      } else if (strcmp (g_tasks [i].two, s_two) != 0) {
         ++n;
         printf (" %3d  |  %-15s  |  %-15s  \n", i, " ", g_tasks [i].two);
         strcpy (s_two, g_tasks [i].two);
      }
   }
   printf (" num     ------one------     ------two------\n");
   printf ("g_ntask = %3d, groups = %3d\n", g_ntask, n);
   /*---(complete)-----------------------*/
   return 0;
}

char
task_dump          (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =    0;
   int         c           =    0;
   /*---(display)------------------------*/
   for (i = 0; i < g_ntask; ++i) {
      if (g_tasks [i].act != 'y')  continue;
      if (c % 5 == 0) printf ("##-----one--------  -------two--------  uiep�  ----------------------------------text-------------------------------- \n");
      printf ("%-18s  %-18s ", g_tasks [i].one, g_tasks [i].two);
      printf (" %c%c%c%c� "    , g_tasks [i].urg, g_tasks [i].imp, g_tasks [i].est, g_tasks [i].prg);
      printf (" %-70.70s "     , g_tasks [i].txt);
      printf ("\n");
      ++c;
   }
   printf ("## end-of-file\n");
   /*---(complete)-----------------------*/
   return 0;
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
   if      (strcmp (a_question, "count"         ) == 0) {
      snprintf (unit_answer, LEN_FULL, "DATA count       : %d", g_ntask);
   }
   else if (strcmp (a_question, "stats"         ) == 0) {
      if (a_num < g_ntask) {
         snprintf (unit_answer, LEN_FULL, "DATA stats  (%2d) : urg %c, imp %c, est %c, prog %c   %3d", a_num, g_tasks [a_num].urg, g_tasks [a_num].imp, g_tasks [a_num].est, g_tasks [a_num].prg, g_tasks [a_num].line);
      } else {
         snprintf (unit_answer, LEN_FULL, "DATA stats  (%2d) : urg -, imp -, est -, prog -     -", a_num);
      }
   }
   else if (strcmp (a_question, "header"        ) == 0) {
      sprintf  (s, "[%.20s]", s_one);
      sprintf  (t, "[%.20s]", s_two);
      snprintf (unit_answer, LEN_FULL, "DATA header      : %2d%-22.22s %2d%s", strlen (s) - 2, s, strlen (t) - 2, t);
   }
   else if (strcmp (a_question, "cats"          ) == 0) {
      if (a_num < g_ntask) {
         sprintf  (s, "[%.20s]", g_tasks [a_num].one);
         sprintf  (t, "[%.20s]", g_tasks [a_num].two);
      }
      snprintf (unit_answer, LEN_FULL, "DATA cats   (%2d) : %2d%-22.22s %2d%s", a_num, strlen (s) - 2, s, strlen (t) - 2, t);
   }
   else if (strcmp (a_question, "text"          ) == 0) {
      if (a_num < g_ntask) {
         sprintf  (s, "[%.35s]", g_tasks [a_num].txt);
      }
      snprintf (unit_answer, LEN_FULL, "DATA text   (%2d) : %2d%s", a_num, strlen (s) - 2, s);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}



/*============================----end-of-source---============================*/
