/*============================----beg-of-source---============================*/
#include   "metis.h"

#define    METIS_REPORT   'r'
#define    METIS_FORMAT   'f'
#define    METIS_DUMP     'd'
#define    METIS_NADA     '-'

static char     s_abbr      = '·';
static char     s_opt       = METIS_NADA;
static char     s_name      [LEN_LABEL] = "";;
static char     s_desc      [LEN_HUND]  = "";;
static char     s_options   [LEN_TERSE] = "";;

struct {
   char        abbr;
   char        name        [LEN_LABEL];
   char        desc        [LEN_HUND];
} static const s_reports [] = {
   { 'l', "list"            , "list all tasks in inventory"                    },
   { 'm', "matrix"          , "urgent-important matrix"                        },
   { '·', ""                , ""                                               },
};

char
poly_rptg_lookup        (char *a_option)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   /*> printf ("lookup å%sæ\n", a_option);                                            <*/
   /*---(defaults)-----------------------*/
   s_abbr = '·';
   s_opt  = METIS_NADA;
   strlcpy (s_name   , ""    , LEN_LABEL);
   strlcpy (s_desc   , ""    , LEN_HUND);
   /*---(walk)---------------------------*/
   while (s_reports [i].abbr != '·') {
      /*> printf ("trying %2d, %s (%s)\n", i, s_reports [i].name, s_reports [i].desc);   <*/
      /*---(identify)--------------------*/
      if      (strcmp (a_option, s_reports [i].name) == 0)                                      s_opt = METIS_REPORT;
      else if (a_option [0] == METIS_REPORT && strcmp (a_option + 1, s_reports [i].name) == 0)   s_opt = METIS_REPORT;
      else if (a_option [0] == METIS_FORMAT && strcmp (a_option + 1, s_reports [i].name) == 0)   s_opt = METIS_FORMAT;
      else if (a_option [0] == METIS_DUMP   && strcmp (a_option + 1, s_reports [i].name) == 0)   s_opt = METIS_DUMP;
      /*---(handle)----------------------*/
      if  (s_opt != METIS_NADA) {
         s_abbr = s_reports [i].abbr;
         strlcpy (s_name   , s_reports [i].name    , LEN_LABEL);
         strlcpy (s_desc   , s_reports [i].desc    , LEN_HUND);
         /*> printf ("FOUND  %c, %c, %c\n", my.g_mode, my.g_scope, my.g_rptg);        <*/
         return 1;
      }
      /*---(next)------------------------*/
      ++i;
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
metis_rptg__header      (void)
{
   printf ("#!/usr/local/bin/metis --report %s\n", s_name);
   printf ("\n");
   printf ("#  %s %s\n", P_NAMESAKE, P_HERITAGE);
   printf ("#  version %s, %s\n", P_VERNUM, P_VERTXT);
   printf ("#  %s\n", s_desc);
   printf ("\n");
   printf ("#@ style     V = printable columnar values\n");
   return 0;
}

char
metis_inventory         (void)
{
   int         i           =    0;
   int         x_max       =    0;
   tTASK      *x_task      = NULL;
   if (s_opt == METIS_REPORT) {
      metis_rptg__header ();
      printf ("#@ x-parse   åÏ--·Ï-------------------·Ï-------------------·Ï--·Ï·Ï·Ï·Ï·Ï·Ï---------------------------------------------------------------------·Ï---·Ï-------------------------------------------------------------------------------æ\n");
      printf ("#@ titles    åref·major················minor················seq·u·i·e·p·s·task·detail····························································line·source··········································································æ\n");
      printf ("\n");
   }
   x_max = metis_task_count ();
   for (i = 0; i < x_max; ++i) {
      if (s_opt != METIS_DUMP) {
         if (i %  5 == 0 && i != 0)  printf ("\n");
         if (i % 25 == 0)  printf ("#ef major--------------- minor--------------- seq u i e p s task-detail----------------------------------------------------------- line source--------------------------------------------------------------------------\n\n");
      }
      metis_task_by_index (i, &x_task);
      if (x_task == NULL) {
         printf ("FOUND A NULL\n");
      }
      printf ("%3d %-20.20s %-20.20s %3d %c %c %c %c %c %-70.70s %4d %-80.80s\n",
            i, x_task->minor->major->name, x_task->minor->name, x_task->seq,
            x_task->urg, x_task->imp, x_task->est, x_task->prg, x_task->shr,
            x_task->txt, x_task->line, x_task->source->path);
   }
   if (s_opt == METIS_REPORT) {
      printf ("\n");
      printf ("# end-of-file.  done, finito, completare, whimper.\n");
   }
   return 0;
}

char
metis_rptg__number      (int x, int v, char *a_out)
{
   if      (v <=   0)   sprintf (a_out, " · ");
   else if (v <=   9)   sprintf (a_out, " %d ", v);
   else if (v <=  99) {
      if (x % 2 == 0)   sprintf (a_out, "%2d ", v);
      else              sprintf (a_out, " %2d", v);
   }
   else if (v <= 999)   sprintf (a_out, "%3d", v);
   else                 sprintf (a_out, "999");
   return 0;
}

char
metis_rptg_matrix       (void)
{
   int         x, y;
   int         v [10][10];
   char        s [10][10][5];
   int         q [ 2][ 2];
   char        t [ 2][ 2][5];
   char       *u = "-ymwdst!";
   char       *i = "anvcwlm-";
   tTASK      *x_task      = NULL;
   q [0][0] = q [0][1] = q [1][0] = q [1][1] = 0;
   for (x = 0; x < 10; ++x) {
      for (y = 0; y < 10; ++y) {
         v [x][y] = 0;
      }
   }
   metis_task_by_cursor (YDLST_HEAD, &x_task);
   while (x_task != NULL) {
      x  = strchr (u, x_task->urg);
      if (x == NULL)  x  = 7;
      else            x -= (int) u;
      y  = strchr (i, x_task->imp);
      if (y == NULL)  y  = 7;
      else            y -= (int) i;
      /*> printf ("%-20.20s   %c %c   %dx %dy\n", x_task->txt, x_task->urg, x_task->imp, x, y);   <*/
      if (x <= 3) {
         if (y <= 3) ++(q [0][0]);
         else        ++(q [0][1]);
      } else {
         if (y <= 3) ++(q [1][0]);
         else        ++(q [1][1]);
      }
      ++(v [x][8]);
      ++(v [8][y]);
      ++(v [x][y]);
      ++(v [8][8]);
      metis_task_by_cursor (YDLST_NEXT, &x_task);
   }
   for (x = 0; x < 10; ++x) {
      for (y = 0; y < 10; ++y) {
         metis_rptg__number (x, v [x][y], s [x][y]);
      }
   }
   metis_rptg__number (0, q [0][0], t [0][0]);
   metis_rptg__number (1, q [1][0], t [1][0]);
   metis_rptg__number (2, q [0][1], t [0][1]);
   metis_rptg__number (3, q [1][1], t [1][1]);
   /*  */ printf ("                                                       \n");
   /*  */ printf ("           7+       urgency       6-                   \n");
   /*  */ printf (" II                                         I          \n");
   /*  */ printf ("    %3s  -   y   m   w   d   s   t   !  %3s            \n", t [0][0], t [1][0]);
   /*  */ printf ("       ƒ€€€€€€€‰€€€€€€€‰€€€€€€€‰€€€€€€€‚               \n");
   y = 0; printf ("     a %3s %3sŒ%3s %3s%3s %3sŒ%3s %3s a %3s absolute\n", s [0][y], s [1][y], s [2][y], s [3][y], s [4][y], s [5][y], s [6][y], s [7][y], s [8][y]);
   /*  */ printf ("        focus Œ              Œ drive                \n");
   y = 1; printf ("     n %3s %3sŒ%3s %3s%3s %3sŒ%3s %3s n %3s need    \n", s [0][y], s [1][y], s [2][y], s [3][y], s [4][y], s [5][y], s [6][y], s [7][y], s [8][y]);
   /*  */ printf ("  i    ‡·······Š·······Š·······Š·······†               \n");
   y = 2; printf ("  m  v %3s %3sŒ%3s %3s%3s %3sŒ%3s %3s v %3s value   \n", s [0][y], s [1][y], s [2][y], s [3][y], s [4][y], s [5][y], s [6][y], s [7][y], s [8][y]);
   /*  */ printf ("  p           Œ              Œ                      \n");
   y = 3; printf ("  o  c %3s %3sŒ%3s %3s%3s %3sŒ%3s %3s c %3s crave   \n", s [0][y], s [1][y], s [2][y], s [3][y], s [4][y], s [5][y], s [6][y], s [7][y], s [8][y]);
   /*  */ printf ("  r    ‡€€€€€€€Š€€€€€€€Š€€€€€€€Š€€€€€€€†               \n");
   y = 4; printf ("  t  w %3s %3sŒ%3s %3s%3s %3sŒ%3s %3s w %3s want    \n", s [0][y], s [1][y], s [2][y], s [3][y], s [4][y], s [5][y], s [6][y], s [7][y], s [8][y]);
   /*  */ printf ("  a           Œ              Œ                      \n");
   y = 5; printf ("  n  l %3s %3sŒ%3s %3s%3s %3sŒ%3s %3s l %3s like    \n", s [0][y], s [1][y], s [2][y], s [3][y], s [4][y], s [5][y], s [6][y], s [7][y], s [8][y]);
   /*  */ printf ("  c    ‡·······Š·······Š·······Š·······†               \n");
   y = 6; printf ("  e  m %3s %3sŒ%3s %3s%3s %3sŒ%3s %3s m %3s might   \n", s [0][y], s [1][y], s [2][y], s [3][y], s [4][y], s [5][y], s [6][y], s [7][y], s [8][y]);
   /*  */ printf ("        dump  Œ              Œ  weed                \n");
   y = 7; printf ("     - %3s %3sŒ%3s %3s%3s %3sŒ%3s %3s - %3s backlog \n", s [0][y], s [1][y], s [2][y], s [3][y], s [4][y], s [5][y], s [6][y], s [7][y], s [8][y]);
   /*  */ printf ("       „€€€€€€€ˆ€€€€€€€ˆ€€€€€€€ˆ€€€€€€€…               \n");
   /*  */ printf ("    %3s  -   y   m   w   d   s   t   !  %3s            \n", t [0][1], t [1][1]);
   /*  */ printf (" IV                                         III        \n");
   y = 8; printf ("        %3s %3s %3s %3s %3s %3s %3s %3s                \n", s [0][y], s [1][y], s [2][y], s [3][y], s [4][y], s [5][y], s [6][y], s [7][y]);
   /*  */ printf ("                                           %3s total   \n", s [8][y]);
   /*  */ printf ("         b   y   m   w   d   s   t   n                 \n");
   /*  */ printf ("         a   e   o   e   a   o   o   o                 \n");
   /*  */ printf ("         c   a   n   e   y   o   d   w                 \n");
   /*  */ printf ("         k   r   t   k   s   n   a   ·                 \n");
   /*  */ printf ("         l   s   h   s       e   y   n                 \n");
   /*  */ printf ("         o       s           s       o                 \n");
   /*  */ printf ("         g                   t       w                 \n");
   /*  */ printf ("                                                       \n");
   return 0;
}

char
metis_reporter           (void)
{
   char        rce         =  -10;
   char        rc          =    0;
   rc = poly_rptg_lookup   (my.run_file);
   --rce;  if (rc < 0)  return rce;
   --rce;  switch (s_abbr) {
   case 'l' : metis_inventory ();  break;
   case 'm' : metis_rptg_matrix ();  break;
   default  : return rce;
   }
   return 0;
}
