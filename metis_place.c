#include   "metis.h"



short g_map [MAX_COLS][MAX_ROWS];



/*====================------------------------------------====================*/
/*===----                        position marking                      ----===*/
/*====================------------------------------------====================*/
static void      o___POSITION________________o (void) {;}

char
metis_place_clear       (void)
{
   /*---(locals)-----------+-----+-----+-*/
   short       x_col, x_row;
   /*---(header)-------------------*/
   DEBUG_DATA   yLOG_senter   (__FUNCTION__);
   DEBUG_DATA   yLOG_snote    ("clear g_map");
   for (x_col = 0; x_col < MAX_COLS; ++x_col) {
      for (x_row = 0; x_row < MAX_ROWS; ++x_row) {
         g_map [x_col][x_row] = -1;
      }
   }
   DEBUG_DATA   yLOG_snote    ("clear counts");
   NCOLS = NROWS = 0;
   DEBUG_DATA   yLOG_sexit    (__FUNCTION__);
   return 0;
}

char
metis_place_map         (short a_col, short a_row, short n, short *c)
{
   /*---(record)----------------*/
   DEBUG_DATA   yLOG_sint     (n);
   g_map [a_col][a_row] = n;
   ++(g_map [MAX_COLS - 1][a_row        ]);
   ++(g_map [a_col        ][MAX_ROWS - 1]);
   /*---(check maxes)-----------*/
   if (NCOLS <= a_col)  NCOLS = a_col + 1;
   if (NROWS <= a_row)  NROWS = a_row + 1;
   /*---(output)----------------*/
   ++(*c);
   DEBUG_DATA   yLOG_sint     (*c);
   return 0;
}

char
metis_place_single      (tTASK **a_task, short a_col, short a_row, short *i, short *c)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------*/
   DEBUG_DATA   yLOG_senter   (__FUNCTION__);
   /*---(defense)------------------*/
   DEBUG_DATA   yLOG_spoint   (a_task);
   --rce;  if (a_task == NULL) {
      DEBUG_DATA   yLOG_sexitr   (__FUNCTION__, rce);
      return rce;

   }
   /*---(check for end)------------*/
   DEBUG_DATA   yLOG_spoint   (*a_task);
   --rce;  if (*a_task == NULL) {
      if (*c == 0) {
         DEBUG_DATA   yLOG_snote    ("ended with zero cards");
         metis_place_map (0, 0, WARN_FILTER, c);
      } else {
         DEBUG_DATA   yLOG_snote    ("done with existing data");
      }
      DEBUG_DATA   yLOG_sexitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(display)------------------*/
   DEBUG_DATA   yLOG_sint     (a_col);
   DEBUG_DATA   yLOG_sint     (a_row);
   /*---(break)--------------------*/
   if (a_col == my.x_skip) {
      DEBUG_DATA   yLOG_snote    ("hit a col/x skip");
      metis_place_map (a_col, a_row, WARN_SPACER, c);
      DEBUG_DATA   yLOG_sexit    (__FUNCTION__);
      return 0;
   }
   if (a_row == my.y_skip) {
      DEBUG_DATA   yLOG_snote    ("hit a row/y skip");
      metis_place_map (a_col, a_row, WARN_SPACER, c);
      DEBUG_DATA   yLOG_sexit    (__FUNCTION__);
      return 0;
   }
   /*---(handle active)------------*/
   DEBUG_DATA   yLOG_schar    ((*a_task)->show);
   if ((*a_task)->show == 'y') {
      /*---(record)----------------*/
      DEBUG_DATA   yLOG_sint     (*i);
      metis_place_map (a_col, a_row, *i, c);
      /*---(save-back)----------------*/
      (*a_task)->x = a_col;
      (*a_task)->y = a_row;
      /*---(output)----------------*/
      rc = 0;
      /*---(done)------------------*/
   } else {
      (*a_task)->x = 0;
      (*a_task)->y = 0;
      DEBUG_DATA   yLOG_snote    ("filtered task");
      rc = 1;
   }
   /*---(complete)-----------------*/
   DEBUG_DATA   yLOG_sint     (rc);
   DEBUG_DATA   yLOG_sexit    (__FUNCTION__);
   /*---(next)---------------------*/
   *i = *i + 1;
   metis_task_by_index (*i, a_task);
   /*---(complete)-----------------*/
   return rc;
}

char
metis_place_assign      (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   short       x_col, x_row;
   short       n           =    0;
   short       i           =    0;
   short       c           =    0;
   tTASK      *x_task      = NULL;
   tTASK      *x_last      = NULL;
   /*---(header)-------------------------*/
   DEBUG_DATA   yLOG_enter    (__FUNCTION__);
   DEBUG_DATA   yLOG_char     ("my.breaks" , my.breaks);
   /*---(prepare)------------------------*/
   metis_place_clear ();
   if (metis_task_count () <= 0) {
      DEBUG_DATA   yLOG_snote    ("ended with zero cards");
      metis_place_map (0, 0, WARN_NODATA, &c);
      DEBUG_DATA   yLOG_exit     (__FUNCTION__);
      return 0;
   }
   metis_task_by_index (i, &x_task);
   DEBUG_DATA   yLOG_point    ("x_task"    , x_task);
   x_last = x_task;
   DEBUG_DATA   yLOG_char     ("format"    , my.format);
   /*---(vertical)-----------------------*/
   if (strchr (FORMAT_VERTS, my.format) != NULL) {
      DEBUG_DATA   yLOG_note     ("vertical format");
      for (x_col = 0; x_col < TCOLS; ++x_col) {
         for (x_row = 0; x_row < TROWS; ++x_row) {
            rc = metis_place_single (&x_task, x_col, x_row, &i, &c);
            if (rc > 0)  --x_row;
            if (rc < 0) break;
         }
         if (rc < 0) break;
      }
   }
   /*---(horizontals)--------------------*/
   else {
      DEBUG_DATA   yLOG_note     ("horizontal format");
      for (x_row = 0; x_row < TROWS; ++x_row) {
         for (x_col = 0; x_col < TCOLS; ++x_col) {
            rc = metis_place_single (&x_task, x_col, x_row, &i, &c);
            if (rc > 0)  --x_col;
            if (rc < 0) break;
         }
         if (rc < 0) break;
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_DATA   yLOG_exit     (__FUNCTION__);
   return c;
}

char
metis_place_show        (void)
{
   int         x_col, x_row;
   printf ("\n");
   printf ("col  tot ", x_col);
   for (x_row = 1; x_row <= TROWS; ++x_row) {
      printf (" %3d", x_row);
   }
   printf ("\n");
   for (x_col = 1; x_col <= TCOLS; ++x_col) {
      if (g_map [x_col][0] <= 0) break;
      printf ("%3d (%3d)", x_col, g_map [x_col][0]);
      for (x_row = 1; x_row <= TROWS; ++x_row) {
         if (g_map [x_col][x_row] <= 0)  break;
         printf (" %3d", g_map [x_col][x_row]);
      }
      printf ("\n");
   }
}

short
metis_place_get         (short a_col, short a_row)
{
   return g_map [a_col][a_row];
}



/*====================------------------------------------====================*/
/*===----                      unit testing                            ----===*/
/*====================------------------------------------====================*/
static void  o___UNITTEST________o () { return; }

char*        /*-> unit test accessor -----------------[ light  [us.B60.2A3.F2]*/ /*-[01.0000.00#.#]-*/ /*-[--.---.---.--]-*/
metis_place__unit       (char *a_question, int a_num)
{
   /*---(locals)-------------------------*/
   char        rc          =    0;
   char        s           [LEN_HUND]   = "[]";
   char        t           [LEN_HUND]   = "[]";
   char        u           [LEN_HUND]   = "[]";
   int         i           =    0;
   /*---(overall)------------------------*/
   strcpy (unit_answer, "PLACE            : question not understood");
   if      (strcmp (a_question, "col"           ) == 0) {
      if (g_map [a_num][MAX_ROWS - 1] >= 0)         sprintf (t, "%c %c å", YSTR_MUCHO  [g_map [a_num][MAX_ROWS - 1] + 1], YSTR_MUCHO  [TROWS]);
      else                                          sprintf (t, "  %c å" , YSTR_MUCHO  [TROWS]);
      for (i = 0; i < 85; ++i) {
         if      (a_num >=  TCOLS)                  sprintf (s, " ");  
         else if (i     >=  TROWS)                  sprintf (s, " ");  
         else if (g_map [a_num][i] == WARN_NODATA)  sprintf (s, "£");
         else if (g_map [a_num][i] == WARN_FILTER)  sprintf (s, "Ï");
         else if (g_map [a_num][i] == WARN_SPACER)  sprintf (s, "°");
         else if (g_map [a_num][i] >  -1)           sprintf (s, "%c", YSTR_MUCHO [g_map [a_num][i]]);
         else                                       sprintf (s, "·");
         ystrlcat (t, s, LEN_HUND);
      }
      ystrlcat (t, "æ", LEN_HUND);
      snprintf (unit_answer, LEN_FULL, "PLACE col   (%2d) : %s", a_num, t);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}

