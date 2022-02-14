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
   if (rc >= 0)  rc = PROG_urgents (argc, argv);
   if (rc >= 0)  rc = PROG_startup (argc, argv);
   /*> if (rc >= 0)  rc = PROG__init    ();                                            <* 
    *> if (rc >= 0)  rc = PROG__args    (argc, argv);                                  <* 
    *> if (rc >= 0)  rc = PROG__begin   ();                                            <*/
   --rce;  if (rc <  0) {
      DEBUG_DATA   yLOG_exitr    (__FUNCTION__, rce);
      PROG_shutdown ();
      return rce;
   }
   /*---(main loop)----------------------*/
   rc = PROG_dispatch ();
   /*> rc = PROG_dawn    ();                                                          <* 
    *> if (rc >= 0)  rc = yVIOPENGL_main  ("keys", "every", NULL);                    <* 
    *> rc = PROG_dusk   ();                                                           <*/
   /*---(wrap-up)------------------------*/
   DEBUG_DATA   yLOG_exit     (__FUNCTION__);
   PROG_shutdown ();
   /*---(complete)-----------------------*/
   return rc;
}
