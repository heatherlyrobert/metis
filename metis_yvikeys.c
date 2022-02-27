#include   "metis.h"


/*
 * metis � dn2#� � get positioning working with wmctrl again                              � M1FDii �
 * metis � dv2�� � get yVIOPENGL to change colors when editing text                       � M1Q5lC �
 *
 */



char
api_yvikeys__resize     (char a_startup)
{
   /*---(locals)-----------+-----+-----+-*/
   char        x_cmd       [LEN_RECD];
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*---(size main)----------------------*/
   if (a_startup == 'y') {
      /*> yVIKEYS_view_config   ("metis_tasklist", P_VERNUM, YVIEW_OPENGL, my.w_wide, my.w_tall, 0);   <*/
      yX11_screensize (&my.s_wide, &my.s_tall, NULL);
      DEBUG_GRAF   yLOG_complex ("screen"    , "%4dw, %4dt", my.s_wide, my.s_tall);
      sleep (1);
   } else {
      yVIEW_resize   (my.w_wide, my.w_tall, 0);
   }
   /*---(size parts)---------------------*/
   /*> yVIKEYS_view_setup    (YVIKEYS_FLOAT, YVIKEYS_FLAT, YVIKEYS_TOPCEN,  10, 280      , -35.0 - my.m_offset, 20       , 0, 0, 0, NULL);   <*/
   /*> yVIKEYS_view_setup    (YVIKEYS_MENUS, YVIKEYS_FLAT, YVIKEYS_TOPCEN,   0,   0      , 0                  , 0        , 0, 0, 0, NULL);   <*/
   /*---(call placement)-----------------*/
   DEBUG_GRAF   yLOG_complex ("placement" , "%4dl, %4dt", my.w_left, my.w_topp);
   sprintf (x_cmd, "wmctrl -r 'metis-okeanides' -e 0,%d,%d,-1,-1", my.w_left, my.w_topp);
   system  (x_cmd);
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
api_yvikeys_init        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   int         x_wide, x_tall;
   char        x_cmd       [LEN_RECD];
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter   (__FUNCTION__);
   /*---(window)-------------------------*/
   api_yvikeys__resize   ('y');
   /*> yVIKEYS_cmds_direct   (":layout min");                                         <* 
    *> yVIKEYS_cmds_direct   (":status show");                                        <* 
    *> yVIKEYS_cmds_direct   (":modes show");                                         <* 
    *> yVIKEYS_cmds_direct   (":title disable");                                      <* 
    *> yVIKEYS_cmds_direct   (":version disable");                                    <* 
    *> yVIKEYS_cmds_direct   (":buffer disable");                                     <* 
    *> yVIKEYS_cmds_direct   (":formula disable");                                    <* 
    *> yVIKEYS_cmds_direct   (":nav disable");                                        <* 
    *> yVIKEYS_cmds_direct   (":alt disable");                                        <* 
    *> yVIKEYS_cmds_direct   (":progress disable");                                   <* 
    *> yVIKEYS_cmds_direct   (":keys disable");                                       <* 
    *> yVIKEYS_cmds_direct   (":command disable");                                    <* 
    *> yVIKEYS_cmds_direct   (":details disable");                                    <* 
    *> yVIKEYS_cmds_direct   (":ribbon disable");                                     <* 
    *> yVIKEYS_cmds_direct   (":grid disable");                                       <* 
    *> yVIKEYS_cmds_direct   (":cursor disable");                                     <* 
    *> yVIKEYS_cmds_direct   (":layers disable");                                     <* 
    *> yVIKEYS_cmds_direct   (":xaxis disable");                                      <* 
    *> yVIKEYS_cmds_direct   (":yaxis disable");                                      <* 
    *> yVIKEYS_view_font     (my.fixed);                                              <*/
   yMAP_config    (YMAP_OFFICE, api_ymap_locator, api_ymap_addressor, api_ymap_sizer, api_ymap_entry, api_ymap_placer, api_ymap_done);
   /*> yVIKEYS_cmds_direct   (":palette 100 rcomp pale earthy");                      <*/
   /*> yVIKEYS_view_colors   (YCOLOR_POS, YCOLOR_BAS, YCOLOR_NEG, YCOLOR_POS);        <*/
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     mapping for map mode                     ----===*/
/*====================------------------------------------====================*/
static void  o___MAPPING_________o () { return; }

/*> char                                                                              <* 
 *> api_yvikeys__unmap      (tMAPPED *a_map)                                          <* 
 *> {                                                                                 <* 
 *>    /+---(locals)-----------+-----------+-+/                                       <* 
 *>    int         i           =    0;                                                <* 
 *>    /+---(lefts)--------------------------+/                                       <* 
 *>    a_map->umin = a_map->gmin = a_map->gamin = a_map->glmin = a_map->gprev = 0;    <* 
 *>    /+---(map)----------------------------+/                                       <* 
 *>    for (i= 0; i < LEN_HUGE; ++i)  a_map->map [i] =  YVIKEYS_EMPTY;                <* 
 *>    /+---(rights)-------------------------+/                                       <* 
 *>    a_map->umax = a_map->gmax = a_map->gamax = a_map->glmax = a_map->gnext = 0;    <* 
 *>    /+---(screen)-------------------------+/                                       <* 
 *>    a_map->ubeg   = a_map->ucur   = a_map->uend   =  0;                            <* 
 *>    a_map->ulen   = a_map->utend  = a_map->uavail =  1;                            <* 
 *>    /+---(grids)--------------------------+/                                       <* 
 *>    a_map->gbeg   = a_map->gcur   = a_map->gend   = 0;                             <* 
 *>    /+---(complete)-----------------------+/                                       <* 
 *>    return  0;                                                                     <* 
 *> }                                                                                 <*/

/*> char                                                                                                  <* 
 *> api_yvikeys__linear        (char a_req, tMAPPED *a_map, int a_tot, int a_win)                         <* 
 *> {                                                                                                     <* 
 *>    /+---(locals)-----------+-----------+-+/                                                           <* 
 *>    int         x_slots     =    0;                                                                    <* 
 *>    int         c           =    0;                                                                    <* 
 *>    int         i           =    0;                                                                    <* 
 *>    int         x_max       =    0;                                                                    <* 
 *>    /+---(header)-------------------------+/                                                           <* 
 *>    DEBUG_MAP    yLOG_enter    (__FUNCTION__);                                                         <* 
 *>    /+---(adjust max)---------------------+/                                                           <* 
 *>    DEBUG_MAP    yLOG_value    ("g_ntask"   , g_ntask);                                                <* 
 *>    DEBUG_MAP    yLOG_value    ("my.nact"   , my.nact);                                                <* 
 *>    DEBUG_MAP    yLOG_value    ("a_tot"     , a_tot);                                                  <* 
 *>    x_slots = a_tot;                                                                                   <* 
 *>    if (x_slots >= my.nact)  x_slots = my.nact;                                                        <* 
 *>    DEBUG_MAP    yLOG_value    ("x_slots"   , x_slots);                                                <* 
 *>    /+---(clear map)----------------------+/                                                           <* 
 *>    for (i = 0; i < LEN_HUGE; ++i) a_map->map [i] = 0;                                                 <* 
 *>    /+---(load map)-----------------------+/                                                           <* 
 *>    for (i = 0; i < g_ntask; ++i) {                                                                    <* 
 *>       /+---(filter)-------------------+/                                                              <* 
 *>       if (c >= x_slots)            break;                                                             <* 
 *>       DEBUG_MAP    yLOG_complex  ("card"      , "%2d, %c %s", i, g_tasks [i].act, g_tasks [i].txt);   <* 
 *>       if (g_tasks [i].act != 'y')  continue;                                                          <* 
 *>       /+---(map it)-------------------+/                                                              <* 
 *>       DEBUG_MAP    yLOG_complex  ("map"       , "put %2d into slot %2d", i, c);                       <* 
 *>       x_max = a_map->map [c] = i;                                                                     <* 
 *>       c++;                                                                                            <* 
 *>       /+---(done)---------------------+/                                                              <* 
 *>    }                                                                                                  <* 
 *>    /+---(unit min/max)-------------------+/                                                           <* 
 *>    a_map->umin  = 0;                                                                                  <* 
 *>    DEBUG_MAP    yLOG_value    ("umin"      , a_map->umin);                                            <* 
 *>    a_map->umax  = x_slots - 1;                                                                        <* 
 *>    DEBUG_MAP    yLOG_value    ("umax"      , a_map->umax);                                            <* 
 *>    /+---(grid mins)----------------------+/                                                           <* 
 *>    a_map->gmin   = a_map->map [a_map->umin];                                                          <* 
 *>    a_map->gamin  = a_map->map [a_map->umin];                                                          <* 
 *>    a_map->glmin  = a_map->map [a_map->umin];                                                          <* 
 *>    a_map->gprev  = a_map->map [a_map->umin];                                                          <* 
 *>    DEBUG_MAP    yLOG_value    ("gmin"      , a_map->gmin);                                            <* 
 *>    /+---(grid maxs)----------------------+/                                                           <* 
 *>    a_map->gnext  = a_map->map [a_map->umax];                                                          <* 
 *>    a_map->glmax  = a_map->map [a_map->umax];                                                          <* 
 *>    a_map->gamax  = a_map->map [a_map->umax];                                                          <* 
 *>    a_map->gmax   = a_map->map [a_map->umax];                                                          <* 
 *>    DEBUG_MAP    yLOG_value    ("gmax"      , a_map->gmax);                                            <* 
 *>    /+---(indexes)------------------------+/                                                           <* 
 *>    DEBUG_MAP   yLOG_snote   ("screen");                                                               <* 
 *>    if (a_req == YVIKEYS_INIT) {                                                                       <* 
 *>       a_map->ubeg   =  0;                                                                             <* 
 *>       a_map->ucur   =  0;                                                                             <* 
 *>       a_map->uend   = my.nact - 1;                                                                    <* 
 *>       if (a_map->uend > a_win - 1)  a_map->uend = a_win - 1;                                          <* 
 *>       a_map->ulen   = a_win;                                                                          <* 
 *>       a_map->uavail = a_win;                                                                          <* 
 *>       a_map->utend  = a_win - 1;                                                                      <* 
 *>       a_map->utend  = a_map->uend;                                                                    <* 
 *>    }                                                                                                  <* 
 *>    /+---(grids)--------------------------+/                                                           <* 
 *>    DEBUG_MAP   yLOG_snote   ("grid");                                                                 <* 
 *>    if (a_req == YVIKEYS_INIT) {                                                                       <* 
 *>       a_map->gbeg   = a_map->map [a_map->ubeg];                                                       <* 
 *>       a_map->gcur   = a_map->gbeg;                                                                    <* 
 *>       a_map->gend   = a_map->map [a_map->uend];                                                       <* 
 *>    }                                                                                                  <* 
 *>    DEBUG_MAP    yLOG_value    ("gbeg"      , a_map->gbeg);                                            <* 
 *>    DEBUG_MAP    yLOG_value    ("gcur"      , a_map->gcur);                                            <* 
 *>    DEBUG_MAP    yLOG_value    ("gend"      , a_map->gend);                                            <* 
 *>    /+---(complete)-----------------------+/                                                           <* 
 *>    DEBUG_MAP    yLOG_exit     (__FUNCTION__);                                                         <* 
 *>    return  0;                                                                                         <* 
 *> }                                                                                                     <*/

char
api_yvikeys_mapper      (char a_req)
{
   /*---(header)-------------------------*/
   DEBUG_MAP    yLOG_enter   (__FUNCTION__);
   /*> yVIKEYS_view_size     (YVIKEYS_MAIN, NULL, &g_xmap.avail, NULL, &g_ymap.avail, NULL);   <*/
   /*> BCOL = g_xmap.ubeg;                                                         <* 
    *> CCOL = g_xmap.ucur;                                                         <* 
    *> ECOL = g_xmap.uend;                                                         <* 
    *> BROW = g_ymap.ubeg;                                                         <* 
    *> CROW = g_ymap.ucur;                                                         <* 
    *> EROW = g_ymap.uend;                                                         <* 
    *> api_yvikeys__unmap (&g_bmap);                                                  <* 
    *> if (strchr (FORMAT_TICKERS, my.format) != NULL) {                              <* 
    *>    api_yvikeys__linear (a_req, &g_xmap, NCOLS, WCOLS);                   <* 
    *>    api_yvikeys__unmap  (&g_ymap);                                              <* 
    *> } else if (strchr (FORMAT_COLUMNS, my.format) != NULL) {                       <* 
    *>    api_yvikeys__unmap  (&g_xmap);                                              <* 
    *>    api_yvikeys__linear (a_req, &g_ymap, NROWS, WROWS);                   <* 
    *> } else {                                                                       <* 
    *>    api_yvikeys__linear (a_req, &g_xmap, NCOLS, WCOLS);                   <* 
    *>    api_yvikeys__linear (a_req, &g_ymap, NROWS, WROWS);                   <* 
    *> }                                                                              <* 
    *> api_yvikeys__unmap (&g_zmap);                                                  <*/
   /*---(complete)-----------------------*/
   DEBUG_MAP    yLOG_exit    (__FUNCTION__);
   return 0;
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
   if      (strcmp (a_how, "clear"   ) == 0)   my.sort  = METIS_ORIG;
   else if (strcmp (a_how, "urg"     ) == 0)   my.sort  = METIS_URG;
   else if (strcmp (a_how, "imp"     ) == 0)   my.sort  = METIS_IMP;
   else if (strcmp (a_how, "est"     ) == 0)   my.sort  = METIS_EST;
   else if (strcmp (a_how, "prg"     ) == 0)   my.sort  = METIS_PRG;
   else if (strcmp (a_how, "shr"     ) == 0)   my.sort  = METIS_SHR;
   else if (strcmp (a_how, "orig"    ) == 0)   my.sort  = METIS_ORIG;
   else if (strcmp (a_how, "date"    ) == 0)   my.sort  = METIS_DATE;
   else                                        return -2;
   /*> metis_data_refresh   ();                                                             <*/
   metis_filter_sort  ();
   metis_filter_set   ();
   yMAP_refresh_full ();
   metis_opengl_draw ();
   metis_opengl_mask();
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
   else if (strcmp (a_which, "prg"     ) == 0)   my.cprg  = a_string [0];
   else if (strcmp (a_which, "shr"     ) == 0)   my.cshr  = a_string [0];
   else if (strcmp (a_which, "regex"   ) == 0)   strlcpy (my.ctxt, a_string, LEN_HUND);
   else if (strcmp (a_which, "clear"   ) == 0)   metis_filter_clear ();
   else                                        return -3;
   /*> metis_data_refresh   ();                                                             <*/
   metis_filter_sort ();
   metis_filter_set ();
   yMAP_refresh_full ();
   metis_opengl_draw ();
   metis_opengl_mask();
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
api_yvikeys_window      (char *a_format)
{
   static      x_save      = '-';
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   if (a_format == NULL) return -1;
   /*---(change format)------------------*/
   if      (strcmp (a_format, "col_rig" ) == 0)  my.format = FORMAT_RSHORT;
   else if (strcmp (a_format, "col_lef" ) == 0)  my.format = FORMAT_LSHORT;
   else if (strcmp (a_format, "long_rig") == 0)  my.format = FORMAT_RLONG;
   else if (strcmp (a_format, "long_lef") == 0)  my.format = FORMAT_LLONG;
   else if (strcmp (a_format, "streamer") == 0)  my.format = FORMAT_STREAMER;
   else if (strcmp (a_format, "ticker"  ) == 0)  my.format = FORMAT_TICKER;
   else if (strcmp (a_format, "baseline") == 0)  my.format = FORMAT_BASELINE;
   else if (strcmp (a_format, "project" ) == 0)  my.format = FORMAT_PROJECT;
   else if (strcmp (a_format, "wide"    ) == 0)  my.format = FORMAT_WIDE;
   else if (strcmp (a_format, "extra"   ) == 0)  my.format = FORMAT_EXTRA;
   /*---(change attributes)--------------*/
   if (strcmp (a_format, "sticky"  ) == 0) {
      system ("wmctrl -r 'metis_tasklist' -b add,sticky");
      return 0;
   }
   else if (strcmp (a_format, "nonstick") == 0) {
      system ("wmctrl -r 'metis_tasklist' -b remove,sticky");
      return 0;
   }
   if (my.format == x_save) {
      DEBUG_DATA   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(update)-------------------------*/
   metis_filter_sort ();
   metis_filter_set ();
   x_save = my.format;
   FORMAT_refresh ();
   api_yvikeys__resize ('-');
   yMAP_refresh_full ();
   metis_opengl_draw ();
   metis_opengl_mask();
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
api_yvikeys_refresh     (void)
{
   DEBUG_DATA   yLOG_enter   (__FUNCTION__);
   metis_data_refresh   ();
   metis_filter_sort ();
   metis_filter_set ();
   FORMAT_refresh ();
   api_yvikeys__resize ('-');
   yMAP_refresh_full ();
   metis_opengl_draw    ();
   metis_opengl_mask    ();
   DEBUG_DATA   yLOG_exit    (__FUNCTION__);
   return 0;
}


