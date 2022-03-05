/*============================----beg-of-source---============================*/
#include   "metis.h"



/*====================------------------------------------====================*/
/*===----                        simple support                        ----===*/
/*====================------------------------------------====================*/
static void  o___SUPPORT_________o () { return; }

char
metis_major_wipe        (tMAJOR *a_dst)
{
   if (a_dst == NULL)  return -1;
   /*---(overall)-----------*/
   a_dst->name     [0] = '\0';
   /*---(files)-------------*/
   a_dst->head      = NULL;
   a_dst->tail      = NULL;
   a_dst->count     = 0;
   /*---(btree)-------------*/
   a_dst->ysort     = NULL;
   /*---(tags)--------------*/
   return 1;
}


/*====================------------------------------------====================*/
/*===----                       memory allccation                      ----===*/
/*====================------------------------------------====================*/
static void  o___MEMORY__________o () { return; }

char
metis_major_new         (char *a_name, char a_force, tMAJOR **r_new)
{  /*---(design notes)-------------------*/
   /*
    * a_force   '-' means r_new must be grounded (safefy), update btree
    *           'y' means r_new will be overwritten, update btree
    *           'r' (rapid) is 'y' plus the btree is NOT updated
    */
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tMAJOR     *x_exist     = NULL;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   DEBUG_DATA   yLOG_char    ("a_force"   , a_force);
   /*---(defense)------------------------*/
   DEBUG_DATA   yLOG_point   ("a_name"    , a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DATA   yLOG_info    ("a_name"    , a_name);
   --rce;  if (a_name [0] == '\0') {
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
   rc = ySORT_by_name (B_MAJOR, a_name, &x_exist);
   DEBUG_DATA   yLOG_point   ("x_exist"   , x_exist);
   --rce;  if (x_exist != NULL) {
      *r_new = x_exist;
      DEBUG_DATA   yLOG_exit    (__FUNCTION__);
      return 1;
   }
   /*---(create)-------------------------*/
   rc = metis_shared_new  (B_MAJOR, &x_exist, '-', metis_major_wipe);
   DEBUG_DATA   yLOG_value   ("new"       , rc);
   --rce;  if (rc < 0) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(populate)-----------------------*/
   strlcpy (x_exist->name, a_name, LEN_LABEL);
   /*---(hook)---------------------------*/
   rc = ySORT_hook (B_MAJOR, x_exist, x_exist->name, &(x_exist->ysort));
   DEBUG_DATA   yLOG_value   ("hook"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   if (a_force != 'r') {
      rc = ySORT_prepare (B_MAJOR);
      DEBUG_DATA   yLOG_value   ("prepare"   , rc);
      --rce;  if (rc < 0) {
         DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(save back)----------------------*/
   *r_new = x_exist;
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
metis_major_free        (tMAJOR **r_old)
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
   DEBUG_DATA   yLOG_point   ("count"     , (*r_old)->count);
   --rce;  if ((*r_old)->count > 0) {
      DEBUG_DATA   yLOG_note    ("major has children, can not free");
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
   rc = metis_shared_free (B_MAJOR, r_old);
   DEBUG_DATA   yLOG_value   ("free"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   rc = ySORT_prepare (B_MAJOR);
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
/*===----                     hooking and unhooking                    ----===*/
/*====================------------------------------------====================*/
static void  o___HOOKING_________o () { return; }

char
metis_major_hook        (tMAJOR *a_major, tMINOR *a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_DATA   yLOG_point   ("a_major"   , a_major);
   --rce;  if (a_major == NULL) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DATA   yLOG_point   ("a_minor"   , a_minor);
   --rce;  if (a_minor == NULL) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(onto major)---------------------*/
   DEBUG_DATA   yLOG_point   ("head"      , a_major->head);
   DEBUG_DATA   yLOG_point   ("tail"      , a_major->tail);
   if (a_major->head  == NULL) {
      DEBUG_DATA   yLOG_note    ("first");
      a_major->head        = a_major->tail  = a_minor;
   } else {
      DEBUG_DATA   yLOG_note    ("append");
      a_minor->prev        = a_major->tail;
      a_major->tail->next  = a_minor;
      a_major->tail        = a_minor;
   }
   /*---(tie function back to file)------*/
   DEBUG_DATA   yLOG_note    ("set file");
   a_minor->major = a_major;
   /*---(update count)-------------------*/
   DEBUG_DATA   yLOG_note    ("increment counts");
   ++(a_major->count);
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
metis_major_unhook      (tMINOR *a_minor)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_DATA   yLOG_point   ("a_minor"   , a_minor);
   --rce;  if (a_minor == NULL) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DATA   yLOG_point   ("count"     , a_minor->count);
   --rce;  if (a_minor->count > 0) {
      DEBUG_DATA   yLOG_note    ("minor has children, can not unhook");
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(out of linked list)-------------*/
   DEBUG_DATA   yLOG_point   ("head"      , a_minor->major->head);
   DEBUG_DATA   yLOG_point   ("tail"      , a_minor->major->tail);
   DEBUG_DATA   yLOG_note    ("unlink");
   if (a_minor->next != NULL)   a_minor->next->prev  = a_minor->prev;
   else                         a_minor->major->tail = a_minor->prev;
   if (a_minor->prev != NULL)   a_minor->prev->next  = a_minor->next;
   else                         a_minor->major->head = a_minor->next;
   DEBUG_DATA   yLOG_point   ("head*"     , a_minor->major->head);
   DEBUG_DATA   yLOG_point   ("tail*"     , a_minor->major->tail);
   /*---(update count)-------------------*/
   DEBUG_DATA   yLOG_note    ("decrement counts");
   --(a_minor->major->count);
   if (a_minor->major->count == 0) {
      a_minor->major->head  = NULL;
      a_minor->major->tail  = NULL;
   }
   /*---(ground)-------------------------*/
   DEBUG_DATA   yLOG_note    ("unset file");
   a_minor->major = NULL;
   a_minor->prev  = NULL;
   a_minor->next  = NULL;
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        searching                             ----===*/
/*====================------------------------------------====================*/
static void  o___SEARCH__________o () { return; }

int  metis_major_count       (void)                            { return ySORT_count     (B_MAJOR); }
char metis_major_by_name     (uchar *a_name, tMAJOR **r_major) { return ySORT_by_name   (B_MAJOR, a_name, r_major); }
char metis_major_by_index    (int n, tMAJOR **r_major)         { return ySORT_by_index  (B_MAJOR, n, r_major); }
char metis_major_by_cursor   (char a_dir, tMAJOR **r_major)    { return ySORT_by_cursor (B_MAJOR, a_dir, r_major); }

char*
metis_major_entry       (int n)
{
   tMAJOR     *x_major     = NULL;
   char        s           [LEN_LABEL] = " -åæ";
   char        t           [LEN_LABEL] = " -åæ";
   metis_major_by_index (n, &x_major);
   if (x_major == NULL)  return "n/a";
   if (x_major->head != NULL)  sprintf (s, "%2då%.20sæ", strlen (x_major->head->name), x_major->head->name);
   if (x_major->tail != NULL)  sprintf (t, "%2då%.20sæ", strlen (x_major->tail->name), x_major->tail->name);
   sprintf (g_print, "%-2d %-20.20s %2d %-24.24s %s", n, x_major->name, x_major->count, s, t);
   return g_print;
}


/*====================------------------------------------====================*/
/*===----                        program level                         ----===*/
/*====================------------------------------------====================*/
static void  o___PROGRAM_________o () { return; }

char
metis_major_init        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(initialize)---------------------*/
   rc = ySORT_btree (B_MAJOR, "majors");
   DEBUG_PROG   yLOG_value   ("init"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
metis_major_cleanse     (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tMAJOR     *x_major     = NULL;
   int         n           =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(walk)---------------------------*/
   rc = metis_major_by_index  (n, &x_major);
   while (x_major != NULL) {
      rc = metis_major_free   (&x_major);
      if (rc < 0) ++n;
      rc = metis_major_by_index  (n, &x_major);
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
metis_major_purge_tasks (tMAJOR *a_major)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tMINOR     *x_minor     = NULL;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_PROG   yLOG_point   ("a_major"   , a_major);
   --rce;  if (a_major == NULL) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(walk)---------------------------*/
   x_minor = a_major->head;
   while (x_minor != NULL) {
      metis_minor_purge_tasks (x_minor);
      x_minor = x_minor->next;
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
metis_resequence_tasks  (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tMAJOR     *x_major     = NULL;
   tMINOR     *x_minor     = NULL;
   tTASK      *x_task      = NULL;
   short       c           =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(walk majors)--------------------*/
   rc = metis_major_by_cursor (YDLST_HEAD, &x_major);
   while (x_major != NULL) {
      /*---(walk minors)-----------------*/
      x_minor = x_major->head;
      while (x_minor != NULL) {
         /*---(walk tasks)---------------*/
         x_task = x_minor->head;
         c = 0;
         while (x_task != NULL) {
            x_task->seq = c++;
            metis_filter_key (x_task);
            x_task = x_task->m_next;
         }
         /*---(next minor)---------------*/
         x_minor = x_minor->next;
         /*---(done with minor)----------*/
      }
      /*---(next major)------------------*/
      rc = metis_major_by_index  (YDLST_NEXT, &x_major);
      /*---(done with major)-------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}


