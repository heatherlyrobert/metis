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





/*===[[ METIS BACKLOG ]]======================================================*
 *
 * metis  dw2·  when filtered to zero cards, display blank card
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
OPENGL__colors          (char *a_valid, char a_color)
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
   case  0  : glColor3f (  1.000,  0.000,  0.000); break;
   case  1  : glColor3f (  0.900,  0.455,  0.000); break;
   case  2  : glColor3f (  0.800,  0.733,  0.000); break;
   case  3  : glColor3f (  0.200,  0.667,  0.200); break;
   case  4  : glColor3f (  0.000,  0.600,  0.600); break;
   case  5  : glColor3f (  0.600,  0.000,  0.800); break;
   case  6  : glColor3f (  0.600,  0.000,  0.600); break;
   case  7  : glColor3f (  0.400,  0.400,  0.400); break;
   default  : glColor3f (  0.700,  0.700,  0.700); break;
   }
   return 0;
}

char
OPENGL__polygon         (float *a_array)
{
   glPushMatrix(); {
      /*> glDisable(GL_BLEND);                                                        <*/
      /*> glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);                          <*/
      /*> glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);                                <*/
      /*> glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE);                                 <*/
      glBegin(GL_POLYGON); {
         glVertex3fv (a_array +  0);
         glVertex3fv (a_array +  3);
         glVertex3fv (a_array +  6);
         glVertex3fv (a_array +  9);
      } glEnd();
      glTranslatef(0.0, 0.0, -2.0);
      glBegin(GL_LINE_STRIP); {
         glVertex3fv (a_array +  0);
         glVertex3fv (a_array +  3);
         glVertex3fv (a_array +  6);
         glVertex3fv (a_array +  9);
         glVertex3fv (a_array +  0);
      } glEnd();
      /*> glEnable(GL_BLEND);                                                         <*/
   } glPopMatrix();
   return 0;
}



/*====================------------------------------------====================*/
/*===----                             fonts                            ----===*/
/*====================------------------------------------====================*/
static void      o___FONTS___________________o (void) {;}

char
font_load(void)
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
font_delete(void)
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
OPENGL_init (void)
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
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
OPENGL_wrap             (void)
{
   return 0;
}

char
OPENGL_resize           (uint a_w, uint a_h)
{
   if (a_h == 0) a_h = 1;
   glViewport(0,  0, my.w_wide, my.w_tall);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(        0,   my.w_wide, -my.w_tall,  0, -500.0,  500.0);
   glMatrixMode(GL_MODELVIEW);
   return 0;
}



/*============================--------------------============================*/
/*===----                             show                             ----===*/
/*============================--------------------============================*/
static void      o___SHOW____________________o (void) {;}

char
OPENGL__panel        (float a_wtop, float a_wlef, float a_wbot, float a_wrig, float a_ttop, float a_tlef, float a_tbot, float a_trig)
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
OPENGL_show__tickers    (float *a_xcur, float *a_ycur)
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
   x_tlef  = (my.bcol  * my.c_offset) / (float) my.t_wide;
   if (my.ncols - my.ccol > my.wcols) {
      x_max  = my.bcol + (my.wcols + my.c_over);
      x_cnt  = my.wcols + my.c_over;
      x_wrig = my.w_wide;
   } else {
      x_max  = my.ncols;
      x_cnt  = my.ncols - my.bcol;
      x_wrig = (x_cnt / (my.wcols + my.c_over)) * my.w_wide;
   }
   x_trig   = (x_max    * my.c_offset) / my.t_wide;
   /*---(output)-------------------------*/
   DEBUG_GRAF   yLOG_complex  ("cols"      , "%3dn, %3db, %3dc, %3de, %3.0fo"        , my.ncols, my.bcol, my.ccol, my.ecol, my.c_offset);
   DEBUG_GRAF   yLOG_complex  ("working"   , "%8.3fc, %8.3fx"                        , x_cnt , x_max );
   /*---(draw)---------------------------*/
   OPENGL__panel (x_wtop, x_wlef, x_wbot, x_wrig, x_ttop, x_tlef, x_tbot, x_trig);
   /*---(panel two/rig)---------------*/
   if (x_wrig < my.w_wide) {
      x_wlef = x_wrig;
      x_wrig = my.w_wide;
      x_tlef = 0.0;
      x_max  = (my.wcols + my.c_over) - x_cnt;
      x_trig = (x_max    * my.c_offset) / my.t_wide;
      /*---(output)-------------------------*/
      DEBUG_GRAF   yLOG_complex  ("cols"      , "%3dn, %3db, %3dc, %3de, %3.0fo"        , my.ncols, my.bcol, my.ccol, my.ecol, my.c_offset);
      DEBUG_GRAF   yLOG_complex  ("working"   , "%8.3fc, %8.3fx"                        , x_cnt , x_max );
      /*---(draw)------------------------*/
      OPENGL__panel (x_wtop, x_wlef, x_wbot, x_wrig, x_ttop, x_tlef, x_tbot, x_trig);
   }
   /*---(current)---------------------*/
   if (a_xcur != NULL)  *a_xcur    = (my.ccol - my.bcol) * my.c_offset;
   if (a_ycur != NULL)  *a_ycur    = -my.m_offset;
   /*---(complete)--------------------*/
   return 0;
}

char
OPENGL_show__columns    (float *a_xcur, float *a_ycur)
{
   /*---(locals)-----------+-----+-----+-*/
   float     x_wlef, x_wrig, x_wtop, x_wbot;
   float     x_tlef, x_trig, x_ttop, x_tbot;
   float     x_max , x_cnt;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_note     ("column formats--------------");
   /*---(consistent)---------------------*/
   x_wlef  = 0.0;
   x_wrig  = my.w_wide;
   x_tlef  = 0.0;
   x_trig  = 1.0;
   /*---(updates)------------------------*/
   x_wtop  = 0.0;
   x_ttop  = 1.0 - ((my.brow  *  my.r_offset) / (float) my.t_tall);
   if (my.nrows - my.brow > my.wrows) {
      x_max  = my.brow + my.wrows;
      x_cnt  = my.wrows;
      x_wbot = -my.w_tall;
   } else {
      x_max  = my.nrows;
      x_cnt  = my.nrows - my.brow;
      x_wbot = -(x_cnt / my.wrows) * my.w_tall;
   }
   x_tbot   = 1.0 - ((x_max    *  my.r_offset) / (float) my.t_tall);
   /*---(output)-------------------------*/
   DEBUG_GRAF   yLOG_complex  ("rows"      , "%3dn, %3db, %3dc, %3de, %3.0fo"        , my.nrows, my.brow, my.crow, my.erow, my.r_offset);
   DEBUG_GRAF   yLOG_complex  ("working"   , "%8.3fc, %8.3fx"                        , x_cnt , x_max );
   /*---(draw)---------------------------*/
   OPENGL__panel (x_wtop, x_wlef, x_wbot, x_wrig, x_ttop, x_tlef, x_tbot, x_trig);
   /*---(current)------------------------*/
   if (a_xcur != NULL)  *a_xcur    = 0.0;
   if (a_ycur != NULL)  *a_ycur    = -(my.crow - my.brow) * my.r_offset;
   /*---(complete)--------------------*/
   return 0;
}

char
OPENGL_show__larges     (float *a_xcur, float *a_ycur)
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
   x_tlef  = (my.bcol        * my.c_offset) / (float) my.t_wide;
   x_trig  = ((my.bcol + 4)  * my.c_offset) / (float) my.t_wide;
   /*---(output)-------------------------*/
   DEBUG_GRAF   yLOG_complex  ("cols"      , "%3dn, %3db, %3dc, %3de, %3.0fo"        , my.ncols, my.bcol, my.ccol, my.ecol, my.c_offset);
   DEBUG_GRAF   yLOG_complex  ("rows"      , "%3dn, %3db, %3dc, %3de, %3.0fo"        , my.nrows, my.brow, my.crow, my.erow, my.r_offset);
   DEBUG_GRAF   yLOG_complex  ("working"   , "%8.3fc, %8.3fx"                        , x_cnt , x_max );
   /*---(draw)---------------------------*/
   OPENGL__panel (x_wtop, x_wlef, x_wbot, x_wrig, x_ttop, x_tlef, x_tbot, x_trig);
   /*---(current)---------------------*/
   if (a_xcur != NULL)  *a_xcur    = (my.ccol - my.bcol) * my.c_offset;
   if (a_ycur != NULL)  *a_ycur    = -(my.crow - my.brow) * my.r_offset;
   /*---(complete)--------------------*/
   return 0;
}

char
OPENGL_show__current    (float a_xcur, float a_ycur)
{
   glColor4f (  1.000,  1.000,  1.000, 1.000);
   glBegin(GL_POLYGON); {
      glVertex3f (a_xcur +   2.0, a_ycur -  6.0,   80.0);
      glVertex3f (a_xcur +  38.0, a_ycur - 43.0,   80.0);
      glVertex3f (a_xcur +  25.0, a_ycur - 43.0,   80.0);
      glVertex3f (a_xcur +   2.0, a_ycur - 20.0,   80.0);
   } glEnd();
}

char       /* PURPOSE : place the texture in the window ----------------------*/
OPENGL_show        (void)
{
   /*---(locals)-------------------------*/
   char        rc          =    0;
   char        x_mode      =  '-';
   char        x_stat      =  '-';
   char        x_help     =   '-';
   static char x_modesave  =  '-';
   static char x_statsave  =   0 ;
   static char x_helpsave =   '-';
   float       x_cur, y_cur;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   DEBUG_GRAF   yLOG_char     ("format"    , my.format);
   /*> x_mode = yVIKEYS_mode ();                                                      <*/
   DEBUG_GRAF   yLOG_char     ("x_mode"    , x_mode);
   DEBUG_GRAF   yLOG_char     ("x_modesave", x_modesave);
   /*> x_stat = yVIKEYS_view_showing (YVIKEYS_STATUS);                                <*/
   DEBUG_GRAF   yLOG_char     ("x_stat"    , x_stat);
   DEBUG_GRAF   yLOG_char     ("x_statsave", x_statsave);
   /*> x_help = yVIKEYS_help ();                                                      <*/
   DEBUG_GRAF   yLOG_char     ("x_help"    , x_help);
   DEBUG_GRAF   yLOG_char     ("x_helpsave", x_helpsave);
   /*---(force mask redraw)--------------*/
   if (x_mode != x_modesave) {
      switch (x_mode) {
      case SMOD_MENUS   : OPENGL_mask ();  break;
      case UMOD_HISTORY : OPENGL_mask ();  break;
      }
      switch (x_modesave) {
      case SMOD_MENUS   : OPENGL_mask ();  break;
      case UMOD_HISTORY : OPENGL_mask ();  break;
      }
   }
   if (x_help != x_helpsave) {
      OPENGL_mask ();
      OPENGL_draw ();
   }
   if (x_stat != x_statsave) {
      OPENGL_mask ();
   }
   x_modesave = x_mode;
   x_statsave = x_stat;
   x_helpsave = x_help;
   /*---(tickers)------------------------*/
   if (strchr (FORMAT_TICKERS, my.format) != 0) {
      OPENGL_show__tickers (&x_cur, &y_cur);
   }
   /*---(columns)------------------------*/
   else if (strchr (FORMAT_COLUMNS, my.format) != 0) {
      OPENGL_show__columns (&x_cur, &y_cur);
   }
   /*---(larges)-------------------------*/
   else {
      OPENGL_show__larges  (&x_cur, &y_cur);
   }
   /*---(current)---------------------*/
   OPENGL_show__current (x_cur, y_cur);
   /*---(complete)-------------------------*/
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                          draw tasks                          ----===*/
/*============================--------------------============================*/
static void      o___TASKS___________________o (void) {;}

char          /*----: lay down the base color --------------------------------*/
OPENGL__base       (char  a_value)
{
   DEBUG_GRAF   yLOG_senter   (__FUNCTION__);
   DEBUG_GRAF   yLOG_sint     (a_value);
   switch (a_value) {
   case '<' :
      glColor3f (  1.000,  0.700,  0.700);
      break;
   case 'o' :
      glColor3f (  0.700,  1.000,  0.700);
      break;
   case '>' :
      glColor3f (  0.700,  0.700,  1.000);
      break;
   case '#' :
      glColor3f (  0.700,  0.450,  0.250);
      break;
   case 'x' :
      glColor3f (  0.500,  0.500,  0.500);
      break;
   case '·' :
      glColor3f (  0.800,  0.800,  0.000);
      break;
   default  :
      glColor3f (  0.700,  0.700,  0.700);
      break;
   }
   glBegin(GL_POLYGON); {
      glVertex3f (      0.0,        0.0,   -1.0);
      glVertex3f (my.c_wide,        0.0,   -1.0);
      glVertex3f (my.c_wide, -my.r_tall,   -1.0);
      glVertex3f (      0.0, -my.r_tall,   -1.0);
   } glEnd();
   DEBUG_GRAF   yLOG_sexit    (__FUNCTION__);
   return 0;
}

/*                              ---top/left--------- ---top/right-------- ---bot/right-------- ---bot/left--------- */
static float   s_urg_bg [12] = {   0.0,   0.0,   0.0, 300.0,   0.0,   0.0, 300.0, -17.0,   0.0,   0.0, -17.0,   0.0, };
static float   s_urg_fg [12] = {   0.0,  -2.0,   2.0, 300.0,  -2.0,  12.0, 300.0, -15.0,   2.0,   0.0, -15.0,   2.0, };

char
OPENGL__urg             (char  a_value)
{
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   glColor4f (  0.000,  0.000,  0.000,  1.000);
   OPENGL__polygon (s_urg_bg);
   OPENGL__colors  (my.urgs, a_value);
   OPENGL__polygon (s_urg_fg);
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}


/*                              ---top/left--------- ---top/right-------- ---bot/right-------- ---bot/left--------- */
static float   s_imp_sh [12] = {   0.0,   0.0,  10.0,  25.0,   0.0,  10.0,  25.0, -22.5,  10.0,   0.0, -22.5,  10.0, };
static float   s_imp_bg [12] = {   0.0, -22.5,  10.0,  25.0,   0.0,  10.0,  70.0, -45.0,  10.0,  23.0, -45.0,  10.0, };
static float   s_imp_fg [12] = {   0.0,   0.0,  12.0,  22.0,   0.0,  12.0,  67.0, -45.0,  12.0,  45.0, -45.0,  12.0, };

char
OPENGL__imp             (char  a_value)
{
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   glColor4f (  0.000,  0.000,  0.000,  1.000);
   OPENGL__polygon (s_imp_bg);
   OPENGL__polygon (s_imp_sh);
   OPENGL__colors  (my.imps, a_value);
   OPENGL__polygon (s_imp_fg);
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}

/*                              ---top/left--------- ---top/right-------- ---bot/right-------- ---bot/left--------- */
static float   s_est_bg [12] = {  30.0, -15.0,   5.0,  58.0, -15.0,   5.0,  58.0, -45.0,   5.0,  30.0, -45.0,   5.0, };
static float   s_est_fg [12] = {   0.0, -17.0,   7.0,  56.0, -17.0,   7.0,  56.0, -45.0,   7.0,   0.0, -45.0,   7.0, };

char
OPENGL__est    (char  a_value)
{
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   glColor4f (0.0, 0.0, 0.0, 1.0);
   OPENGL__polygon (s_est_bg);
   OPENGL__colors  (my.ests, a_value);
   OPENGL__polygon (s_est_fg);
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}

char               /* PURPOSE : draw two top line bullets --------------------*/
OPENGL__bullets         (void)
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
OPENGL__text            (int a_index, char *a_major, char *a_minor, char *a_text)
{
   char        temp        [LEN_LABEL];
   glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
   glPushMatrix(); {
      glColor4f (0.0, 0.0, 0.0, 1.0);
      glTranslatef (  53.0,  -8.0,  40.0);
      glTranslatef (   0.0, txf_off,   0.0);
      if (a_index >= 0) {
         snprintf (temp, 10, "%d", a_index + 1);
         yFONT_print  (my.pretty,  8, YF_MIDCEN, temp);
      }
      glTranslatef (  72.0,   0.0,   0.0);
      yFONT_print  (my.pretty,  8, YF_MIDCEN, a_major);
      glTranslatef ( 105.0,   0,   0);
      yFONT_print  (my.pretty,  8, YF_MIDCEN, a_minor);
      glTranslatef (-160.0, -10.0,   0.0);
      glTranslatef (   0.0, txf_off - 1.0,   0.0);
      glColor4f (0.0, 0.0, 0.0, 1.0);
      yFONT_printw (my.pretty,  8, YF_TOPLEF, a_text, 205, 35, txf_space);
   } glPopMatrix();
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   return 0;
}

char
OPENGL__cats            (char a_urg, char a_imp, char a_est, char a_prg)
{
   char        temp        [LEN_LABEL];
   glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
   glPushMatrix(); {
      glColor4f (0.0, 0.0, 0.0, 1.0);
      glTranslatef(  21.0, -15.0,  40.0);
      glTranslatef(   0.0, txf_off,   0.0);
      snprintf (temp, 4, "%c", a_urg);
      yFONT_print (my.pretty,  9, YF_BASCEN, temp);
      glTranslatef(  12.0, -12.0,   0.0);
      snprintf (temp, 4, "%c", a_imp);
      yFONT_print (my.pretty,  9, YF_BASCEN, temp);
      glTranslatef(  12.0, -12.0,   0.0);
      snprintf (temp, 4, "%c", a_est);
      yFONT_print (my.pretty,  9, YF_BASCEN, temp);
      glTranslatef( 247.0,   2.0,   0.0);
      snprintf (temp, 4, "%c", a_prg);
      yFONT_print (my.pretty,  8, YF_MIDCEN, temp);
   } glPopMatrix();
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   return 0;
}

char
OPENGL__debug           (int a_total, int a_seq, int a_line)
{
   char        temp        [LEN_LABEL];
   glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
   glPushMatrix(); {
      if (my.lines == 'y') {
         glTranslatef ( -15.0,   0.0,   0.0);
         snprintf (temp, 4, "%d", a_total);
         yFONT_print (my.pretty,  8, YF_MIDCEN, temp);
         glTranslatef ( -15.0,   0.0,   0.0);
         snprintf (temp, 4, "%d", a_seq);
         yFONT_print (my.pretty,  8, YF_MIDCEN, temp);
         glTranslatef ( -15.0,   0.0,   0.0);
         snprintf (temp, 4, "%d", a_line);
         yFONT_print (my.pretty,  8, YF_MIDCEN, temp);
      }
   } glPopMatrix();
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   return 0;
}

/*> char                                                                                         <* 
 *> OPENGL__text_OLD        (int a_index)                                                        <* 
 *> {                                                                                            <* 
 *>    char  temp[100];                                                                          <* 
 *>    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);                                              <* 
 *>    /+---(major text)----------------------------+/                                           <* 
 *>    glPushMatrix(); {                                                                         <* 
 *>       glColor4f (0.0, 0.0, 0.0, 1.0);                                                        <* 
 *>       glTranslatef(  53.0,  -8.0,  40.0);                                                    <* 
 *>       glTranslatef(   0.0, txf_off,   0.0);                                                  <* 
 *>       /+> snprintf(temp, 10, "%1d/%1d", a_index + 1, g_ntask - 1);                     <+/   <* 
 *>       snprintf(temp, 10, "%d", a_index + 1);                                                 <* 
 *>       /+> if (a_index < g_ntask - 1) yFONT_print  (my.pretty,  7, YF_MIDCEN, temp);    <+/   <* 
 *>       yFONT_print  (my.pretty,  8, YF_MIDCEN, temp);                                         <* 
 *>       glTranslatef(  72.0,   0.0,   0.0);                                                    <* 
 *>       yFONT_print  (my.pretty,  8, YF_MIDCEN, g_tasks [a_index].one);                        <* 
 *>       glTranslatef( 105.0,   0,   0);                                                        <* 
 *>       yFONT_print  (my.pretty,  8, YF_MIDCEN, g_tasks [a_index].two);                        <* 
 *>       glTranslatef(-160.0, -10.0,   0.0);                                                    <* 
 *>       glTranslatef(   0.0, txf_off - 1.0,   0.0);                                            <* 
 *>       glColor4f (0.0, 0.0, 0.0, 1.0);                                                        <* 
 *>       yFONT_printw (my.pretty,  8, YF_TOPLEF, g_tasks [a_index].txt, 205, 35, txf_space);    <* 
 *>    } glPopMatrix();                                                                          <* 
 *>    /+---(letters)-------------------------------+/                                           <* 
 *>    glPushMatrix(); {                                                                         <* 
 *>       glColor4f (0.0, 0.0, 0.0, 1.0);                                                        <* 
 *>       glTranslatef(  21.0, -15.0,  40.0);                                                    <* 
 *>       glTranslatef(   0.0, txf_off,   0.0);                                                  <* 
 *>       snprintf(temp, 4, "%c", g_tasks [a_index].urg);                                        <* 
 *>       yFONT_print(my.pretty,  9, YF_BASCEN, temp);                                           <* 
 *>       glTranslatef(  12.0, -12.0,   0.0);                                                    <* 
 *>       snprintf(temp, 4, "%c", g_tasks [a_index].imp);                                        <* 
 *>       yFONT_print(my.pretty,  9, YF_BASCEN, temp);                                           <* 
 *>       glTranslatef(  12.0, -12.0,   0.0);                                                    <* 
 *>       snprintf(temp, 4, "%c", g_tasks [a_index].est);                                        <* 
 *>       yFONT_print(my.pretty,  9, YF_BASCEN, temp);                                           <* 
 *>       glTranslatef( 247.0,   2.0,   0.0);                                                    <* 
 *>       snprintf(temp, 4, "%c", g_tasks [a_index].prg);                                        <* 
 *>       yFONT_print(my.pretty,  8, YF_MIDCEN, temp);                                           <* 
 *>       if (my.lines == 'y') {                                                                 <* 
 *>          glTranslatef( -15.0,   0.0,   0.0);                                                 <* 
 *>          snprintf(temp, 4, "%d", g_ntask);                                                   <* 
 *>          yFONT_print(my.pretty,  8, YF_MIDCEN, temp);                                        <* 
 *>          glTranslatef( -15.0,   0.0,   0.0);                                                 <* 
 *>          snprintf(temp, 4, "%d", g_tasks [a_index].seq);                                     <* 
 *>          yFONT_print(my.pretty,  8, YF_MIDCEN, temp);                                        <* 
 *>          glTranslatef( -15.0,   0.0,   0.0);                                                 <* 
 *>          snprintf(temp, 4, "%d", g_tasks [a_index].line);                                    <* 
 *>          yFONT_print(my.pretty,  8, YF_MIDCEN, temp);                                        <* 
 *>       }                                                                                      <* 
 *>    } glPopMatrix();                                                                          <* 
 *>    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);                                        <* 
 *>    return 0;                                                                                 <* 
 *> }                                                                                            <*/

char               /* PURPOSE : draw two and bottom borders ------------------*/
OPENGL__borders         (void)
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
OPENGL__card     (int a_index)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   DEBUG_GRAF   yLOG_value    ("a_index"   , a_index);
   --rce;  if (a_index < 0) {
      DEBUG_GRAF   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_GRAF   yLOG_value    ("g_ntask"   , g_ntask);
   --rce;  if (a_index >= g_ntask) {
      DEBUG_GRAF   yLOG_exitr    (__FUNCTION__, rce);
      return rce;
   }
   /*> printf("draw = %2d\n", a_index);                                               <*/
   OPENGL__base    (g_tasks [a_index].prg);
   OPENGL__urg     (g_tasks [a_index].urg);
   OPENGL__est     (g_tasks [a_index].est);
   OPENGL__imp     (g_tasks [a_index].imp);
   OPENGL__bullets ();
   OPENGL__text    (a_index, g_tasks [a_index].one, g_tasks [a_index].two, g_tasks [a_index].txt);
   OPENGL__cats    (g_tasks [a_index].urg, g_tasks [a_index].imp, g_tasks [a_index].est, g_tasks [a_index].prg);
   OPENGL__debug   (g_ntask, g_tasks [a_index].seq, g_tasks [a_index].line);
   OPENGL__borders ();
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
OPENGL__clearall    (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   /*---(clear)--------------------------*/
   for (i = 0; i <= g_ntask; ++i) {
      g_tasks [i].pos  = -1;
      g_tasks [i].col  = -1;
      g_tasks [i].row  = -1;
      g_tasks [i].x    = -1;
      g_tasks [i].y    = -1;
   }
   /*---(complete)-------------------------*/
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}

char         /*--> drawing single col or row --------[------ [---------------]*/
OPENGL__colrow      (int a_max, short a_xinc, short a_yinc)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   int         c           =    0;
   short       x_pos       =    0;
   short       y_pos       =    0;
   short       x_col       =    0;
   short       x_row       =    0;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   /*---(adjust max)---------------------*/
   DEBUG_GRAF   yLOG_value    ("g_ntask"   , g_ntask);
   if (a_max >= my.nact)  a_max = my.nact;
   DEBUG_GRAF   yLOG_value    ("a_max"     , a_max);
   glPushMatrix(); {
      for (i = 0; i <= g_ntask; ++i) {
         /*---(filter)-------------------*/
         DEBUG_GRAF   yLOG_complex  ("card"      , "%2d, %c %s", i, g_tasks [i].act, g_tasks [i].txt);
         if (g_tasks [i].act != 'y')  continue;
         if (c >= a_max)              break;      
         /*---(draw)---------------------*/
         OPENGL__card (i);
         /*---(update card)--------------*/
         g_tasks [i].pos = c++;
         g_tasks [i].x   = x_pos;
         g_tasks [i].y   = y_pos;
         g_tasks [i].col = x_pos;
         g_tasks [i].row = y_pos;
         /*---(update position)----------*/
         x_pos += a_xinc;
         y_pos += a_yinc;
         if (a_xinc > 0)  x_col++;
         if (a_yinc > 0)  x_row++;
         glTranslatef (a_xinc, a_yinc, 0.0);
         /*---(output)-------------------*/
         DEBUG_GRAF   yLOG_complex  ("placed"    , "%4dp, %4dx, %4dy, %4dc, %4dr", g_tasks [i].pos, g_tasks [i].x, g_tasks [i].y, g_tasks [i].col, g_tasks [i].row);
      }
   } glPopMatrix();
   /*---(complete)-------------------------*/
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
opengl__draw_prep       (void)
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
OPENGL_normal      (void)
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
   DEBUG_GRAF   yLOG_value    ("my.tcols"  , my.tcols);
   DEBUG_GRAF   yLOG_value    ("my.trows"  , my.trows);
   /*---(draw)---------------------------*/
   switch (my.format) {
   case FORMAT_TICKER   : case FORMAT_BASELINE :
      OPENGL__colrow (my.tcols, my.c_offset,   0.0);
      break;
   case FORMAT_RSHORT   : case FORMAT_LSHORT   :
   case FORMAT_RLONG    : case FORMAT_LLONG    :
   case FORMAT_STREAMER :
      OPENGL__colrow (my.trows,   0.0, -my.r_offset);
      break;
   case FORMAT_WIDE     : case FORMAT_PROJECT  : case FORMAT_EXTRA    :
      glPushMatrix(); {
         for (j = 0; j < my.wcols; ++j) {
            glPushMatrix(); {
               for (i = 0; i < my.wrows; ++i) {
                  n = format_check (my.ccol + j + 1, i + 1);
                  if (n < 0)  continue;
                  g_tasks [n].pos = j * my.wrows + i;
                  g_tasks [n].x   = x_pos;
                  g_tasks [n].y   = y_pos;
                  g_tasks [n].col = j;
                  g_tasks [n].row = i;
                  y_pos += my.r_offset;
                  OPENGL__card (n);
                  glTranslatef(0.0, -my.r_offset,   0.0);
               }
            } glPopMatrix();
            x_pos += my.c_offset;
            glTranslatef (my.c_offset, 0.0, 0.0);
         }
      } glPopMatrix();
      break;
   }
   /*---(complete)-------------------------*/
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
OPENGL_help_one         (char a_type, int a_col)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   int         x_max       =    0;
   char       *p           = NULL;
   char        x_major     [LEN_LABEL];
   char        x_label     [LEN_LABEL];
   char        x_desc      [LEN_HUND ];
   char        x_text      [LEN_RECD ];
   char        x_abbr      =  '-';
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   /*---(prepare)------------------------*/
   DEBUG_GRAF   yLOG_char     ("a_type"    , a_type);
   --rce;  switch (a_type) {
   case 'u' : p = my.urgs;  strlcpy (x_major, "urgency"   , LEN_LABEL); break;
   case 'i' : p = my.imps;  strlcpy (x_major, "importance",  LEN_LABEL); break;
   case 'e' : p = my.ests;  strlcpy (x_major, "estimate"  , LEN_LABEL); break;
   case 'p' : p = my.prgs;  strlcpy (x_major, "progress"  , LEN_LABEL); break;
   default  : 
              DEBUG_GRAF   yLOG_exitr    (__FUNCTION__, rce);
              return rce;
   }
   DEBUG_GRAF   yLOG_info     ("p"         , p);
   x_max = strlen (p);
   DEBUG_GRAF   yLOG_value    ("x_max"     , x_max);
   /*---(draw)---------------------------*/
   glPushMatrix(); {
      DEBUG_GRAF   yLOG_value    ("a_col"     , a_col);
      DEBUG_GRAF   yLOG_value    ("c_offset"  , my.c_offset);
      glTranslatef (a_col * my.c_offset, 0.0, 0.0);
      for (i = 0; i < x_max; ++i) {
         x_abbr = p [i];
         DATA_catinfo (a_type, x_abbr, x_label, x_desc);
         sprintf (x_text, "%s, %s", x_label, x_desc);
         if (a_type == 'p')   OPENGL__base    (x_abbr);
         else                 OPENGL__base    (0);
         if (a_type == 'u')   OPENGL__urg     (x_abbr);
         else                 OPENGL__urg     (0);
         if (a_type == 'i')   OPENGL__imp     (x_abbr);
         else                 OPENGL__imp     (0);
         if (a_type == 'e')   OPENGL__est     (x_abbr);
         else                 OPENGL__est     (0);
         OPENGL__bullets ();
         OPENGL__text    (-1, x_major, x_label, x_text);
         switch (a_type) {
         case 'u' :OPENGL__cats    (x_abbr, ' ', ' ', ' ');  break;
         case 'i' :OPENGL__cats    (' ', x_abbr, ' ', ' ');  break;
         case 'e' :OPENGL__cats    (' ', ' ', x_abbr, ' ');  break;
         case 'p' :OPENGL__cats    (' ', ' ', ' ', x_abbr);  break;
         }
         OPENGL__borders ();
         glTranslatef(0.0, -my.r_offset,   0.0);
      }
   } glPopMatrix();
   /*---(complete)-------------------------*/
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
OPENGL_help_full    (void)
{
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   OPENGL_help_one  ('u', 0);
   OPENGL_help_one  ('i', 1);
   OPENGL_help_one  ('e', 2);
   OPENGL_help_one  ('p', 3);
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
OPENGL_draw        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        x_help      =  '-';
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   /*---(prepare)------------------------*/
   opengl__draw_prep ();
   /*> x_help = yVIKEYS_help ();                                                      <*/
   DEBUG_GRAF   yLOG_char     ("x_help"    , x_help);
   /*---(choose drawer)------------------*/
   switch (x_help) {
   case 'f' :
      OPENGL_help_full ();
      break;
   case 'u' : case 'i' : case 'e' : case 'p' :
      OPENGL_help_one  (x_help, 0);
      break;
   default  :
      OPENGL_normal    ();
      break;
   }
   /*---(mipmaps)------------------------*/
   yGLTEX_done (my.g_tex);
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}



/*============================--------------------============================*/
/*===----                             masking                          ----===*/
/*============================--------------------============================*/
static void      o___MASK____________________o (void) {;}

char
OPENGL_mask             (void)
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
   DEBUG_GRAF   yLOG_value    ("my.wcols"  , my.wcols);
   DEBUG_GRAF   yLOG_value    ("my.wrows"  , my.wrows);
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
         x_max = my.wcols + my.c_over;
         if (x_max >= my.nact)  x_max = my.nact;
         for (i = 0; i < x_max; ++i)  XFillRectangle (YX_DISP, bounds, gc, i * my.c_offset, my.m_offset, my.c_wide, my.r_tall);
         break;
      case FORMAT_RSHORT   : case FORMAT_LSHORT   : case FORMAT_RLONG    : case FORMAT_LLONG    :
         x_max = my.wrows;
         if (x_max >= my.nact)  x_max = my.nact;
         DEBUG_GRAF   yLOG_value    ("x_max"     , x_max);
         for (i = 0; i < x_max; ++i) {
            XFillRectangle (YX_DISP, bounds, gc,   0, i * my.r_offset, my.c_wide, my.r_tall);
         }
         break;
      case FORMAT_STREAMER :
         x_max = my.wrows + my.r_over;
         if (x_max >= my.nact)  x_max = my.nact;
         DEBUG_GRAF   yLOG_value    ("x_max"     , x_max);
         for (i = 0; i < x_max; ++i) {
            XFillRectangle (YX_DISP, bounds, gc,   0, i * my.r_offset, my.c_wide, my.r_tall);
         }
         break;
      case FORMAT_WIDE     : case FORMAT_PROJECT  : case FORMAT_EXTRA    :
         for (j = 0; j < (my.wcols + my.c_over); ++j) {
            for (i = 0; i < (my.wrows + my.r_over); ++i) {
               if (format_check (my.bcol + j + 1, i + 1) < 0)  continue;
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
   /*> if (yVIKEYS_view_showing (YVIKEYS_STATUS)) {                                    <* 
    *>    XFillRectangle (YX_DISP, bounds, gc, 0.0, my.w_ftall - 15, my.w_wide, 15);   <* 
    *> }                                                                               <*/
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
      snprintf (unit_answer, LEN_FULL, "OPENGL curr      : row %3db %3dc %3de, col %3db %3dc %3de", my.brow, my.crow, my.erow, my.bcol, my.ccol, my.ecol);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


/*============================----end-of-source---============================*/
