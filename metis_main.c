#include   "metis.h"


int
main               (int argc, char *argv[])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_show      =  '-';
   /*---(initialize)---------------------*/
   if (rc >= 0)  rc = PROG_urgents (argc, argv);
   if (rc >= 0)  rc = PROG_startup (argc, argv);
   --rce;  if (rc <  0) {
      DEBUG_DATA   yLOG_exitr    (__FUNCTION__, rce);
      PROG_shutdown ();
      return rce;
   }
   /*---(driver)-------------------------*/
   rc = yJOBS_driver (P_ONELINE, metis_yjobs);
   DEBUG_PROG   yLOG_value    ("driver"    , rc);
   IF_RUNNING   x_show = 'y';
   /*---(handle report)------------------*/
   IF_LOCALRPT  {
      metis_text_driver ();
   }
   /*---(main loop)----------------------*/
   else if (x_show == 'y') {
      DEBUG_DATA   yLOG_enter    (__FUNCTION__);
      /*---(visual setup)-------------------*/
      rc = PROG_dawn    ();
      DEBUG_PROG   yLOG_value    ("dawn"      , rc);
      if (rc <  0) { PROG_shutdown (); return -3; }
      /*---(main-loop)----------------------*/
      if (my.png == '-') {
         rc = yVIOPENGL_main  ("keys", "every", NULL);
         DEBUG_PROG   yLOG_value    ("main"      , rc);
      }
      /*---(visual teardown)----------------*/
      rc = PROG_dusk     ();
      DEBUG_PROG   yLOG_value    ("dusk"      , rc);
      DEBUG_DATA   yLOG_exit     (__FUNCTION__);
   }
   /*---(wrap-up)------------------------*/
   PROG_shutdown ();
   /*---(complete)-----------------------*/
   return rc;
}
