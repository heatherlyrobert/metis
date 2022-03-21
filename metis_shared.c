/*============================----beg-of-source---============================*/
#include   "metis.h"




/*====================------------------------------------====================*/
/*===----                       memory allccation                      ----===*/
/*====================------------------------------------====================*/
static void  o___MEMORY__________o () { return; }

char
metis_shared_new        (char a_abbr, void **r_new, char a_force, char *a_wiper (void *))
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_size      =    0;
   void       *x_new       = NULL;
   int         x_tries     =    0;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_senter  (__FUNCTION__);
   DEBUG_DATA   yLOG_schar   (a_abbr);
   /*---(check return)-------------------*/
   DEBUG_DATA   yLOG_spoint  (r_new);
   --rce;  if (r_new == NULL) {
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DATA   yLOG_spoint  (*r_new);
   --rce;  if (a_force != 'y' && *r_new != NULL) {
      DEBUG_DATA   yLOG_snote   ("already set");
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(default)------------------------*/
   *r_new = NULL;
   /*---(check size)---------------------*/
   --rce;  switch (a_abbr) {
   case B_MAJOR  : x_size = sizeof (tMAJOR);   break;
   case B_MINOR  : x_size = sizeof (tMINOR);   break;
   case B_SOURCE : x_size = sizeof (tSOURCE);  break;
   case B_TASK   : x_size = sizeof (tTASK);    break;
   case B_WORLD  : x_size = sizeof (tWORLD);   break;
   default       :
      DEBUG_DATA   yLOG_snote   ("unknown btree abbr");
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
      break;
   }
   /*---(allocate)-----------------------*/
   while (x_new == NULL) {
      ++x_tries;
      x_new = malloc (x_size);
      if (x_tries > 3)   break;
   }
   DEBUG_DATA   yLOG_sint    (x_tries);
   DEBUG_DATA   yLOG_spoint  (x_new);
   --rce;  if (x_new == NULL) {
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(wipe)---------------------------*/
   rc = 0;
   if (a_wiper != NULL)  rc = a_wiper (x_new);
   /*---(save return)--------------------*/
   *r_new = x_new;
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
   return rc;
}

char
metis_shared_free       (char a_abbr, void **r_old)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_senter  (__FUNCTION__);
   DEBUG_DATA   yLOG_sint    (a_abbr);
   /*---(check return)-------------------*/
   DEBUG_DATA   yLOG_spoint  (r_old);
   --rce;  if (r_old == NULL) {
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DATA   yLOG_spoint  (*r_old);
   --rce;  if (*r_old == NULL) {
      DEBUG_DATA   yLOG_snote   ("never set");
      DEBUG_DATA   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(clear and return)---------------*/
   free (*r_old);
   /*---(save return)--------------------*/
   *r_old = NULL;
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
metis_shared_verify     (uchar *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rci         =    0;
   tSTAT       st;
   /*---(defense)------------------------*/
   if (a_name == NULL)                   return 0;
   if (strcmp (a_name, "") == 0)         return 0;
   /*---(check for existance)------------*/
   rci = lstat (a_name, &st);
   DEBUG_FILE   yLOG_value   ("lstat"     , rci);
   --rce; if (rci < 0) {
      DEBUG_FILE   yLOG_note    ("file does not exist, can not read");
      return rce;
   }
   /*---(check for regular file)---------*/
   --rce;  if (!S_ISREG (st.st_mode)) {
      DEBUG_FILE   yLOG_note    ("not a regular file, rejected");
      return rce;
   }
   /*---(output)-------------------------*/
   DEBUG_FILE   yLOG_note    ("confirmed as existing and is a regular file");
   /*---(complete)-----------------------*/
   return 1;
}


