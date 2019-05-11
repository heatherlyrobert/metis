/*============================----beg-of-source---============================*/
#include   "metis.h"


/*
 * metis  dw1#�  define task format when included in source file
 * metis  dw4#�  filter all source files in current directory for metis tasks
 * metis  wl2��  add descriptions to g_decode table
 * metis  dw4��  switch g_tasks to linked-list rather that array
 * metis  dw4��  switch g_tasks string elements to malloc rather than array
 * metis  dw8��  add g_tasks sorting field, sort chosing, and sorting function
 * metis  sn2>�  quick reporting on accepted tasks for debugging, validation
 * metis  tw1#�  remove repeating of tasks in lists for clarity
 * metis  tn1��  fix refresh tasks to increase/decrease count also
 * metis  dw1#�  add source file line to task record to help updates
 *
 */


tCARD       g_tasks [MAX_CARDS];
int         g_ntask       =  0;


char        g_urg    [LEN_LABEL] = "";
char        g_imp    [LEN_LABEL] = "";
char        g_est    [LEN_LABEL] = "";
char        g_prog   [LEN_LABEL] = "";

#define  MAX_DECODE     100
typedef  struct  cDECODE  tDECODE;
struct cDECODE {
   char        type;
   char        abbr;
   char        label       [LEN_LABEL];
   char        desc        [LEN_DESC];
   float       red;
   float       grn;
   float       blu;
};
tDECODE   g_decode   [MAX_DECODE] = {
   /*---(urgency)------------------------*/
   { 'u', '�', "undefined"   , ""                                               , 0.000, 0.000, 0.000 },
   { 'u', 't', "today"       , ""                                               , 0.000, 0.000, 0.000 },
   { 'u', 's', "soonest"     , ""                                               , 0.000, 0.000, 0.000 },
   { 'u', 'd', "days"        , ""                                               , 0.000, 0.000, 0.000 },
   { 'u', 'w', "weeks"       , ""                                               , 0.000, 0.000, 0.000 },
   { 'u', 'm', "months"      , ""                                               , 0.000, 0.000, 0.000 },
   { 'u', 'q', "quarters"    , ""                                               , 0.000, 0.000, 0.000 },
   { 'u', 'y', "years"       , ""                                               , 0.000, 0.000, 0.000 },
   { 'u', 'b', "backlog"     , ""                                               , 0.000, 0.000, 0.000 },
   /*---(importance)---------------------*/
   { 'i', '�', "undefined"   , ""                                               , 0.000, 0.000, 0.000 },
   { 'i', 'a', "absolute"    , "true life or death for project, app, or me"     , 0.000, 0.000, 0.000 },
   { 'i', 'n', "need"        , "must be completed to finish objective"          , 0.000, 0.000, 0.000 },
   { 'i', 'w', "want"        , ""                                               , 0.000, 0.000, 0.000 },
   { 'i', 'l', "like"        , ""                                               , 0.000, 0.000, 0.000 },
   { 'i', 'm', "might"       , ""                                               , 0.000, 0.000, 0.000 },
   { 'i', 'i', "idea"        , ""                                               , 0.000, 0.000, 0.000 },
   /*---(estimate)-----------------------*/
   { 'e', '�', "undefined"   , ""                                               , 0.000, 0.000, 0.000 },
   { 'e', '!', "5m-ish"      , ""                                               , 0.000, 0.000, 0.000 },
   { 'e', 's', "15m"         , ""                                               , 0.000, 0.000, 0.000 },
   { 'e', 'm', "30m"         , ""                                               , 0.000, 0.000, 0.000 },
   { 'e', '1', "60m"         , ""                                               , 0.000, 0.000, 0.000 },
   { 'e', '2', "120m"        , ""                                               , 0.000, 0.000, 0.000 },
   { 'e', '4', "240m"        , ""                                               , 0.000, 0.000, 0.000 },
   { 'e', '8', "480m"        , ""                                               , 0.000, 0.000, 0.000 },
   { 'e', '+', "longer"      , ""                                               , 0.000, 0.000, 0.000 },
   /*---(progress)-----------------------*/
   { 'p', '�', "undefined"   , ""                                               , 0.000, 0.000, 0.000 },
   { 'p', '<', "starting"    , ""                                               , 0.000, 0.000, 0.000 },
   { 'p', 'o', "active"      , ""                                               , 0.000, 0.000, 0.000 },
   { 'p', '>', "checking"    , ""                                               , 0.000, 0.000, 0.000 },
   { 'p', '#', "done"        , ""                                               , 0.000, 0.000, 0.000 },
   { 'p', 'x', "cancelled"   , ""                                               , 0.000, 0.000, 0.000 },
};


static char      s_one [LEN_LABEL] = "blank";    /* group one  (save)                   */
static char      s_two [LEN_LABEL] = "blank";    /* group two  (save)                   */



/*====================------------------------------------====================*/
/*===----                           utility                            ----===*/
/*====================------------------------------------====================*/
static void      o___UTILITY_________________o (void) {;}



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
   /*---(task data)-------------------*/
   g_tasks [a_num].one [0]  = '\0';
   g_tasks [a_num].two [0]  = '\0';
   g_tasks [a_num].urg      = '-';
   g_tasks [a_num].imp      = '-';
   g_tasks [a_num].est      = '-';
   g_tasks [a_num].flg      = '-';
   g_tasks [a_num].txt [0]  = '\0';
   /*---(filtering)-------------------*/
   g_tasks [a_num].act      = '-';
   g_tasks [a_num].seq      = -1;
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
   DEBUG_DATA   yLOG_enter    (__FUNCTION__);
   for (i = 0; i < MAX_DECODE; ++i) {
      sprintf (t, "%c", g_decode [i].abbr);
      switch (g_decode [i].type) {
      case 'u' : strlcat (g_urg , t, LEN_LABEL);   break;
      case 'i' : strlcat (g_imp , t, LEN_LABEL);   break;
      case 'e' : strlcat (g_est , t, LEN_LABEL);   break;
      case 'p' : strlcat (g_prog, t, LEN_LABEL);   break;
      }
   }
   /*> for (i = 0; i < MAX_CARDS; ++i) {                                              <* 
    *>    DATA__clear (i);                                                            <* 
    *> }                                                                              <*/
   g_ntask = 0;
   DEBUG_DATA   yLOG_exit     (__FUNCTION__);
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
   --rce;  if (a_stats == NULL)       return rce;
   --rce;  if (strlen (a_stats) < 4)  return rce;
   --rce;  if (strlen (a_stats) > 5)  return rce;
   /*---(urgency)-------------------------*/
   --rce;
   if (strchr (g_urg , a_stats [0]) != NULL)  g_tasks [g_ntask].urg = a_stats [0];
   else  {
      g_tasks [g_ntask].urg = '?';
      rc = -rce;
   }
   /*---(importance)----------------------*/
   --rce;
   if (strchr (g_imp , a_stats [1]) != NULL)  g_tasks [g_ntask].imp = a_stats [1];
   else  {
      g_tasks [g_ntask].imp = '?';
      rc = -rce;
   }
   /*---(progress)------------------------*/
   --rce;
   if (strchr (g_est , a_stats [2]) != NULL)  g_tasks [g_ntask].est = a_stats [2];
   else  {
      g_tasks [g_ntask].est = '?';
      rc = -rce;
   }
   /*---(tick/flag)-----------------------*/
   --rce;
   if (strchr (g_prog, a_stats [3]) != NULL)  g_tasks [g_ntask].flg = a_stats [3];
   else  {
      g_tasks [g_ntask].flg = '?';
      rc = -rce;
   }
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
DATA__detail       (char *a_recd, int a_line)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_recd      [LEN_RECD];
   int         x_len       =    0;
   char       *p           = NULL;
   char       *q           = "";
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter    (__FUNCTION__);
   DEBUG_DATA   yLOG_value    ("g_ntask"   , g_ntask);
   DEBUG_DATA   yLOG_info     ("a_recd"    , a_recd);
   /*---(cleanse)-----------------------*/
   DATA__clear  (g_ntask);
   /*---(defenses)----------------------*/
   --rce;  if (a_recd     == NULL) {
      DEBUG_DATA   yLOG_exitr    (__FUNCTION__, rce);
      return  rce;
   }
   strlcpy (x_recd, a_recd, LEN_RECD);
   x_len = strlen (x_recd);
   DEBUG_DATA   yLOG_value    ("x_len"     , x_len);
   --rce;  if (x_len <  10) {
      DEBUG_DATA   yLOG_exitr    (__FUNCTION__, rce);
      return  rce;
   }
   /*---(task prefix)-------------------*/
   p = strtok  (x_recd, q);
   --rce;  if (p == NULL) {
      DEBUG_DATA   yLOG_exitr    (__FUNCTION__, rce);
      return  rce;
   }
   strltrim (p, ySTR_BOTH, LEN_LABEL);
   rc = DATA__stats (p);
   --rce;  if (rc < 0) {
      DEBUG_DATA   yLOG_exitr    (__FUNCTION__, rce);
      return  rce;
   }
   /*---(text)--------------------------*/
   p = strtok  (NULL, q);
   if (p == NULL) {
      DEBUG_DATA   yLOG_exitr    (__FUNCTION__, rce);
      return  rce;
   }
   strltrim (p, ySTR_BOTH, LEN_HUND);
   strlcpy  (g_tasks [g_ntask].txt, p, LEN_HUND);
   /*---(categories)--------------------*/
   strlcpy  (g_tasks [g_ntask].one, s_one, LEN_LABEL);
   strlcpy  (g_tasks [g_ntask].two, s_two, LEN_LABEL);
   g_tasks [g_ntask].line  = a_line;
   ++g_ntask;
   ++nactive;
   DEBUG_DATA   yLOG_value    ("g_ntask"   , g_ntask);
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_exit     (__FUNCTION__);
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
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter    (__FUNCTION__);
   /*---(open)---------------------------*/
   f = fopen (a_source, "r");
   --rce;  if (f == NULL) {
      DEBUG_DATA   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   strlcpy (x_proj, a_source, LEN_LABEL);
   p = strchr (x_proj, '_');
   if (p != NULL)  p [0] = '\0';
   sprintf (x_recd, "%s  %s ", x_proj, a_source);
   DATA__header (x_recd);
   nactive  = 0;
   --rce;  while (1) {
      /*---(read)------------------------*/
      fgets (x_recd, LEN_RECD, f);
      ++a;
      DEBUG_DATA   yLOG_value    ("a"         , a);
      if (feof (f))  {
         DEBUG_DATA   yLOG_exitr    (__FUNCTION__, rce);
         return rce;
      }
      /*---(filter)----------------------*/
      x_len = strlen (x_recd);
      DEBUG_DATA   yLOG_value    ("x_len"     , x_len);
      if (x_len < 10)    continue;
      /*---(remove newline)--------------*/
      if (x_recd [x_len - 1] == '\n')  x_recd [--x_len] = '\0';
      DEBUG_DATA   yLOG_info     ("x_recd"    , x_recd);
      /*---(read)------------------------*/
      if      (strncmp (x_recd, "\* metis ", 10) == 0) {
         DEBUG_DATA   yLOG_note     ("single-line or open comment (1)");
         if (strncmp (x_recd + x_len - 3, "*/", 2) == 0)  x_recd [x_len - 3] = '\0';
         DATA__detail (x_recd + 10, a);
      }
      else if (strncmp (x_recd, "   \* metis ", 13) == 0) {
         DEBUG_DATA   yLOG_note     ("single-line or open comment (2)");
         if (strncmp (x_recd + x_len - 3, "*/", 2) == 0)  x_recd [x_len - 3] = '\0';
         DATA__detail (x_recd + 13, a);
      }
      else if (strncmp (x_recd, " * metis " , 10) == 0) {
         DEBUG_DATA   yLOG_note     ("continuing comment (1)");
         if (strncmp (x_recd + x_len - 3, "*/", 2) == 0)  x_recd [x_len - 3] = '\0';
         DATA__detail (x_recd + 10, a);
      }
      else if (strncmp (x_recd, "    * metis " , 13) == 0) {
         DEBUG_DATA   yLOG_note     ("continuing comment (2)");
         if (strncmp (x_recd + x_len - 3, "*/", 2) == 0)  x_recd [x_len - 3] = '\0';
         DATA__detail (x_recd + 13, a);
      }
      else if (strncmp (x_recd, "# metis " ,  9) == 0) {
         DEBUG_DATA   yLOG_note     ("unit test comment");
         DATA__detail (x_recd + 9, a);
      }
   }
   fclose(f);
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_exit     (__FUNCTION__);
   return 0;
}

char             /* [G-----] read all tasks from the file --------------------*/
DATA_read          (void)
{
   /*---(locals)--------------------------------*/
   char    rc = 0;
   char    msg[100];
   char      card_file[200] = "";
   int         a            =    0;
   /*---(process)-------------------------------*/
   DEBUG_I  printf("task_read\n");
   nactive  = 0;
   /*---(identify file)------------------*/
   /*> snprintf(card_file, 190, "%s/g_hlosdo/metis.tasks", getenv("HOME"));           <*/
   snprintf(card_file, 190, "/home/member/g_hlosdo/metis_new.tasks", getenv("HOME"));
   DEBUG_I  printf("   file = %s\n", card_file);
   /*---(open)---------------------------*/
   f = fopen(card_file, "r");
   if (f == NULL) {
      DEBUG_I  printf("   FAILURE, file could not be openned\n\n");
      printf("FATAL : can not open card file <<%s>>\n", card_file);
      exit (-2);
   }
   DEBUG_I  printf("   openned for read-only access\n");
   DEBUG_I  printf("   reading tasks\n");
   while (DATA__recd() == 0) {
      ++a;
      if (g_recd [0] == '#')   continue;
      if (g_recd [0] == '\0')  continue;
      if (strncmp (g_recd, "  ", 2) == 0) rc = DATA__detail (g_recd, a);
      else                                rc = DATA__header (g_recd);
      /*> rc = DATA__detail ();                                                       <*/
      /*> if (rc < 0)                         return 0;                               <* 
       *> if (g_ntask >=  MAX_CARDS - 2)    break;                                     <*/
   }
   DEBUG_I  printf("   closing task file\n");
   if (f == NULL) return 0;
   fclose(f);
   DEBUG_I  printf("   total of %d tasks\n", g_ntask);
   /*> printf("read %d tasks\n", g_ntask);                                             <*/
   /*---(final card)----------------------------*/
   /*> if (arg_heads == 1) {                                                                        <* 
    *>    strcpy(g_tasks [g_ntask].one, "criteria");                                                    <* 
    *>    if (g_ntask >  0) {                                                                        <* 
    *>       snprintf(msg, 100, "found = %d", g_ntask);                                              <* 
    *>       strcpy(g_tasks [g_ntask].two, msg);                                                        <* 
    *>    } else if (g_ntask <= 0) {                                                                 <* 
    *>       strcpy(g_tasks [g_ntask].two, "NO CARDS");                                                 <* 
    *>    }                                                                                         <* 
    *>    g_tasks [g_ntask].urg      = ' ';                                                             <* 
    *>    g_tasks [g_ntask].imp      = ' ';                                                             <* 
    *>    g_tasks [g_ntask].est      = ' ';                                                             <* 
    *>    g_tasks [g_ntask].flg      = '+';                                                             <* 
    *>    snprintf(msg, 100, "u=<%c>, i=<%c>, e=<%c>, f=<%c>, <<%s>>", urg, imp, est, flg, one);    <* 
    *>    strcpy(g_tasks [g_ntask].txt, msg);                                                           <* 
    *>    DEBUG_I  printf("   adding criteria task\n");                                             <* 
    *>    DEBUG_I  printf("      %2d) %c.%c.%c.%c %-60.60s <|\n", g_ntask + 1,                       <* 
    *>          g_tasks [g_ntask].urg , g_tasks [g_ntask].imp ,                                             <* 
    *>          g_tasks [g_ntask].est , g_tasks [g_ntask].flg,                                              <* 
    *>          g_tasks [g_ntask].txt );                                                                <* 
    *>    ++g_ntask;                                                                                 <* 
    *> }                                                                                            <*/
   /*---(complete)------------------------------*/
   DEBUG_I  printf("   complete\n\n");
   filter_primary ();
   return 0;
}

char         /*--> make a list of input files --------------------------------*/
DATA_sources       (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         rc          =    0;          /* generic return code            */
   char        rce         =  -10;          /* return code for errors         */
   DIR        *x_dir       = NULL;          /* directory pointer              */
   tDIRENT    *x_file      = NULL;          /* directory entry pointer        */
   char        x_name      [LEN_TITLE];      /* file name                      */
   int         x_len       =    0;
   char        x_type      =  '-';
   int         x_read      =    0;          /* count of entries reviewed      */
   int         x_good      =    0;          /* count of entries processed     */
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
   /*---(process entries)----------------*/
   DEBUG_INPT   yLOG_note    ("processing entries");
   while (1) {
      /*---(read a directory entry)------*/
      x_file = readdir (x_dir);
      DEBUG_INPT   yLOG_point   ("x_file"    , x_file);
      if (x_file == NULL)  break;
      ++x_read;
      /*---(filter by name)--------------*/
      strlcpy (x_name, x_file->d_name, LEN_TITLE);
      DEBUG_INPT   yLOG_info    ("x_name"    , x_name);
      x_len = strlen (x_name);
      DEBUG_INPT   yLOG_value   ("x_len"     , x_len);
      if (x_name [0] == '.')  {
         DEBUG_INPT   yLOG_note    ("hidden, SKIP");
         continue;
      }
      /*---(cut on suffix len)-----------*/
      DEBUG_INPT   yLOG_char    ("x_len - 2" , x_name [x_len - 2]);
      if (x_name [x_len - 2] != '.')  {
         DEBUG_INPT   yLOG_note    ("does not have a one-char suffix, SKIP");
         continue;
      }
      /*---(cut on suffix)---------------*/
      x_type = x_name [x_len - 1];
      DEBUG_INPT   yLOG_char    ("x_len - 1" , x_type);
      if (strchr ("ch", x_name [x_len - 1]) == NULL) {
         DEBUG_INPT   yLOG_note    ("not a c or h file, SKIP");
         continue;
      }
      /*---(filter unit test)*-----------*/
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
      if (g_tasks [i].flg == 'h') {
         printf ("   %c-%c-%c-%c  ", g_tasks [i].urg, g_tasks [i].imp, g_tasks [i].est, g_tasks [i].flg);
      } else {
         printf ("   %c %c %c %c  ", g_tasks [i].urg, g_tasks [i].imp, g_tasks [i].est, g_tasks [i].flg);
      }
      printf ("   %-15s     %-15s  ", g_tasks [i].one, g_tasks [i].two);
      printf ("   %-70.70s  "       , g_tasks [i].txt);
      printf ("   %c %3d  "         , g_tasks [i].act, g_tasks [i].seq);
      printf ("   %7d %5d %5d   \n" , g_tasks [i].pos, g_tasks [i].col, g_tasks [i].row);
   }
   printf ("\n");
   printf (" num     u i e f     ------one------     ------two------     ----------------------------------text--------------------------------     a seq     --pos-- -col- -row-\n");
   printf ("g_ntask = %3d, max_disp %3d, nactive = %3d\n", g_ntask, max_disp, nactive);
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
         snprintf (unit_answer, LEN_FULL, "DATA stats  (%2d) : urg %c, imp %c, est %c, prog %c   %3d", a_num, g_tasks [a_num].urg, g_tasks [a_num].imp, g_tasks [a_num].est, g_tasks [a_num].flg, g_tasks [a_num].line);
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
