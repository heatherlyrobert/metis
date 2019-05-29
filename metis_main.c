#include   "metis.h"

/*===[[ METIS BACKLOG ]]======================================================*
 *  metis  -----  tbd
 *
 */


int
main               (int argc, char *argv[])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(initialize)---------------------*/
   if (rc >= 0)  rc = yURG_logger  (argc, argv);
   if (rc >= 0)  rc = yURG_urgs    (argc, argv);
   if (rc >= 0)  rc = PROG_init    ();
   if (rc >= 0)  rc = PROG_args    (argc, argv);
   if (rc >= 0)  rc = PROG_begin   ();
   if (rc >= 0)  rc = PROG_final   ();
   --rce;  if (rc <  0) {
      DEBUG_DATA   yLOG_exitr    (__FUNCTION__, rce);
      PROG_end    ();
      return rce;
   }
   format_change  (my.format);
   /*---(main loop)----------------------*/
   /*> rc = yVIKEYS_main ("500us", "10ms", NULL);                                     <*/
   rc = yVIKEYS_main ("keys", "every", NULL);
   /*---(wrap-up)------------------------*/
   PROG_wrap   ();
   DEBUG_DATA   yLOG_exit     (__FUNCTION__);
   PROG_end    ();
   /*---(complete)-----------------------*/
   return rc;
}
