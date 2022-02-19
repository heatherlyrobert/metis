/*============================----beg-of-source---============================*/
#include   "metis.h"



/*
 * 12345 § 12345 § 12345678901-12345678901-12345678901-12345678901-12345678901-12345678901- § ---beg---- § ---end---- §
 *
 * metis § dw2#Ï § add data refresh command and check                                       § 1645047879 § 1645055000 §
 * metis § dw2·· § add data refresh to menus                                                § 1645047880 § ·········· §
 * metis § ww4-· § add mark to tasks so that they can be selected to a short list           § 1645047881 § ·········· §
 * metis § ww4-· § allow forced voids for appearance, like row 18 or col 2 or 2x/4y         § 1645047882 § ·········· §
 * metis § ww4-· § add sharing flag to control database usage and marking                   § 1645047883 § ·········· §
 * metis § wl4-· § switch beg and end dates to pseudo-mongo (6 chars)                       § 1645162236 § ·········· §
 *
 */


tCARD       g_tasks [MAX_CARDS];
int         g_ntask       =  0;



#define  MAX_DECODE      50
typedef  struct  cDECODE  tDECODE;
struct cDECODE {
   char        type;
   char        abbr;
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
   { 'u', '*', "scheduled"   , "operates on a schedule or specifically called"                },
   /*---(importance)---------------------*/
   { 'i', 'a', "absolute"    , "this is a true life or death for project, app, or me"         },
   { 'i', 'n', "need"        , "must be completed, fact it is required to meeet objective"    },
   { 'i', 'v', "value"       , "adds solid, logic additional value to the objective"          },
   { 'i', 'c', "crave"       , "very strong want, true belief that this is necessary"         },
   { 'i', 'w', "want"        , "desired, but not absolutely needed, in the final product"     },
   { 'i', 'l', "like"        , "nice to have, but only if it comes unforced/naturally"        },
   { 'i', 'm', "might"       , "plausable, could be done, but there is no real push"          },
   { 'i', '-', "unset"       , "not been assigned an importance"                              },
   /*---(estimate)-----------------------*/
   { 'e', '*', "huge"        , "longer than a full day of work"                               },
   { 'e', '8', "480m"        , "full day of work, or possibly until start of the next day"    },
   { 'e', '4', "240m"        , "half day of work, which means serious focus and dedication"   },
   { 'e', '2', "120m"        , "couple hours, meaning dedicated focus and continuous time"    },
   { 'e', '1', "60m"         , "full hour of work is a moderate task that needs focus"        },
   { 'e', 'h', "30m"         , "half hour of work is usually smaller, but needs attention"    },
   { 'e', 'q', "15m"         , "quick task that likely takes a little thought and prep"       },
   { 'e', '!', "5m"          , "very fast task to complete, likely without support or prep"   },
   { 'e', '-', "backlog"     , "not been assigned an estimate yet"                            },
   /*---(progress)-----------------------*/
   { 'p', '-', "backlog"     , "work that has not been prepared or acted upon yet"            },
   { 'p', '<', "starting"    , "on longer tasks, indicates pre-work complete and task ready"  },
   { 'p', 'o', "active"      , "selected for today and/or working it right now"               },
   { 'p', '>', "checking"    , "on longer tasks, indicates post-work confirmation required"   },
   { 'p', '#', "done"        , "successfully completed and any checking done"                 },
   { 'p', 'x', "cancelled"   , "detiremened this effort is no longer necessary"               },
   { 'p', 'd', "duplicate"   , "covered by another task, but maybe additional detail"         },
   /*---(share)--------------------------*/
   { 's', 'p', "primary"     , "keep on primary task list"                                    },
   { 's', 'h', "private"     , "do not allow into shared database"                            },
   { 's', '-', "shared"      , "allow to be picked up by shared database"                     },
   { 's', 'Ï', "archived"    , "final check before deleting in source"                        },
   /*---(done)---------------------------*/
   {  0 ,  0 , ""            , ""                                                             },
};


static char      s_one [LEN_LABEL] = "blank";    /* group one  (save)                   */
static char      s_two [LEN_LABEL] = "blank";    /* group two  (save)                   */



/*====================------------------------------------====================*/
/*===----                           utility                            ----===*/
/*====================------------------------------------====================*/
static void      o___UTILITY_________________o (void) {;}

char
DATA_catinfo            (char a_type, char a_abbr, char *a_label, char *a_desc)
{
   int         i           =    0;
   for (i = 0; i < MAX_DECODE; ++i) {
      if (g_decode [i].type == 0)       break;
      if (g_decode [i].type != a_type)  continue;
      if (g_decode [i].abbr != a_abbr)  continue;
      if (a_label != NULL)  strlcpy (a_label, g_decode [i].label, LEN_LABEL);
      if (a_desc  != NULL)  strlcpy (a_desc , g_decode [i].desc , LEN_HUND );
      return 0;
   }
   return -1;
}


/*============================--------------------============================*/
/*===----                            single                            ----===*/
/*============================--------------------============================*/
static void      o___SINGLE__________________o (void) {;}

#define   MAXLINE  1000
FILE     *f         = NULL;         /* input file                          */
char      g_recd [LEN_RECD] = "";      /* record from stdin                   */
char*     p         = NULL;         /* strtok() parsing pointer            */
char     *q         = "";         /* strtok() delimiters                 */


char             /* [p-----] initialize a single task ------------------------*/
DATA__clear        (int a_num)
{
   /*---(master data)-----------------*/
   g_tasks [a_num].one [0]  = '\0';
   g_tasks [a_num].two [0]  = '\0';
   g_tasks [a_num].urg      = '-';
   g_tasks [a_num].imp      = '-';
   g_tasks [a_num].est      = '-';
   g_tasks [a_num].prg      = '-';
   g_tasks [a_num].shr      = '-';
   g_tasks [a_num].txt [0]  = '\0';
   /*> g_tasks [a_num].beg      =   0;                                                <*/
   /*> g_tasks [a_num].end      =   0;                                                <*/
   /*---(source data)-----------------*/
   g_tasks [a_num].line     =  -1;
   g_tasks [a_num].seq      =  -1;
   /*---(filtering)-------------------*/
   g_tasks [a_num].act      = '-';
   g_tasks [a_num].key [0]  = '\0';
   /*---(visualization)---------------*/
   g_tasks [a_num].pos      =  0;
   g_tasks [a_num].col      = -1;
   g_tasks [a_num].row      = -1;
   /*---(complete)-----------------------*/
   return 0;
}

char
DATA_init               (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        i           =    0;
   char        t           [LEN_LABEL];
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter    (__FUNCTION__);
   strlcpy (my.urgs, "", LEN_LABEL);
   strlcpy (my.imps, "", LEN_LABEL);
   strlcpy (my.ests, "", LEN_LABEL);
   strlcpy (my.prgs, "", LEN_LABEL);
   strlcpy (my.shrs, "", LEN_LABEL);
   for (i = 0; i < MAX_DECODE; ++i) {
      if (g_decode [i].type == 0)                   break;
      sprintf (t, "%c", g_decode [i].abbr);
      switch (g_decode [i].type) {
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
   for (i = 0; i < 100; ++i) {
      DATA__clear (i);
   }
   g_ntask  = 0;
   my.nact  = 0;
   yCMD_add (YCMD_M_FILE   , "refresh"     , "r"   , ""     , api_yvikeys_refresh , ""                   );
   /*> yVIKEYS_cmds_add (YVIKEYS_M_DATASET, "dump"        , ""    , ""     , task_dump           , ""                   );   <*/
   DEBUG_INPT   yLOG_exit     (__FUNCTION__);
   return 0;
}

char             /* [------] read a single task from file --------------------*/
DATA__recd         (void)
{
   while (1) {
      fgets(g_recd, MAXLINE, f);
      if (feof(f))               return -1;
      /*---(get rid of the newline)-------------*/
      p = strchr(g_recd, '\n');              /* look for a newline              */
      if (p != NULL)   strcpy(p, "\0");    /* if found, copy '\0' over it     */
      /*---(defenses)---------------------------*/
      if (strncmp(g_recd, "# end_of_visible", 16) == 0) return 1;
      if (g_recd[0] == '#' || g_recd[0] == '\0')   continue;
      break;
   }
   /*---(complete)-----------------------*/
   return 0;
}


char
DATA__stats        (char *a_stats)
{
   /*------------------------------------*/
   /*  return negative if catastrophic for the task
    *  return positive if warning about illegal value (force default)
    */
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_len       =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter    (__FUNCTION__);
   DEBUG_INPT   yLOG_point    ("a_stats"   , a_stats);
   --rce;  if (a_stats == NULL) {
      DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   x_len = strlen (a_stats);
   DEBUG_INPT   yLOG_value    ("x_len"     , x_len);
   --rce;  if (x_len < 4 || x_len > 5) {
      DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*---(urgency)-------------------------*/
   --rce;
   if      (a_stats [0] == '·')                     g_tasks [g_ntask].urg = '-';
   else if (strchr (my.urgs, a_stats [0]) != NULL)  g_tasks [g_ntask].urg = a_stats [0];
   else  {
      g_tasks [g_ntask].urg = '?';
      rc = -rce;
   }
   /*---(importance)----------------------*/
   --rce;
   if      (a_stats [1] == '·')                     g_tasks [g_ntask].imp = '-';
   else if (strchr (my.imps, a_stats [1]) != NULL)  g_tasks [g_ntask].imp = a_stats [1];
   else  {
      g_tasks [g_ntask].imp = '?';
      rc = -rce;
   }
   /*---(progress)------------------------*/
   --rce;
   if      (a_stats [2] == '·')                     g_tasks [g_ntask].est = '-';
   else if (strchr (my.ests, a_stats [2]) != NULL)  g_tasks [g_ntask].est = a_stats [2];
   else  {
      g_tasks [g_ntask].est = '?';
      rc = -rce;
   }
   /*---(tick/flag)-----------------------*/
   --rce;
   if      (a_stats [3] == '·')                     g_tasks [g_ntask].prg = '-';
   else if (strchr (my.prgs, a_stats [3]) != NULL)  g_tasks [g_ntask].prg = a_stats [3];
   else  {
      g_tasks [g_ntask].prg = '?';
      rc = -rce;
   }
   /*---(shares)--------------------------*/
   --rce;
   if      (a_stats [4] == '·')                     g_tasks [g_ntask].shr = '-';
   else if (a_stats [4] == 'Ï') {
      DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   else if (strchr (my.shrs, a_stats [4]) != NULL)  g_tasks [g_ntask].shr = a_stats [3];
   else  {
      g_tasks [g_ntask].shr = '?';
      rc = -rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_value    ("rc"        , rc);
   DEBUG_INPT   yLOG_exit     (__FUNCTION__);
   return rc;
}

char
DATA__header            (char *a_recd)
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
DATA__detail       (char *a_source, int a_line, char *a_recd)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_recd      [LEN_RECD];
   int         x_len       =    0;
   char       *p           = NULL;
   char       *q           = "§";
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter    (__FUNCTION__);
   DEBUG_INPT   yLOG_value    ("g_ntask"   , g_ntask);
   DEBUG_INPT   yLOG_info     ("a_recd"    , a_recd);
   /*---(cleanse)-----------------------*/
   DATA__clear  (g_ntask);
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
   /*---(statistics)--------------------*/
   p = strtok  (NULL, q);
   --rce;  if (p == NULL) {
      DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
      return  rce;
   }
   strltrim (p, ySTR_BOTH, LEN_LABEL);
   rc = DATA__stats (p);
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
   strlcpy  (g_tasks [g_ntask].txt, p, LEN_HUND);
   /*---(unique/start)------------------*/
   p = strtok  (NULL, q);
   if (p != NULL) {
      strltrim (p, ySTR_BOTH, LEN_LABEL);
      g_tasks [g_ntask].beg = atoi (p);
   }
   /*---(unique/end)--------------------*/
   if (p != NULL) {
      p = strtok  (NULL, q);
      if (p != NULL) {
         strltrim (p, ySTR_BOTH, LEN_LABEL);
         g_tasks [g_ntask].end = atoi (p);
      }
   }
   /*---(categories)--------------------*/
   strlcpy  (g_tasks [g_ntask].one, s_one, LEN_LABEL);
   strlcpy  (g_tasks [g_ntask].two, s_two, LEN_LABEL);
   strlcpy  (g_tasks [g_ntask].source, a_source, LEN_LABEL);
   g_tasks [g_ntask].line  = a_line;
   g_tasks [g_ntask].seq   = g_ntask;
   ++g_ntask;
   DEBUG_INPT   yLOG_value    ("g_ntask"   , g_ntask);
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit     (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                            driver                            ----===*/
/*====================------------------------------------====================*/
static void      o___DRIVER__________________o (void) {;}

char
DATA__file         (char *a_source)
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
   f = fopen (a_source, "r");
   --rce;  if (f == NULL) {
      DEBUG_INPT   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   strlcpy (x_proj, a_source, LEN_LABEL);
   p = strchr (x_proj, '_');
   if (p != NULL)  p [0] = '\0';
   sprintf (x_recd, "%s  %s ", x_proj, a_source);
   DATA__header (x_recd);
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
      strldchg (x_recd, '', '§', LEN_RECD);
      strltrim (x_recd, ySTR_SINGLE, LEN_RECD);
      x_len = strlen (x_recd);
      DEBUG_INPT   yLOG_value    ("x_len"     , x_len);
      if (x_len < 10)    continue;
      /*---(remove newline)--------------*/
      if (x_recd [x_len - 1] == '\n')  x_recd [--x_len] = '\0';
      DEBUG_INPT   yLOG_info     ("x_recd"    , x_recd);
      /*---(check prefix)----------------*/
      if        (strncmp (x_recd, "/* metis "   ,  9) == 0) {
         if (strncmp (x_recd + 9, "§ ", 2) == 0) {
            DEBUG_INPT   yLOG_note     ("FOUND single-line or open comment (1) version");
         } else {
            DEBUG_INPT   yLOG_note     ("prefixed as single-line or open comment, but no field separator");
            continue;
         }
      } else if (strncmp (x_recd, "* metis "    ,  8) == 0) {
         if (strncmp (x_recd + 8, "§ ", 2) == 0) {
            DEBUG_INPT   yLOG_note     ("FOUND continuing comment (2) version");
         } else {
            DEBUG_INPT   yLOG_note     ("prefixed as continuing comment, but no field separator");
            continue;
         }
      } else if (strncmp (x_recd, "#> metis "   ,  9) == 0) {
         if (strncmp (x_recd + 9, "§ ", 2) == 0) {
            DEBUG_INPT   yLOG_note     ("FOUND traditional non-code comment (3) version");
         } else {
            DEBUG_INPT   yLOG_note     ("prefixed as traditional non-code comment, but no field separator");
            continue;
         }
      } else continue;
      /*---(check metis tag)-------------*/
      DATA__detail (a_source, a, x_recd);
      /*> if      (strncmp (x_recd, "/+ metis ", 10) == 0) {                              <* 
       *>    DEBUG_INPT   yLOG_note     ("FOUND, single-line or open comment (1)");        <* 
       *>    if (strncmp (x_recd + x_len - 3, "+/", 2) == 0)  x_recd [x_len - 3] = '\0';   <* 
       *>    DATA__detail (a_source, a, x_recd + 10);                                      <* 
       *> }                                                                                <* 
       *> else if (strncmp (x_recd, "   /+ metis ", 13) == 0) {                           <* 
       *>    DEBUG_INPT   yLOG_note     ("FOUND, single-line or open comment (2)");        <* 
       *>    if (strncmp (x_recd + x_len - 3, "+/", 2) == 0)  x_recd [x_len - 3] = '\0';   <* 
       *>    DATA__detail (a_source, a, x_recd + 13);                                      <* 
       *> }                                                                                <* 
       *> else if (strncmp (x_recd, " * metis " , 10) == 0) {                             <* 
       *>    DEBUG_INPT   yLOG_note     ("FOUND, continuing comment (1)");                 <* 
       *>    if (strncmp (x_recd + x_len - 3, "+/", 2) == 0)  x_recd [x_len - 3] = '\0';   <* 
       *>    DATA__detail (a_source, a, x_recd + 10);                                      <* 
       *> }                                                                                <* 
       *> else if (strncmp (x_recd, "    * metis " , 13) == 0) {                          <* 
       *>    DEBUG_INPT   yLOG_note     ("FOUND, continuing comment (2)");                 <* 
       *>    if (strncmp (x_recd + x_len - 3, "+/", 2) == 0)  x_recd [x_len - 3] = '\0';   <* 
       *>    DATA__detail (a_source, a, x_recd + 13);                                      <* 
       *> }                                                                                <* 
       *> else if (strncmp (x_recd, "# metis " ,  8) == 0) {                               <* 
       *>    DEBUG_INPT   yLOG_note     ("FOUND, unit test comment");                      <* 
       *>    DATA__detail (a_source, a, x_recd +  9);                                      <* 
       *> }                                                                                <*/
   }
   fclose(f);
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit     (__FUNCTION__);
   return 0;
}

char             /* [G-----] read all tasks from the file --------------------*/
DATA__stdin        (void)
{
   /*---(locals)--------------------------------*/
   char    rc = 0;
   char    msg[100];
   /*> char      card_file[200] = "";                                                 <*/
   int         a            =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(identify file)------------------*/
   /*> snprintf(card_file, 190, "/home/member/g_hlosdo/metis_new.tasks", getenv("HOME"));   <*/
   /*---(open)---------------------------*/
   while (1) {
      DEBUG_INPT   yLOG_value   ("a"         , a);
      fgets (g_recd, MAXLINE, stdin);
      if (feof(stdin))   break;
      /*---(get rid of the newline)-------------*/
      p = strchr(g_recd, '\n');              /* look for a newline              */
      if (p != NULL)   strcpy(p, "\0");    /* if found, copy '\0' over it     */
      /*---(defenses)---------------------------*/
      if (strncmp(g_recd, "# end_of_visible", 16) == 0)   break;
      ++a;
      if (g_recd [0] == '#')   continue;
      if (g_recd [0] == '\0')  continue;
      if (strncmp (g_recd, "  ", 2) == 0) rc = DATA__detail ("stdin", a, g_recd);
      else                                rc = DATA__header (g_recd);
      /*> rc = DATA__detail ();                                                       <*/
      /*> if (rc < 0)                         return 0;                               <* 
       *> if (g_ntask >=  MAX_CARDS - 2)    break;                                     <*/
   }
   /*---(complete)------------------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char             /* [G-----] read all tasks from the file --------------------*/
DATA__read         (char *a_filename)
{
   /*---(locals)--------------------------------*/
   char    rc = 0;
   char    msg[100];
   /*> char      card_file[200] = "";                                                 <*/
   int         a            =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(identify file)------------------*/
   /*> snprintf(card_file, 190, "/home/member/g_hlosdo/metis_new.tasks", getenv("HOME"));   <*/
   /*---(open)---------------------------*/
   f = fopen(a_filename, "r");
   if (f == NULL) {
      printf("FATAL : can not open card file <<%s>>\n", a_filename);
      exit (-2);
   }
   while (1) {
      fgets (g_recd, MAXLINE, f);
      if (feof(f))   break;
      /*---(get rid of the newline)-------------*/
      p = strchr(g_recd, '\n');              /* look for a newline              */
      if (p != NULL)   strcpy(p, "\0");    /* if found, copy '\0' over it     */
      /*---(defenses)---------------------------*/
      if (strncmp(g_recd, "# end_of_visible", 16) == 0)   break;
      ++a;
      if (g_recd [0] == '#')   continue;
      if (g_recd [0] == '\0')  continue;
      if (strncmp (g_recd, "  ", 2) == 0) rc = DATA__detail (a_filename, a, g_recd);
      else                                rc = DATA__header (g_recd);
      /*> rc = DATA__detail ();                                                       <*/
      /*> if (rc < 0)                         return 0;                               <* 
       *> if (g_ntask >=  MAX_CARDS - 2)    break;                                     <*/
   }
   if (f == NULL) return 0;
   fclose(f);
   /*---(complete)------------------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char DATA__master        (void) { return DATA__read    (FILE_MASTER); }
char DATA__custom        (void) { return DATA__read    (my.file);     }

char         /*--> make a list of input files --------------------------------*/
DATA__sources       (char *a_suffix)
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
      /*---(save)------------------------*/
      DATA__file (x_name);
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

char
DATA_refresh       (void)
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
   DATA_init ();
   /*---(update)-------------------------*/
   switch (my.source) {
   case DATA_SOURCES :
      rc = DATA__sources (".h");
      rc = DATA__sources (".c");
      rc = DATA__sources (".unit");
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
      if (c % 5 == 0) printf ("##-----one--------  -------two--------  uiep·  ----------------------------------text-------------------------------- \n");
      printf ("%-18s  %-18s ", g_tasks [i].one, g_tasks [i].two);
      printf (" %c%c%c%c· "    , g_tasks [i].urg, g_tasks [i].imp, g_tasks [i].est, g_tasks [i].prg);
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
