#include   "metis.h"


/*===[[ METIS BACKLOG ]]======================================================*
 * metis  tn2#�  add yvikeys setup logic, but not drawing control (yet)
 * metis  tn2#�  cause yvikeys to call back metis to draw main (gonna be ugly)
 * metis  tn1#�  force the floating command window to display at specific location
 * metis  dn2#�  setup the mapper so movement keys work realistically
 * metis  dn1#�  use colored hatching on black diagonal to show current task
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



/*====================------------------------------------====================*/
/*===----                     mapping for map mode                     ----===*/
/*====================------------------------------------====================*/
static void  o___MAPPING_________o () { return; }

char
api_yvikeys__unmap      (tMAPPED *a_map)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =    0;
   /*---(lefts)--------------------------*/
   a_map->umin = a_map->gmin = a_map->gamin = a_map->glmin = a_map->gprev = 0;
   /*---(map)----------------------------*/
   for (i= 0; i < LEN_HUGE; ++i)  a_map->map [i] =  YVIKEYS_EMPTY;
   /*---(rights)-------------------------*/
   a_map->umax = a_map->gmax = a_map->gamax = a_map->glmax = a_map->gnext = 0;
   /*---(screen)-------------------------*/
   a_map->ubeg   = a_map->ucur   = a_map->uend   =  0;
   a_map->ulen   = a_map->utend  = a_map->uavail =  1;
   /*---(grids)--------------------------*/
   a_map->gbeg   = a_map->gcur   = a_map->gend   = 0;
   /*---(complete)-----------------------*/
   return  0;
}

char
api_yvikeys__mapy          (char a_req)
{
   /*---(locals)-----------+-----------+-*/
   int         x_slots     =    0;
   int         c           =    0;
   int         i           =    0;
   int         x_max       =    0;
   /*---(header)-------------------------*/
   DEBUG_MAP    yLOG_enter    (__FUNCTION__);
   /*---(adjust max)---------------------*/
   DEBUG_MAP    yLOG_value    ("g_ntask"   , g_ntask);
   DEBUG_MAP    yLOG_value    ("my.nact"   , my.nact);
   DEBUG_MAP    yLOG_value    ("my.trows"  , my.trows);
   x_slots = my.trows;
   if (x_slots >= my.nact)  x_slots = my.nact;
   DEBUG_MAP    yLOG_value    ("x_slots"   , x_slots);
   /*---(clear map)----------------------*/
   for (i = 0; i < LEN_HUGE; ++i) g_ymap.map [i] = 0;
   /*---(load map)-----------------------*/
   for (i = 0; i < g_ntask; ++i) {
      /*---(filter)-------------------*/
      if (c >= x_slots)            break;
      DEBUG_MAP    yLOG_complex  ("card"      , "%2d, %c %s", i, g_tasks [i].act, g_tasks [i].txt);
      if (g_tasks [i].act != 'y')  continue;
      /*---(map it)-------------------*/
      DEBUG_DATA   yLOG_complex  ("map"       , "put %2d into slot %2d", i, c);
      x_max = g_ymap.map [c] = i;
      c++;
      /*---(done)---------------------*/
   }
   /*---(unit min/max)-------------------*/
   g_ymap.umin  = 0;
   DEBUG_MAP    yLOG_value    ("umin"      , g_ymap.umin);
   g_ymap.umax  = x_slots - 1;
   DEBUG_MAP    yLOG_value    ("umax"      , g_ymap.umax);
   /*---(grid mins)----------------------*/
   g_ymap.gmin   = g_ymap.map [g_ymap.umin];
   g_ymap.gamin  = g_ymap.map [g_ymap.umin];
   g_ymap.glmin  = g_ymap.map [g_ymap.umin];
   g_ymap.gprev  = g_ymap.map [g_ymap.umin];
   DEBUG_MAP    yLOG_value    ("gmin"      , g_ymap.gmin);
   /*---(grid maxs)----------------------*/
   g_ymap.gnext  = g_ymap.map [g_ymap.umax];
   g_ymap.glmax  = g_ymap.map [g_ymap.umax];
   g_ymap.gamax  = g_ymap.map [g_ymap.umax];
   g_ymap.gmax   = g_ymap.map [g_ymap.umax];
   DEBUG_MAP    yLOG_value    ("gmax"      , g_ymap.gmax);
   /*---(indexes)------------------------*/
   DEBUG_MAP   yLOG_snote   ("screen");
   if (a_req == YVIKEYS_INIT) {
      g_ymap.ubeg   =  0;
      g_ymap.ucur   =  0;
      g_ymap.uend   = my.nact - 1;
      if (g_ymap.uend > 11)  g_ymap.uend = 11;
      g_ymap.ulen   = 12;
      g_ymap.uavail = 12;
      g_ymap.utend  = 11;
      g_ymap.utend  = g_ymap.uend;
   }
   /*---(grids)--------------------------*/
   DEBUG_MAP   yLOG_snote   ("grid");
   if (a_req == YVIKEYS_INIT) {
      g_ymap.gbeg   = g_ymap.map [g_ymap.ubeg];
      g_ymap.gcur   = g_ymap.gbeg;
      g_ymap.gend   = g_ymap.map [g_ymap.uend];
   }
   DEBUG_MAP    yLOG_value    ("gbeg"      , g_ymap.gbeg);
   DEBUG_MAP    yLOG_value    ("gcur"      , g_ymap.gcur);
   DEBUG_MAP    yLOG_value    ("gend"      , g_ymap.gend);
   /*---(complete)-----------------------*/
   DEBUG_MAP    yLOG_exit     (__FUNCTION__);
   return  0;
}

char
api_yvikeys_mapper      (char a_req)
{
   /*---(header)-------------------------*/
   DEBUG_MAP    yLOG_enter   (__FUNCTION__);
   /*> yVIKEYS_view_size     (YVIKEYS_MAIN, NULL, &g_xmap.avail, NULL, &g_ymap.avail, NULL);   <*/
   my.bcol = g_xmap.ubeg;
   my.ccol = g_xmap.ucur;
   my.ecol = g_xmap.uend;
   my.brow = g_ymap.ubeg;
   my.crow = g_ymap.ucur;
   my.erow = g_ymap.uend;
   api_yvikeys__unmap (&g_bmap);
   api_yvikeys__unmap (&g_xmap);
   api_yvikeys__mapy  (a_req);
   api_yvikeys__unmap (&g_zmap);
   /*---(complete)-----------------------*/
   DEBUG_MAP    yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*-> break label into coordinates -------[ ------ [gc.722.112.13]*/ /*-[01.0000.304.#]-*/ /*-[--.---.---.--]-*/
api_yvikeys_locator     (char *a_label, int *a_buf, int *a_x, int *a_y, int *a_z)
{
   return -1;  /* -1 means not recognized as an address */
}

char         /*-> return address for coordinates -----[ ------ [gc.722.112.13]*/ /*-[01.0000.304.#]-*/ /*-[--.---.---.--]-*/
api_yvikeys_addressor   (char *a_label, int a_buf, int a_x, int a_y, int a_z)
{
   return -1;  /* -1 means not recognized as an address */
}



/*====================------------------------------------====================*/
/*===----                     mapping for map mode                     ----===*/
/*====================------------------------------------====================*/
static void  o___COMMANDS________o () { return; }

char
api_yvikeys_sort        (char *a_how)
{
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   if (a_how == NULL) return -1;
   if      (strcmp (a_how, "clear"   ) == 0) { my.sort  = 'o'; my.order = 'a'; }
   else if (strcmp (a_how, "urg"     ) == 0)   my.sort  = 'u';
   else if (strcmp (a_how, "imp"     ) == 0)   my.sort  = 'i';
   else if (strcmp (a_how, "est"     ) == 0)   my.sort  = 'e';
   else if (strcmp (a_how, "flg"     ) == 0)   my.sort  = 'f';
   else if (strcmp (a_how, "names"   ) == 0)   my.sort  = 'n';
   else if (strcmp (a_how, "ascend"  ) == 0)   my.order = 'a';
   else if (strcmp (a_how, "descend" ) == 0)   my.order = 'd';
   else                                        return -2;
   DATA_refresh   ();
   SORT_refresh   ();
   FILTER_refresh ();
   api_yvikeys_mapper (YVIKEYS_INIT);
   OPENGL_draw ();
   OPENGL_mask();
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
api_yvikeys_filter      (char *a_which, char *a_string)
{
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   if (a_which  == NULL) return -1;
   if (a_string == NULL) return -2;
   if      (strcmp (a_which, "urg"     ) == 0)   my.curg  = a_string [0];
   else if (strcmp (a_which, "imp"     ) == 0)   my.cimp  = a_string [0];
   else if (strcmp (a_which, "est"     ) == 0)   my.cest  = a_string [0];
   else if (strcmp (a_which, "flg"     ) == 0)   my.cflg  = a_string [0];
   else if (strcmp (a_which, "one"     ) == 0)   strlcpy (my.cone, a_string, LEN_LABEL);
   else if (strcmp (a_which, "two"     ) == 0)   strlcpy (my.ctwo, a_string, LEN_LABEL);
   else if (strcmp (a_which, "txt"     ) == 0)   strlcpy (my.ctxt, a_string, LEN_HUND);
   else if (strcmp (a_which, "clear"   ) == 0)   FILTER_clear ();
   else                                        return -3;
   DATA_refresh   ();
   SORT_refresh   ();
   FILTER_refresh ();
   api_yvikeys_mapper (YVIKEYS_INIT);
   OPENGL_draw ();
   OPENGL_mask();
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
api_yvikeys_window      (char *a_format)
{
   char        x_cmd       [LEN_RECD];
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   if (a_format == NULL) return -1;
   /*---(change format)-------------------------*/
   if      (strcmp (a_format, "col_rig" ) == 0)  format_column ('r');
   else if (strcmp (a_format, "long_rig") == 0)  format_column ('R');
   else if (strcmp (a_format, "col_lef" ) == 0)  format_column ('l');
   else if (strcmp (a_format, "long_lef") == 0)  format_column ('L');
   else if (strcmp (a_format, "ticker"  ) == 0)  format_ticker ('t');
   else if (strcmp (a_format, "baseline") == 0)  format_ticker ('b');
   else if (strcmp (a_format, "project" ) == 0)  format_projects ();
   else if (strcmp (a_format, "wide"    ) == 0)  format_wideview ();
   else if (strcmp (a_format, "extra"   ) == 0)  format_extra    ();
   else if (strcmp (a_format, "sticky"  ) == 0) {
      system ("wmctrl -r 'metis_tasklist' -b add,sticky");
      return 0;
   }
   else if (strcmp (a_format, "nonstick") == 0) {
      system ("wmctrl -r 'metis_tasklist' -b remove,sticky");
      return 0;
   }
   else                                       return -2;
   sprintf (x_cmd, "wmctrl -r 'metis_tasklist' -e 0,%d,%d,%d,%d", win_x, win_y, win_w, win_h);
   system (x_cmd);
   OPENGL_draw ();
   OPENGL_mask();
   /*---(complete)------------------------------*/
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return 0;
}


