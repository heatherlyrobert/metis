/*============================----beg-of-source---============================*/
#include   "metis.h"

/*
 * metis § mi2·· § simplier card text format for mind map layout                          § N9I18A §  · § maybe no seq numbers, or age and source line
 * metis § tv1·· § text report should not open window/opengl (faster)                     § N9I1Bd §  · §
 */

char
metis_text__topp        (int x, int y)
{
   yASCII_print (x, y, "ƒ²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²‚", YASCII_CLEAR);
   return 0;
}

char
metis_text__bott        (int x, int y)
{
   yASCII_print (x, y, "„²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²…", YASCII_CLEAR);
   return 0;
}

char
metis_text__blank       (int x, int y)
{
   /*---(header)-------------------------*/
   DEBUG_PROG  yLOG_enter   (__FUNCTION__);
   /*---(placeholder)--------------------*/
   yASCII_print (x, y + 0, "Œ ·····  ································· ··· Œ", YASCII_CLEAR);
   yASCII_print (x, y + 1, "Œ ·····  ································· ··· Œ", YASCII_CLEAR);
   yASCII_print (x, y + 2, "Œ                                              Œ", YASCII_CLEAR);
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
metis_text__holder       (int x, int y)
{
   /*---(locals)-----------+-----------+-*/
   char        i           =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG  yLOG_enter   (__FUNCTION__);
   /*---(top line)-----------------------*/
   metis_text__topp (x, y++);
   /*---(fill in)------------------------*/
   for (i = 0; i < 8; ++i) {
      metis_text__blank (x, y);
      y += 3;
   }
   /*---(bot line)-----------------------*/
   metis_text__bott (x, y - 1);
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
metis_text__card        (char n, int x, int y, tTASK *a_task)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        x_txt       [LEN_FULL]  = "";
   long        v           =    0;
   char        t           [LEN_FULL]  = "";
   /*---(header)-------------------------*/
   DEBUG_PROG  yLOG_enter   (__FUNCTION__);
   /*---(numbers)------------------------*/
   DEBUG_PROG  yLOG_value   ("n"         , n);
   sprintf (t, "Œ %02d·%02d", n, a_task->seq);
   yASCII_print  (x +  0, y + 0, t, YASCII_CLEAR);
   /*---(characteristics)----------------*/
   sprintf (t, "Œ %c·%c·%c", a_task->imp, a_task->urg, a_task->est);
   yASCII_print  (x +  0, y + 1, t, YASCII_CLEAR);
   /*---(text)---------------------------*/
   yASCII_printw (x +  9, y, 33, 2, a_task->txt, YASCII_FILL);
   /*---(source line)--------------------*/
   sprintf (t, "%3d Œ", a_task->line);
   yASCII_print  (x + 43, y + 0, t, YASCII_CLEAR);
   /*---(age)----------------------------*/
   ystr4mongo (a_task->epoch, &v);
   ystrlage (v, '-', t);
   sprintf (t, "%3s Œ", t);
   yASCII_print  (x + 43, y + 1, t, YASCII_CLEAR);
   /*---(blank line)---------------------*/
   yASCII_print  (x +  0, y + 2, "Œ", YASCII_CLEAR);
   yASCII_print  (x + 47, y + 2, "Œ", YASCII_CLEAR);
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
metis_text__block       (char *n, int x, int y, tTASK **b_task, int xt, int yt, char a_align, int xe, int ye, int xl, int yl, int a_sub)
{
   /*---(locals)-----------+-----------+-*/
   char        rc          =    0;
   char        i           =    0;
   char        x_end       =  '-';
   char        x_save      [LEN_DESC]  = "";
   char        x_minor     [LEN_DESC]  = "";
   char        x_dir       =  'Ö';
   int         c           =    0;
   char        x_sub       [LEN_TERSE] = "";
   /*---(header)-------------------------*/
   DEBUG_PROG  yLOG_enter   (__FUNCTION__);
   /*---(save minor)---------------------*/
   ystrlcpy (x_save, (*b_task)->minor->name, LEN_DESC);
   /*---(top line)-----------------------*/
   metis_text__topp (x, y++);
   /*---(maximum of eight)---------------*/
   for (i = 0; i < 8; ++i) {
      DEBUG_PROG  yLOG_complex ("task"      , "%4d, %p, %c", rc, *b_task, x_end);
      if (rc < 0 || *b_task == NULL || x_end == 'y' || (i > 0 && (*b_task)->seq == 0)) {
         metis_text__blank (x, y);
      } else {
         metis_text__card (*n, x, y, *b_task);
         ++(*n);
         ++c;
      }
      y += 3;
      if (i > 0 && *b_task != NULL && (*b_task)->seq == 0) {
         x_end = 'y';
         continue;
      }
      rc = metis_task_by_cursor  (YDLST_NEXT, b_task);
   }
   /*---(bot line)-----------------------*/
   metis_text__bott (x, y - 1);
   /*---(run through left-overs)---------*/
   while (*b_task != NULL && (*b_task)->seq != 0) {
      rc = metis_task_by_cursor  (YDLST_NEXT, b_task);
      ++c;
   }
   /*---(minor text)---------------------*/
   switch (a_align) {
   case '<' : x_dir = '×';  break;
   case '>' : x_dir = 'Ö';  break;
   }
   if (a_sub >= 0)  sprintf (x_sub, "%d", a_sub);
   else             strcpy  (x_sub, "!");
   yASCII_connector (xt, yt, x_dir, xt + xe, yt + ye, '-', x_sub, xt + xl, yt + yl);
   sprintf (x_save, "%s (%d)", x_save, c);
   ystrlpad (x_save, x_minor, '?', a_align, 23);
   switch (a_align) {
   case '<' : yASCII_print  (xt +  2, yt, x_minor, YASCII_CLEAR);  break;
   case '>' : yASCII_print  (xt - 24, yt, x_minor, YASCII_CLEAR);  break;
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG   yLOG_exit     (__FUNCTION__);
   return 0;
}

char
metis_text_driver       (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rc          =    0;
   tTASK      *x_task      = NULL;
   char        t           [LEN_FULL]  = "";
   int         x           =   13;
   int         y           =    0;
   char        n           =    0;
   char        x_txt       [LEN_FULL]  = "";
   long        v           =    0;
   char        x_sub       =    0;
   int         xt, yt, xe, ye, xl, yl;
   char        x_align;
   char        x_major     [LEN_DESC]  = "";
   /*---(header)-------------------------*/
   DEBUG_PROG  yLOG_enter   (__FUNCTION__);
   /*---(prepare space)------------------*/
   rc = yASCII_new   (300, 100);
   DEBUG_PROG   yLOG_value    ("new"       , rc);
   /*---(create cards)-------------------*/
   rc = metis_task_by_cursor  (YDLST_HEAD, &x_task);
   if (x_task!= NULL)  ystrlpad (x_task->minor->major->name, x_major, '.', '|', 35);
   while (x_sub < 10) {
      switch (x_sub) {
      case  0 :  xt =  53;  yt = 28;  xe = -2;  ye = -4;  xl = -3;  yl = -2;  x_align = '<';  break;
      case  1 :  xt =  79;  yt = 26;  xe =  2;  ye = -2;  xl =  3;  yl = -1;  x_align = '>';  break;
      case  2 :  xt = 138;  yt = 26;  xe = -2;  ye = -2;  xl = -3;  yl = -1;  x_align = '<';  break;
      case  3 :  xt = 167;  yt = 28;  xe =  2;  ye = -4;  xl =  3;  yl = -2;  x_align = '>';  break;
      case  4 :  xt =  51;  yt = 31;  xe = -4;  ye =  0;  xl = -2;  yl =  1;  x_align = '<';  x =   0;  y += 25;  break;
      case  5 :  xt = 169;  yt = 31;  xe =  4;  ye =  0;  xl =  2;  yl =  1;  x_align = '>';  x = 173;            break;
      case  6 :  xt =  53;  yt = 46;  xe = -2;  ye =  4;  xl = -3;  yl =  2;  x_align = '<';  x =  13;  y += 25;  break;
      case  7 :  xt =  79;  yt = 48;  xe =  2;  ye =  2;  xl =  3;  yl =  1;  x_align = '>';  break;
      case  8 :  xt = 138;  yt = 48;  xe = -2;  ye =  2;  xl = -3;  yl =  1;  x_align = '<';  break;
      case  9 :  xt = 167;  yt = 46;  xe =  2;  ye =  4;  xl =  3;  yl =  2;  x_align = '>';  break;
      }
      if (x_task == NULL) {
         metis_text__holder (x, y);
         x += 49;
         ++x_sub;
      /*> } else if (x_task->minor->major->name [0] == 'y' && strcmp (x_task->minor->name, "_priv.h") == 0) {   <* 
       *>    metis_text__block  (&n, 85, 25, &x_task, 136, 44, '<', -4, 0, -2, 1, -1);                          <*/
      /*> } else if (x_task->minor->major->name [0] != 'y' && strcmp (x_task->minor->name, ".h") == 0) {   <* 
       *>    metis_text__block  (&n, 85, 25, &x_task, 136, 44, '<', -4, 0, -2, 1, -1);                     <*/
      } else {
         metis_text__block  (&n, x, y, &x_task, xt, yt, x_align, xe, ye, xl, yl, x_sub);
         x += 49;
         ++x_sub;
      }
   }
   /*---(application)--------------------*/
   ystrlpad (P_NAMESAKE, x_txt, '?', '|', 35);
   yASCII_print  ( 49, 36, x_txt, YASCII_CLEAR);
   ystrlpad (P_SUBJECT , x_txt, '?', '|', 35);
   yASCII_print  ( 49, 37, x_txt, YASCII_CLEAR);
   yASCII_print  ( 49, 39, x_major, YASCII_CLEAR);
   sprintf (x_txt, "minor : %-4d   tasks : %-4d", metis_minor_count (), metis_task_count  ());
   yASCII_print  ( 54, 41, x_txt, YASCII_CLEAR);
   /*---(matrix)-------------------------*/
   metis_rptg_block  (87, 26);
   /*---(legend)-------------------------*/
   yASCII_print  (136, 34, "---imp---   ---urg---   ---est--- ", YASCII_CLEAR);
   yASCII_print  (136, 35, "a·absolute  !·now-now   q·quick   ", YASCII_CLEAR);
   yASCII_print  (136, 36, "r·require   t·today     h·30 min  ", YASCII_CLEAR);
   yASCII_print  (136, 37, "v·value     s·soonest   1·hour    ", YASCII_CLEAR);
   yASCII_print  (136, 38, "c·crave     d·days      2·hours   ", YASCII_CLEAR);
   yASCII_print  (136, 39, "g·good      w·weeks     4·hours   ", YASCII_CLEAR);
   yASCII_print  (136, 40, "l·like      m·months    8·hours   ", YASCII_CLEAR);
   yASCII_print  (136, 41, "i·idea      y·years     +·longer  ", YASCII_CLEAR);
   yASCII_print  (136, 42, "-·backlog   -·backlog   -·backlog ", YASCII_CLEAR);
   yASCII_print  (136, 44, "  prg    -  <  *  ,  >  #  x  /   ", YASCII_CLEAR);
   /*---(write result)-------------------*/
   rc = yASCII_write ();
   DEBUG_PROG   yLOG_value    ("write"     , rc);
   /*---(free space)---------------------*/
   rc = yASCII_free  ();
   DEBUG_PROG   yLOG_value    ("free"      , rc);
   /*---(overall)------------------------*/
   DEBUG_PROG   yLOG_exit     (__FUNCTION__);
   return 0;
}
