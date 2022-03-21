/*============================----beg-of-source---============================*/
#include   "metis.h"

/*
 * metis § mg2·· § add pass, warn, and fail counts to each system confirm output line     § M25JET §  · §
 * metis § mg4·· § do not delete archived tasks from database during update/gather        § M25JHc §  · §
 *
 *
 *
 *
 *
 */


/*====================------------------------------------====================*/
/*===----                       basic support                          ----===*/
/*====================------------------------------------====================*/
static void  o___SUPPORT_________o () { return; }

char
metis_world_cli         (char *a_name, char a_loud)
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
      if (a_loud == 'y')  yURG_err (YURG_FATAL, "world <name>, name can not be null");
      DEBUG_TOPS  yLOG_exitr (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_ARGS  yLOG_info    ("a_name"    , a_name);
   strlcpy (x_recd, a_name, LEN_RECD);
   /*---(check length)-------------------*/
   l = strlen (x_recd);
   DEBUG_ARGS  yLOG_value   ("l"         , l);
   --rce;  if (l <= 0) {
      if (a_loud == 'y')  yURG_err (YURG_FATAL, "world <name>, name can not be blank/empty");
      DEBUG_TOPS  yLOG_exitr (__FUNCTION__, rce);
      return rce;
   }
   /*---(check characters)---------------*/
   --rce;  for (i = 0; i < l; ++i) {
      if (strchr (x_valid, x_recd [i]) != NULL)  continue;
      if (a_loud == 'y')  yURG_err (YURG_FATAL, "world <name>, name can not have a <%c> at character %d", x_recd [i], i);
      DEBUG_TOPS  yLOG_char  ("bad char"  , x_recd [i]);
      DEBUG_TOPS  yLOG_exitr (__FUNCTION__, rce);
      return rce;
   }
   /*---(check extension)----------------*/
   if (l  < 5 || strcmp (x_recd + l - 4, ".txt") != 0) {
      strlcat (x_recd, ".txt", LEN_RECD);
      DEBUG_ARGS  yLOG_info    ("fixed"     , x_recd);
   }
   /*---(copy)---------------------------*/
   strlcpy (my.n_world, x_recd, LEN_RECD);
   DEBUG_ARGS  yLOG_info    ("world"     , my.n_world);
   /*---(complete)-----------------------*/
   DEBUG_FILE   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                   clearing and checking                      ----===*/
/*====================------------------------------------====================*/
static void  o___CLEANSING_______o () { return; }

char
metis_world__wipe       (tWORLD *a_world)
{
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(master)-------------------------*/
   a_world->type        =  '-';
   a_world->path [0]    = '\0';
   a_world->ysort       = NULL;
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 1;
}



/*====================------------------------------------====================*/
/*===----                       memory allccation                      ----===*/
/*====================------------------------------------====================*/
static void  o___MEMORY__________o () { return; }

char
metis_world_new         (char a_type, char* a_path, char a_force, tWORLD **r_new)
{  /*---(design notes)-------------------*/
   /*
    * a_force   '-' means r_new must be grounded (safefy), update btree
    *           'y' means r_new will be overwritten, update btree
    *           'r' (rapid) is 'y' plus the btree is NOT updated
    */
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tWORLD     *x_exist     = NULL;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   DEBUG_DATA   yLOG_char    ("a_force"   , a_force);
   /*---(defense)------------------------*/
   DEBUG_DATA   yLOG_char    ("a_type"    , a_type);
   --rce;  if (a_type == 0 || strchr ("sf", a_type) == NULL) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DATA   yLOG_point   ("a_path"    , a_path);
   --rce;  if (a_path == NULL) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DATA   yLOG_point   ("r_new"     , r_new);
   --rce;  if (r_new == NULL) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DATA   yLOG_point   ("*r_new"    , *r_new);
   --rce;  if (*r_new != NULL && strchr ("yr", a_force) == NULL) {
      DEBUG_DATA   yLOG_note    ("already set and not force");
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(default)------------------------*/
   *r_new = NULL;
   /*---(check for duplicate)------------*/
   rc = ySORT_by_name (B_WORLD, a_path, &x_exist);
   DEBUG_DATA   yLOG_point   ("x_exist"   , x_exist);
   --rce;  if (x_exist != NULL) {
      *r_new = x_exist;
      DEBUG_DATA   yLOG_exit    (__FUNCTION__);
      return 1;
   }
   /*---(create)-------------------------*/
   rc = metis_shared_new  (B_WORLD, &x_exist, '-', metis_world__wipe);
   DEBUG_DATA   yLOG_value   ("new"       , rc);
   --rce;  if (rc < 0) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(populate)-----------------------*/
   strlcpy (x_exist->path, a_path, LEN_PATH);
   x_exist->type = a_type;
   /*---(hook)---------------------------*/
   rc = ySORT_hook (B_WORLD, x_exist, x_exist->path, &(x_exist->ysort));
   DEBUG_DATA   yLOG_value   ("hook"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   rc = ySORT_prepare (B_WORLD);
   DEBUG_DATA   yLOG_value   ("prepare"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save back)----------------------*/
   *r_new = x_exist;
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
metis_world_free        (tWORLD **r_old)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_DATA   yLOG_point   ("r_old"     , r_old);
   --rce;  if (r_old == NULL) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DATA   yLOG_point   ("*r_old"    , *r_old);
   --rce;  if (*r_old == NULL) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(unhook from btree)--------------*/
   rc = ySORT_unhook (&((*r_old)->ysort));
   DEBUG_DATA   yLOG_value   ("btree"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(free memory)--------------------*/
   rc = metis_shared_free (B_WORLD, r_old);
   DEBUG_DATA   yLOG_value   ("free"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   rc = ySORT_prepare (B_WORLD);
   DEBUG_DATA   yLOG_value   ("prepare"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        searching                             ----===*/
/*====================------------------------------------====================*/
static void  o___SEARCH__________o () { return; }

int  metis_world_count       (void)                            { return ySORT_count     (B_WORLD); }
char metis_world_by_path     (uchar *a_path, tWORLD **r_world) { return ySORT_by_name   (B_WORLD, a_path, r_world); }
char metis_world_by_index    (int n, tWORLD **r_world)         { return ySORT_by_index  (B_WORLD, n, r_world); }
char metis_world_by_cursor   (char a_dir, tWORLD **r_world)    { return ySORT_by_cursor (B_WORLD, a_dir, r_world); }

char*
metis_world_entry       (int n)
{
   tWORLD     *x_world     = NULL;
   char        t           [LEN_HUND]  = " -åæ";
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   metis_world_by_index (n, &x_world);
   if (x_world == NULL) {
      strcpy (g_print, "n/a");
   } else {
      sprintf (t, "%2då%.50sæ", strlen (x_world->path), x_world->path);
      sprintf (g_print, "%-2d  %c  %s", n, x_world->type, t);
   }
   DEBUG_PROG   yLOG_info    ("world"     , g_print);
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return g_print;
}



/*====================------------------------------------====================*/
/*===----                        export and import                     ----===*/
/*====================------------------------------------====================*/
static void  o___EXIM____________o () { return; }

char
metis_world__open       (char a_dir)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_mode      [LEN_TERSE] = "";
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("f_world"         , my.f_world);
   --rce;  if (my.f_world != NULL) {
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   --rce;  if (a_dir == 'r') {
      rc = metis_shared_verify (my.n_world);
      DEBUG_INPT   yLOG_value   ("verify"     , rc);
      if (rc < 0) {
         DEBUG_INPT   yLOG_exit    (__FUNCTION__);
         return rce;
      }
   }
   /*---(prepare)------------------------*/
   switch (a_dir) {
   case 'r' :
      strlcpy (x_mode  , "rt"       , LEN_TERSE);
      break;
   case 'w' :
      strlcpy (x_mode  , "wt"       , LEN_TERSE);
      break;
   default  :
      DEBUG_INPT   yLOG_note    ("type unknown");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(open flow)----------------------*/
   my.f_world = fopen (my.n_world, x_mode);
   DEBUG_INPT   yLOG_point   ("f_world"         , my.f_world);
   --rce;  if (my.f_world == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(note)---------------------------*/
   DEBUG_INPT   yLOG_note    ("openned successfully");
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
metis_world__close      (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   char        rc          =   0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_point   ("f_world"         , my.f_world);
   --rce;  if (my.f_world == NULL) {
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(close file)---------------------*/
   rc = fclose (my.f_world);
   DEBUG_INPT   yLOG_point   ("fclose"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return  rce;
   }
   /*---(ground and clear names)---------*/
   my.f_world = NULL;
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
metis_world__import     (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_recd      [LEN_RECD]  = "";
   int         x_len       =    0;
   tWORLD     *x_world     = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   yURG_msg ('-', "world registry is in file å%sæ", my.n_world);
   /*---(open)---------------------------*/
   rc = metis_world__open ('r');
   DEBUG_INPT   yLOG_value   ("open"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return  rce;
   }
   /*---(walk)---------------------------*/
   while (1) {
      /*---(read)------------------------*/
      fgets  (x_recd, LEN_RECD, my.f_world);
      if (feof (my.f_world)) {
         DEBUG_INPT   yLOG_note    ("end of file");
         break;
      }
      DEBUG_INPT   yLOG_info    ("x_recd"    , x_recd);
      /*---(filter)----------------------*/
      if (x_recd [0] == '\0')  {
         DEBUG_INPT   yLOG_note    ("blank");
         continue;
      }
      if (x_recd [0] == '#')  {
         DEBUG_INPT   yLOG_note    ("comment");
         continue;
      }
      if (x_recd [0] == ' ')  {
         DEBUG_INPT   yLOG_note    ("indented");
         continue;
      }
      if (strchr ("fs", x_recd [0]) == NULL) {
         DEBUG_INPT   yLOG_note    ("not f or s type");
         continue;
      }
      if (x_recd [3] != '/')  {
         DEBUG_INPT   yLOG_note    ("does not have absolute path");
         continue;
      }
      /*---(prepare)---------------------*/
      x_len = strlen (x_recd);
      if (x_len > 0)  x_recd [--x_len] = '\0';
      DEBUG_INPT   yLOG_value   ("x_len"     , x_len);
      DEBUG_INPT   yLOG_info    ("x_recd"    , x_recd);
      /*---(save)------------------------*/
      metis_world_new (x_recd [0], x_recd + 3, 'y', &x_world);
      /*---(done)------------------------*/
   }
   /*---(close)--------------------------*/
   rc = metis_world__close ();
   DEBUG_INPT   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return  rce;
   }
   yURG_msg ('-', "imported %d entries from world registry", metis_world_count ());
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
metis_world__export     (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tWORLD     *x_world     = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(open)---------------------------*/
   rc = metis_world__open ('w');
   DEBUG_INPT   yLOG_value   ("open"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return  rce;
   }
   /*---(walk-through)-------------------*/
   DEBUG_DATA   yLOG_value   ("count"     , metis_world_count ());
   rc = metis_world_by_cursor (YDLST_HEAD, &x_world);
   while (rc >= 0 && x_world != NULL) {
      DEBUG_DATA   yLOG_info  ("->name"    , x_world->path);
      fprintf (my.f_world, "%c  %s\n", x_world->type, x_world->path);
      rc = metis_world_by_cursor (YDLST_NEXT, &x_world);
   }
   /*---(close)--------------------------*/
   rc = metis_world__close ();
   DEBUG_INPT   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return  rce;
   }
   yURG_msg ('-', "exported %d entries to world registry", metis_world_count ());
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                    registering and unregistering             ----===*/
/*====================------------------------------------====================*/
static void  o___REGISTER________o () { return; }

char
metis_world_register    (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_home      [LEN_PATH]  = "";
   char        x_name      [LEN_PATH]  = "";
   char        x_path      [LEN_PATH]  = "";
   tWORLD     *x_world     = NULL;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   rc = strlhere (x_home, x_name);
   DEBUG_INPT   yLOG_value   ("strlhere"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("x_home"    , x_home);
   /*---(check file)---------------------*/
   DEBUG_INPT   yLOG_char    ("source"    , my.source);
   --rce;  switch (my.source) {
   case DATA_FILE    :
      if (strldcnt (my.file, '/', LEN_PATH) == 0) {
         sprintf (x_path, "%s/%s", x_home, my.file);
      } else {
         strlcpy (x_path, my.file, LEN_PATH);
      }
      break;
   case DATA_SOURCES :
      strlcpy (x_path, x_home, LEN_PATH);
      break;
   default  :
      DEBUG_INPT   yLOG_note    ("can not register this type");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("x_path"    , x_path);
   /*---(import)-------------------------*/
   rc = metis_world_purge_all ();
   rc = metis_world__import ();
   /*---(add)----------------------------*/
   rc = metis_world_new (my.source, x_path, '-', &x_world);
   --rce;  if (rc < 0) {
      yURG_err ('f', "project could not be added");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(export)-------------------------*/
   rc = metis_world__export ();
   --rce;  if (rc < 0) {
      yURG_err ('f', "project could not be added");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(purge)--------------------------*/
   rc = metis_world_purge_all ();
   --rce;  if (rc < 0) {
      yURG_err ('f', "project could not be added");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
metis_world_unregister  (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_home      [LEN_PATH]  = "";
   char        x_name      [LEN_PATH]  = "";
   char        x_path      [LEN_PATH]  = "";
   tWORLD     *x_world     = NULL;
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
   /*---(check file)---------------------*/
   DEBUG_INPT   yLOG_char    ("source"    , my.source);
   --rce;  switch (my.source) {
   case DATA_FILE    :
      if (strldcnt (my.file, '/', LEN_PATH) == 0) {
         sprintf (x_path, "%s/%s", x_home, my.file);
      } else {
         strlcpy (x_path, my.file, LEN_PATH);
      }
      break;
   case DATA_SOURCES :
      strlcpy (x_path, x_home, LEN_PATH);
      break;
   default  :
      DEBUG_INPT   yLOG_note    ("can not register this type");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(import)-------------------------*/
   rc = metis_world_purge_all ();
   rc = metis_world__import ();
   /*---(find)---------------------------*/
   rc = metis_world_by_path (x_path, &x_world);
   --rce;  if (x_world == NULL) {
      yURG_err ('w', "project is not in registry, nothing to withdraw");
      DEBUG_INPT   yLOG_exit    (__FUNCTION__);
      return 1;
   } else {
      yURG_msg ('-', "project exists in registry, prepared for change");
   }
   /*---(remove)-------------------------*/
   rc = metis_world_free (&x_world);
   --rce;  if (rc < 0) {
      yURG_err ('f', "project could not be withdrawn");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(export)-------------------------*/
   rc = metis_world__export ();
   --rce;  if (rc < 0) {
      yURG_err ('f', "project could not be added");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(purge)--------------------------*/
   rc = metis_world_purge_all ();
   --rce;  if (rc < 0) {
      yURG_err ('f', "project could not be added");
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
metis_world_system      (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_curr      [LEN_RECD]  = "";
   char       *p           = NULL;
   tWORLD     *x_world     = NULL;
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(save present directory)---------*/
   p = getcwd (x_curr, LEN_RECD);
   DEBUG_INPT   yLOG_point   ("getcwd"    , p);
   --rce;  if (p == NULL) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(purge)--------------------------*/
   rc = metis_world_purge_all ();
   DEBUG_INPT   yLOG_value   ("purge"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return  rce;
   }
   /*---(import)-------------------------*/
   rc = metis_world__import ();
   DEBUG_INPT   yLOG_value   ("import"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_note    ("no existing file");
   }
   /*---(import)-------------------------*/
   rc = metis_db_read        ();
   DEBUG_INPT   yLOG_value   ("import"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_note    ("no existing file");
   }
   /*---(walk-through)-------------------*/
   DEBUG_DATA   yLOG_value   ("count"     , metis_world_count ());
   rc = metis_world_by_cursor (YDLST_HEAD, &x_world);
   while (rc >= 0 && x_world != NULL) {
      /*---(show output)-----------------*/
      DEBUG_DATA   yLOG_point ("x_world"   , x_world);
      DEBUG_DATA   yLOG_info  ("->name"    , x_world->path);
      IF_VERBOSE   yURG_msg (' ', "");
      IF_CONFIRM  yURG_msg_live ();
      yURG_msg     (YURG_BASE, "%c  %s", x_world->type, x_world->path);
      IF_CONFIRM  yURG_msg_mute ();
      /*---(purge old data)--------------*/
      rc = metis_source_purge (x_world->type, x_world->path);
      DEBUG_INPT   yLOG_value   ("purge"     , rc);
      /*---(prepare)---------------------*/
      my.source = x_world->type;
      if (x_world->type == 's') {
         rc = chdir (x_world->path);
         DEBUG_INPT   yLOG_value   ("chdir"     , rc);
         if (rc < 0) continue;
      } else {
         strlcpy (my.file, x_world->path, LEN_PATH);
      }
      /*---(refresh)---------------------*/
      rc = metis_data_refresh ();
      DEBUG_INPT   yLOG_value   ("here"      , rc);
      /*---(next)------------------------*/
      rc = metis_world_by_cursor (YDLST_NEXT, &x_world);
      /*---(done)------------------------*/
   }
   /*---(export)-------------------------*/
   rc = metis_db_write       ();
   DEBUG_INPT   yLOG_value   ("export"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_note    ("no existing file");
   }
   /*---(return to current)--------------*/
   rc = chdir (x_curr);
   DEBUG_INPT   yLOG_value   ("chdir"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}




/*====================------------------------------------====================*/
/*===----                        program level                         ----===*/
/*====================------------------------------------====================*/
static void  o___PROGRAM_________o () { return; }

char
metis_world_init        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(initialize)---------------------*/
   rc = ySORT_btree (B_WORLD, "registry");
   DEBUG_PROG   yLOG_value   ("btree"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   metis_world_cli (F_WORLD, '-');
   my.f_world = NULL;
   /*> poly_world__purge ();                                                          <*/
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
metis_world_purge_all   (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tWORLD     *x_world      = NULL;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(walk)---------------------------*/
   rc = metis_world_by_cursor (YDLST_HEAD, &x_world);
   while (x_world != NULL) {
      metis_world_free   (&x_world);
      rc = metis_world_by_cursor (YDLST_HEAD, &x_world);
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
metis_world_wrap        (void)
{
   metis_world_purge_all ();
   return 0;
}




/*====================------------------------------------====================*/
/*===----                         unit testing                         ----===*/
/*====================------------------------------------====================*/
static void  o___UNITTEST________o () { return; }

char*        /*-[ unit test accessor ]---------------------[us2---·A-7·6--·B21-]¬[----·T-B1H---·---·----]¬[---·------]*/
metis_world__unit       (char *a_question, int i)
{
   /*---(locals)-----------+-----------+-*/
   char        rc          =    0;
   char        x_exist     =  '-';
   char        t           [LEN_LABEL] = " 0åæ";
   char        s           [LEN_HUND]  = " 0åæ";
   tWORLD     *u           = NULL;
   /*---(defense)------------------------*/
   snprintf (unit_answer, LEN_RECD, "WORLD unit       : question unknown");
   /*---(simple)-------------------------*/
   if      (strcmp (a_question, "count"     )     == 0) {
      /*> snprintf (unit_answer, LEN_RECD, "WORLD count      : %3dn", poly_world__count ());   <*/
   }
   else if (strcmp (a_question, "file"      )     == 0) {
      rc = metis_shared_verify (my.n_world);
      if      (rc >  0)  x_exist = 'y';
      else if (rc <= 0)  x_exist = '-';
      snprintf (unit_answer, LEN_RECD, "WORLD file       : %c  %-10p  %c  %2då%sæ",
            (my.f_world  == NULL) ? '-' : 'y', my.f_world,
            x_exist, strlen (my.n_world), my.n_world);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


