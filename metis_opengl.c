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
 *  metis  -----  tbd
 *
 */

float     alpha     = 0.0;
float     step      = 0;


/*> char   face_sm [30] = "verdana_sm";                                               <*/
/*> char   face_sm [30] = "comfortaa";                                                <*/
char   face_sm [30] = "sansation";
int    txf_sm;
char   face_bg [30] = "comfortaa";
int    txf_bg;
float  txf_off   = 0.00;
float  txf_space = 1.15;


/*---(opengl objects)--------------------*/
uint      g_tex     =     0;                /* texture for image              */
uint      g_fbo     =     0;                /* framebuffer                    */
uint      g_dep     =     0;                /* depth buffer                   */


char        /* PURPOSE : process the xwindows event stream                    */
prog_event    (void)
{
   /*---(locals)--------------------------------*/
   XKeyEvent  *key_event;
   char        x_keys      [LEN_TERSE];
   int         x_bytes     =    0;
   char        x_search    [LEN_HUND];
   my.update = 0;
   int        loop = 0;
   /*---(header)-------------------------*/
   DEBUG_LOOP   yLOG_enter    (__FUNCTION__);
   while (1) {
      XNextEvent(DISP, &EVNT);
      my.update = 1;
      my.pcol = my.ccol;
      my.prow = my.crow;
      /*---(start)----------------------------*/
      switch(EVNT.type) {
      case FocusIn:
         if ((int) (alpha * 10) == 3) alpha = 0.0;
         break;

      case FocusOut:
         if ((int) (alpha * 10) == 0) alpha = 0.3;
         break;

      case Expose:
         break;

      case ConfigureNotify:
         break;

      case KeyPress:
         key_event  = (XKeyEvent *) &EVNT;
         x_bytes = XLookupString((XKeyEvent *) &EVNT, x_keys, 5, NULL, NULL);
         if (x_bytes < 1) break;
         g_minor = x_keys [0];
         DEBUG_LOOP   yLOG_char     ("g_major"   , g_major);
         DEBUG_LOOP   yLOG_char     ("g_minor"   , g_minor);
         if (g_major == ' ') {
            /*---(mode changes)-------*/
            switch (g_minor) {
            case 'Q': return 1;                             break;
            case 'u': alpha = 0.8; g_major = 'u';           break;
            case 'i': alpha = 0.8; g_major = 'i';           break;
            case 'e': alpha = 0.8; g_major = 'e';           break;
            case 'f': alpha = 0.8; g_major = 'f';           break;
            case 'a': FILTER_clear(); return my.format;     break;
            case '/': alpha = 0.8; g_major = 's'; x_search[0] = '\0'; break;
            }
            /*---(filters and tasks)--*/
            switch (g_minor) {
            case '[':
            case '?': my.incr  = STOP;   my.move   = 0.0;   break;
            case '_': my.incr  = STOP;   my.move   = 0.0;  my.ccol = my.crow = 0; my.move = 0.0;  break;
            case 'l': my.incr  = STOP;   ++my.ccol;      break;
            case 'j': my.incr  = STOP;   ++my.crow;      break;
            case ',': my.incr  = my.play;                break;
            case '+': my.incr += my.change;              break;
            case '-': my.incr -= my.change;              break;
            case '.': my.incr  = STOP;                   break;
            case 'h': my.incr  = STOP;   --my.ccol;      break;
            case 'k': my.incr  = STOP;   --my.crow;      break;
                      /*> case ']':                                                             <*/
                      /*> case '~': my.incr  = STOP;   my.move   = 0.0; my.crow = my.nrows - 1; break;   <*/
            }
            /*---(task related)-------*/
            switch (g_minor) {
            case 'p': task_list();                       break;
            case 'r': task_refresh();                    break;
                      /*> case 'J': font_change(); task_refresh();                    break;    <*/
            }
            /*---(format changes)-----*/
            switch (g_minor) {
            case 'C' : return 'c';    break;
            case 'L' : return 'l';    break;
            case 'T' : return 't';    break;
            case 'B' : return 'b';    break;
            case '1' : return '1';    break;
            case '2' : return '2';    break;
            case 'W' : return 'w';    break;
            case 'X' : return 'x';    break;
            case 'P' : return 'p';    break;
            case '0' : my.curg = my.cimp = my.cest = ' '; strcpy(one, "all"); my.cflg = '<'; return 't'; break;
            case 'S' : my.sort = 's'; return my.format;  break;
            case 'U' : my.sort = 'u'; return my.format;  break;
            }
            /*---(verify currents)----*/
            DEBUG_GRAF   yLOG_value    ("my.ccol"   , my.ccol);
            DEBUG_GRAF   yLOG_value    ("my.crow"   , my.crow);
            range_check();

         } else {
            if (g_major == 's' && g_minor != 13) {
               char   ch  = g_minor;
               /*> printf("ch = %3d\n", ch);                                       <*/
               if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || ch == '_' || ch == 8) {
                  char   temp[10];
                  sprintf (temp, "%c", g_minor);
                  strcat  (my.ctxt, temp);
                  /*> printf("search = <<%s>>\n", x_search);                       <*/
               }
            } else {
               switch (g_major) {
               case 'u': if (strchr (my.urgs, g_minor) != 0) my.curg  = g_minor;  else my.curg  = ' '; break;
               case 'i': if (strchr (my.imps, g_minor) != 0) my.cimp  = g_minor;  else my.cimp  = ' '; break;
               case 'e': if (strchr (my.ests, g_minor) != 0) my.cest  = g_minor;  else my.cest  = ' '; break;
               case 'f': if (strchr (my.flgs, g_minor) != 0) my.cflg  = g_minor;  else my.cflg  = ' '; break;
               case 's': if (x_search[0] == '\0') strlcpy (one, "all",    LEN_LABEL);
                            else                  strlcpy (one, x_search, LEN_LABEL);
                            break;
               }
               g_major = ' ';
               alpha    = 0.0;
               /*> task_refresh();                                                 <*/
               return my.format;
            }
         }
         /*> if (my.incr >  my.mspeed)  my.incr =  my.mspeed;                         <* 
          *> if (my.incr < -my.mspeed)  my.incr = -my.mspeed;                         <* 
          *> if (strchr("ts", my.format) == 0)    my.action = 0;                      <*/
         break;

      default:
         break;
      }
      /*---(check for automation)--------*/
      /*> if (my.sigusr2 == 1) {                                                      <* 
       *>    my.sigusr2 = 0;                                                          <* 
       *>    font_change ();                                                          <* 
       *>    task_refresh();                                                          <* 
       *> }                                                                           <*/
      /*> if (my.sighup == 1) {                                                       <* 
       *>    my.sighup = 0;                                                           <* 
       *>    task_refresh();                                                          <* 
       *> }                                                                           <*/
      /*> if (strchr("ts", my.format) != 0) {                                         <* 
       *>    if (my.action)  my.move -=  my.incr;                                     <* 
       *>    if (my.move <  0.0) my.move +=  1.0;                                     <* 
       *>    if (my.move >  1.0) my.move -=  1.0;                                     <* 
       *> }                                                                           <*/
      /*> printf ("action = %1d, move = %8.6f, increment = %8.6f\n", my.action, my.move, my.incr);   <*/
      /*---(timer)----------------------------------------*/
      /*> if ((my.action == 1 && my.incr != 0) || my.update == 1) {                             <* 
       *>    /+> printf ("loop %4d\n", ++loop);                                           <+/   <* 
       *>    draw_texture();                                                                    <* 
       *>    if (my.format == 'p' && my.ccol != my.pcol) mask();                                <* 
       *> }                                                                                     <*/
      DEBUG_LOOP   yLOG_value    ("my.ccol"   , my.ccol);
      my.update = 0;
      draw_texture();
      /*> usleep( 5000);                                                              <*/
      /*> usleep(  50);                                                               <*/
   }
   /*---(complete)------------------------------*/
   DEBUG_LOOP   yLOG_exit     (__FUNCTION__);
   return 0;
}


/*====================------------------------------------====================*/
/*===----                             fonts                            ----===*/
/*====================------------------------------------====================*/
static void      o___FONTS___________________o (void) {;}

char
font_load(void)
{
   txf_sm  = yFONT_load(face_sm);
   if (txf_sm <  0) {
      fprintf(stderr, "Problem loading %s\n", face_sm);
      exit(1);
   }
   txf_bg  = yFONT_load(face_bg);
   if (txf_bg <  0) {
      fprintf(stderr, "Problem loading %s\n", face_bg);
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
      strcpy(face_bg, "teacher_k");
      strcpy(face_sm, "teacher_k");
      txf_off   = -1.00;
      txf_space =  1.75;
   } else {
      xfont = 0;
      strcpy(face_bg, "comfortaa");
      strcpy(face_sm, "comfortaa");
      txf_off   =  0.00;
      txf_space =  1.15;
   }
   return 0;
}

char
font_delete(void)
{
   yFONT_free(txf_sm);
   yFONT_free(txf_bg);
   return 0;
}




/*============================--------------------============================*/
/*===----                             draw                             ----===*/
/*============================--------------------============================*/
static void      o___OPENGL__________________o (void) {;}

char       /* PURPOSE : place the texture in the window ----------------------*/
draw_texture_OLD   (void)
{
   /*---(locals)-------------------------*/
   float     offset, ratio;
   float     tx1, ty1, tx2, ty2;
   float     wx1, wy1, wx2, wy2;
   float     z;
   int       i;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   /*---(horizontal views)---------------*/
   if (strchr("tbwpx"  , my.format) != 0) {
      offset    = (my.ccol * 320.0) / tex_w;
      if (my.format == 't') offset    = (my.ccol * 300.0) / tex_w;
      if (my.format == 'b') offset    = (my.ccol * 325.0) / tex_w;
      offset += my.move;
      ratio   = (float) win_w / tex_w;
      ty1     = 0.0;
      ty2     = 1.0;
      tx1     = 0.0 + offset;
      tx2     = 0.0 + offset + ratio;
   }
   /*---(vertical views)-----------------*/
   else {
      offset    = (my.crow * 60.0) / tex_h;
      if (my.format == 's') offset    = (my.crow * 44.0) / tex_h;
      offset += my.move;
      ratio   = (float) win_h / tex_h;
      ty1     = 1.0 - offset - ratio;
      ty2     = 1.0 - offset;
      tx1     = 0.0;
      tx2     = 1.0;
   }
   DEBUG_GRAF   yLOG_value    ("my.ccol"   , my.ccol);
   DEBUG_GRAF   yLOG_value    ("my.crow"   , my.crow);
   DEBUG_GRAF   yLOG_double   ("offset"    , offset);
   DEBUG_GRAF   yLOG_double   ("ratio"     , ratio);
   DEBUG_GRAF   yLOG_double   ("ty1"       , ty1);
   DEBUG_GRAF   yLOG_double   ("ty2"       , ty2);
   DEBUG_GRAF   yLOG_double   ("tx1"       , tx1);
   DEBUG_GRAF   yLOG_double   ("tx2"       , tx2);
   /*---(window coordinates)-------------*/
   wx1     = 0.0;
   wx2     = win_w;
   wy1     = 0.0;
   wy2     = -win_h;
   z       = -10.0;
   /*> printf("win_h = %d, tex_h = %d, ratio = %f, win_w = %d, tex_w = %d\n", win_h, tex_h, ratio, win_w, tex_w);   <*/
   /*> printf("wy1 = %f, wy2 = %f, ty1 = %f, ty2 = %f\n", wy1, wy2, ty1, ty2);        <*/
   /*---(prepare drawing)----------------*/
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();
   /*---(draw it)------------------------*/
   glBindTexture   (GL_TEXTURE_2D, g_tex);
   glBegin(GL_POLYGON); {
      glTexCoord2f ( tx1,  ty2);
      glVertex3f   ( wx1,  wy1,    z);
      glTexCoord2f ( tx2,  ty2);
      glVertex3f   ( wx2,  wy1,    z);
      glTexCoord2f ( tx2,  ty1);
      glVertex3f   ( wx2,  wy2,    z);
      glTexCoord2f ( tx1,  ty1);
      glVertex3f   ( wx1,  wy2,    z);
   } glEnd();
   glBindTexture   (GL_TEXTURE_2D, 0);
   z = -5.0;
   /*---(shade if not-focused)-----------*/
   if (g_major != ' ') {
      glColor4f (0.0f, 0.0f, 0.0f, 0.5);
      glBegin(GL_POLYGON); {
         glVertex3f   ( wx1,  wy1,    z);
         glVertex3f   ( wx2,  wy1,    z);
         glVertex3f   ( wx2,  wy2,    z);
         glVertex3f   ( wx1,  wy2,    z);
      } glEnd();
   }
   /*---(force the redraw)---------------*/
   glXSwapBuffers(DISP, BASE);
   glFlush();
   /*---(complete)-------------------------*/
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
draw__panel          (float a_wtop, float a_wlef, float a_wbot, float a_wrig, float a_ttop, float a_tlef, float a_tbot, float a_trig)
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
/*> wx1     = 0.0;                                                                                                         <* 
 *> wx2     = win_w;                                                                                                       <* 
 *> wy1     = 0.0;                                                                                                         <* 
 *> wy2     = -win_h;                                                                                                      <* 
 *> z       = -10.0;                                                                                                       <* 
 *> /+> printf("win_h = %d, tex_h = %d, ratio = %f, win_w = %d, tex_w = %d\n", win_h, tex_h, ratio, win_w, tex_w);   <+/   <* 
 *> /+> printf("wy1 = %f, wy2 = %f, ty1 = %f, ty2 = %f\n", wy1, wy2, ty1, ty2);        <+/                                 <* 
 *> /+---(prepare drawing)----------------+/                                                                               <* 
 *> glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                                                                    <* 
 *> glLoadIdentity();                                                                                                      <* 
 *> /+---(draw it)------------------------+/                                                                               <* 
 *> glBindTexture   (GL_TEXTURE_2D, g_tex);                                                                                <* 
 *> glBegin(GL_POLYGON); {                                                                                                 <* 
 *>    glTexCoord2f ( tx1,  ty2);                                                                                          <* 
 *>    glVertex3f   ( wx1,  wy1,    z);                                                                                    <* 
 *>    glTexCoord2f ( tx2,  ty2);                                                                                          <* 
 *>    glVertex3f   ( wx2,  wy1,    z);                                                                                    <* 
 *>    glTexCoord2f ( tx2,  ty1);                                                                                          <* 
 *>    glVertex3f   ( wx2,  wy2,    z);                                                                                    <* 
 *>    glTexCoord2f ( tx1,  ty1);                                                                                          <* 
 *>    glVertex3f   ( wx1,  wy2,    z);                                                                                    <* 
 *> } glEnd();                                                                                                             <*/

char       /* PURPOSE : place the texture in the window ----------------------*/
draw_texture       (void)
{
   /*---(locals)-------------------------*/
   float     offset, ratio;
   float     tx1, ty1, tx2, ty2;
   float     wx1, wy1, wx2, wy2;
   float     x_tlef, x_trig, x_ttop, x_tbot;
   float     x_wlef, x_wrig, x_wtop, x_wbot;
   float     x_cnt;
   float     x_max;
   float     z;
   int       i;
   float       x_wide      =  300;
   float       x_tall      =   60;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   /*---(prepare drawing)----------------*/
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();
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
   DEBUG_GRAF   yLOG_value    ("my.crow"   , my.crow);
   DEBUG_GRAF   yLOG_value    ("my.ccol"   , my.ccol);
   /*---(horizontal views)---------------*/
   if (strchr("tbwpx"  , my.format) != 0) {
      DEBUG_GRAF   yLOG_note     ("horizontal formats----------");
      /*---(consistent)------------------*/
      x_wtop  = 0.0;
      x_wbot  = -win_h;
      x_ttop  = 1.0;
      x_tbot  = 0.0;
      if (my.format != 't')  x_wide = 325;
      /*---(panel one/lef)---------------*/
      DEBUG_GRAF   yLOG_note     ("panel one/lef---------------");
      x_wlef  = 0.0;
      x_tlef  = (my.ccol  * x_wide) / tex_w;
      if (my.ncols - my.ccol > my.wcols) {
         x_max  = my.ccol + my.wcols;
         x_cnt  = my.wcols;
         x_wrig = win_w;
      } else {
         x_max  = my.ncols;
         x_cnt  = my.ncols - my.ccol;
         x_wrig = (x_cnt / my.wcols) * win_w;
      }
      x_trig   = (x_max    * x_wide) / tex_w;
      DEBUG_GRAF   yLOG_value    ("x_max"     , x_max);
      DEBUG_GRAF   yLOG_value    ("x_cnt"     , x_cnt);
      DEBUG_GRAF   yLOG_double   ("x_tlef"    , x_tlef);
      DEBUG_GRAF   yLOG_double   ("x_trig"    , x_trig);
      DEBUG_GRAF   yLOG_double   ("x_wlef"    , x_wlef);
      DEBUG_GRAF   yLOG_double   ("x_wrig"    , x_wrig);
      draw__panel (x_wtop, x_wlef, x_wbot, x_wrig, x_ttop, x_tlef, x_tbot, x_trig);
      /*---(panel two/rig)---------------*/
      if (my.ccol > 0) {
         DEBUG_GRAF   yLOG_note     ("panel two/rig---------------");
         x_cnt   = my.wcols - x_cnt;
         x_tlef  = 0.0;
         x_trig  = (x_cnt  * x_wide) / tex_w;
         x_wlef  = x_wrig;
         x_wrig  = (x_cnt / my.wcols) * win_w + x_wlef;
         DEBUG_GRAF   yLOG_value    ("x_cnt"     , x_cnt);
         DEBUG_GRAF   yLOG_double   ("x_tlef"    , x_tlef);
         DEBUG_GRAF   yLOG_double   ("x_trig"    , x_trig);
         DEBUG_GRAF   yLOG_double   ("x_wlef"    , x_wlef);
         DEBUG_GRAF   yLOG_double   ("x_wrig"    , x_wrig);
         draw__panel (x_wtop, x_wlef, x_wbot, x_wrig, x_ttop, x_tlef, x_tbot, x_trig);
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
      x_ttop  = 1.0 - ((my.crow  *  x_tall) / tex_h);
      if (my.nrows - my.crow > my.wrows) {
         x_max  = my.crow + my.wrows;
         x_cnt  = my.wrows;
         x_wbot = -win_h;
      } else {
         x_max  = my.nrows;
         x_cnt  = my.nrows - my.crow;
         x_wbot = -(x_cnt / my.wrows) * win_h;
      }
      x_tbot   = 1.0 - ((x_max    *  x_tall) / tex_h);
      DEBUG_GRAF   yLOG_value    ("x_max"     , x_max);
      DEBUG_GRAF   yLOG_value    ("x_cnt"     , x_cnt);
      DEBUG_GRAF   yLOG_double   ("x_ttop"    , x_ttop);
      DEBUG_GRAF   yLOG_double   ("x_tbot"    , x_tbot);
      DEBUG_GRAF   yLOG_double   ("x_wtop"    , x_wtop);
      DEBUG_GRAF   yLOG_double   ("x_wbot"    , x_wbot);
      draw__panel (x_wtop, x_wlef, x_wbot, x_wrig, x_ttop, x_tlef, x_tbot, x_trig);
      /*---(panel two)-------------------*/
      if (my.crow > 0) {
         DEBUG_GRAF   yLOG_note     ("panel two/bot---------------");
         x_ttop  = 1.0;
         x_tbot  = 1.0 - ((my.crow  *  x_tall) / tex_h);
         x_cnt   = my.nrows - x_cnt;
         x_wtop  = x_wbot;
         x_wbot  = x_wtop - (x_cnt / my.wrows) * win_h;
         DEBUG_GRAF   yLOG_value    ("x_cnt"     , x_cnt);
         DEBUG_GRAF   yLOG_double   ("x_ttop"    , x_ttop);
         DEBUG_GRAF   yLOG_double   ("x_tbot"    , x_tbot);
         DEBUG_GRAF   yLOG_double   ("x_wtop"    , x_wtop);
         DEBUG_GRAF   yLOG_double   ("x_wbot"    , x_wbot);
         draw__panel (x_wtop, x_wlef, x_wbot, x_wrig, x_ttop, x_tlef, x_tbot, x_trig);
      }
   }
   /*---(force the redraw)---------------*/
   glXSwapBuffers(DISP, BASE);
   glFlush();
   /*---(complete)-------------------------*/
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}

char               /* PURPOSE : create a texture -----------------------------*/
texture_create     (void)
{
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   /*---(generate)-----------------------*/
   glGenFramebuffersEXT         (1, &g_fbo);
   glGenTextures                (1, &g_tex);
   glGenRenderbuffersEXT        (1, &g_dep);
   /*---(bind)---------------------------*/
   DEBUG_GRAF   yLOG_note     ("bind");
   glBindFramebufferEXT         (GL_FRAMEBUFFER_EXT,  g_fbo);
   glBindTexture                (GL_TEXTURE_2D,       g_tex);
   /*---(settings)-----------------------*/
   DEBUG_GRAF   yLOG_note     ("settings");
   glTexParameteri              (GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri              (GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri              (GL_TEXTURE_2D,  GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri              (GL_TEXTURE_2D,  GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexEnvi                    (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
   glTexParameteri              (GL_TEXTURE_2D,  GL_GENERATE_MIPMAP, GL_TRUE);
   /*---(copy)---------------------------*/
   DEBUG_GRAF   yLOG_note     ("allocate");
   glTexImage2D                 (GL_TEXTURE_2D, 0, GL_RGBA, tex_w, tex_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
   DEBUG_GRAF   yLOG_note     ("copy");
   glFramebufferTexture2DEXT    (GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, g_tex, 0);
   /*---(depth)--------------------------*/
   DEBUG_GRAF   yLOG_note     ("depth");
   glBindRenderbufferEXT        (GL_RENDERBUFFER_EXT, g_dep);
   glRenderbufferStorageEXT     (GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, tex_w, tex_h);
   glFramebufferRenderbufferEXT (GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, g_dep);
   /*---(unbind)-------------------------*/
   DEBUG_GRAF   yLOG_note     ("unbind");
   glBindFramebufferEXT         (GL_FRAMEBUFFER_EXT, 0);
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}

char               /* PURPOSE : delete a framebuffer -------------------------*/
texture_free       (void)
{
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   /*---(generate)-----------------------*/
   glDeleteTextures                (1, &g_tex);
   glDeleteRenderbuffersEXT        (1, &g_dep);
   glDeleteFramebuffersEXT         (1, &g_fbo);
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
draw_init(void)
{
   glShadeModel(GL_SMOOTH);
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glClearDepth(1.0f);
   glEnable(GL_TEXTURE_2D);    /* NEW */
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_ALPHA_TEST);
   glEnable(GL_BLEND);
   glAlphaFunc(GL_GEQUAL, 0.0125);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
   /*---(special polygon antialiasing)----------*/
   /*> glEnable(GL_POLYGON_SMOOTH);                                                   <*/
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
   glFlush();
   return 0;
}

char
OPENGL__colrow      (int a_max, int a_xinc, int a_yinc)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   int         x_pos       =    0;
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
   /*---(adjust max)---------------------*/
   DEBUG_GRAF   yLOG_value    ("g_ntask"   , g_ntask);
   if (a_max >= my.nact)  a_max = my.nact;
   DEBUG_GRAF   yLOG_value    ("a_max"     , a_max);
   glPushMatrix(); {
      for (i = 0; i <= g_ntask; ++i) {
         DEBUG_GRAF   yLOG_complex  ("card"      , "%2d, %c %s", i, g_tasks [i].act, g_tasks [i].txt);
         if (g_tasks [i].act != 'y')  continue;
         if (i > a_max)               break;      
         draw_card (i);
         glTranslatef (a_xinc, a_yinc, 0.0);
         x_pos += (a_xinc + a_yinc);
         g_tasks [i].pos = x_pos;
         DEBUG_GRAF   yLOG_pair     (i           , x_pos);
      }
   } glPopMatrix();
   /*---(complete)-------------------------*/
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
draw_main          (void)
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
   texture_create();
   /*---(setup)--------------------------*/
   DEBUG_GRAF   yLOG_note     ("setup opengl view");
   glViewport(0,  0, tex_w, tex_h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(        0,   tex_w, -tex_h,  0, -500.0,  500.0);
   glMatrixMode(GL_MODELVIEW);
   glBindTexture   (GL_TEXTURE_2D, 0);
   glBindFramebufferEXT         (GL_FRAMEBUFFER_EXT,  g_fbo);
   /*> printf("   bound texture\n");                                                  <*/
   /*---(draw)---------------------------*/
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
               draw_card((j * 12) + i);
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
               draw_card(i);
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
               draw_card((j * 16) + i);
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
   glBindFramebufferEXT         (GL_FRAMEBUFFER_EXT, 0);
   glBindTexture   (GL_TEXTURE_2D, g_tex);
   glGenerateMipmapEXT(GL_TEXTURE_2D);
   glBindTexture   (GL_TEXTURE_2D, 0);
   /*> printf("   draw complete\n");                                                  <*/
   /*---(complete)-------------------------*/
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
draw_title    (void)
{
   /*---(locals)-----------+-----------+-*/
   char        msg         [200];
   /*---(color)--------------------------*/
   glColor3f(  0.800,  0.733,  0.000);
   glBegin(GL_POLYGON); {
      glVertex3f(    2.0,   -2.0,   -1.0);
      glVertex3f(  298.0,   -2.0,   -1.0);
      glVertex3f(  298.0,  -14.0,   -1.0);
      glVertex3f(    2.0,  -14.0,   -1.0);
   } glEnd();
   glColor3f(  0.600,  0.600,  0.600);
   glBegin(GL_POLYGON); {
      glVertex3f(    2.0,  -15.0,   -1.0);
      glVertex3f(  298.0,  -15.0,   -1.0);
      glVertex3f(  298.0,  -44.0,   -1.0);
      glVertex3f(    2.0,  -44.0,   -1.0);
   } glEnd();
   /*---(major text)---------------------*/
   glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
   glPushMatrix(); {
      glColor4f (0.0, 0.0, 0.0, 1.0);
      glTranslatef( 150.0,  -8.0,  20.0);
      glTranslatef(   0.0, txf_off,   0.0);
      yFONT_print  (txf_sm,  7, YF_MIDCEN, "metis, goddess of cunning and wise-counsel");
      glTranslatef(-145.0, -14.0,  20.0);
      snprintf(msg, 100, "%-3d : u=<%c>, i=<%c>, e=<%c>, f=<%c>, <<%s>>", g_ntask, my.curg, my.cimp, my.cest, my.cflg, one);
      yFONT_print  (txf_sm,  7, YF_MIDLEF, msg);
      glTranslatef(   0.0, -14.0,  20.0);
      yFONT_print  (txf_sm,  7, YF_MIDLEF, "mov=jk_G, spc=JrQ, pos=CL12PWXSTB, ani=,.+-");
   } glPopMatrix();
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   /*---(complete)-----------------------*/
   return 0;
}

char
draw_card     (int a_index)
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
   card_base  (g_tasks [a_index].flg);
   urgency    (g_tasks [a_index].urg);
   estimate   (g_tasks [a_index].est);
   importance (g_tasks [a_index].imp);
   bullets    ();
   text       (a_index);
   borders    ();
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
draw_resize(uint a_w, uint a_h)
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
/*===----                            widgits                           ----===*/
/*============================--------------------============================*/
static void      o___WIDGITS_________________o (void) {;}

char
text          (int a_index)
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
      /*> if (a_index < g_ntask - 1) yFONT_print  (txf_sm,  7, YF_MIDCEN, temp);    <*/
      yFONT_print  (txf_sm,  8, YF_MIDCEN, temp);
      glTranslatef(  72.0,   0.0,   0.0);
      yFONT_print  (txf_sm,  8, YF_MIDCEN, g_tasks [a_index].one);
      glTranslatef( 105.0,   0,   0);
      yFONT_print  (txf_sm,  8, YF_MIDCEN, g_tasks [a_index].two);
      glTranslatef(-160.0, -10.0,   0.0);
      glTranslatef(   0.0, txf_off - 1.0,   0.0);
      glColor4f (0.0, 0.0, 0.0, 1.0);
      yFONT_printw (txf_sm,  8, YF_TOPLEF, g_tasks [a_index].txt, 205, 35, txf_space);
   } glPopMatrix();
   /*---(letters)-------------------------------*/
   glPushMatrix(); {
      glColor4f (0.0, 0.0, 0.0, 1.0);
      glTranslatef(  21.0, -15.0,  40.0);
      glTranslatef(   0.0, txf_off,   0.0);
      snprintf(temp, 4, "%c", g_tasks [a_index].urg);
      yFONT_print(txf_sm,  9, YF_BASCEN, temp);
      glTranslatef(  12.0, -12.0,   0.0);
      snprintf(temp, 4, "%c", g_tasks [a_index].imp);
      yFONT_print(txf_sm,  9, YF_BASCEN, temp);
      glTranslatef(  12.0, -12.0,   0.0);
      snprintf(temp, 4, "%c", g_tasks [a_index].est);
      yFONT_print(txf_sm,  9, YF_BASCEN, temp);
      glTranslatef( 247.0,   2.0,   0.0);
      snprintf(temp, 4, "%c", g_tasks [a_index].flg);
      yFONT_print(txf_bg,  8, YF_MIDCEN, temp);
      glTranslatef( -15.0,   0.0,   0.0);
      snprintf(temp, 4, "%d", g_ntask);
      yFONT_print(txf_bg,  8, YF_MIDCEN, temp);
      glTranslatef( -15.0,   0.0,   0.0);
      snprintf(temp, 4, "%d", g_tasks [a_index].line);
      yFONT_print(txf_bg,  8, YF_MIDCEN, temp);
   } glPopMatrix();
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   return 0;
}

char
colors             (char *a_valid, char a_color)
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

char               /* PURPOSE : draw two and bottom borders ------------------*/
borders            (void)
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

char               /* PURPOSE : draw two top line bullets --------------------*/
bullets            (void)
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

/*                              ---top/left--------- ---top/right-------- ---bot/right-------- ---bot/left--------- */
static float   s_urg_bg [12] = {   0.0,   0.0,   0.0, 300.0,   0.0,   0.0, 300.0, -17.0,   0.0,   0.0, -17.0,   0.0, };
static float   s_urg_fg [12] = {   0.0,  -2.0,   2.0, 300.0,  -2.0,  12.0, 300.0, -15.0,   2.0,   0.0, -15.0,   2.0, };

/*                              ---top/left--------- ---top/right-------- ---bot/right-------- ---bot/left--------- */
static float   s_est_bg [12] = {  30.0, -15.0,   5.0,  58.0, -15.0,   5.0,  58.0, -45.0,   5.0,  30.0, -45.0,   5.0, };
static float   s_est_fg [12] = {   0.0, -17.0,   7.0,  56.0, -17.0,   7.0,  56.0, -45.0,   7.0,   0.0, -45.0,   7.0, };

/*                              ---top/left--------- ---top/right-------- ---bot/right-------- ---bot/left--------- */
/*> static float   s_imp_bg [12] = { -23.0,   0.0,  10.0,  25.0,   0.0,  10.0,  70.0, -45.0,  10.0,  23.0, -45.0,  10.0, };   <*/
static float   s_imp_sh [12] = {   0.0,   0.0,  10.0,  25.0,   0.0,  10.0,  25.0, -22.5,  10.0,   0.0, -22.5,  10.0, };
static float   s_imp_bg [12] = {   0.0, -22.5,  10.0,  25.0,   0.0,  10.0,  70.0, -45.0,  10.0,  23.0, -45.0,  10.0, };
static float   s_imp_fg [12] = {   0.0,   0.0,  12.0,  22.0,   0.0,  12.0,  67.0, -45.0,  12.0,  45.0, -45.0,  12.0, };

char
urgency     (char  a_value)
{
   glColor4f (  0.000,  0.000,  0.000,  1.000);
   OPENGL__polygon (s_urg_bg);
   colors (my.urgs, a_value);
   OPENGL__polygon (s_urg_fg);
   return 0;
}

char
importance  (char  a_value)
{
   glColor4f (  0.000,  0.000,  0.000,  1.000);
   OPENGL__polygon (s_imp_bg);
   OPENGL__polygon (s_imp_sh);
   colors (my.imps, a_value);
   OPENGL__polygon (s_imp_fg);
   return 0;
}

char
estimate    (char  a_value)
{
   glColor4f (0.0, 0.0, 0.0, 1.0);
   OPENGL__polygon (s_est_bg);
   colors (my.ests, a_value);
   OPENGL__polygon (s_est_fg);
   return 0;
}

char          /*----: lay down the base color --------------------------------*/
card_base          (char  a_value)
{
   /*> switch (a_value) {                                                             <* 
    *> case '?' : case '+' : case '·' :                                               <* 
    *>    glColor3f(  0.700,  0.500,  0.700); break;                                  <* 
    *>    glColor3f(  0.800,  0.800,  0.800);                                         <* 
    *>    break;                                                                      <* 
    *> case '<' : glColor3f(  0.900,  0.500,  0.500); break;                          <* 
    *> case 'o' : glColor3f(  0.500,  0.900,  0.900); break;                          <* 
    *> case '>' : glColor3f(  0.700,  0.500,  0.700); break;                          <* 
    *> case '-' : glColor3f(  0.500,  0.900,  0.500); break;                          <* 
    *> case '#' : glColor3f(  0.000,  0.600,  0.400); break;                          <* 
    *> case 'x' : glColor3f(  0.450,  0.450,  0.450); break;                          <* 
    *> default  : glColor3f(  0.400,  0.400,  0.400); break;                          <* 
    *> }                                                                              <*/
   switch (a_value) {
   case '?' : case '+' : case '·' :
      glColor3f(  0.700,  0.500,  0.300);
      break;
   case '<' : case 'o' : case '>' :
      glColor3f(  0.800,  0.800,  0.800);
      break;
   case '#' : case 'x' : default  :
      glColor3f(  0.400,  0.400,  0.400);
      break;
   }
   glBegin(GL_POLYGON); {
      glVertex3f(    0.0,    0.0,   -1.0);
      glVertex3f(  300.0,    0.0,   -1.0);
      glVertex3f(  300.0,  -45.0,   -1.0);
      glVertex3f(    0.0,  -45.0,   -1.0);
   } glEnd();
   return 0;
}

char
mask       (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int       i         = 0;
   int       j         = 0;
   int       k         = 0;
   Pixmap    bounds    = XCreatePixmap(DISP, BASE, win_w, win_h, 1);
   int       x_col      = 0;
   int         x_max       =    0;
   float       x_inc       =    0;
   /*---(prepare)------------------------*/
   GC        gc        = XCreateGC(DISP, bounds, 0, NULL);
   /*---(header)-------------------------*/
   DEBUG_GRAF   yLOG_enter    (__FUNCTION__);
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
   /*---(set mask)-----------------------*/
   XShapeCombineMask (DISP, BASE, ShapeBounding, 0, 0, bounds, ShapeSet);
   /*---(free)---------------------------*/
   XFreePixmap (DISP, bounds);
   XFreeGC     (DISP, gc);
   /*---(complete)-----------------------*/
   DEBUG_GRAF   yLOG_exit     (__FUNCTION__);
   return 0;
}


/*============================----end-of-source---============================*/
