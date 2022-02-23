/*============================----beg-of-source---============================*/
/*===[[ HEADER ]]=============================================================*

 *   focus         : (MH) mind_hacking
 *   niche         : (ta) task_mgmt
 *   application   : metis       (titan goddess of cunning and wise counsel)
 *   purpose       : simple, light, clean, and powerful task management system
 *
 *   module        : metis_opengl
 *   purpose       : segregate opengl code from other program elements
 * 
 */
/*============================================================================*/



#include   "metis.h"





/*
 * metis § dw4#Ï § fix artifacting in OPENGL__polygon which looks like dashes at edges    § M1FDip §
 * metis § dw2xÏ § fit urgency letter in urgency area -- less confusing                   § M1FDiq §
 * metis § dw2xÏ § fit estimate letter in estimate area -- less confusing                 § M1FDir §
 * metis § dw2#Ï § fit progress letter in progress area -- less confusing                 § M1FDis §
 * metis § sn4#Ï § get texture drawn to match yMAP position                               § M1FDit §
 * metis § ww1#Ï § lighten normal blue as it dulls text too much                          § M1FDiu §
 *
 * metis § dw2·· § when filtered to zero cards, display blank card                        § M1FDiv §
 * metis § dw4·· § use yCOLOR to establish pallete and color selection                    § M1FDiw §
 * metis § wv4·· § continuation header-footer that show top/more and bot/more             § M1K24Q §
 */

float     g_alpha     = 0.0;
float     step      = 0;


/*> char   face_pretty [30] = "verdana_sm";                                               <*/
/*> char   face_pretty [30] = "comfortaa";                                                <*/
char   face_pretty [30] = "sansation";
/*> char   face_fixed  [30] = "comfortaa";                                            <*/
/*> char   face_fixed [30] = "courier";                                               <*/
char   face_fixed [30] = "hack";
float  txf_off   = 0.00;
float  txf_space = 1.15;








/*====================------------------------------------====================*/
/*===----                        drawing primative                     ----===*/
/*====================------------------------------------====================*/
static void      o___PRIMATIVE_______________o (void) {;}

char
metis_opengl_color      (char *a_valid, char a_color, float a_alpha)
{
   char       *p           = NULL;
   char        i           =    0;
   DEBUG_GRAF   yLOG_info     ("a_valid"    , a_valid);
   DEBUG_GRAF   yLOG_char     ("a_color"    , a_color);
   if      (a_color == 0  ) i = 100;
   else if (a_color == '·') i = 100;
   else {
      p = strchr (a_valid, a_color);
      if (p == NULL)    i = 100;
      else              i = p - a_valid;
   }
   DEBUG_GRAF   yLOG_value    ("i"          , i);
   switch (i) {
   case  0  : glColor4f (  0.800,  0.100,  0.100, a_alpha); break;
   case  1  : glColor4f (  0.800,  0.400,  0.000, a_alpha); break;
   case  2  : glColor4f (  0.800,  0.800,  0.000, a_alpha); break;
   case  3  : glColor4f (  0.000,  0.800,  0.000, a_alpha); break;
   case  4  : glColor4f (  0.000,  0.800,  0.800, a_alpha); break;
   case  5  : glColor4f (  0.300,  0.300,  1.000, a_alpha); break;
   case  6  : glColor4f (  0.800,  0.000,  0.800, a_alpha); break;
   case  7  : glColor4f (  0.700,  0.700,  0.700, a_alpha); break;
   default  : glColor4f (  0.700,  0.700,  0.700, a_alpha); break;
   }
   return 0;
}



/*====================------------------------------------====================*/
/*===----                             fonts                            ----===*/
/*====================------------------------------------====================*/
static void      o___FONTS___________________o (void) {;}

char
metis_opengl_font_load  (void)
{
   DEBUG_GRAF   yLOG_senter   (__FUNCTION__);
   my.fixed  = yFONT_load(face_fixed);
   DEBUG_GRAF   yLOG_sint     (my.fixed);
   if (my.fixed <  0) {
      fprintf(stderr, "Problem loading %s\n", face_fixed);
      exit(1);
   }
   my.pretty  = yFONT_load(face_pretty);
   DEBUG_GRAF   yLOG_sint     (my.pretty);
   if (my.pretty <  0) {
      fprintf(stderr, "Problem loading %s\n", face_pretty);
      exit(1);
   }
   DEBUG_GRAF   yLOG_sexit    (__FUNCTION__);
   return 0;
}

char
metis_opengl_font_close (void)
{
   DEBUG_GRAF   yLOG_senter   (__FUNCTION__);
   yFONT_free (my.fixed);
   my.fixed  = -1;
   DEBUG_GRAF   yLOG_sint     (my.fixed);
   yFONT_free (my.pretty);
   my.pretty = -1;
   DEBUG_GRAF   yLOG_sint     (my.pretty);
   DEBUG_GRAF   yLOG_sexit    (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                           program-level                      ----===*/
/*============================--------------------============================*/
static void      o___PROGRAM_________________o (void) {;}

char
metis_opengl_init       (void)
{
   int         i           =    0;
   int         j           =    0;
   int         f           =  '-';
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   my.g_tex     =    -1;
   my.g_fbo     =    -1;
   my.g_dep     =    -1;
   yGLTEX_config ();
   /*> glClearColor(0.0f, 0.0f, 0.0f, 1.0f);                                          <*/
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
metis_opengl_wrap       (void)
{
   return 0;
}

char
OPENGL_resize           (uint a_w, uint a_h)
{
   if (a_h == 0) a_h = 1;
   /*> glViewport(0,  0, my.w_wide, my.w_tall);                                       <* 
    *> glMatrixMode(GL_PROJECTION);                                                   <* 
    *> glLoadIdentity();                                                              <* 
    *> glOrtho(        0,   my.w_wide, -my.w_tall,  0, -500.0,  500.0);               <* 
    *> glMatrixMode(GL_MODELVIEW);                                                    <*/
   return 0;
}



/*============================--------------------============================*/
/*===----                             show                             ----===*/
/*============================--------------------============================*/
static void      o___SHOW____________________o (void) {;}

char
metis_opengl__panel     (float a_wtop, float a_wlef, float a_wbot, float a_wrig, float a_ttop, float a_tlef, float a_tbot, float a_trig)
{
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   /*---(display)------------------------*/
   DEBUG_GRAF   yLOG_complex  ("window"    , "%8.3ft, %8.3fl, %8.3fr, %8.3ft, %8.3fb", my.w_tall, a_wlef, a_wrig, a_wtop, a_wbot);
   DEBUG_GRAF   yLOG_complex  ("texture"   , "%8.3ft, %8.3fl, %8.3fr, %8.3ft, %8.3fb", my.t_tall, a_tlef, a_trig, a_ttop, a_tbot);
   /*---(draw it)------------------------*/
   glBindTexture   (GL_TEXTURE_2D, my.g_tex);
   glBegin(GL_POLYGON); {
      /*---(top/lef)--------*/
      glTexCoord2f ( a_tlef,  a_ttop);
      glVertex3f   ( a_wlef,  a_wtop,  -10.0);
      /*---(top/rig)--------*/
      glTexCoord2f ( a_trig,  a_ttop);
      glVertex3f   ( a_wrig,  a_wtop,  -10.0);
      /*---(bot/rig)--------*/
      glTexCoord2f ( a_trig,  a_tbot);
      glVertex3f   ( a_wrig,  a_wbot,  -10.0);
      /*---(bot/lef)--------*/
      glTexCoord2f ( a_tlef,  a_tbot);
      glVertex3f   ( a_wlef,  a_wbot,  -10.0);
      /*---(done)-----------*/
   } glEnd();
   glBindTexture   (GL_TEXTURE_2D, 0);
   /*---(complete)-------------------------*/
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}

char          /*--> display tickers ------------------------------------------*/
metis_opengl__tickers   (float *a_xcur, float *a_ycur)
{
   /*---(locals)-----------+-----+-----+-*/
   float     x_wlef, x_wrig, x_wtop, x_wbot;
   float     x_tlef, x_trig, x_ttop, x_tbot;
   float     x_max , x_cnt;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_note     ("ticker formats--------------");
   /*---(consistent)---------------------*/
   x_wtop  = -my.m_offset;
   x_wbot  = x_wtop - my.r_tall;
   x_ttop  = 1.0;
   x_tbot  = 0.0;
   /*---(panel one/lef)---------------*/
   x_wlef  = 0.0;
   x_tlef  = (BCOL  * my.c_offset) / (float) my.t_wide;
   if (NCOLS - CCOL > WCOLS) {
      x_max  = BCOL + (WCOLS + my.c_over);
      x_cnt  = WCOLS + my.c_over;
      x_wrig = my.w_wide;
   } else {
      x_max  = NCOLS;
      x_cnt  = NCOLS - BCOL;
      x_wrig = (x_cnt / (WCOLS + my.c_over)) * my.w_wide;
   }
   x_trig   = (x_max    * my.c_offset) / my.t_wide;
   /*---(output)-------------------------*/
   DEBUG_GRAF   yLOG_complex  ("cols"      , "%3dn, %3db, %3dc, %3de, %3.0fo"        , NCOLS, BCOL, CCOL, ECOL, my.c_offset);
   DEBUG_GRAF   yLOG_complex  ("working"   , "%8.3fc, %8.3fx"                        , x_cnt , x_max );
   /*---(draw)---------------------------*/
   metis_opengl__panel (x_wtop, x_wlef, x_wbot, x_wrig,    x_ttop, x_tlef, x_tbot, x_trig);
   /*---(panel two/rig)---------------*/
   if (x_wrig < my.w_wide) {
      x_wlef = x_wrig;
      x_wrig = my.w_wide;
      x_tlef = 0.0;
      x_max  = (WCOLS + my.c_over) - x_cnt;
      x_trig = (x_max    * my.c_offset) / my.t_wide;
      /*---(output)-------------------------*/
      DEBUG_GRAF   yLOG_complex  ("cols"      , "%3dn, %3db, %3dc, %3de, %3.0fo"        , NCOLS, BCOL, CCOL, ECOL, my.c_offset);
      DEBUG_GRAF   yLOG_complex  ("working"   , "%8.3fc, %8.3fx"                        , x_cnt , x_max );
      /*---(draw)------------------------*/
      metis_opengl__panel (x_wtop, x_wlef, x_wbot, x_wrig, x_ttop, x_tlef, x_tbot, x_trig);
   }
   /*---(current)---------------------*/
   if (a_xcur != NULL)  *a_xcur    = (CCOL - BCOL) * my.c_offset;
   if (a_ycur != NULL)  *a_ycur    = -my.m_offset;
   /*---(complete)--------------------*/
   return 0;
}

char
metis_opengl__columns   (float *a_xcur, float *a_ycur)
{
   /*---(locals)-----------+-----+-----+-*/
   float     x_wlef, x_wrig, x_wtop, x_wbot;
   float     x_tlef, x_trig, x_ttop, x_tbot;
   float     x_max , x_cnt;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_note     ("column formats--------------");
   DEBUG_GRAF   yLOG_complex  ("rows"      , "%3dw, %3dt, %3dn, %3db, %3dc, %3de, %3.0fo", WROWS, TROWS, NROWS, BROW, CROW, EROW, my.r_offset);
   /*---(consistent)---------------------*/
   x_wlef  = 0.0;
   x_wrig  = my.w_wide;
   x_tlef  = 0.0;
   x_trig  = 1.0;
   /*---(updates)------------------------*/
   x_wtop  = 0.0;
   x_ttop  = 1.0 - ((BROW  *  my.r_offset) / (float) my.t_tall);
   if (NROWS - BROW > WROWS) {
      x_max  = BROW + WROWS;
      x_cnt  = WROWS;
      x_wbot = -my.w_tall;
   } else {
      x_max  = NROWS;
      x_cnt  = NROWS - BROW;
      x_wbot = -(x_cnt / WROWS) * my.w_tall;
   }
   x_tbot   = 1.0 - ((x_max    *  my.r_offset) / (float) my.t_tall);
   /*---(output)-------------------------*/
   DEBUG_GRAF   yLOG_complex  ("working"   , "%8.3fc, %8.3fx"                        , x_cnt , x_max );
   /*---(draw)---------------------------*/
   metis_opengl__panel (x_wtop, x_wlef, x_wbot, x_wrig, x_ttop, x_tlef, x_tbot, x_trig);
   /*---(current)------------------------*/
   if (a_xcur != NULL)  *a_xcur    = 0.0;
   if (a_ycur != NULL)  *a_ycur    = -(CROW - BROW) * my.r_offset;
   /*---(complete)--------------------*/
   return 0;
}

char
metis_opengl__larges    (float *a_xcur, float *a_ycur)
{
   /*---(locals)-----------+-----+-----+-*/
   float     x_wlef, x_wrig, x_wtop, x_wbot;
   float     x_tlef, x_trig, x_ttop, x_tbot;
   float     x_max , x_cnt;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_note     ("large formats---------------");
   /*---(consistent)---------------------*/
   x_wtop  = 0.0;
   x_wbot  = -my.w_tall;
   x_ttop  = 1.0;
   x_tbot  = 0.0;
   x_wlef  = 0.0;
   x_wrig  = my.w_wide;
   /*---(panel one/lef)---------------*/
   x_tlef  = (BCOL        * my.c_offset) / (float) my.t_wide;
   x_trig  = ((BCOL + 4)  * my.c_offset) / (float) my.t_wide;
   /*---(output)-------------------------*/
   DEBUG_GRAF   yLOG_complex  ("cols"      , "%3dn, %3db, %3dc, %3de, %3.0fo"        , NCOLS, BCOL, CCOL, ECOL, my.c_offset);
   DEBUG_GRAF   yLOG_complex  ("rows"      , "%3dn, %3db, %3dc, %3de, %3.0fo"        , NROWS, BROW, CROW, EROW, my.r_offset);
   DEBUG_GRAF   yLOG_complex  ("working"   , "%8.3fc, %8.3fx"                        , x_cnt , x_max );
   /*---(draw)---------------------------*/
   metis_opengl__panel (x_wtop, x_wlef, x_wbot, x_wrig, x_ttop, x_tlef, x_tbot, x_trig);
   /*---(current)---------------------*/
   if (a_xcur != NULL)  *a_xcur    = (CCOL - BCOL) * my.c_offset;
   if (a_ycur != NULL)  *a_ycur    = -(CROW - BROW) * my.r_offset;
   /*---(complete)--------------------*/
   return 0;
}

char
metis_opengl__current   (float a_xcur, float a_ycur)
{
   glColor4f (  1.000,  1.000,  1.000, 1.000);
   /*> glBegin(GL_POLYGON); {                                                         <* 
    *>    glVertex3f (a_xcur +   2.0, a_ycur -  6.0,   80.0);                         <* 
    *>    glVertex3f (a_xcur +  38.0, a_ycur - 43.0,   80.0);                         <* 
    *>    glVertex3f (a_xcur +  25.0, a_ycur - 43.0,   80.0);                         <* 
    *>    glVertex3f (a_xcur +   2.0, a_ycur - 20.0,   80.0);                         <* 
    *> } glEnd();                                                                     <*/
   /*> glBegin(GL_POLYGON); {                                                         <* 
    *>    glVertex3f (a_xcur +  10.0, a_ycur - 16.0,   80.0);                         <* 
    *>    glVertex3f (a_xcur +  30.0, a_ycur - 35.0,   80.0);                         <* 
    *>    glVertex3f (a_xcur +  25.0, a_ycur - 35.0,   80.0);                         <* 
    *>    glVertex3f (a_xcur +  10.0, a_ycur - 21.0,   80.0);                         <* 
    *> } glEnd();                                                                     <*/
   glBegin(GL_POLYGON); {
      glVertex3f (a_xcur +   2.0, a_ycur - 20.0,   80.0);
      glVertex3f (a_xcur +   8.0, a_ycur - 14.0,   80.0);
      glVertex3f (a_xcur +  14.0, a_ycur - 20.0,   80.0);
      glVertex3f (a_xcur +   8.0, a_ycur - 26.0,   80.0);
   } glEnd();
}

char       /* PURPOSE : place the texture in the window ----------------------*/
metis_opengl_show  (void)
{
   /*---(locals)-------------------------*/
   char        rc          =    0;
   char        x_mode      =  '-';
   char        x_stat      =  '-';
   static char x_modesave  =  '-';
   static char x_statsave  =   0 ;
   float       x_cur, y_cur;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   DEBUG_GRAF   yLOG_char     ("format"    , my.format);
   /*> x_mode = yVIKEYS_mode ();                                                      <*/
   DEBUG_GRAF   yLOG_char     ("x_mode"    , x_mode);
   DEBUG_GRAF   yLOG_char     ("x_modesave", x_modesave);
   /*> x_stat = yVIEW_showing (YVIEW_STATUS);                                         <*/
   DEBUG_GRAF   yLOG_char     ("x_stat"    , x_stat);
   DEBUG_GRAF   yLOG_char     ("x_statsave", x_statsave);
   /*---(force mask redraw)--------------*/
   /*> if (x_mode != x_modesave) {                                                    <* 
    *>    switch (x_mode) {                                                           <* 
    *>    case SMOD_MENUS   : metis_opengl_mask ();  break;                           <* 
    *>    case UMOD_HISTORY : metis_opengl_mask ();  break;                           <* 
    *>    }                                                                           <* 
    *>    switch (x_modesave) {                                                       <* 
    *>    case SMOD_MENUS   : metis_opengl_mask ();  break;                           <* 
    *>    case UMOD_HISTORY : metis_opengl_mask ();  break;                           <* 
    *>    }                                                                           <* 
    *> }                                                                              <* 
    *> if (x_stat != x_statsave) {                                                    <* 
    *>    metis_opengl_mask ();                                                       <* 
    *> }                                                                              <* 
    *> x_modesave = x_mode;                                                           <* 
    *> x_statsave = x_stat;                                                           <*/
   /*---(tickers)------------------------*/
   if (strchr (FORMAT_TICKERS, my.format) != 0) {
      /*> metis_opengl__tickers (&x_cur, &y_cur);                                     <*/
   }
   /*---(columns)------------------------*/
   else if (strchr (FORMAT_COLUMNS, my.format) != 0) {
      metis_opengl__columns (&x_cur, &y_cur);
   }
   /*---(larges)-------------------------*/
   else {
      /*> metis_opengl__larges  (&x_cur, &y_cur);                                     <*/
   }
   /*---(current)---------------------*/
   metis_opengl__current  (x_cur, y_cur);
   /*---(complete)-------------------------*/
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                          draw tasks                          ----===*/
/*============================--------------------============================*/
static void      o___TASKS___________________o (void) {;}

char          /*----: lay down the base color --------------------------------*/
metis_opengl__base      (char  a_value)
{
   DEBUG_GRAF   yLOG_senter   (__FUNCTION__);
   DEBUG_GRAF   yLOG_sint     (a_value);
   /*---(back)---------------------------*/
   glColor4f (  1.000,  1.000,  1.000,  1.000);
   glBegin(GL_POLYGON); {
      glVertex3f (      0.0,        0.0,  -10.0);
      glVertex3f (my.c_wide,        0.0,  -10.0);
      glVertex3f (my.c_wide, -my.r_tall,  -10.0);
      glVertex3f (      0.0, -my.r_tall,  -10.0);
   } glEnd();
   /*---(aesthetic tint)-----------------*/
   metis_opengl_color  (my.urgs, a_value, 0.3);
   glBegin(GL_POLYGON); {
      glVertex3f (      0.0,        0.0,  -10.0);
      glVertex3f (my.c_wide,        0.0,  -10.0);
      glVertex3f (my.c_wide, -my.r_tall,  -10.0);
      glVertex3f (      0.0, -my.r_tall,  -10.0);
   } glEnd();
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_sexit    (__FUNCTION__);
   return 0;
}

char
metis_opengl__urg       (char a_value, int z)
{
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   glColor4f (  0.000,  0.000,  0.000,  1.000);
   glBegin(GL_POLYGON); {
      glVertex3f (  0.0,   0.0, z + 0.0);
      glVertex3f (300.0,   0.0, z + 0.0);
      glVertex3f (300.0, -17.0, z + 0.0);
      glVertex3f (  0.0, -17.0, z + 0.0);
   } glEnd();
   metis_opengl_color  (my.urgs, a_value, 1.0);
   glBegin(GL_POLYGON); {
      glVertex3f (  2.0,  -2.0, z + 5.0);
      glVertex3f (298.0,  -2.0, z + 5.0);
      glVertex3f (298.0, -15.0, z + 5.0);
      glVertex3f (  2.0, -15.0, z + 5.0);
   } glEnd();
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
metis_opengl__imp       (char a_value, int z)
{
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   glColor4f (  0.000,  0.000,  0.000,  1.000);
   glBegin(GL_POLYGON); {
      glVertex3f (  0.0,   0.0, z + 0.0);
      glVertex3f ( 25.0,   0.0, z + 0.0);
      glVertex3f ( 70.0, -45.0, z + 0.0);
      glVertex3f ( 20.0, -45.0, z + 0.0);
      glVertex3f (  0.0, -25.0, z + 0.0);
   } glEnd();
   metis_opengl_color  (my.imps, a_value, 1.0);
   glBegin(GL_POLYGON); {
      glVertex3f (  2.0,  -2.0, z + 5.0);
      glVertex3f ( 23.0,  -2.0, z + 5.0);
      glVertex3f ( 65.0, -43.0, z + 5.0);
      glVertex3f ( 43.0, -43.0, z + 5.0);
   } glEnd();
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
metis_opengl__est       (char a_value, int z)
{
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   glColor4f (0.0, 0.0, 0.0, 1.0);
   glBegin(GL_POLYGON); {
      glVertex3f (  0.0, -15.0, z + 0.0);
      glVertex3f ( 57.0, -15.0, z + 0.0);
      glVertex3f ( 57.0, -45.0, z + 0.0);
      glVertex3f (  0.0, -45.0, z + 0.0);
   } glEnd();
   metis_opengl_color  (my.ests, a_value, 1.0);
   glBegin(GL_POLYGON); {
      glVertex3f (  2.0, -17.0, z + 5.0);
      glVertex3f ( 55.0, -17.0, z + 5.0);
      glVertex3f ( 55.0, -43.0, z + 5.0);
      glVertex3f (  2.0, -43.0, z + 5.0);
   } glEnd();
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
metis_opengl__prg       (char a_value, int z)
{
   int         i           =    0;
   int         y           =    0;
   int         n           =    0;
   char        t           [LEN_TERSE] = "";
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   glColor4f (0.0, 0.0, 0.0, 1.0);
   glPushMatrix(); {
      glTranslatef( 285.0, -15.0,  20.0);
      glBegin(GL_POLYGON); {
         glVertex3f(    0.0,    0.0, z + 0.0);
         glVertex3f(   15.0,    0.0, z + 0.0);
         glVertex3f(   15.0,  -30.0, z + 0.0);
         glVertex3f(    0.0,  -30.0, z + 0.0);
      } glEnd();
   } glPopMatrix();
   switch (a_value) {
   case '·' :  glColor4f (1.00, 1.00, 1.00, 1.00);   break;
   case '-' :  glColor4f (1.00, 1.00, 1.00, 1.00);   break;
   case '<' :  glColor4f (0.60, 0.60, 0.60, 1.00);   break;
   case 'o' :  glColor4f (0.45, 0.45, 0.45, 1.00);   break;
   case '>' :  glColor4f (0.25, 0.25, 0.25, 1.00);   break;
   case '#' :  glColor4f (0.00, 0.00, 0.00, 1.00);   break;
   case 'x' :  glColor4f (0.00, 0.00, 0.50, 1.00);   break;
   case 'd' :  glColor4f (0.50, 0.00, 0.00, 1.00);   break;
   default  :  glColor4f (1.00, 1.00, 1.00, 1.00);   break;
   }
   glPushMatrix(); {
      glTranslatef( 287.0, -17.0,  40.0);
      glBegin(GL_POLYGON); {
         glVertex3f(    0.0,    0.0, z + 5.0);
         glVertex3f(   11.0,    0.0, z + 5.0);
         glVertex3f(   11.0,  -26.0, z + 5.0);
         glVertex3f(    0.0,  -26.0, z + 5.0);
      } glEnd();
   } glPopMatrix();
   switch (a_value) {
   case '·' : case '-' : case '<' : case 'o' :
      glColor4f (0.00, 0.00, 0.00, 1.00);   break;
   case '>' : case '#' : case 'x' :
      glColor4f (1.00, 1.00, 1.00, 1.00);   break;
   default  :
      glColor4f (1.00, 1.00, 1.00, 1.00);   break;
   }
   glPushMatrix(); {
      glTranslatef( 292.0, -25.0, 80.0);
      snprintf (t, 4, "%c", a_value);
      yFONT_print (my.pretty,  8, YF_BASCEN, t);
   } glPopMatrix();
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
metis_opengl__age       (char *a_epoch, int z)
{
   long        v           =    0;
   int         x_age       =    0;
   char        x_unit      =  '-';
   char        t           [LEN_TERSE] = "";
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   str4mongo (a_epoch, &v);
   strlage (v, t);
   /*> if (a_value == 0)  {                                                           <* 
    *>    strcpy (t, "??");                                                           <* 
    *> } else {                                                                       <* 
    *>    x_age = 0;                                                                  <* 
    *>    x_age = time (NULL) - a_value;                                              <* 
    *>    if (x_age < 0)  x_age = 0;  /+ tasks added since start +/                   <* 
    *>    x_unit = 's';                                                               <* 
    *>    if (x_age >= 60) {                                                          <* 
    *>       x_age /= 60; x_unit = 'm';                                               <* 
    *>       if (x_age >= 60) {                                                       <* 
    *>          x_age /= 60; x_unit = 'h';                                            <* 
    *>          if (x_age >= 24) {                                                    <* 
    *>             x_age /= 24; x_unit = 'd';                                         <* 
    *>             if (x_age > 30) {                                                  <* 
    *>                x_age /= 30; x_unit = 'm';                                      <* 
    *>                if (x_age >= 12) {                                              <* 
    *>                   x_age /= 12; x_unit = 'y';                                   <* 
    *>                   if (x_age > 99) { x_age  = 99; x_unit = '!'; }               <* 
    *>                }                                                               <* 
    *>             }                                                                  <* 
    *>          }                                                                     <* 
    *>       }                                                                        <* 
    *>    }                                                                           <* 
    *>    sprintf (t, "%2d%c", x_age, x_unit);                                        <* 
    *> }                                                                              <*/
   glColor4f (0.0, 0.0, 0.0, 1.0);
   glPushMatrix(); {
      glTranslatef( 282.0, -35.0, 80.0);
      yFONT_print (my.pretty,  6, YF_BASRIG, t);
   } glPopMatrix();
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}

char               /* PURPOSE : draw two top line bullets --------------------*/
metis_opengl__bullets   (void)
{
   glColor4f (0.0, 0.0, 0.0, 1.0);
   glPushMatrix(); {
      glTranslatef(  75.0,  -9.0,  20.0);
      glBegin(GL_POLYGON); {
         glVertex3f(   -3.0,    3.0,    5.0);
         glVertex3f(    3.0,    3.0,    5.0);
         glVertex3f(    3.0,   -3.0,    5.0);
         glVertex3f(   -3.0,   -3.0,    5.0);
      } glEnd();
   } glPopMatrix();
   glPushMatrix(); {
      glTranslatef( 175.0,  -9.0,  20.0);
      glBegin(GL_POLYGON); {
         glVertex3f(   -3.0,    3.0,    5.0);
         glVertex3f(    3.0,    3.0,    5.0);
         glVertex3f(    3.0,   -3.0,    5.0);
         glVertex3f(   -3.0,   -3.0,    5.0);
      } glEnd();
   } glPopMatrix();
   return 0;
}

char
metis_opengl__text      (int a_index, char *a_major, char *a_minor, char *a_text)
{
   char        temp        [LEN_LABEL];
   glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
   glPushMatrix(); {
      glColor4f (0.0, 0.0, 0.0, 1.0);
      glTranslatef (  53.0,  -9.0,  40.0);
      glTranslatef (   0.0, txf_off,   0.0);
      if (a_index >= 0) {
         snprintf (temp, 10, "%d", a_index + 1);
         yFONT_print  (my.pretty,  8, YF_MIDCEN, temp);
      }
      glTranslatef (  72.0,   0.0,   0.0);
      yFONT_print  (my.pretty,  8, YF_MIDCEN, a_major);
      glTranslatef ( 105.0,   0,   0);
      yFONT_print  (my.pretty,  8, YF_MIDCEN, a_minor);
      /*> glTranslatef (-160.0, -10.0,   0.0);                                        <*/
      glTranslatef (-160.0, -18.0,   0.0);
      glTranslatef (   0.0, txf_off - 1.0,   0.0);
      glColor4f (0.0, 0.0, 0.0, 1.0);
      yFONT_printw (my.pretty,  8, YF_TOPLEF, a_text, 195, 35, txf_space);
   } glPopMatrix();
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   return 0;
}

char
metis_opengl__cats      (char a_urg, char a_imp, char a_est)
{
   char        temp        [LEN_LABEL];
   glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
   glPushMatrix(); {
      glColor4f (0.0, 0.0, 0.0, 1.0);
      glTranslatef(  21.0,  -6.0,  40.0);
      glTranslatef(   0.0, txf_off,   0.0);
      snprintf (temp, 4, "%c", a_urg);
      yFONT_print (my.pretty,  9, YF_BASCEN, temp);
      glTranslatef(  12.0, -12.0,   0.0);
      snprintf (temp, 4, "%c", a_imp);
      yFONT_print (my.pretty,  9, YF_BASCEN, temp);
      glTranslatef(  12.0, -12.0,   0.0);
      snprintf (temp, 4, "%c", a_est);
      yFONT_print (my.pretty,  9, YF_BASCEN, temp);
   } glPopMatrix();
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   return 0;
}

char               /* PURPOSE : draw two and bottom borders ------------------*/
metis_opengl__borders   (void)
{
   glColor4f (0.0f, 0.0f, 0.0f, 1.0f);
   glBegin(GL_POLYGON); {
      glVertex3f(   0.0,     0.0,    50.0);
      glVertex3f( 300.0,     0.0,    50.0);
      glVertex3f( 300.0,    -2.0,    50.0);
      glVertex3f(   0.0,    -2.0,    50.0);
   } glEnd();
   glBegin(GL_POLYGON); {
      glVertex3f(   0.0,   -43.0,    50.0);
      glVertex3f( 300.0,   -43.0,    50.0);
      glVertex3f( 300.0,   -45.0,    50.0);
      glVertex3f(   0.0,   -45.0,    50.0);
   } glEnd();
   glBegin(GL_POLYGON); {
      glVertex3f(   0.0,     0.0,    50.0);
      glVertex3f(   2.0,     0.0,    50.0);
      glVertex3f(   2.0,   -45.0,    50.0);
      glVertex3f(   0.0,   -45.0,    50.0);
   } glEnd();
   glBegin(GL_POLYGON); {
      glVertex3f( 298.0,     0.0,    50.0);
      glVertex3f( 300.0,     0.0,    50.0);
      glVertex3f( 300.0,   -45.0,    50.0);
      glVertex3f( 298.0,   -45.0,    50.0);
   } glEnd();
   /*---(complete)------------------------------*/
   return 0;
}

char
metis_opengl__card      (int a_index)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   tTASK      *x_task      = NULL;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   DEBUG_GRAF   yLOG_value    ("a_index"   , a_index);
   metis_task_by_index (a_index, &x_task);
   DEBUG_GRAF   yLOG_point    ("x_task"    , x_task);
   --rce;  if (x_task == NULL) {
      DEBUG_GRAF   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   metis_opengl__base    (x_task->urg);
   metis_opengl__urg     (x_task->urg,  0);
   metis_opengl__est     (x_task->est, 10);
   metis_opengl__imp     (x_task->imp, 20);
   metis_opengl__prg     (x_task->prg, 20);
   metis_opengl__age     (x_task->epoch, 20);
   metis_opengl__bullets ();
   metis_opengl__text    (a_index, x_task->minor->major->name, x_task->minor->name, x_task->txt);
   metis_opengl__cats    (x_task->urg, x_task->imp, x_task->est);
   metis_opengl__borders ();
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}

char         /*--> drawing single col or row --------[------ [---------------]*/
metis_opengl__colrow    (int a_max, short a_xinc, short a_yinc)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   short       x_pos       =    0;
   short       y_pos       =    0;
   short       x_col       =    0;
   short       x_row       =    0;
   int         n           =    0;
   int         c           =    0;
   tTASK      *x_task      = NULL;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   /*---(adjust max)---------------------*/
   DEBUG_GRAF   yLOG_value    ("g_ntask"   , g_ntask);
   DEBUG_GRAF   yLOG_value    ("a_max"     , a_max);
   n = metis_task_count ();
   DEBUG_GRAF   yLOG_value    ("n"         , n);
   glPushMatrix(); {
      for (i = 0; i < n; ++i) {
         metis_task_by_index (i, &x_task);
         /*---(filter)-------------------*/
         DEBUG_GRAF   yLOG_complex  ("card"      , "%2d, %c %s", i, x_task->show, x_task->txt);
         /*> if (x_task->show != 'y')     continue;                                   <*/
         if (c >= a_max)              break;      
         /*---(draw)---------------------*/
         metis_opengl__card (i);
         /*---(update card)--------------*/
         /*> g_tasks [i].pos = c++;                                                   <* 
          *> g_tasks [i].x   = x_pos;                                                 <* 
          *> g_tasks [i].y   = y_pos;                                                 <* 
          *> g_tasks [i].col = x_pos;                                                 <* 
          *> g_tasks [i].row = y_pos;                                                 <*/
         /*---(update position)----------*/
         x_pos += a_xinc;
         y_pos += a_yinc;
         if (a_xinc > 0)  x_col++;
         if (a_yinc > 0)  x_row++;
         glTranslatef (a_xinc, a_yinc, 0.0);
         /*---(output)-------------------*/
         DEBUG_GRAF   yLOG_complex  ("placed"    , "%4dp, %4dx, %4dy, %4dc, %4dr", g_tasks [i].pos, g_tasks [i].x, g_tasks [i].y, g_tasks [i].col, g_tasks [i].row);
         ++c;
         /*---(done)---------------------*/
      }
   } glPopMatrix();
   /*---(complete)-------------------------*/
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
metis_opengl__prep      (void)
{
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   /*---(create objects)-----------------*/
   /*> printf("   entered\n");                                                        <*/
   DEBUG_GRAF   yLOG_note     ("delete old texture");
   if (my.g_tex != -1)  yGLTEX_free (&my.g_tex, &my.g_fbo, &my.g_dep);
   DEBUG_GRAF   yLOG_note     ("create texture");
   yGLTEX_new (&my.g_tex, &my.g_fbo, &my.g_dep, my.t_wide, my.t_tall);
   /*---(setup)--------------------------*/
   DEBUG_GRAF   yLOG_note     ("setup opengl view");
   yGLTEX_draw (my.g_fbo, YGLTEX_TOPLEF, my.t_wide, my.t_tall, 1.0);
   glColor4f (0.0f, 0.0f, 0.0f, 1.0f);
   /*---(complete)-------------------------*/
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
metis_opengl_draw        (void)
{
   /*---(locals)-------------------------*/
   int       i;                             /* loop iterator -- word          */
   int       j;                             /* loop iterator -- word          */
   int       rc      = 0;                   /* simple return code             */
   int       w       = 0;
   int       pos     = 0;
   int       x_max   = 0;
   float     x_inc   = 0.0;
   int       n       =    0;
   short       x_pos       =    0;
   short       y_pos       =    0;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   /*---(debugging)----------------------*/
   DEBUG_GRAF   yLOG_value    ("my.nact"   , my.nact);
   DEBUG_GRAF   yLOG_value    ("TCOLS"  , TCOLS);
   DEBUG_GRAF   yLOG_value    ("TROWS"  , TROWS);
   /*---(prepare)------------------------*/
   metis_opengl__prep ();
   /*---(draw)---------------------------*/
   switch (my.format) {
   case FORMAT_TICKER   : case FORMAT_BASELINE :
      metis_opengl__colrow (TCOLS, my.c_offset,   0.0);
      break;
   case FORMAT_RSHORT   : case FORMAT_LSHORT   :
   case FORMAT_RLONG    : case FORMAT_LLONG    :
   case FORMAT_STREAMER :
      metis_opengl__colrow (TROWS,   0.0, -my.r_offset);
      break;
   case FORMAT_WIDE     : case FORMAT_PROJECT  : case FORMAT_EXTRA    :
      glPushMatrix(); {
         for (j = 0; j < WCOLS; ++j) {
            glPushMatrix(); {
               for (i = 0; i < WROWS; ++i) {
                  n = format_check (CCOL + j + 1, i + 1);
                  if (n < 0)  continue;
                  /*> g_tasks [n].pos = j * WROWS + i;                                <* 
                   *> g_tasks [n].x   = x_pos;                                        <* 
                   *> g_tasks [n].y   = y_pos;                                        <* 
                   *> g_tasks [n].col = j;                                            <* 
                   *> g_tasks [n].row = i;                                            <*/
                  y_pos += my.r_offset;
                  metis_opengl__card (n);
                  glTranslatef(0.0, -my.r_offset,   0.0);
               }
            } glPopMatrix();
            x_pos += my.c_offset;
            glTranslatef (my.c_offset, 0.0, 0.0);
         }
      } glPopMatrix();
      break;
   }
   /*---(mipmaps)------------------------*/
   if (strchr ("y+", my.png) != NULL) {
      yGLTEX_tex2png ("/tmp/metis.png", my.t_wide, my.t_tall);
   }
   yGLTEX_done    (my.g_tex);
   /*---(complete)-------------------------*/
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                             masking                          ----===*/
/*============================--------------------============================*/
static void      o___MASK____________________o (void) {;}

char
metis_opengl_mask             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int       i         = 0;
   int       j         = 0;
   int       k         = 0;
   Pixmap    bounds;
   GC        gc;
   int       x_col      = 0;
   int         x_max       =    0;
   float       x_inc       =    0;
   char        x_mode      =  '-';
   char        x_status    =  '-';
   char        x_help      =  '-';
   char        x_abbr      =  '-';
   char        x_types     [LEN_LABEL] = "uiep";
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   /*---(prepare)------------------------*/
   bounds    = XCreatePixmap (YX_DISP, YX_BASE, my.w_wide, my.w_ftall, 1);
   gc        = XCreateGC     (YX_DISP, bounds, 0, NULL);
   /*> x_mode = yVIKEYS_mode ();                                                      <*/
   DEBUG_GRAF   yLOG_char     ("x_mode"    , x_mode);
   DEBUG_GRAF   yLOG_char     ("g_major"   , g_major);
   DEBUG_GRAF   yLOG_char     ("g_minor"   , g_minor);
   DEBUG_GRAF   yLOG_char     ("my.format" , my.format);
   DEBUG_GRAF   yLOG_value    ("WCOLS"  , WCOLS);
   DEBUG_GRAF   yLOG_value    ("WROWS"  , WROWS);
   DEBUG_GRAF   yLOG_value    ("my.nact"   , my.nact);
   /*> x_status = yVIKEYS_view_size     (YVIKEYS_STATUS, NULL, NULL, NULL, NULL, NULL);   <*/
   DEBUG_GRAF   yLOG_char     ("x_status"  , x_status);
   XSetForeground (YX_DISP, gc, 0);
   XFillRectangle (YX_DISP, bounds, gc, 0, 0, my.w_wide, my.w_ftall);
   XSetForeground (YX_DISP, gc, 1);
   /*---(establish mask)-----------------*/
   /*> x_help = yVIKEYS_help ();                                                      <*/
   DEBUG_GRAF   yLOG_char     ("x_help"    , x_help);
   if (strchr (x_types, x_help) != NULL) {
      switch (x_help) {
      case 'u' : x_max = strlen (my.urgs);  break;
      case 'i' : x_max = strlen (my.imps);  break;
      case 'e' : x_max = strlen (my.ests);  break;
      case 'p' : x_max = strlen (my.prgs);  break;
      }
      DEBUG_GRAF   yLOG_value    ("x_max"     , x_max);
      for (i = 0; i < x_max; ++i) {
         XFillRectangle (YX_DISP, bounds, gc,   0, i * my.r_offset, my.c_wide, my.r_tall);
      }
   }
   else if (x_help == 'f') {
      for (j = 0; j < 4; ++j) {
         x_abbr = x_types [j];
         switch (x_abbr) {
         case 'u' : x_max = strlen (my.urgs);  break;
         case 'i' : x_max = strlen (my.imps);  break;
         case 'e' : x_max = strlen (my.ests);  break;
         case 'p' : x_max = strlen (my.prgs);  break;
         }
         for (i = 0; i < x_max; ++i) {
            XFillRectangle (YX_DISP, bounds, gc, j * my.c_offset, i * my.r_offset, my.c_wide, my.r_tall);
         }
      }
   }
   else {
      switch (my.format) {
      case FORMAT_TICKER   : case FORMAT_BASELINE :
         x_max = WCOLS + my.c_over;
         if (x_max >= my.nact)  x_max = my.nact;
         for (i = 0; i < x_max; ++i)  XFillRectangle (YX_DISP, bounds, gc, i * my.c_offset, my.m_offset, my.c_wide, my.r_tall);
         break;
      case FORMAT_RSHORT   : case FORMAT_LSHORT   : case FORMAT_RLONG    : case FORMAT_LLONG    :
         x_max = WROWS;
         if (x_max >= my.nact)  x_max = my.nact;
         DEBUG_GRAF   yLOG_value    ("x_max"     , x_max);
         for (i = 0; i < x_max; ++i) {
            XFillRectangle (YX_DISP, bounds, gc,   0, i * my.r_offset, my.c_wide, my.r_tall);
         }
         break;
      case FORMAT_STREAMER :
         x_max = WROWS + my.r_over;
         if (x_max >= my.nact)  x_max = my.nact;
         DEBUG_GRAF   yLOG_value    ("x_max"     , x_max);
         for (i = 0; i < x_max; ++i) {
            XFillRectangle (YX_DISP, bounds, gc,   0, i * my.r_offset, my.c_wide, my.r_tall);
         }
         break;
      case FORMAT_WIDE     : case FORMAT_PROJECT  : case FORMAT_EXTRA    :
         for (j = 0; j < (WCOLS + my.c_over); ++j) {
            for (i = 0; i < (WROWS + my.r_over); ++i) {
               if (format_check (BCOL + j + 1, i + 1) < 0)  continue;
               XFillRectangle(YX_DISP, bounds, gc,  j * my.c_offset, i * my.r_offset, my.c_wide, my.r_tall);
            }
         }
         break;
      }
   }
   /*---(check for menu)-----------------*/
   if (x_mode == SMOD_MENUS) {
      DEBUG_GRAF   yLOG_note     ("draw the main menu mask");
      XFillRectangle (YX_DISP, bounds, gc, my.w_wide / 2 - 140, 40, 280, 200);
   }
   /*---(check for status bar)-----------*/
   if (yVIEW_showing (YVIEW_STATUS)) {
      XFillRectangle (YX_DISP, bounds, gc, 0.0, my.w_ftall - 15, my.w_wide, 15);
   }
   /*---(set mask)-----------------------*/
   XShapeCombineMask (YX_DISP, YX_BASE, ShapeBounding, 0, 0, bounds, ShapeSet);
   /*---(free)---------------------------*/
   XFreePixmap (YX_DISP, bounds);
   XFreeGC     (YX_DISP, gc);
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      unit testing                            ----===*/
/*====================------------------------------------====================*/
static void  o___UNITTEST________o () { return; }

char*        /*-> unit test accessor -----------------[ light  [us.B60.2A3.F2]*/ /*-[01.0000.00#.#]-*/ /*-[--.---.---.--]-*/
OPENGL__unit       (char *a_question, int a_num)
{
   /*---(locals)-------------------------*/
   char        rc          =    0;
   char        s           [LEN_HUND]   = "[]";
   char        t           [LEN_HUND]   = "[]";
   char        u           [LEN_HUND]   = "[]";
   /*---(overall)------------------------*/
   strcpy (unit_answer, "OPENGL           : question not understood");
   if      (strcmp (a_question, "position"      ) == 0) {
      snprintf (unit_answer, LEN_FULL, "OPENGL pos  (%2d) : %c %4dp %4dx %4dy %4dc %4dr", a_num, g_tasks [a_num].act, g_tasks [a_num].pos, g_tasks [a_num].x, g_tasks [a_num].y, g_tasks [a_num].col, g_tasks [a_num].row);
   }
   else if (strcmp (a_question, "current"       ) == 0) {
      snprintf (unit_answer, LEN_FULL, "OPENGL curr      : row %3db %3dc %3de, col %3db %3dc %3de", BROW, CROW, EROW, BCOL, CCOL, ECOL);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


/*============================----end-of-source---============================*/
