/*============================----beg-of-source---============================*/
#include   "metis.h"



/*====================------------------------------------====================*/
/*===----                        simple support                        ----===*/
/*====================------------------------------------====================*/
static void  o___SUPPORT_________o () { return; }

char
metis_source_wipe        (tSOURCE *a_dst)
{
   if (a_dst == NULL)  return -1;
   /*---(overall)-----------*/
   a_dst->path     [0] = '\0';
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
metis_source_new         (char *a_path, char a_force, tSOURCE **r_new)
{  /*---(design notes)-------------------*/
   /*
    * a_force   '-' means r_new must be grounded (safefy), update btree
    *           'y' means r_new will be overwritten, update btree
    *           'r' (rapid) is 'y' plus the btree is NOT updated
    */
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tSOURCE    *x_exist     = NULL;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   DEBUG_DATA   yLOG_char    ("a_force"   , a_force);
   /*---(defense)------------------------*/
   DEBUG_DATA   yLOG_point   ("a_path"    , a_path);
   --rce;  if (a_path == NULL) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DATA   yLOG_info    ("a_path"    , a_path);
   --rce;  if (a_path [0] == '\0') {
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
   rc = ySORT_by_name (B_SOURCE, a_path, &x_exist);
   DEBUG_DATA   yLOG_point   ("x_exist"   , x_exist);
   --rce;  if (x_exist != NULL) {
      *r_new = x_exist;
      DEBUG_DATA   yLOG_exit    (__FUNCTION__);
      return 1;
   }
   /*---(create)-------------------------*/
   rc = metis_shared_new  (B_SOURCE, &x_exist, '-', metis_source_wipe);
   DEBUG_DATA   yLOG_value   ("new"       , rc);
   --rce;  if (rc < 0) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(populate)-----------------------*/
   strlcpy (x_exist->path, a_path, LEN_PATH);
   /*---(hook)---------------------------*/
   rc = ySORT_hook (B_SOURCE, x_exist, x_exist->path, &(x_exist->ysort));
   DEBUG_DATA   yLOG_value   ("hook"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   if (a_force != 'r') {
      rc = ySORT_prepare (B_SOURCE);
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
metis_source_free        (tSOURCE **r_old)
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
   rc = metis_shared_free (B_SOURCE, r_old);
   DEBUG_DATA   yLOG_value   ("free"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   rc = ySORT_prepare (B_SOURCE);
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
metis_source_hook        (tSOURCE *a_source, tTASK *a_task)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_DATA   yLOG_point   ("a_source"  , a_source);
   --rce;  if (a_source == NULL) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DATA   yLOG_point   ("a_task"    , a_task);
   --rce;  if (a_task == NULL) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DATA   yLOG_point   ("->source"  , a_task->source);
   --rce;  if (a_task->source != NULL) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(onto major)---------------------*/
   DEBUG_DATA   yLOG_point   ("head"      , a_source->head);
   DEBUG_DATA   yLOG_point   ("tail"      , a_source->tail);
   if (a_source->head  == NULL) {
      DEBUG_DATA   yLOG_note    ("first");
      a_source->head        = a_source->tail  = a_task;
   } else {
      DEBUG_DATA   yLOG_note    ("append");
      a_task->s_prev          = a_source->tail;
      a_source->tail->s_next  = a_task;
      a_source->tail        = a_task;
   }
   /*---(tie function back to file)------*/
   DEBUG_DATA   yLOG_note    ("set file");
   a_task->source = a_source;
   /*---(update count)-------------------*/
   DEBUG_DATA   yLOG_note    ("increment counts");
   ++(a_source->count);
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
metis_source_unhook      (tTASK *a_task)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_DATA   yLOG_point   ("a_minor"   , a_task);
   --rce;  if (a_task == NULL) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DATA   yLOG_point   ("->source"  , a_task->source);
   --rce;  if (a_task->source == NULL) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(out of linked list)-------------*/
   DEBUG_DATA   yLOG_point   ("head"      , a_task->source->head);
   DEBUG_DATA   yLOG_point   ("tail"      , a_task->source->tail);
   DEBUG_DATA   yLOG_note    ("unlink");
   if (a_task->s_next != NULL)   a_task->s_next->s_prev  = a_task->s_prev;
   else                          a_task->source->tail    = a_task->s_prev;
   if (a_task->s_prev != NULL)   a_task->s_prev->s_next  = a_task->s_next;
   else                          a_task->source->head    = a_task->s_next;
   DEBUG_DATA   yLOG_point   ("head*"     , a_task->source->head);
   DEBUG_DATA   yLOG_point   ("tail*"     , a_task->source->tail);
   /*---(update count)-------------------*/
   DEBUG_DATA   yLOG_note    ("decrement counts");
   --(a_task->source->count);
   if (a_task->source->count == 0) {
      a_task->source->head  = NULL;
      a_task->source->tail  = NULL;
   }
   /*---(ground)-------------------------*/
   DEBUG_DATA   yLOG_note    ("unset file");
   a_task->source = NULL;
   a_task->s_prev = NULL;
   a_task->s_next = NULL;
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        searching                             ----===*/
/*====================------------------------------------====================*/
static void  o___SEARCH__________o () { return; }

int  metis_source_count       (void)                              { return ySORT_count     (B_SOURCE); }
char metis_source_by_name     (uchar *a_name, tSOURCE **r_source) { return ySORT_by_name   (B_SOURCE, a_name, r_source); }
char metis_source_by_index    (int n, tSOURCE **r_source)         { return ySORT_by_index  (B_SOURCE, n, r_source); }
char metis_source_by_cursor   (char a_dir, tSOURCE **r_source)    { return ySORT_by_cursor (B_SOURCE, a_dir, r_source); }

char*
metis_source_entry       (int n)
{
   tSOURCE    *x_source     = NULL;
   char        s           [LEN_LABEL] = " -åæ";
   char        t           [LEN_LABEL] = " -åæ";
   metis_source_by_index (n, &x_source);
   if (x_source == NULL)  return "n/a";
   if (x_source->head != NULL)  sprintf (s, "%2då%.20sæ", strlen (x_source->head->txt), x_source->head->txt);
   if (x_source->tail != NULL)  sprintf (t, "%2då%.20sæ", strlen (x_source->tail->txt), x_source->tail->txt);
   sprintf (g_print, "%-2d %-40.40s %2d %-24.24s %s", n, x_source->path, x_source->count, s, t);
   return g_print;
}


/*====================------------------------------------====================*/
/*===----                        program level                         ----===*/
/*====================------------------------------------====================*/
static void  o___PROGRAM_________o () { return; }

char
metis_source_init        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(initialize)---------------------*/
   rc = ySORT_btree (B_SOURCE, "source");
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
metis_source_wrap        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tSOURCE    *x_source     = NULL;
   int         n           =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(walk)---------------------------*/
   rc = metis_source_by_index  (n, &x_source);
   while (x_source != NULL) {
      rc = metis_source_free   (&x_source);
      if (rc < 0) ++n;
      rc = metis_source_by_index  (n, &x_source);
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}


