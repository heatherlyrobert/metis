/*============================----beg-of-source---============================*/
#include   "metis.h"



tAUDIT      g_audit;

/*
 *
 */



/*====================------------------------------------====================*/
/*===----                       basic support                          ----===*/
/*====================------------------------------------====================*/
static void  o___SUPPORT_________o () { return; }

char
metis_db_cli            (char *a_name, char a_loud)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_recd      [LEN_RECD]  = "";
   int         l           =    0;
   char       *x_valid     = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_/.";
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_FILE   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_ARGS  yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL) {
      if (a_loud == 'y')  yURG_err (YURG_FATAL, "database <name>, name can not be null");
      DEBUG_PROG  yLOG_exitr (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ARGS  yLOG_info    ("a_name"    , a_name);
   ystrlcpy (x_recd, a_name, LEN_RECD);
   /*---(check length)-------------------*/
   l = strlen (x_recd);
   DEBUG_ARGS  yLOG_value   ("l"         , l);
   --rce;  if (l <= 0) {
      if (a_loud == 'y')  yURG_err (YURG_FATAL, "database <name>, name can not be blank/empty");
      DEBUG_PROG  yLOG_exitr (__FUNCTION__, rce);
      return rce;
   }
   /*---(check characters)---------------*/
   --rce;  for (i = 0; i < l; ++i) {
      if (strchr (x_valid, x_recd [i]) != NULL)  continue;
      if (a_loud == 'y')  yURG_err (YURG_FATAL, "database <name>, name can not have a <%c> at character %d", x_recd [i], i);
      DEBUG_PROG  yLOG_char  ("bad char"  , x_recd [i]);
      DEBUG_PROG  yLOG_exitr (__FUNCTION__, rce);
      return rce;
   }
   /*---(check extension)----------------*/
   if (l >= 4 && strcmp (x_recd + l - 3, ".db") != 0) {
      ystrlcat (x_recd, ".db", LEN_RECD);
      DEBUG_ARGS  yLOG_info    ("fixed"     , x_recd);
   }
   /*---(copy)---------------------------*/
   ystrlcpy (my.n_db, x_recd, LEN_RECD);
   DEBUG_ARGS  yLOG_info    ("database"  , my.n_db);
   /*---(complete)-----------------------*/
   DEBUG_FILE   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
metis_db_init           (void)
{
   metis_db_cli (F_DB, '-');
   my.f_db = NULL;
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      file control                            ----===*/
/*====================------------------------------------====================*/
static void  o___FILES___________o () { return; }

char
metis_db__read_head     (FILE *a_file, char a_name [LEN_TERSE], ushort *a_var)
{
   short       n           =    0;
   fread  (&n, sizeof (short), 1, a_file);
   DEBUG_FILE   yLOG_value   (a_name      , n);
   if (a_var != NULL)  *a_var = n;
   return 0;
}

char
metis_db__write_head    (FILE *a_file, char a_name [LEN_TERSE], ushort a_var)
{
   DEBUG_FILE   yLOG_value   (a_name      , a_var);
   fwrite (&a_var, sizeof (short), 1, a_file);
   return 0;
}

char
metis_db__open          (char a_name [LEN_PATH], char a_mode, short *b_nmajor, short *b_nminor, short *b_nsource, short *b_ntask, char a_heartbeat [LEN_DESC], FILE **b_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_mode      [LEN_TERSE] = "";
   int         n           =    0;
   char        t           [LEN_HUND]  = "";
   FILE       *f           = NULL;
   /*---(header)-------------------------*/
   DEBUG_FILE   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_FILE   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name      == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_FILE   yLOG_point   ("b_nmajor"  , b_nmajor);
   --rce;  if (b_nmajor    == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_FILE   yLOG_point   ("b_nminor"  , b_nminor);
   --rce;  if (b_nminor    == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_FILE   yLOG_point   ("b_nsource" , b_nsource);
   --rce;  if (b_nsource   == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_FILE   yLOG_point   ("b_ntask"   , b_ntask);
   --rce;  if (b_ntask     == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_FILE   yLOG_point   ("a_heart...", a_heartbeat);
   --rce;  if (a_heartbeat == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_FILE   yLOG_point   ("b_file"    , b_file);
   --rce;  if (b_file      == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_point   ("*b_file"   , *b_file);
   --rce;  if (*b_file != NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(set mode)-----------------------*/
   DEBUG_FILE   yLOG_char    ("a_mode"    , a_mode);
   --rce;  switch (a_mode) {
   case 'r' :
      ystrlcpy (x_mode, "rb", LEN_TERSE);
      break;
   case 'w' :
      ystrlcpy (x_mode, "wb", LEN_TERSE);
      break;
   default  :
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_FILE   yLOG_info    ("x_mode"    , x_mode);
   /*---(open)---------------------------*/
   DEBUG_FILE   yLOG_info    ("a_name"    , a_name);
   f = fopen (a_name, x_mode);
   DEBUG_FILE   yLOG_point   ("f"         , f);
   --rce;  if (f == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(header)-------------------------*/
   switch (a_mode) {
   case 'r' :
      /*---(name)------------------------*/
      fread  (&(g_audit.name)  , LEN_LABEL, 1, f);
      DEBUG_FILE   yLOG_info    ("name"      , g_audit.name);
      /*---(version)---------------------*/
      fread  (&(g_audit.vernum), LEN_SHORT, 1, f);
      DEBUG_FILE   yLOG_info    ("vernum"    , g_audit.vernum);
      /*---(stats)-----------------------*/
      metis_db__read_head  (f, "major" , b_nmajor);
      metis_db__read_head  (f, "minor" , b_nminor);
      metis_db__read_head  (f, "source", b_nsource);
      metis_db__read_head  (f, "task"  , b_ntask);
      /*---(heartbeat)-------------------*/
      fread  (a_heartbeat, LEN_DESC, 1, f);
      DEBUG_FILE   yLOG_info    ("heartbeat" , a_heartbeat);
      /*---(done)------------------------*/
      break;
   case 'w' :
      /*---(name)------------------------*/
      for (n = 0; n < LEN_LABEL; n++)  t [n] = '·';
      ystrlcpy (t, P_BASENAME, LEN_LABEL);
      fwrite (t, LEN_LABEL, 1, f);
      DEBUG_FILE   yLOG_info    ("name"      , t);
      /*---(version)---------------------*/
      for (n = 0; n < LEN_LABEL; n++)  t [n] = '·';
      ystrlcpy (t, P_VERNUM  , LEN_SHORT);
      fwrite (t, LEN_SHORT, 1, f);
      DEBUG_FILE   yLOG_info    ("vernum"    , t);
      /*---(stats)-----------------------*/
      metis_db__write_head (f, "major" , *b_nmajor);
      metis_db__write_head (f, "minor" , *b_nminor);
      metis_db__write_head (f, "source", *b_nsource);
      metis_db__write_head (f, "task"  , *b_ntask);   
      /*---(heartbeat)-------------------*/
      for (n = 0; n < LEN_DESC;  n++)  t [n] = '·';
      ystrlcpy (t, a_heartbeat, LEN_DESC);
      fwrite (t, LEN_DESC , 1, f);
      /*---(done)------------------------*/
      fflush (f);
      break;
   }
   /*---(save-back)----------------------*/
   if (b_file != NULL)  *b_file = f;
   /*---(complete)-----------------------*/
   DEBUG_FILE   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
metis_db__close         (FILE **b_file)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_FILE   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("b_file"    , b_file);
   --rce;  if (b_file == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_point   ("*b_file"   , *b_file);
   --rce;  if (*b_file == NULL) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(close)--------------------------*/
   rc = fclose (*b_file);
   DEBUG_OUTP   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_FILE   yLOG_exitr   (__FUNCTION__, rce);
      return rce; 
   }
   /*---(ground pointer)-----------------*/
   *b_file = NULL;
   DEBUG_INPT   yLOG_point   ("*b_file"   , *b_file);
   /*---(complete)-----------------------*/
   DEBUG_FILE   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     writing full database                    ----===*/
/*====================------------------------------------====================*/
static void  o___WRITING_________o () { return; }

char         /*===[[ write binary file ]]=================[ ------ [ ------ ]=*/
metis_db__write_sources (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tSOURCE    *x_source    = NULL;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   rc = metis_source_by_cursor (YDLST_HEAD, &x_source);
   DEBUG_PROG   yLOG_point   ("x_source"   , x_source);
   /*---(walk projects)------------------*/
   while (x_source != NULL) {
      /*---(write)-----------------------*/
      x_source->ref = my.nsource;
      DEBUG_OUTP   yLOG_complex ("source"    , "%3d#, %3ds, %s", my.nsource, x_source->ref, x_source->path);
      fwrite (x_source  , sizeof (tSOURCE), 1, my.f_db);
      /*---(next)------------------------*/
      ++my.nsource;
      rc = metis_source_by_cursor (YDLST_NEXT, &x_source);
      DEBUG_PROG   yLOG_point   ("x_source"   , x_source);
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*===[[ write binary file ]]=================[ ------ [ ------ ]=*/
metis_db__write_task    (tMINOR *x_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tTASK      *x_task      = NULL;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   DEBUG_OUTP   yLOG_value   ("count"     , x_minor->count);
   x_task = x_minor->head;
   DEBUG_OUTP   yLOG_point   ("x_task"    , x_task);
   /*---(walk projects)------------------*/
   while (x_task != NULL) {
      /*---(write)-----------------------*/
      x_task->ref = x_task->source->ref;
      DEBUG_OUTP   yLOG_complex ("task"      , "%3d#, %3ds, %s", my.ntask, x_task->ref, x_task->txt);
      fwrite (x_task  , sizeof (tTASK), 1, my.f_db);
      /*---(next)------------------------*/
      ++my.ntask;
      x_task = x_task->m_next;
      DEBUG_OUTP   yLOG_point   ("x_task"    , x_task);
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*===[[ write binary file ]]=================[ ------ [ ------ ]=*/
metis_db__write_minor   (tMAJOR *x_major)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tMINOR     *x_minor     = NULL;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   DEBUG_OUTP   yLOG_value   ("count"     , x_major->count);
   x_minor = x_major->head;
   DEBUG_OUTP   yLOG_point   ("x_minor"   , x_minor);
   /*---(walk projects)------------------*/
   while (rc >= 0 && x_minor != NULL) {
      /*---(write)-----------------------*/
      DEBUG_OUTP   yLOG_info    ("file"      , x_minor->name);
      fwrite (x_minor  , sizeof (tMINOR), 1, my.f_db);
      /*---(dive)------------------------*/
      rc = metis_db__write_task (x_minor);
      if (rc < 0) {
         DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rc);
         return rc;
      }
      /*---(next)------------------------*/
      ++my.nminor;
      x_minor = x_minor->next;
      DEBUG_OUTP   yLOG_point   ("x_minor"   , x_minor);
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*===[[ write binary file ]]=================[ ------ [ ------ ]=*/
metis_db_write          (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tMAJOR     *x_major     = NULL;
   short       x_nmajor, x_nminor, x_nsource, x_ntask;
   /*---(header)-------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   my.nmajor     = my.nminor     = my.nsource     = my.ntask     = 0;
   g_audit.major = g_audit.minor = g_audit.source = g_audit.task = 0;
   x_nmajor  = metis_major_count ();
   x_nminor  = metis_minor_count ();
   x_nsource = metis_source_count ();
   x_ntask   = metis_task_count ();
   /*---(open)---------------------------*/
   rc = metis_db__open (my.n_db, 'w', &(x_nmajor),  &(x_nminor), &(x_nsource), &(x_ntask), my.heartbeat, &(my.f_db));
   DEBUG_OUTP   yLOG_value   ("open"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(sources)------------------------*/
   rc = metis_db__write_sources ();
   DEBUG_OUTP   yLOG_value   ("sources"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   rc = metis_major_by_cursor (YDLST_HEAD, &x_major);
   DEBUG_PROG   yLOG_point   ("x_major"    , x_major);
   /*---(walk projects)------------------*/
   while (x_major != NULL) {
      /*---(write)-----------------------*/
      DEBUG_OUTP   yLOG_info    ("project"   , x_major->name);
      fwrite (x_major  , sizeof (tMAJOR), 1, my.f_db);
      /*---(dive)------------------------*/
      rc = metis_db__write_minor (x_major);
      if (rc < 0) {
         DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rc);
         return rc;
      }
      /*---(next)------------------------*/
      ++my.nmajor;
      rc = metis_major_by_cursor (YDLST_NEXT, &x_major);
      DEBUG_PROG   yLOG_point   ("x_major"    , x_major);
      /*---(done)------------------------*/
   }
   /*---(close)--------------------------*/
   rc = metis_db__close (&(my.f_db));
   DEBUG_OUTP   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     reading full database                    ----===*/
/*====================------------------------------------====================*/
static void  o___READING_________o () { return; }

char         /*===[[ write binary file ]]=================[ ------ [ ------ ]=*/
metis_db__read_sources  (int n)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   tSOURCE     x_raw;
   tSOURCE    *x_source    = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(walk projects)------------------*/
   DEBUG_INPT   yLOG_value   ("n"         , n);
   for (i = 0; i < n; ++i) {
      /*---(read)---------------------------*/
      fread  (&x_raw, sizeof (tSOURCE), 1, my.f_db);
      DEBUG_OUTP   yLOG_complex ("x_raw"     , "%3d#, %3ds, %s", my.nsource, x_raw.ref, x_raw.path);
      /*---(allocate)-----------------------*/
      metis_source_new (x_raw.path, x_raw.type, 'y', &x_source);
      DEBUG_INPT   yLOG_point   ("x_source"  , x_source);
      --rce;  if (x_source == NULL) {
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      x_source->ref  = x_raw.ref;
      ++my.nsource;
      /*---(done)---------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*===[[ write binary file ]]=================[ ------ [ ------ ]=*/
metis_db__read_task     (tMINOR *a_minor, ushort n)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   ushort      i           =    0;
   tTASK       x_raw;
   tTASK      *x_task      = NULL;
   tSOURCE    *x_source    =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(walk projects)------------------*/
   DEBUG_INPT   yLOG_value   ("n"         , n);
   for (i = 0; i < n; ++i) {
      /*---(read)---------------------------*/
      fread  (&x_raw, sizeof (tTASK), 1, my.f_db);
      DEBUG_OUTP   yLOG_complex ("x_raw"     , "%3d#, %3ds, %s", my.ntask, x_raw.ref, x_raw.txt);
      /*---(allocate)-----------------------*/
      rc = metis_task_new (a_minor, 'y', &x_task);
      DEBUG_INPT   yLOG_point   ("x_task"   , x_task);
      --rce;  if (rc < 0 || x_task == NULL) {
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(move key data)------------------*/
      x_task->urg    = x_raw.urg;
      x_task->imp    = x_raw.imp;
      x_task->est    = x_raw.est;
      x_task->prg    = x_raw.prg;
      x_task->shr    = x_raw.shr;
      ystrlcpy (x_task->txt  , x_raw.txt  , LEN_HUND);
      ystrlcpy (x_task->epoch, x_raw.epoch, LEN_TERSE);
      x_task->days   = x_raw.days;
      x_task->line   = x_raw.line;
      /*---(hook to source)-----------------*/
      x_task->ref    = x_raw.ref;
      metis_source_by_index (x_task->ref, &x_source);
      DEBUG_INPT   yLOG_point   ("x_source"  , x_source);
      --rce;  if (x_source < 0) {
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      rc = metis_source_hook (x_source, x_task);
      DEBUG_INPT   yLOG_value   ("hook"      , rc);
      --rce;  if (rc < 0) {
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      ++my.ntask;
      /*---(done)---------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*===[[ write binary file ]]=================[ ------ [ ------ ]=*/
metis_db__read_minor    (tMAJOR *a_major, ushort n)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   ushort      i           =    0;
   tMINOR      x_raw;
   tMINOR     *x_minor     = NULL;
   ushort      x_count     =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   DEBUG_INPT   yLOG_point   ("a_major"   , a_major);
   /*---(walk projects)------------------*/
   DEBUG_INPT   yLOG_value   ("files"     , n);
   for (i = 0; i < n; ++i) {
      /*---(read)---------------------------*/
      fread  (&x_raw , sizeof (tMINOR), 1, my.f_db);
      DEBUG_INPT   yLOG_info    ("x_raw"     , x_raw.name);
      /*---(allocate)-----------------------*/
      metis_minor_new (a_major, x_raw.name, 'y', &x_minor);
      DEBUG_INPT   yLOG_point   ("x_minor"   , x_minor);
      --rce;  if (x_minor == NULL) {
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(clear the pointers)-------------*/
      x_count       = x_raw.count;
      /*---(dive)---------------------------*/
      rc = metis_db__read_task (x_minor, x_count);
      DEBUG_INPT   yLOG_value   ("tasks"     , rc);
      --rce;  if (rc < 0) {
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      ++my.nminor;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*===[[ write binary file ]]=================[ ------ [ ------ ]=*/
metis_db_read           (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   ushort      x_expect    =    0;
   ushort      x_actual    =    0;
   ushort      x_count     =    0;
   ushort      i           =    0;
   tMAJOR      x_raw;
   tMAJOR     *x_major     = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(initialize)---------------------*/
   metis_data_purge_all ();
   /*---(prepare)------------------------*/
   my.nmajor     = my.nminor     = my.nsource     = my.ntask     = 0;
   g_audit.major = g_audit.minor = g_audit.source = g_audit.task = 0;
   /*---(open)---------------------------*/
   rc = metis_db__open (my.n_db, 'r', &(g_audit.major),  &(g_audit.minor), &(g_audit.source), &(g_audit.task), g_audit.heartbeat, &(my.f_db));
   DEBUG_INPT   yLOG_value   ("open"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_expect = g_audit.major;
   DEBUG_INPT   yLOG_value   ("majors"    , x_expect);
   /*---(sources)------------------------*/
   rc = metis_db__read_sources (g_audit.source);
   DEBUG_OUTP   yLOG_value   ("sources"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_OUTP   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(walk projects)------------------*/
   for (i = 0; i < x_expect; ++i) {
      /*---(read)---------------------------*/
      fread  (&x_raw, sizeof (tMAJOR), 1, my.f_db);
      DEBUG_INPT   yLOG_info    ("x_raw"     , x_raw.name);
      /*---(allocate)-----------------------*/
      metis_major_new (x_raw.name, 'y', &x_major);
      DEBUG_INPT   yLOG_point   ("x_major"   , x_major);
      --rce;  if (x_major == NULL) {
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(get the count)------------------*/
      x_count = x_raw.count;
      DEBUG_INPT   yLOG_value   ("x_count"   , x_count);
      /*---(dive)---------------------------*/
      rc = metis_db__read_minor (x_major, x_count);
      DEBUG_INPT   yLOG_value   ("minors"    , rc);
      --rce;  if (rc < 0) {
         DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      /*---(done)---------------------------*/
      ++x_actual;
      ++my.nmajor;
   }
   /*---(close)--------------------------*/
   rc = metis_db__close (&(my.f_db));
   DEBUG_INPT   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(final check)--------------------*/
   --rce;  if (x_actual != x_expect) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void  o___UNITTEST________o () { return; }

char*        /*-> tbd --------------------------------[ light  [us.JC0.271.X1]*/ /*-[01.0000.00#.!]-*/ /*-[--.---.---.--]-*/
metis_db__unit          (char *a_question)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   char        x_exist     =  '-';
   int         n           =    0;
   char        x_file      [LEN_LABEL] = "";
   /*---(defense)------------------------*/
   snprintf (unit_answer, LEN_RECD, "DB unit          : question unknown");
   /*---(simple)-------------------------*/
   if      (strcmp (a_question, "file"      )     == 0) {
      rc = metis_shared_verify (my.n_db);
      if      (rc >  0)  x_exist = 'y';
      else if (rc <= 0)  x_exist = '-';
      sprintf (x_file, "%-10.10p", my.f_db);
      snprintf (unit_answer, LEN_RECD, "DB file          : %c  %-10.10s  %c  %2då%sæ",
            (my.f_db     == NULL) ? '-' : 'y', x_file,
            x_exist, strlen (my.n_db), my.n_db);
   }
   else if (strcmp (a_question, "count"     )     == 0) {
      snprintf (unit_answer, LEN_RECD, "DB count         : C %4dj %4dn %4ds %4dt § N %4dj %4dn %4ds %4dt § A %4dj %4dn %4ds %4dt",
            metis_major_count (), metis_minor_count (), metis_source_count (), metis_task_count (), 
            my.nmajor, my.nminor, my.nsource, my.ntask,
            g_audit.major, g_audit.minor, g_audit.source, g_audit.task);
   }
   else if (strcmp (a_question, "heartbeat" )     == 0) {
      snprintf (unit_answer, LEN_RECD, "DB heartbeat     : å%sæ", g_audit.heartbeat);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


