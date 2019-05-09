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
uint      tex       =     0;                /* texture for image              */
uint      fbo       =     0;                /* framebuffer                    */
uint      depth     =     0;                /* depth buffer                   */


char        /* PURPOSE : process the xwindows event stream                    */
prog_event    (void)
{
   /*---(locals)--------------------------------*/
   XKeyEvent *key_event;
   char       the_key[5];
   int        the_bytes;
   char       x_search[100];
   my.update = 0;
   int        loop = 0;
   /*---(event loop)----------------------------*/
   DEBUG_E  printf("prog_event () begin\n");
   while (1) {
      XNextEvent(DISP, &EVNT);
      my.update = 1;
      my.pcol = my.ccol;
      my.prow = my.crow;
      /*---(start)----------------------------*/
      switch(EVNT.type) {
      case FocusIn:
         DEBUG_E  printf("   prog_event () focus in\n");
         if ((int) (alpha * 10) == 3) alpha = 0.0;
         break;

      case FocusOut:
         DEBUG_E  printf("   prog_event () focus out\n");
         if ((int) (alpha * 10) == 0) alpha = 0.3;
         break;

      case Expose:
         DEBUG_E  printf("   prog_event () expose\n");
         break;

      case ConfigureNotify:
         DEBUG_E  printf("   prog_event () configure\n");
         break;

      case KeyPress:
         DEBUG_E  printf("   prog_event () keypress\n");
         key_event  = (XKeyEvent *) &EVNT;
         the_bytes = XLookupString((XKeyEvent *) &EVNT, the_key, 5, NULL, NULL);
         if (the_bytes < 1) break;
         if (arg_keys == ' ') {
            /*---(mode changes)-------*/
            switch (the_key[0]) {
            case 'Q': return 1;                   break;
            case 'u': alpha = 0.8; arg_keys = 'u';       break;
            case 'i': alpha = 0.8; arg_keys = 'i';       break;
            case 'e': alpha = 0.8; arg_keys = 'e';       break;
            case 'f': alpha = 0.8; arg_keys = 'f';       break;
            case '/':  
            case 's': alpha = 0.8; arg_keys = 's'; x_search[0] = '\0'; break;
            }
            /*---(filters and tasks)--*/
            switch (the_key[0]) {
            case '[':
            case '?': my.incr  = STOP;   my.move   = 0.0;   break;
            case '_': my.incr  = STOP;   my.move   = 0.0;  my.ccol = my.crow = 0; my.move = 0.0;  break;
            case 'l': my.incr  = STOP;   ++my.ccol;      break;
            case '>': my.incr  = STOP;   ++my.ccol;      break;
            case 'j': my.incr  = STOP;   ++my.crow;      break;
            case ',': my.incr  = my.play;                break;
            case '+': my.incr += my.change;              break;
            case '-': my.incr -= my.change;              break;
            case '.': my.incr  = STOP;                   break;
            case 'h': my.incr  = STOP;   --my.ccol;      break;
            case '<': my.incr  = STOP;   --my.ccol;      break;
            case 'k': my.incr  = STOP;   --my.crow;      break;
            case ']':
            case 'G': my.incr  = STOP;   my.move   = 0.0; my.ccol = my.ncols - my.wcols; my.crow = my.nrows - my.wrows; break;
            }
            /*---(task related)-------*/
            switch (the_key[0]) {
            case 'p': task_list();                       break;
            case 'r': task_refresh();                    break;
            case 'J': font_change(); task_refresh();                    break;
            case 'a': urg = imp = est = flg = ' '; strcpy(one, "all"); texture_free(); draw_init(); draw_main(); mask(); my.ccol = my.crow = 0; /* offset = 0.0; */ draw_resize(win_w, win_h); break;
            case 'H': if (arg_heads)  arg_heads  = 0; else arg_heads  = 1; return my.format;                    break;
            case 'F': if (arg_filter) arg_filter = 0; else arg_filter = 1; return my.format;                    break;
            }
            /*---(format changes)-----*/
            switch (the_key[0]) {
            case 'C' : return 'c';    break;
            case 'L' : return 'l';    break;
            case 'S' : return 's';    break;
            case 'T' : return 't';    break;
            case 'B' : return 'b';    break;
            case '1' : return '1';    break;
            case '2' : return '2';    break;
            case 'W' : return 'w';    break;
            case 'X' : return 'x';    break;
            case 'P' : return 'p';    break;
            case '0' : urg = imp = est = ' '; strcpy(one, "all"); flg = '<'; return 't'; break;
            }
            /*---(verify currents)----*/
            range_check();

         } else {
            if (arg_keys == 's' && the_key[0] != 13) {
               char   ch  = the_key[0];
               /*> printf("ch = %3d\n", ch);                                       <*/
               if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || ch == '_' || ch == 8) {
                  char   temp[10];
                  sprintf(temp, "%c", the_key[0]);
                  strcat(x_search, temp);
                  /*> printf("search = <<%s>>\n", x_search);                       <*/
               }
            } else {
               switch (arg_keys) {
               case 'u': if (strchr(" tdwmqyb", the_key[0]) != 0) urg  = the_key[0];  else urg  = ' '; break;
               case 'i': if (strchr(" anwlmis", the_key[0]) != 0) imp  = the_key[0];  else imp  = ' '; break;
               case 'e': if (strchr(" 0123456", the_key[0]) != 0) est  = the_key[0];  else est  = ' '; break;
               case 'f': if (strchr(" -x#<o>" , the_key[0]) != 0) flg  = the_key[0];  else flg  = ' '; break;
               case 's': if (x_search[0] == '\0') strcpy(one, "all");
                            else strncpy(one, x_search, 20);
                            break;
               }
               arg_keys = ' ';
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
         DEBUG_E  printf("   prog_event () default\n");
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
      my.update = 0;
      draw_texture();
      /*> usleep( 5000);                                                              <*/
      /*> usleep(  50);                                                               <*/
   }
   DEBUG_E  printf("prog_event () end\n");
   /*---(complete)------------------------------*/
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
draw_texture       (void)
{
   DEBUG_G  printf("draw_texture () beg\n");
   /*---(locals)-------------------------*/
   float     offset, ratio;
   float     tx1, ty1, tx2, ty2;
   float     wx1, wy1, wx2, wy2;
   float     z;
   int       i;
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
   glBindTexture   (GL_TEXTURE_2D, tex);
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
   if (arg_keys != ' ') {
      glColor4f (0.0f, 0.0f, 0.0f, 0.5);
      glBegin(GL_POLYGON); {
         glVertex3f   ( wx1,  wy1,    z);
         glVertex3f   ( wx2,  wy1,    z);
         glVertex3f   ( wx2,  wy2,    z);
         glVertex3f   ( wx1,  wy2,    z);
      } glEnd();
   }
   /*---(locate current)-----------------*/
   /*> ctask = 0;                                                                     <* 
    *> for (i = 0; i < MAX_CARDS; ++i) {                                              <* 
    *>    if (g_tasks [i].pos >= offset + (step / 12)) break;                            <* 
    *>    ctask = i;                                                                  <* 
    *> }                                                                              <*/
   /*> printf("offset = %f, curr = %2d, pos = %f\n", offset, ctask + 1, g_tasks [ctask].pos);   <*/
   /*---(force the redraw)---------------*/
   glXSwapBuffers(DISP, BASE);
   glFlush();
   DEBUG_G  printf("draw_texture () end\n");
   /*---(complete)-------------------------*/
   return 0;
}

char               /* PURPOSE : create a texture -----------------------------*/
texture_create     (void)
{
   DEBUG_G  printf("texture_create () beg\n");
   /*---(generate)-----------------------*/
   glGenFramebuffersEXT         (1, &fbo);
   glGenTextures                (1, &tex);
   glGenRenderbuffersEXT        (1, &depth);
   /*---(bind)---------------------------*/
   DEBUG_G  printf("texture_create () bind\n");
   glBindFramebufferEXT         (GL_FRAMEBUFFER_EXT,  fbo);
   glBindTexture                (GL_TEXTURE_2D,       tex);
   /*---(settings)-----------------------*/
   DEBUG_G  printf("texture_create () settings\n");
   glTexParameteri              (GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri              (GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri              (GL_TEXTURE_2D,  GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri              (GL_TEXTURE_2D,  GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexEnvi                    (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
   glTexParameteri              (GL_TEXTURE_2D,  GL_GENERATE_MIPMAP, GL_TRUE);
   /*---(copy)---------------------------*/
   DEBUG_G  printf("texture_create () allocate\n");
   glTexImage2D                 (GL_TEXTURE_2D, 0, GL_RGBA, tex_w, tex_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
   DEBUG_G  printf("texture_create () copy\n");
   glFramebufferTexture2DEXT    (GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, tex, 0);
   /*---(depth)--------------------------*/
   DEBUG_G  printf("texture_create () depth\n");
   glBindRenderbufferEXT        (GL_RENDERBUFFER_EXT, depth);
   glRenderbufferStorageEXT     (GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, tex_w, tex_h);
   glFramebufferRenderbufferEXT (GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth);
   /*---(unbind)-------------------------*/
   DEBUG_G  printf("texture_create () unbind\n");
   glBindFramebufferEXT         (GL_FRAMEBUFFER_EXT, 0);
   /*---(complete)-----------------------*/
   DEBUG_G  printf("texture_create () end\n");
   return 0;
}

char               /* PURPOSE : delete a framebuffer -------------------------*/
texture_free       (void)
{
   DEBUG_G  printf("texture_free () beg\n");
   /*---(generate)-----------------------*/
   glDeleteTextures                (1, &tex);
   glDeleteRenderbuffersEXT        (1, &depth);
   glDeleteFramebuffersEXT         (1, &fbo);
   /*---(complete)-----------------------*/
   DEBUG_G  printf("texture_free () end\n");
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
draw_main          (void)
{
   DEBUG_G  printf("draw_main () beg\n");
   /*---(locals)-------------------------*/
   int       i;                             /* loop iterator -- word          */
   int       j;                             /* loop iterator -- word          */
   int       rc      = 0;                   /* simple return code             */
   int       w       = 0;
   int       pos     = 0;
   /*---(create objects)-----------------*/
   /*> printf("   entered\n");                                                        <*/
   texture_create();
   /*> printf("   texture create done\n");                                            <*/
   /*---(setup)--------------------------*/
   glViewport(0,  0, tex_w, tex_h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(        0,   tex_w, -tex_h,  0, -500.0,  500.0);
   glMatrixMode(GL_MODELVIEW);
   glBindTexture   (GL_TEXTURE_2D, 0);
   glBindFramebufferEXT         (GL_FRAMEBUFFER_EXT,  fbo);
   /*> printf("   bound texture\n");                                                  <*/
   /*---(draw)---------------------------*/
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   /*---(tasks)--------------------------*/
   glColor4f (0.0f, 0.0f, 0.0f, 1.0f);
   /*> printf("g_ntask = %3d\n", g_ntask);                                              <*/
   /*> printf("   starting draw\n");                                                  <*/
   glPushMatrix(); {
      BASELINE {
         for (i = 0; i < 10; ++i) {
            draw_card(i);
            glTranslatef(  325.0,   0.0,   0.0);
            pos += 325;
            g_tasks [i].pos = pos;
         }
      } else TICKER {
         for (i = 0; i < 10; ++i) {
            draw_card(i);
            glTranslatef(  300.0,   0.0,   0.0);
            pos += 300;
            g_tasks [i].pos = pos;
         }
      } else STREAMER {
         draw_title ();
         glTranslatef(    0.0, -45.0,   0.0);
         pos -=  45;
         for (i = 0; i < g_ntask; ++i) {
            draw_card(i);
            glTranslatef(    0.0, -44.0,   0.0);
            pos -=  44;
            g_tasks [i].pos = pos;
         }
      } else COLUMN {
         for (i = 0; i < g_ntask; ++i) {
            draw_card(i);
            glTranslatef(    0.0,  -60.0,   0.0);
            pos -= 60;
            g_tasks [i].pos = pos;
         }
      } else LONG {
         for (i = 0; i < g_ntask; ++i) {
            draw_card(i);
            glTranslatef(    0.0,  -60.0,   0.0);
            pos -= 60;
            g_tasks [i].pos = pos;
         }
      } else WIDEVIEW {
         /*> printf("   ncols = %2d\n", my.ncols);                                    <*/
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
      } else PROJECT {
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
         /*> printf("done drawing\n");                                                <*/
      } else EXTRA {
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
      }
   } glPopMatrix();
   /*> printf("   ending draw\n");                                                    <*/
   /*> printf("width = %d\n", w);                                                     <*/
   /*---(mipmaps)------------------------*/
   glBindFramebufferEXT         (GL_FRAMEBUFFER_EXT, 0);
   glBindTexture   (GL_TEXTURE_2D, tex);
   glGenerateMipmapEXT(GL_TEXTURE_2D);
   glBindTexture   (GL_TEXTURE_2D, 0);
   /*> printf("   draw complete\n");                                                  <*/
   /*---(complete)-------------------------*/
   DEBUG_G  printf("draw_main () end\n");
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
      snprintf(msg, 100, "%-3d : u=<%c>, i=<%c>, e=<%c>, f=<%c>, <<%s>>", g_ntask, urg, imp, est, flg, one);
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
   /*> printf("draw = %2d\n", a_index);                                               <*/
   card_base  (g_tasks [a_index].flg);
   estimate   (g_tasks [a_index].est);
   importance (g_tasks [a_index].imp);
   urgency    (g_tasks [a_index].urg);
   bullets    ();
   text       (a_index);
   borders    ();
   /*> if (g_tasks [a_index].flg == '#') complete   ();                                 <*/
   /*> if (g_tasks [a_index].flg == 'X') complete   ();                                 <*/
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
      glTranslatef(  53.0,  -8.0,  20.0);
      glTranslatef(   0.0, txf_off,   0.0);
      /*> snprintf(temp, 10, "%1d/%1d", a_index + 1, g_ntask - 1);                     <*/
      snprintf(temp, 10, "%d", a_index + 1);
      /*> if (a_index < g_ntask - 1) yFONT_print  (txf_sm,  7, YF_MIDCEN, temp);    <*/
      yFONT_print  (txf_sm,  7, YF_MIDCEN, temp);
      glTranslatef(  72.0,   0.0,   0.0);
      yFONT_print  (txf_sm,  7, YF_MIDCEN, g_tasks [a_index].one);
      glTranslatef( 105.0,   0,   0);
      yFONT_print  (txf_sm,  7, YF_MIDCEN, g_tasks [a_index].two);
      glTranslatef(-160.0, -10.0,   0.0);
      glTranslatef(   0.0, txf_off,   0.0);
      glColor4f (0.0, 0.0, 0.0, 1.0);
      yFONT_printw (txf_sm,  7, YF_TOPLEF, g_tasks [a_index].txt, 205, 35, txf_space);
   } glPopMatrix();
   /*---(letters)-------------------------------*/
   glPushMatrix(); {
      glColor4f (0.0, 0.0, 0.0, 1.0);
      glTranslatef(  20.0, -15.0,  20.0);
      glTranslatef(   0.0, txf_off,   0.0);
      snprintf(temp, 4, "%c", g_tasks [a_index].urg);
      yFONT_print(txf_sm,  9, YF_BASCEN, temp);
      glTranslatef(  12.0, -12.0,   0.0);
      snprintf(temp, 4, "%c", g_tasks [a_index].imp);
      yFONT_print(txf_sm,  9, YF_BASCEN, temp);
      glTranslatef(  12.0, -12.0,   0.0);
      snprintf(temp, 4, "%c", g_tasks [a_index].est);
      yFONT_print(txf_sm,  9, YF_BASCEN, temp);
      /*> glTranslatef( 245.0,  10.0,   0.0);                                         <* 
       *> snprintf(temp, 4, "%c", g_tasks [a_index].flg);                             <* 
       *> yFONT_print(txf_bg, 16, YF_MIDCEN, temp);                                   <*/
   } glPopMatrix();
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

char               /* PURPOSE : draw two and bottom borders ------------------*/
complete           (void)
{
   glColor4f (0.0f, 0.0f, 0.0f, 0.3f);
   glBegin(GL_POLYGON); {
      glVertex3f(   0.0,     0.0,    70.0);
      glVertex3f( 300.0,     0.0,    70.0);
      glVertex3f( 300.0,   -45.0,    70.0);
      glVertex3f(   0.0,   -45.0,    70.0);
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
urgency     (char  a_value)
{
   glBegin(GL_POLYGON); {
      glColor4f (0.0, 0.0, 0.0, 1.0);
      glVertex3f(    0.0,    0.0,    0.0);
      glVertex3f(  300.0,    0.0,    0.0);
      glVertex3f(  300.0,  -17.0,    0.0);
      glVertex3f(    0.0,  -17.0,    0.0);
   } glEnd();
   /*---(color)---------------------------------*/
   switch (a_value) {
   case 't' : glColor3f(  1.000,  0.000,  0.000); break;
   case 'd' : glColor3f(  1.000,  0.455,  0.000); break;
   case 'w' : glColor3f(  0.800,  0.733,  0.000); break;
   case 'm' : glColor3f(  0.200,  0.667,  0.200); break;
   case 'q' : glColor3f(  0.000,  0.600,  0.600); break;
   case 'y' : glColor3f(  0.800,  0.000,  0.800); break;
   case 'b' : glColor3f(  0.400,  0.400,  0.400); break;
   default  : glColor3f(  0.400,  0.400,  0.400); break;
   }
   /*---(bar)-----------------------------------*/
   glBegin(GL_POLYGON); {
      glVertex3f(    0.0,   -2.0,    2.0);
      glVertex3f(  300.0,   -2.0,    2.0);
      glVertex3f(  300.0,  -15.0,    2.0);
      glVertex3f(    0.0,  -15.0,    2.0);
   } glEnd();
   /*---(complete)------------------------------*/
   return 0;
}

char
polygon     (float *a_array)
{
   glPushMatrix(); {
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glBegin(GL_POLYGON); {
         glVertex3fv(a_array +  0);
         glVertex3fv(a_array +  3);
         glVertex3fv(a_array +  6);
         glVertex3fv(a_array +  9);
      } glEnd();
      glTranslatef(0.0, 0.0, -2.0);
      glBegin(GL_LINE_STRIP); {
         glVertex3fv(a_array +  0);
         glVertex3fv(a_array +  3);
         glVertex3fv(a_array +  6);
         glVertex3fv(a_array +  9);
         glVertex3fv(a_array +  0);
      } glEnd();
   } glPopMatrix();
   return 0;
}

float   impf[12] = {
   -00.0,   -2.0,   12.0,
   +22.0,   -2.0,   12.0,
   +62.0,  -43.0,   12.0,
   +40.0,  -43.0,   12.0,
};
float   impb[12] = {
   -20.0,   -2.0,   10.0,
   +25.0,   -2.0,   10.0,
   +65.0,  -43.0,   10.0,
   +20.0,  -43.0,   10.0,
};

char
importance  (char  a_value)
{
   glColor3f(  0.000,  0.000,  0.000);
   polygon (impb);
   switch (a_value) {
   case 'a' : glColor3f(  1.000,  0.000,  0.000); break;
   case 'n' : glColor3f(  1.000,  0.455,  0.000); break;
   case 'w' : glColor3f(  0.800,  0.733,  0.000); break;
   case 'l' : glColor3f(  0.200,  0.667,  0.200); break;
   case 'm' : glColor3f(  0.000,  0.600,  0.600); break;
   case 'i' : glColor3f(  0.800,  0.000,  0.800); break;
   case 's' : glColor3f(  0.400,  0.400,  0.400); break;
   default  : glColor3f(  0.000,  0.000,  0.000); break;
   }
   polygon (impf);
   return 0;
}

char
estimate    (char  a_value)
{
   glColor4f (0.0, 0.0, 0.0, 1.0);
   glBegin(GL_POLYGON); {
      glVertex3f(   30.0,  -15.0,    0.0);
      glVertex3f(   58.0,  -15.0,    0.0);
      glVertex3f(   58.0,  -45.0,    0.0);
      glVertex3f(   30.0,  -45.0,    0.0);
   } glEnd();
   switch (a_value) {
   case '!' : glColor3f(  1.000,  0.000,  0.000); break;
   case 's' : glColor3f(  1.000,  0.455,  0.000); break;
   case 'm' : glColor3f(  0.800,  0.733,  0.000); break;
   case '1' : glColor3f(  0.200,  0.667,  0.200); break;
   case '2' : glColor3f(  0.000,  0.600,  0.600); break;
   case '4' : glColor3f(  0.800,  0.000,  0.800); break;
   case '8' : glColor3f(  0.400,  0.400,  0.400); break;
   default  : glColor3f(  0.000,  0.000,  0.000); break;
   }
   glBegin(GL_POLYGON); {
      glVertex3f(    0.0,  -17.0,    0.0);
      glVertex3f(   56.0,  -17.0,    0.0);
      glVertex3f(   56.0,  -45.0,    0.0);
      glVertex3f(    0.0,  -45.0,    0.0);
   } glEnd();
   return 0;
}

char          /*----: lay down the base color --------------------------------*/
card_base          (char  a_value)
{
   switch (a_value) {
   case '?' : case '+' : case '·' :
      glColor3f(  0.800,  0.800,  0.800);
      break;
   case '<' : glColor3f(  0.900,  0.500,  0.500); break;
   case 'o' : glColor3f(  0.500,  0.900,  0.900); break;
   case '>' : glColor3f(  0.700,  0.500,  0.700); break;
   case '-' : glColor3f(  0.500,  0.900,  0.500); break;
   case '#' : glColor3f(  0.000,  0.600,  0.400); break;
   case 'x' : glColor3f(  0.450,  0.450,  0.450); break;
   default  : glColor3f(  0.400,  0.400,  0.400); break;
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
   /*---(locals)-------------------------*/
   int       i         = 0;
   int       j         = 0;
   int       k         = 0;
   Pixmap    bounds    = XCreatePixmap(DISP, BASE, win_w, win_h, 1);
   int       _col      = 0;
   /*---(prepare)------------------------*/
   GC        gc        = XCreateGC(DISP, bounds, 0, NULL);
   XSetForeground(DISP, gc, 0);
   XFillRectangle(DISP, bounds, gc, 0, 0, win_w, win_h);
   XSetForeground(DISP, gc, 1);
   /*---(establish mask)-----------------*/
   TICKER {
      for (i = 0; i < 10; ++i) {
         XFillRectangle(DISP, bounds, gc, i * 300, 0, 300, 45);
      }
   } else BASELINE {
      for (i = 0; i < 10; ++i) {
         XFillRectangle(DISP, bounds, gc, i * 325, 0, 300, 45);
      }
   } else STREAMER {
      /*> for (i = 0; i < my.wrows; ++i) {                                            <* 
       *>    if (i >= g_ntask) continue;                                            <* 
       *>    XFillRectangle(DISP, bounds, gc,   0, i * 44, 300, 44);                  <* 
       *> }                                                                           <*/
      XFillRectangle(DISP, bounds, gc,   0, 0, 300, 44 * g_ntask + 45);
   } else COLUMN {
      for (i = 0; i < my.wrows; ++i) {
         if (g_tasks [i].imp != '?') XFillRectangle(DISP, bounds, gc,   0, i * 60, 300, 45);
      }
   } else LONG   {
      for (i = 0; i < my.wrows; ++i) {
         if (g_tasks [i].imp != '?') XFillRectangle(DISP, bounds, gc,   0, i * 60, 300, 45);
      }
   } else WIDEVIEW {
      for (j = 0; j < 4; ++j) {
         for (i = 0; i < 12; ++i) {
            if (g_tasks [j* my.wrows + i].imp != '?') XFillRectangle(DISP, bounds, gc,  j * 320, i * 60, 300, 45);
         }
      }
   } else PROJECT  {
      for (j = 0; j < my.wcols; ++j) {
         _col = my.ccol + j;
         if (_col >= my.ncols) _col -= my.ncols;
         for (i = 0; i < my.wrows; ++i) {
            for (k = 0; k < g_ntask; ++k) {
               if (g_tasks [k].col != _col + 1)           continue;
               if (g_tasks [k].row != i + 1)              continue;
               XFillRectangle(DISP, bounds, gc,  j * 320, i * 60, 300, 45);
            }
         }
      }
   } else EXTRA {
      for (j = 0; j < 4; ++j) {
         for (i = 0; i < 16; ++i) {
            if (g_tasks [j * 16 + i].imp != '?') XFillRectangle(DISP, bounds, gc,  j * 320, i * 60, 300, 45);
         }
      }
   }
   /*---(set mask)-----------------------*/
   XShapeCombineMask(DISP, BASE, ShapeBounding, 0, 0, bounds, ShapeSet);
   /*---(free)---------------------------*/
   XFreePixmap(DISP, bounds);
   XFreeGC(DISP, gc);
   /*---(complete)-----------------------*/
   return 0;
}


/*============================----end-of-source---============================*/
