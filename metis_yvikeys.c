#include   "metis.h"


/*===[[ METIS BACKLOG ]]======================================================*
 * metis  tn2#·  add yvikeys setup logic, but not drawing control (yet)
 * metis  tn2#·  cause yvikeys to call back metis to draw main (gonna be ugly)
 * metis  tn1··  force the floating command window to display at specific location
 * metis  dn2··  setup the mapper so movement keys work realistically
 * metis  dn1··  use colored hatching on black diagonal to show current task
 *
 *
 */


char
api_ykikeys_init        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   int         x_wide, x_tall;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*---(window)-------------------------*/
   /*> yVIKEYS_view_config   ("metis_column", VER_NUM, YVIKEYS_OPENGL, 300, 60 * 12, 0);                                                 <* 
    *> yVIKEYS_view_setup    (YVIKEYS_MAIN     , YVIKEYS_FLAT, YVIKEYS_TOPLEF, 0, 0, 300, 60 * 12, 0, 0, YCOLOR_BAS    , OPENGL_show);   <* 
    *> yVIKEYS_cmds_direct   (":layout minimal");                                                                                        <*/
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}
