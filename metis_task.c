/*============================----beg-of-source---============================*/
#include   "metis.h"



/*====================------------------------------------====================*/
/*===----                        simple support                        ----===*/
/*====================------------------------------------====================*/
static void  o___SUPPORT_________o () { return; }

char             /* [p-----] initialize a single task ------------------------*/
metis_task_wipe         (tTASK *a_dst)
{
   /*---(parent)------------*/
   a_dst->minor    = NULL;
   a_dst->seq      =   0;
   a_dst->ref      =   0;
   /*---(master data)-----------------*/
   a_dst->urg      = '·';
   a_dst->imp      = '·';
   a_dst->est      = '·';
   a_dst->prg      = '·';
   a_dst->shr      = '·';
   a_dst->txt [0]  = '\0';
   a_dst->epoch [0]= '\0';
   a_dst->days     =   0;
   /*---(within parent)---------------*/
   a_dst->m_prev   = NULL;
   a_dst->m_next   = NULL;
   /*---(source data)-----------------*/
   a_dst->source   = NULL;
   a_dst->s_prev   = NULL;
   a_dst->s_next   = NULL;
   a_dst->line     =  -1;
   /*---(filtering)-------------------*/
   a_dst->show     = 'y';
   a_dst->note     = '·';
   a_dst->key [0]  = '\0';
   a_dst->srch [0] = '\0';
   a_dst->x        =    0;
   a_dst->y        =    0;
   /*---(btree)-----------------------*/
   a_dst->ysort    = NULL;
   a_dst->unique   = NULL;
   /*---(complete)--------------------*/
   return 1;
}



/*====================------------------------------------====================*/
/*===----                       memory allccation                      ----===*/
/*====================------------------------------------====================*/
static void  o___MEMORY__________o () { return; }

char
metis_task_new         (tMINOR *a_minor, char a_force, tTASK **r_new)
{  /*---(design notes)-------------------*/
   /*
    * a_force   '-' means r_new must be grounded (safefy), update btree
    *           'y' means r_new will be overwritten, update btree
    *           'r' (rapid) is 'y' plus the btree is NOT updated
    */
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tTASK      *x_exist     = NULL;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   DEBUG_DATA   yLOG_char    ("a_force"   , a_force);
   /*---(defense)------------------------*/
   DEBUG_DATA   yLOG_point   ("a_minor"   , a_minor);
   --rce;  if (a_minor == NULL) {
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
   /*---(create)-------------------------*/
   rc = metis_shared_new  (B_TASK, &x_exist, '-', metis_task_wipe);
   DEBUG_DATA   yLOG_value   ("new"       , rc);
   --rce;  if (rc < 0) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(populate)-----------------------*/
   sprintf (x_exist->txt, "#%04d", ySORT_count (B_TASK));
   x_exist->seq = a_minor->count;
   sprintf (x_exist->key, "%-20.20s %-30.30s %3d", a_minor->major->name, a_minor->name, x_exist->seq);
   /*---(hook)---------------------------*/
   rc = ySORT_hook (B_TASK, x_exist, x_exist->key, &(x_exist->ysort));
   DEBUG_DATA   yLOG_value   ("hook"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   rc = ySORT_prepare (B_TASK);
   DEBUG_DATA   yLOG_value   ("prepare"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(hook to major)------------------*/
   rc = metis_minor_hook (a_minor, x_exist);
   DEBUG_DATA   yLOG_value   ("hook"      , rc);
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
metis_task_free        (tTASK **r_old)
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
   /*---(unhook from minor)--------------*/
   rc = metis_minor_unhook (*r_old);
   DEBUG_DATA   yLOG_value   ("minor"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(hook to unique)-----------------*/
   --rce;  if ((*r_old)->unique != NULL) {
      rc = ySORT_unhook (&(*r_old)->unique);
      DEBUG_DATA   yLOG_value   ("unique"    , rc);
      if (rc < 0) {
         DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
      rc = ySORT_prepare (B_UNIQUE);
   }
   /*---(unhook from source)-------------*/
   --rce;  if ((*r_old)->source != NULL) {
      rc = metis_source_unhook (*r_old);
      DEBUG_DATA   yLOG_value   ("source"    , rc);
      if (rc < 0) {
         DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(unhook from btree)--------------*/
   rc = ySORT_unhook (&((*r_old)->ysort));
   DEBUG_DATA   yLOG_value   ("btree"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(free memory)--------------------*/
   rc = metis_shared_free (B_TASK, r_old);
   DEBUG_DATA   yLOG_value   ("free"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   rc = ySORT_prepare (B_TASK);
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

int  metis_task_count        (void)                            { return ySORT_count     (B_TASK); }
char metis_task_by_index     (int n, tTASK **r_task)           { return ySORT_by_index  (B_TASK, n, r_task); }
char metis_task_by_cursor    (char a_dir, tTASK **r_task)      { return ySORT_by_cursor (B_TASK, a_dir, r_task); }

int  metis_epoch_count       (void)                            { return ySORT_count     (B_UNIQUE); }
char metis_epoch_by_index    (int n, tTASK **r_task)           { return ySORT_by_index  (B_UNIQUE, n, r_task); }
char metis_epoch_by_cursor   (char a_dir, tTASK **r_task)      { return ySORT_by_cursor (B_UNIQUE, a_dir, r_task); }
char metis_epoch_by_name     (uchar *a_name, tMINOR **r_minor) { return ySORT_by_name   (B_UNIQUE, a_name, r_minor); }

char
metis_task_by_aindex    (int n, tTASK**r_task)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tTASK      *x_task      = NULL;
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(compile)------------------------*/
   DEBUG_DATA   yLOG_point   ("r_task"    , r_task);
   --rce;  if (r_task == NULL) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (r_task != NULL)  *r_task = NULL;
   /*---(walk)---------------------------*/
   rc = metis_task_by_cursor (YDLST_HEAD, &x_task);
   while (x_task != NULL) {
      if (x_task->show == 'y') {
         if (n == c) {
            DEBUG_DATA   yLOG_info    ("found in"  , x_task->txt);
            *r_task = x_task;
            DEBUG_PROG   yLOG_exit    (__FUNCTION__);
            return 0;
         }
         ++c;
      }
      rc = metis_task_by_cursor (YDLST_NEXT, &x_task);
   }
   /*---(complete)-----------------------*/
   --rce;
   DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
   return rce;
}

/*> int                                                                               <* 
 *> metis_task_by_regex     (char *a_regex, tTASK **r_task)                           <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                       <* 
 *>    char        rce         =  -10;                                                <* 
 *>    char        rc          =    0;                                                <* 
 *>    tTASK      *x_task      = NULL;                                                <* 
 *>    int         c           =    0;                                                <* 
 *>    /+---(header)-------------------------+/                                       <* 
 *>    DEBUG_PROG   yLOG_enter   (__FUNCTION__);                                      <* 
 *>    /+---(compile)------------------------+/                                       <* 
 *>    DEBUG_DATA   yLOG_point   ("a_regex"   , a_regex);                             <* 
 *>    --rce;  if (a_regex == NULL) {                                                 <* 
 *>       DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);                              <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    rc = yREGEX_comp (a_regex);                                                    <* 
 *>    DEBUG_DATA   yLOG_value   ("comp"      , rc);                                  <* 
 *>    --rce;  if (rc < 0) {                                                          <* 
 *>       DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);                              <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    if (r_task != NULL)  *r_task = NULL;                                           <* 
 *>    /+---(walk)---------------------------+/                                       <* 
 *>    rc = metis_task_by_cursor (YDLST_HEAD, &x_task);                               <* 
 *>    while (x_task != NULL) {                                                       <* 
 *>       x_task->note = '·';                                                         <* 
 *>       rc = yREGEX_filter (x_task->txt);                                           <* 
 *>       if (rc > 0) {                                                               <* 
 *>          DEBUG_DATA   yLOG_info    ("found in"  , x_task->txt);                   <* 
 *>          if (r_task != NULL && *r_task == NULL)  *r_task = x_task;                <* 
 *>          x_task->note = 'r';                                                      <* 
 *>          ++c;                                                                     <* 
 *>       }                                                                           <* 
 *>       rc = metis_task_by_cursor (YDLST_NEXT, &x_task);                            <* 
 *>    }                                                                              <* 
 *>    DEBUG_DATA   yLOG_value   ("c"         , c);                                   <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    DEBUG_PROG   yLOG_exit    (__FUNCTION__);                                      <* 
 *>    return c;                                                                      <* 
 *> }                                                                                 <*/

char*
metis_task_entry       (int n)
{
   tTASK      *x_task      = NULL;
   char        t           [LEN_HUND]  = " -åæ";
   char        s           [LEN_LABEL] = "·";
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   metis_task_by_index (n, &x_task);
   if (x_task == NULL) {
      strcpy (g_print, "n/a");
   } else {
      sprintf (t, "%2då%.25sæ", strlen (x_task->txt), x_task->txt);
      if (strlen (x_task->epoch) == 6) sprintf (s, "%6.6s", x_task->epoch);
      sprintf (g_print, "%-2d %-20.20s %-20.20s %2d %-29.29s %c %c %c %c %c  %c %c  %s",
            n, x_task->minor->major->name, x_task->minor->name, x_task->seq, t,
            x_task->urg, x_task->imp, x_task->est, x_task->prg, x_task->shr,
            x_task->show, x_task->note, s);
   }
   DEBUG_PROG   yLOG_info    ("task"      , g_print);
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return g_print;
}


/*====================------------------------------------====================*/
/*===----                        program level                         ----===*/
/*====================------------------------------------====================*/
static void  o___PROGRAM_________o () { return; }

char
metis_task_init        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(initialize)---------------------*/
   rc = ySORT_btree (B_TASK, "tasks");
   DEBUG_PROG   yLOG_value   ("init"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_PROG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   rc = ySORT_btree (B_UNIQUE, "unique");
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
metis_task_purge_all   (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tTASK      *x_task      = NULL;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(walk)---------------------------*/
   rc = metis_task_by_cursor (YDLST_HEAD, &x_task);
   while (x_task != NULL) {
      metis_task_free   (&x_task);
      rc = metis_task_by_cursor (YDLST_HEAD, &x_task);
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}


