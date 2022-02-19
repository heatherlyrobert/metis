/*============================----beg-of-source---============================*/
#include   "metis.h"



/*====================------------------------------------====================*/
/*===----                        simple support                        ----===*/
/*====================------------------------------------====================*/
static void  o___SUPPORT_________o () { return; }

char
metis_minor_wipe        (tMINOR *a_dst)
{
   if (a_dst == NULL)  return -1;
   /*---(parent)------------*/
   a_dst->major     = NULL;
   /*---(overall)-----------*/
   a_dst->name     [0] = '\0';
   /*---(within parent)-----*/
   a_dst->prev      = NULL;
   a_dst->next      = NULL;
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
metis_minor_new         (tMAJOR *a_major, char *a_name, char a_force, tMINOR **r_new)
{  /*---(design notes)-------------------*/
   /*
    * a_force   '-' means r_new must be grounded (safefy), update btree
    *           'y' means r_new will be overwritten, update btree
    *           'r' (rapid) is 'y' plus the btree is NOT updated
    */
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tMINOR     *x_exist     = NULL;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   DEBUG_DATA   yLOG_char    ("a_force"   , a_force);
   /*---(defense)------------------------*/
   DEBUG_DATA   yLOG_point   ("a_major"   , a_major);
   --rce;  if (a_major == NULL) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
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
   rc = ySORT_by_name (B_MINOR, a_name, &x_exist);
   DEBUG_DATA   yLOG_point   ("x_exist"   , x_exist);
   --rce;  while (x_exist != NULL) {
      DEBUG_DATA   yLOG_point   ("->name"    , x_exist->name);
      if (strcmp (x_exist->name, a_name) != 0)  break;
      if (x_exist->major == a_major) {
         *r_new = x_exist;
         DEBUG_DATA   yLOG_exit    (__FUNCTION__);
         return 1;
      }
      rc = ySORT_by_cursor (B_MINOR, YDLST_NEXT, &x_exist);
   }
   /*---(create)-------------------------*/
   rc = metis_shared_new  (B_MINOR, &x_exist, '-', metis_minor_wipe);
   DEBUG_DATA   yLOG_value   ("new"       , rc);
   --rce;  if (rc < 0) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(populate)-----------------------*/
   strlcpy (x_exist->name, a_name, LEN_LABEL);
   /*---(hook)---------------------------*/
   rc = ySORT_hook (B_MINOR, x_exist, x_exist->name, &(x_exist->ysort));
   DEBUG_DATA   yLOG_value   ("hook"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   if (a_force != 'r') {
      rc = ySORT_prepare (B_MINOR);
      DEBUG_DATA   yLOG_value   ("prepare"   , rc);
      --rce;  if (rc < 0) {
         DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(hook to major)------------------*/
   rc = metis_major_hook (a_major, x_exist);
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
metis_minor_free        (tMINOR **r_old)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tMINOR     *x_exist     = NULL;
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
   /*---(unhook from major)--------------*/
   rc = metis_major_unhook (*r_old);
   DEBUG_DATA   yLOG_value   ("unhook"    , rc);
   --rce;  if (rc < 0) {
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
   rc = metis_shared_free (B_MINOR, r_old);
   DEBUG_DATA   yLOG_value   ("free"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(prepare)------------------------*/
   rc = ySORT_prepare (B_MINOR);
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

/*> char                                                                              <* 
 *> metis_minor_hook        (tMINOR *a_major, tMINOR *a_minor)                        <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                       <* 
 *>    char        rce         =  -10;                                                <* 
 *>    char        rc          =    0;                                                <* 
 *>    /+---(header)-------------------------+/                                       <* 
 *>    DEBUG_DATA   yLOG_enter   (__FUNCTION__);                                      <* 
 *>    /+---(defense)------------------------+/                                       <* 
 *>    DEBUG_DATA   yLOG_point   ("a_major"   , a_major);                             <* 
 *>    --rce;  if (a_major == NULL) {                                                 <* 
 *>       DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);                              <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    /+---(onto major)---------------------+/                                       <* 
 *>    DEBUG_DATA   yLOG_point   ("head"      , a_major->head);                       <* 
 *>    DEBUG_DATA   yLOG_point   ("tail"      , a_major->tail);                       <* 
 *>    if (a_major->head  == NULL) {                                                  <* 
 *>       DEBUG_DATA   yLOG_note    ("first");                                        <* 
 *>       a_major->head        = a_major->tail  = a_minor;                            <* 
 *>    } else {                                                                       <* 
 *>       DEBUG_DATA   yLOG_note    ("append");                                       <* 
 *>       a_minor->prev        = a_major->tail;                                       <* 
 *>       a_major->tail->next  = a_minor;                                             <* 
 *>       a_major->tail        = a_minor;                                             <* 
 *>    }                                                                              <* 
 *>    /+---(tie function back to file)------+/                                       <* 
 *>    DEBUG_DATA   yLOG_note    ("set file");                                        <* 
 *>    a_minor->major = a_major;                                                      <* 
 *>    /+---(update count)-------------------+/                                       <* 
 *>    DEBUG_DATA   yLOG_note    ("increment counts");                                <* 
 *>    ++(a_major->count);                                                            <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    DEBUG_DATA   yLOG_exit    (__FUNCTION__);                                      <* 
 *>    return 0;                                                                      <* 
 *> }                                                                                 <*/

/*> char                                                                              <* 
 *> metis_minor_unhook      (tMINOR *a_major)                                         <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----+-----+-+/                                       <* 
 *>    char        rce         =  -10;                                                <* 
 *>    char        rc          =    0;                                                <* 
 *>    /+---(header)-------------------------+/                                       <* 
 *>    DEBUG_DATA   yLOG_enter   (__FUNCTION__);                                      <* 
 *>    /+---(defense)------------------------+/                                       <* 
 *>    DEBUG_DATA   yLOG_point   ("a_major"   , a_major);                             <* 
 *>    --rce;  if (a_major == NULL) {                                                 <* 
 *>       DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);                              <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    /+---(unhook from btree)--------------+/                                       <* 
 *>    rc = ySORT_unhook (&a_major->ysort);                                           <* 
 *>    DEBUG_DATA   yLOG_value   ("btree"     , rc);                                  <* 
 *>    --rce;  if (rc < 0) {                                                          <* 
 *>       DEBUG_DATA   yLOG_exitr   (__FUNCTION__, rce);                              <* 
 *>       return rce;                                                                 <* 
 *>    }                                                                              <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    DEBUG_DATA   yLOG_exit    (__FUNCTION__);                                      <* 
 *>    return 0;                                                                      <* 
 *> }                                                                                 <*/



/*====================------------------------------------====================*/
/*===----                        searching                             ----===*/
/*====================------------------------------------====================*/
static void  o___SEARCH__________o () { return; }

int  metis_minor_count       (void)                            { return ySORT_count     (B_MINOR); }
char metis_minor_by_name     (uchar *a_name, tMINOR **r_minor) { return ySORT_by_name   (B_MINOR, a_name, r_minor); }
char metis_minor_by_index    (int n, tMINOR **r_minor)         { return ySORT_by_index  (B_MINOR, n, r_minor); }
char metis_minor_by_cursor   (char a_dir, tMINOR **r_minor)    { return ySORT_by_cursor (B_MINOR, a_dir, r_minor); }

char*
metis_minor_entry       (int n)
{
   tMINOR     *x_minor     = NULL;
   char        s           [LEN_LABEL] = " -åæ";
   char        t           [LEN_LABEL] = " -åæ";
   metis_minor_by_index (n, &x_minor);
   if (x_minor == NULL)  return "n/a";
   if (x_minor->head != NULL)  sprintf (s, "%2då%.20sæ", strlen (x_minor->head->txt), x_minor->head->txt);
   if (x_minor->tail != NULL)  sprintf (t, "%2då%.20sæ", strlen (x_minor->tail->txt), x_minor->tail->txt);
   sprintf (g_print, "%-2d %-20.20s %-20.20s %2d %-24.24s %s", n, x_minor->major->name, x_minor->name, x_minor->count, s, t);
   return g_print;
}


/*====================------------------------------------====================*/
/*===----                        program level                         ----===*/
/*====================------------------------------------====================*/
static void  o___PROGRAM_________o () { return; }

char
metis_minor_init        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(initialize)---------------------*/
   rc = ySORT_btree (B_MINOR, "major");
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
metis_minor_wrap        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   tMINOR     *x_minor     = NULL;
   /*---(header)-------------------------*/
   DEBUG_PROG   yLOG_enter   (__FUNCTION__);
   /*---(walk)---------------------------*/
   rc = metis_minor_by_cursor (YDLST_HEAD, &x_minor);
   while (x_minor != NULL) {
      metis_minor_free   (&x_minor);
      rc = metis_minor_by_cursor (YDLST_HEAD, &x_minor);
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit    (__FUNCTION__);
   return 0;
}


