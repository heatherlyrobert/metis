#include   "metis.h"


int
main               (int argc, char *argv[])
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(initialize)---------------------*/
   if (rc >= 0)  rc = yURG_logger  (argc, argv);
   if (rc >= 0)  rc = yURG_urgs    (argc, argv);
   if (rc >= 0)  rc = PROG_init    ();
   if (rc >= 0)  rc = PROG_args    (argc, argv);
   if (rc >= 0)  rc = PROG_begin   ();
   if (rc >= 0)  rc = PROG_final   ();
   if (rc <  0) {
      PROG_end ();
      return -1;
   }
   task_list      ();
   format_change  ('c');
   /*> task_structure ();                                                             <*/
   /*> return 0;                                                                      <*/
   /*---(main loop)----------------------*/
   while (rc >= 0 && rc != 1) {
      rc = prog_event ();
      printf ("format change requested\n");
      if (rc >  1) format_change (rc);
   }
   /*---(wrap-up)------------------------*/
   PROG_wrap   ();
   PROG_end    ();
   /*---(complete)-----------------------*/
   return rc;
}
