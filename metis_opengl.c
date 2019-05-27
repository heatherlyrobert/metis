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
 * metis  tn2#·  update mask to show menu space/box in menu mode
 * metis  dn1··  data refresh adds to mask, but not drawing texture
 * metis  tn2#·  draw main menu in circular format
 * metis  dn2x·  draw menu texture to make usage fast
 *
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


/*---(opengl objects)--------------------*/
uint      g_tex     =     0;                /* texture for image              */
uint      g_fbo     =     0;                /* framebuffer                    */
uint      g_dep     =     0;                /* depth buffer                   */







/*====================------------------------------------====================*/
/*===----                        drawing primative                     ----===*/
/*====================------------------------------------====================*/
static void      o___PRIMATIVE_______________o (void) {;}

char
OPENGL__colors          (char *a_valid, char a_color)
{
   char       *p           = NULL;
   char        i           =    0;
   p = strchr (a_valid, a_color);
   if (p == NULL)   i = 100;
   else             i = p - a_valid;
   switch (i) {
   case  0  : glColor3f (  1.000,  0.000,  0.000); break;
   case  1  : glColor3f (  1.000,  0.455,  0.000); break;
   case  2  : glColor3f (  0.800,  0.733,  0.000); break;
   case  3  : glColor3f (  0.200,  0.667,  0.200); break;
   case  4  : glColor3f (  0.000,  0.600,  0.600); break;
   case  5  : glColor3f (  0.400,  0.000,  0.600); break;
   case  6  : glColor3f (  0.800,  0.000,  0.800); break;
   case  7  : glColor3f (  0.400,  0.400,  0.400); break;
   default  : glColor3f (  0.250,  0.250,  0.250); break;
   }
   return 0;
}

char
OPENGL__polygon         (float *a_array)
{
   glPushMatrix(); {
      /*> glDisable(GL_BLEND);                                                        <*/
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
   my.fixed  = yFONT_load(face_fixed);
   if (my.fixed <  0) {
      fprintf(stderr, "Problem loading %s\n", face_fixed);
      exit(1);
   }
   my.pretty  = yFONT_load(face_pretty);
   if (my.pretty <  0) {
      fprintf(stderr, "Problem loading %s\n", face_pretty);
      exit(1);
   }
   return 0;
}

char
font_change        (void)
{
   static char xfont = 0;
   if (xfont == 0) {
      xfont = 1;
      strcpy(face_fixed, "teacher_k");
      strcpy(face_pretty, "teacher_k");
      txf_off   = -1.00;
      txf_space =  1.75;
   } else {
      xfont = 0;
      strcpy(face_fixed, "comfortaa");
      strcpy(face_pretty, "comfortaa");
      txf_off   =  0.00;
      txf_space =  1.15;
   }
   return 0;
}

char
font_delete(void)
{
   yFONT_free (my.fixed);
   yFONT_free (my.pretty);
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
   yGLTEX_init ();
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
   glViewport(0,  0, win_w, win_h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(        0,   win_w, -win_h,  0, -500.0,  500.0);
   glMatrixMode(GL_MODELVIEW);
   return 0;
}



/*============================--------------------============================*/
/*===----                          menu display                        ----===*/
/*============================--------------------============================*/
static void      o___MENU____________________o (void) {;}

static char   s_menus   [LEN_LABEL];

char
OPENGL_menu_start       (void)
{
   strlcpy (s_menus, "\\", LEN_LABEL);
   strlcpy (my.keys, ""  , LEN_LABEL);
   OPENGL_mask ();
   return 0;
}

char
OPENGL_menu_cont        (void)
{
   char        t           [LEN_LABEL];
   sprintf (t, "%c", g_minor);
   strlcat (s_menus, t, LEN_LABEL);
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
   DEBUG_GRAF   yLOG_double   ("a_wtop"     , a_wtop);
   DEBUG_GRAF   yLOG_double   ("a_wlef"     , a_wlef);
   DEBUG_GRAF   yLOG_double   ("a_wbot"     , a_wbot);
   DEBUG_GRAF   yLOG_double   ("a_wrig"     , a_wrig);
   DEBUG_GRAF   yLOG_double   ("a_ttop"     , a_ttop);
   DEBUG_GRAF   yLOG_double   ("a_tlef"     , a_tlef);
   DEBUG_GRAF   yLOG_double   ("a_tbot"     , a_tbot);
   DEBUG_GRAF   yLOG_double   ("a_trig"     , a_trig);
   /*---(draw it)------------------------*/
   glBindTexture   (GL_TEXTURE_2D, g_tex);
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

char       /* PURPOSE : place the texture in the window ----------------------*/
OPENGL_show        (void)
{
   /*---(locals)-------------------------*/
   char        rc          =    0;
   float     offset, ratio;
   float     tx1, ty1, tx2, ty2;
   float     wx1, wy1, wx2, wy2;
   float     x_tlef, x_trig, x_ttop, x_tbot;
   float     x_wlef, x_wrig, x_wtop, x_wbot;
   float     x_cnt;
   float     x_max;
   float     z;
   int       i;
   float       x_wwide     =  300;
   float       x_wtall     =   60;
   int      x_left, x_bott, x_wide, x_tall;
   int      x_xmin, x_xmax;
   int      x_ymin, x_ymax;
   char        x_mode      =  '-';
   static char x_modesave  =  '-';
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   x_mode = yVIKEYS_mode ();
   DEBUG_GRAF   yLOG_char     ("x_mode"    , x_mode);
   DEBUG_GRAF   yLOG_char     ("x_modesave", x_modesave);
   yVIKEYS_view_size     (YVIKEYS_MAIN, &x_left, &x_wide, &x_bott, &x_tall, NULL);
   DEBUG_GRAF   yLOG_complex  ("size"      , "%3dl, %3dw, %3db, %3dt", x_left, x_wide, x_bott, x_tall);
   yVIKEYS_view_bounds   (YVIKEYS_MAIN, &x_xmin, &x_xmax, &x_ymin, &x_ymax);
   DEBUG_GRAF   yLOG_complex  ("bounds"    , "%3dx to %3dx, %3dy to %3dy", x_xmin, x_xmax, x_ymin, x_ymax);
   /*---(prepare drawing)----------------*/
   DEBUG_GRAF   yLOG_value    ("g_ntask"   , g_ntask);
   DEBUG_GRAF   yLOG_value    ("my.nact"   , my.nact);
   DEBUG_GRAF   yLOG_value    ("win_h"     , win_h);
   DEBUG_GRAF   yLOG_value    ("win_w"     , win_w);
   DEBUG_GRAF   yLOG_value    ("tex_h"     , tex_h);
   DEBUG_GRAF   yLOG_value    ("tex_w"     , tex_w);
   DEBUG_GRAF   yLOG_value    ("my.wcols"  , my.wcols);
   DEBUG_GRAF   yLOG_value    ("my.wrows"  , my.wrows);
   DEBUG_GRAF   yLOG_value    ("my.tcols"  , my.tcols);
   DEBUG_GRAF   yLOG_value    ("my.trows"  , my.trows);
   DEBUG_GRAF   yLOG_value    ("my.ncols"  , my.ncols);
   DEBUG_GRAF   yLOG_value    ("my.nrows"  , my.nrows);
   DEBUG_GRAF   yLOG_value    ("my.brow"   , my.brow);
   DEBUG_GRAF   yLOG_value    ("my.crow"   , my.crow);
   DEBUG_GRAF   yLOG_value    ("my.erow"   , my.erow);
   DEBUG_GRAF   yLOG_value    ("my.bcol"   , my.bcol);
   DEBUG_GRAF   yLOG_value    ("my.ccol"   , my.ccol);
   DEBUG_GRAF   yLOG_value    ("my.ecol"   , my.ecol);
   /*---(horizontal views)---------------*/
   if (strchr("tbwpx"  , my.format) != 0) {
      DEBUG_GRAF   yLOG_note     ("horizontal formats----------");
      /*---(consistent)------------------*/
      x_wtop  = 0.0;
      x_wbot  = -win_h;
      x_ttop  = 1.0;
      x_tbot  = 0.0;
      if (my.format != 't')  x_wwide = 325;
      /*---(panel one/lef)---------------*/
      DEBUG_GRAF   yLOG_note     ("panel one/lef---------------");
      x_wlef  = 0.0;
      x_tlef  = (my.ccol  * x_wwide) / tex_w;
      if (my.ncols - my.ccol > my.wcols) {
         x_max  = my.ccol + my.wcols;
         x_cnt  = my.wcols;
         x_wrig = win_w;
      } else {
         x_max  = my.ncols;
         x_cnt  = my.ncols - my.ccol;
         x_wrig = (x_cnt / my.wcols) * win_w;
      }
      x_trig   = (x_max    * x_wwide) / tex_w;
      DEBUG_GRAF   yLOG_value    ("x_max"     , x_max);
      DEBUG_GRAF   yLOG_value    ("x_cnt"     , x_cnt);
      DEBUG_GRAF   yLOG_double   ("x_tlef"    , x_tlef);
      DEBUG_GRAF   yLOG_double   ("x_trig"    , x_trig);
      DEBUG_GRAF   yLOG_double   ("x_wlef"    , x_wlef);
      DEBUG_GRAF   yLOG_double   ("x_wrig"    , x_wrig);
      OPENGL__panel (x_wtop, x_wlef, x_wbot, x_wrig, x_ttop, x_tlef, x_tbot, x_trig);
      /*---(panel two/rig)---------------*/
      if (my.ccol > 0) {
         DEBUG_GRAF   yLOG_note     ("panel two/rig---------------");
         x_cnt   = my.wcols - x_cnt;
         x_tlef  = 0.0;
         x_trig  = (x_cnt  * x_wwide) / tex_w;
         x_wlef  = x_wrig;
         x_wrig  = (x_cnt / my.wcols) * win_w + x_wlef;
         DEBUG_GRAF   yLOG_value    ("x_cnt"     , x_cnt);
         DEBUG_GRAF   yLOG_double   ("x_tlef"    , x_tlef);
         DEBUG_GRAF   yLOG_double   ("x_trig"    , x_trig);
         DEBUG_GRAF   yLOG_double   ("x_wlef"    , x_wlef);
         DEBUG_GRAF   yLOG_double   ("x_wrig"    , x_wrig);
         OPENGL__panel (x_wtop, x_wlef, x_wbot, x_wrig, x_ttop, x_tlef, x_tbot, x_trig);
      }
   }
   /*---(vertical views)-----------------*/
   else {
      DEBUG_GRAF   yLOG_note     ("vertical formats------------");
      /*---(consistent)------------------*/
      x_wlef  = 0.0;
      x_wrig  = win_w;
      x_tlef  = 0.0;
      x_trig  = 1.0;
      /*---(panel one)-------------------*/
      DEBUG_GRAF   yLOG_note     ("panel one/top---------------");
      x_wtop  = 0.0;
      x_ttop  = 1.0 - ((my.brow  *  x_wtall) / tex_h);
      if (my.nrows - my.brow > my.wrows) {
         x_max  = my.brow + my.wrows;
         x_cnt  = my.wrows;
         x_wbot = -win_h;
      } else {
         x_max  = my.nrows;
         x_cnt  = my.nrows - my.brow;
         x_wbot = -(x_cnt / my.wrows) * win_h;
      }
      x_tbot   = 1.0 - ((x_max    *  x_wtall) / tex_h);
      DEBUG_GRAF   yLOG_value    ("x_max"     , x_max);
      DEBUG_GRAF   yLOG_value    ("x_cnt"     , x_cnt);
      DEBUG_GRAF   yLOG_double   ("x_ttop"    , x_ttop);
      DEBUG_GRAF   yLOG_double   ("x_tbot"    , x_tbot);
      DEBUG_GRAF   yLOG_double   ("x_wtop"    , x_wtop);
      DEBUG_GRAF   yLOG_double   ("x_wbot"    , x_wbot);
      OPENGL__panel (x_wtop, x_wlef, x_wbot, x_wrig, x_ttop, x_tlef, x_tbot, x_trig);
      /*---(panel two)-------------------*/
      if (my.brow > 0) {
         DEBUG_GRAF   yLOG_note     ("panel two/bot---------------");
         x_ttop  = 1.0;
         x_tbot  = 1.0 - ((my.brow  *  x_wtall) / tex_h);
         x_cnt   = my.nrows - x_cnt;
         x_wtop  = x_wbot;
         x_wbot  = x_wtop - (x_cnt / my.wrows) * win_h;
         DEBUG_GRAF   yLOG_value    ("x_cnt"     , x_cnt);
         DEBUG_GRAF   yLOG_double   ("x_ttop"    , x_ttop);
         DEBUG_GRAF   yLOG_double   ("x_tbot"    , x_tbot);
         DEBUG_GRAF   yLOG_double   ("x_wtop"    , x_wtop);
         DEBUG_GRAF   yLOG_double   ("x_wbot"    , x_wbot);
         OPENGL__panel (x_wtop, x_wlef, x_wbot, x_wrig, x_ttop, x_tlef, x_tbot, x_trig);
      }
      /*---(current)---------------------*/
      glColor4f (  1.000,  1.000,  1.000, 1.000);
      glBegin(GL_POLYGON); {
         glVertex3f (    2.0, -(my.crow - my.brow) * 60.0 -  6.0,  100.0);
         glVertex3f (   38.0, -(my.crow - my.brow) * 60.0 - 43.0,  100.0);
         glVertex3f (   25.0, -(my.crow - my.brow) * 60.0 - 43.0,  100.0);
         glVertex3f (    2.0, -(my.crow - my.brow) * 60.0 - 20.0,  100.0);
      } glEnd();
      /*---(done)------------------------*/
   }
   /*---(force the redraw)---------------*/
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
   x_modesave = x_mode;
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
   switch (a_value) {
   case '?' : case '+' : case '·' :
      glColor3f (  0.700,  0.500,  0.300);
      break;
   case '<' : case 'o' : case '>' :
      glColor3f (  0.800,  0.800,  0.800);
      break;
   case '#' : case 'x' : default  :
      glColor3f (  0.400,  0.400,  0.400);
      break;
   }
   glBegin(GL_POLYGON); {
      glVertex3f (    0.0,    0.0,   -1.0);
      glVertex3f (  300.0,    0.0,   -1.0);
      glVertex3f (  300.0,  -45.0,   -1.0);
      glVertex3f (    0.0,  -45.0,   -1.0);
   } glEnd();
   return 0;
}

/*                              ---top/left--------- ---top/right-------- ---bot/right-------- ---bot/left--------- */
static float   s_urg_bg [12] = {   0.0,   0.0,   0.0, 300.0,   0.0,   0.0, 300.0, -17.0,   0.0,   0.0, -17.0,   0.0, };
static float   s_urg_fg [12] = {   0.0,  -2.0,   2.0, 300.0,  -2.0,  12.0, 300.0, -15.0,   2.0,   0.0, -15.0,   2.0, };

char
OPENGL__urg             (char  a_value)
{
   glColor4f (  0.000,  0.000,  0.000,  1.000);
   OPENGL__polygon (s_urg_bg);
   OPENGL__colors  (my.urgs, a_value);
   OPENGL__polygon (s_urg_fg);
   return 0;
}


/*                              ---top/left--------- ---top/right-------- ---bot/right-------- ---bot/left--------- */
static float   s_imp_sh [12] = {   0.0,   0.0,  10.0,  25.0,   0.0,  10.0,  25.0, -22.5,  10.0,   0.0, -22.5,  10.0, };
static float   s_imp_bg [12] = {   0.0, -22.5,  10.0,  25.0,   0.0,  10.0,  70.0, -45.0,  10.0,  23.0, -45.0,  10.0, };
static float   s_imp_fg [12] = {   0.0,   0.0,  12.0,  22.0,   0.0,  12.0,  67.0, -45.0,  12.0,  45.0, -45.0,  12.0, };

char
OPENGL__imp             (char  a_value)
{
   glColor4f (  0.000,  0.000,  0.000,  1.000);
   OPENGL__polygon (s_imp_bg);
   OPENGL__polygon (s_imp_sh);
   OPENGL__colors  (my.imps, a_value);
   OPENGL__polygon (s_imp_fg);
   return 0;
}

/*                              ---top/left--------- ---top/right-------- ---bot/right-------- ---bot/left--------- */
static float   s_est_bg [12] = {  30.0, -15.0,   5.0,  58.0, -15.0,   5.0,  58.0, -45.0,   5.0,  30.0, -45.0,   5.0, };
static float   s_est_fg [12] = {   0.0, -17.0,   7.0,  56.0, -17.0,   7.0,  56.0, -45.0,   7.0,   0.0, -45.0,   7.0, };

char
OPENGL__est    (char  a_value)
{
   glColor4f (0.0, 0.0, 0.0, 1.0);
   OPENGL__polygon (s_est_bg);
   OPENGL__colors  (my.ests, a_value);
   OPENGL__polygon (s_est_fg);
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
OPENGL__text            (int a_index)
{
   char  temp[100];
   glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
   /*---(major text)----------------------------*/
   glPushMatrix(); {
      glColor4f (0.0, 0.0, 0.0, 1.0);
      glTranslatef(  53.0,  -8.0,  40.0);
      glTranslatef(   0.0, txf_off,   0.0);
      /*> snprintf(temp, 10, "%1d/%1d", a_index + 1, g_ntask - 1);                     <*/
      snprintf(temp, 10, "%d", a_index + 1);
      /*> if (a_index < g_ntask - 1) yFONT_print  (my.pretty,  7, YF_MIDCEN, temp);    <*/
      yFONT_print  (my.pretty,  8, YF_MIDCEN, temp);
      glTranslatef(  72.0,   0.0,   0.0);
      yFONT_print  (my.pretty,  8, YF_MIDCEN, g_tasks [a_index].one);
      glTranslatef( 105.0,   0,   0);
      yFONT_print  (my.pretty,  8, YF_MIDCEN, g_tasks [a_index].two);
      glTranslatef(-160.0, -10.0,   0.0);
      glTranslatef(   0.0, txf_off - 1.0,   0.0);
      glColor4f (0.0, 0.0, 0.0, 1.0);
      yFONT_printw (my.pretty,  8, YF_TOPLEF, g_tasks [a_index].txt, 205, 35, txf_space);
   } glPopMatrix();
   /*---(letters)-------------------------------*/
   glPushMatrix(); {
      glColor4f (0.0, 0.0, 0.0, 1.0);
      glTranslatef(  21.0, -15.0,  40.0);
      glTranslatef(   0.0, txf_off,   0.0);
      snprintf(temp, 4, "%c", g_tasks [a_index].urg);
      yFONT_print(my.pretty,  9, YF_BASCEN, temp);
      glTranslatef(  12.0, -12.0,   0.0);
      snprintf(temp, 4, "%c", g_tasks [a_index].imp);
      yFONT_print(my.pretty,  9, YF_BASCEN, temp);
      glTranslatef(  12.0, -12.0,   0.0);
      snprintf(temp, 4, "%c", g_tasks [a_index].est);
      yFONT_print(my.pretty,  9, YF_BASCEN, temp);
      glTranslatef( 247.0,   2.0,   0.0);
      snprintf(temp, 4, "%c", g_tasks [a_index].flg);
      yFONT_print(my.pretty,  8, YF_MIDCEN, temp);
      glTranslatef( -15.0,   0.0,   0.0);
      snprintf(temp, 4, "%d", g_ntask);
      yFONT_print(my.pretty,  8, YF_MIDCEN, temp);
      glTranslatef( -15.0,   0.0,   0.0);
      snprintf(temp, 4, "%d", g_tasks [a_index].line);
      yFONT_print(my.pretty,  8, YF_MIDCEN, temp);
   } glPopMatrix();
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   return 0;
}

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
   OPENGL__base    (g_tasks [a_index].flg);
   OPENGL__urg     (g_tasks [a_index].urg);
   OPENGL__est     (g_tasks [a_index].est);
   OPENGL__imp     (g_tasks [a_index].imp);
   OPENGL__bullets ();
   OPENGL__text    (a_index);
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
OPENGL_draw        (void)
{
   /*---(locals)-------------------------*/
   int       i;                             /* loop iterator -- word          */
   int       j;                             /* loop iterator -- word          */
   int       rc      = 0;                   /* simple return code             */
   int       w       = 0;
   int       pos     = 0;
   int       x_max   = 0;
   float     x_inc   = 0.0;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   /*---(create objects)-----------------*/
   /*> printf("   entered\n");                                                        <*/
   DEBUG_GRAF   yLOG_note     ("create texture");
   yGLTEX_new (&g_tex, &g_fbo, &g_dep, tex_w, tex_h);
   /*---(setup)--------------------------*/
   DEBUG_GRAF   yLOG_note     ("setup opengl view");
   yGLTEX_draw_start (g_fbo, YGLTEX_TOPLEF, tex_w, tex_h, 1.0);
   /*---(tasks)--------------------------*/
   DEBUG_GRAF   yLOG_note     ("draw tasks");
   glColor4f (0.0f, 0.0f, 0.0f, 1.0f);
   /*> printf("g_ntask = %3d\n", g_ntask);                                              <*/
   /*> printf("   starting draw\n");                                                  <*/
   DEBUG_GRAF   yLOG_value    ("my.nact"   , my.nact);
   DEBUG_GRAF   yLOG_value    ("my.tcols"  , my.tcols);
   DEBUG_GRAF   yLOG_value    ("my.trows"  , my.trows);
   switch (my.format) {
   case FORMAT_TICKER   :
      OPENGL__colrow (my.tcols, 300.0,   0.0);
      break;
   case FORMAT_BASELINE :
      OPENGL__colrow (my.tcols, 325.0,   0.0);
      break;
   case FORMAT_COLUMN   :
      OPENGL__colrow (my.trows,   0.0, -60.0);
      break;
   case FORMAT_LONG     :
      OPENGL__colrow (my.trows,   0.0, -60.0);
      break;
   case FORMAT_WIDE     :
      for (j = 0; j < my.ncols; ++j) {
         pos = 0;
         glPushMatrix(); {
            for (i = 0; i < 12; ++i) {
               if ((j * 12) + i >= MAX_CARDS) continue;
               OPENGL__card((j * 12) + i);
               glTranslatef(    0.0,  -60.0,   0.0);
               pos -= 60;
               g_tasks [j * 12 + i].pos = pos;
            }
         } glPopMatrix();
         glTranslatef(    320.0,  0.0,   0.0);
      }
      break;
   case FORMAT_PROJECT  :
      for (j = 1; j <= my.ncols; ++j) {
         /*> printf("drawing col %2d\n", j);                                       <*/
         pos = 0;
         glPushMatrix(); {
            for (i = 0; i < g_ntask; ++i) {
               if (g_tasks [i].col != j) continue;
               if (pos == 0 && g_tasks [i].row == 2) glTranslatef(    0.0,  -60.0,   0.0);
               /*> printf("   card  %3d\n", i);                                    <*/
               OPENGL__card(i);
               glTranslatef(    0.0,  -60.0,   0.0);
               pos -= 60;
               g_tasks [i].pos = pos;
            }
         } glPopMatrix();
         glTranslatef(    320.0,  0.0,   0.0);
      }
      break;
   case FORMAT_EXTRA    :
      for (j = 0; j < my.ncols; ++j) {
         pos = 0;
         glPushMatrix(); {
            for (i = 0; i < 16; ++i) {
               if ((j * 16) + i >= MAX_CARDS) continue;
               OPENGL__card((j * 16) + i);
               glTranslatef(    0.0,  -60.0,   0.0);
               pos -= 60;
               g_tasks [j * 16 + i].pos = pos;
            }
         } glPopMatrix();
         glTranslatef(    320.0,  0.0,   0.0);
      }
      break;
   }
   /*> printf("   ending draw\n");                                                    <*/
   /*> printf("width = %d\n", w);                                                     <*/
   /*---(mipmaps)------------------------*/
   yGLTEX_draw_end (g_tex);
   /*---(complete)-------------------------*/
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
   Pixmap    bounds    = XCreatePixmap(DISP, BASE, win_w, win_h, 1);
   int       x_col      = 0;
   int         x_max       =    0;
   float       x_inc       =    0;
   char        x_mode      =  '-';
   /*---(prepare)------------------------*/
   GC        gc        = XCreateGC(DISP, bounds, 0, NULL);
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   x_mode = yVIKEYS_mode ();
   DEBUG_GRAF   yLOG_char     ("x_mode"    , x_mode);
   DEBUG_GRAF   yLOG_char     ("g_major"   , g_major);
   DEBUG_GRAF   yLOG_char     ("g_minor"   , g_minor);
   DEBUG_GRAF   yLOG_char     ("my.format" , my.format);
   DEBUG_GRAF   yLOG_value    ("my.wcols"  , my.wcols);
   DEBUG_GRAF   yLOG_value    ("my.wrows"  , my.wrows);
   DEBUG_GRAF   yLOG_value    ("my.nact"   , my.nact);
   XSetForeground(DISP, gc, 0);
   XFillRectangle(DISP, bounds, gc, 0, 0, win_w, win_h);
   XSetForeground(DISP, gc, 1);
   /*---(establish mask)-----------------*/
   switch (my.format) {
   case FORMAT_TICKER   :
      x_inc = 300.0;
   case FORMAT_BASELINE :
      if (x_inc == 0.0)  x_inc = 325.0;
      DEBUG_GRAF   yLOG_value    ("x_inc"     , x_inc);
      x_max = my.wcols;
      if (x_max >= my.nact)  x_max = my.nact;
      for (i = 0; i < x_max; ++i)  XFillRectangle (DISP, bounds, gc, i * x_inc, 0, 300, 45);
      break;
   case FORMAT_COLUMN   :
   case FORMAT_LONG     :
      x_inc = 60.0;
      x_max = my.wrows;
      if (x_max >= my.nact)  x_max = my.nact;
      DEBUG_GRAF   yLOG_value    ("x_max"     , x_max);
      for (i = 0; i < x_max; ++i) {
         XFillRectangle (DISP, bounds, gc,   0, i * x_inc, 300, 45);
      }
      break;
   case FORMAT_WIDE     :
      for (j = 0; j < 4; ++j) {
         for (i = 0; i < 12; ++i) {
            if (g_tasks [j* my.wrows + i].imp != '?') XFillRectangle(DISP, bounds, gc,  j * 320, i * 60, 300, 45);
         }
      }
      break;
   case FORMAT_PROJECT  :
      for (j = 0; j < my.wcols; ++j) {
         x_col = my.ccol + j;
         if (x_col >= my.ncols) x_col -= my.ncols;
         for (i = 0; i < my.wrows; ++i) {
            for (k = 0; k < g_ntask; ++k) {
               if (g_tasks [k].col != x_col + 1)           continue;
               if (g_tasks [k].row != i + 1)              continue;
               XFillRectangle(DISP, bounds, gc,  j * 320, i * 60, 300, 45);
            }
         }
      }
      break;
   case FORMAT_EXTRA    :
      for (j = 0; j < 4; ++j) {
         for (i = 0; i < 16; ++i) {
            if (g_tasks [j * 16 + i].imp != '?') XFillRectangle(DISP, bounds, gc,  j * 320, i * 60, 300, 45);
         }
      }
      break;
   }
   /*---(check for menu)-----------------*/
   if (x_mode == SMOD_MENUS) {
      DEBUG_GRAF   yLOG_note     ("draw the main menu mask");
      XFillRectangle (DISP, bounds, gc, 10, 70, 280, 200);
   }
   /*---(set mask)-----------------------*/
   XShapeCombineMask (DISP, BASE, ShapeBounding, 0, 0, bounds, ShapeSet);
   /*---(free)---------------------------*/
   XFreePixmap (DISP, bounds);
   XFreeGC     (DISP, gc);
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
   /*---(complete)-----------------------*/
   return unit_answer;
}


/*============================----end-of-source---============================*/
