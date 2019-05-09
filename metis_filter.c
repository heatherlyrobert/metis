/*============================----beg-of-source---============================*/
/*===[[ HEADER ]]=============================================================*

 *   focus         : (MH) mind_hacking
 *   niche         : (ta) task_mgmt
 *   application   : metis       (titan goddess of cunning and wise counsel)
 *   purpose       : simple, light, clean, and powerful task management system
 *
 *   module        : metis_filter
 *   purpose       : segregate data filtering from other program elements
 * 
 */
/*============================================================================*/

#include   "metis.h"



char             /* [------] perform global filtering on tasks ---------------*/
filter_primary     (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;
   /*---(prepare)------------------------*/
   nactive = 0;
   /*---(display)------------------------*/
   for (i = 0; i < g_ntask; ++i) {
      /*---(default)---------------------*/
      g_tasks [i].act  = '-';
      if (nactive >= max_disp)  continue;
      /*> printf ("max_disp = %3d, nactive = %3d\n", max_disp, nactive);              <*/
      g_tasks [i].act  = 'y';
      ++nactive;
      /*---(headers)---------------------*/
      if (g_tasks [i].flg == 'h') {
         printf ("header\n");
         if (!arg_heads) {
            g_tasks [i].act  = '-';
            --nactive;
         }
         continue;
      }
      /*---(flags)-----------------------*/
      if (arg_filter) {
         if (  (urg  != ' ' && g_tasks [i].urg != urg)) {
            g_tasks [i].act = '-';
            --nactive;
            continue;
         }
         if (  (imp  != ' ' && g_tasks [i].imp != imp)) {
            g_tasks [i].act = '-';
            --nactive;
            continue;
         }
         if (  (est  != ' ' && g_tasks [i].est != est)) {
            g_tasks [i].act = '-';
            --nactive;
            continue;
         }
         if (  (flg  != ' ' && g_tasks [i].flg != flg)) {
            g_tasks [i].act = '-';
            --nactive;
            continue;
         }
      }
      /*---(categories)------------------*/
      /*> if (arg_filter && one[0] != '\0') {                                         <* 
       *>    if (  (strstr (g_tasks [i].one, one) == NULL) &&                           <* 
       *>          (strstr (g_tasks [i].two, one) == NULL)) {                           <* 
       *>       g_tasks [i].act = '-';                                                  <* 
       *>       --nactive;                                                            <* 
       *>       continue;                                                             <* 
       *>    }                                                                        <* 
       *> }                                                                           <*/
   }
   /*---(complete)-----------------------*/
   return 0;
}


/*============================----end-of-source---============================*/
