/*============================----beg-of-source---============================*/
#include   "metis.h"


/*
 * metis § dv4<· § unit test task dump reporting                                          § N2GMeX §  · §
 * metis § dv2<· § unit test task matrix reporting                                        § N2GMeZ §  · §
 */

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
metis_rptg__header      (FILE *f)
{
   fprintf (f, "##   %s %s\n", P_NAMESAKE, P_HERITAGE);
   fprintf (f, "##   version %s, %s\n", P_VERNUM, P_VERTXT);
   fprintf (f, "##   inventory of tasks from current source\n");
   fprintf (f, "\n");
   fprintf (f, "#@ style     V = printable columnar values\n");
   return 0;
}

char
metis_inventory         (FILE *f)
{
   int         i           =    0;
   int         x_max       =    0;
   tTASK      *x_task      = NULL;
   char        x_days      [LEN_TERSE] = "";
   char       *x_heads     = "#ef major--------------- minor-------------------- seq u i e p s  task-detail----------------------------------------------------------- epoch- age dys line source--------------------------------------------------------------------------\n";
   char       *x_div       = "#·· ···················· ························· ··· · · · · ·  ······································································ ······ ··· ··· ···· ················································································\n";
   long        v           =    0;
   char        x_age       [LEN_TERSE] = "";
   metis_rptg__header (f);
   fprintf (f, "#@ x-parse  14åÏ--·Ï-------------------·Ï------------------------·Ï--·Ï·Ï·Ï·Ï·Ï··Ï---------------------------------------------------------------------·Ï-----·Ï--·Ï--·Ï---·Ï-------------------------------------------------------------------------------æ\n");
   fprintf (f, "#@ titles     åref·major················minor·····················seq·u·i·e·p·s··task·detail····························································epoch··age·dys·line·source··········································································æ\n");
   fprintf (f, "\n");
   x_max = metis_task_count ();
   for (i = 0; i < x_max; ++i) {
      /*> if (i %  5 == 0 && i != 0)  fprintf (f, "\n");                              <*/
      if (i % 25 == 0) {
         if (i != 0)  fprintf (f, x_div);
         fprintf (f, x_heads);
      }
      if (i %  5 == 0)  fprintf (f, x_div);
      metis_task_by_index (i, &x_task);
      if (x_task == NULL) {
         fprintf (f, "FOUND A NULL\n");
      }
      str4mongo (x_task->epoch, &v);
      strlage (v, '-', x_age);
      if (x_task->days > 0)  sprintf  (x_days, "%3d", x_task->days);
      else                   strlcpy  (x_days, "  ·", LEN_TERSE);
      fprintf (f, "%3d %-20.20s %-25.25s %3d %c %c %c %c %c  %-70.70s %-6.6s %-3.3s %-3.3s %4d %-80.80s\n",
            i, x_task->minor->major->name, x_task->minor->name, x_task->seq,
            x_task->urg, x_task->imp, x_task->est, x_task->prg, x_task->shr,
            x_task->txt, x_task->epoch, x_age, x_days, x_task->line, x_task->source->path);
   }
   fprintf (f, x_div);
   fprintf (f, x_heads);
   fprintf (f, "\n");
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
metis_rptg_matrix       (FILE *f)
{
   int         x, y;
   int         v [10][10];
   char        s [10][10][5];
   int         q [ 2][ 2];
   char        t [ 2][ 2][5];
   char        u           [LEN_LABEL] = "";
   char        i           [LEN_LABEL] = "";
   /*> char       *u = "-ymwdst!";                                                    <* 
    *> char       *i = "anvcgli-";                                                    <*/
   strlrev (u, METIS_URGS, LEN_LABEL);
   strlcpy (i, METIS_IMPS, LEN_LABEL);
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
   /*  */ fprintf (f, "                                                       \n");
   /*  */ fprintf (f, "                    urgency                            \n");
   /*  */ fprintf (f, " II                                         I          \n");
   /*  */ fprintf (f, "    %3s  -   y   m   w   d   s   t   !  %3s            \n", t [0][0], t [1][0]);
   /*  */ fprintf (f, "       ƒ€€€€€€€‰€€€€€€€‰€€€€€€€‰€€€€€€€‚               \n");
   y = 0; fprintf (f, "     a %3s %3sŒ%3s %3s%3s %3sŒ%3s %3s a %3s absolute - no choice, core\n"         , s [0][y], s [1][y], s [2][y], s [3][y], s [4][y], s [5][y], s [6][y], s [7][y], s [8][y]);
   /*  */ fprintf (f, "              Œ              Œ                      \n");
   y = 1; fprintf (f, "     n %3s %3sŒ%3s %3s%3s %3sŒ%3s %3s n %3s need     - required as a fact\n"      , s [0][y], s [1][y], s [2][y], s [3][y], s [4][y], s [5][y], s [6][y], s [7][y], s [8][y]);
   /*  */ fprintf (f, "  i    ‡·······Š·······Š·······Š·······†               \n");
   y = 2; fprintf (f, "  m  v %3s %3sŒ%3s %3s%3s %3sŒ%3s %3s v %3s value    - definable value\n"         , s [0][y], s [1][y], s [2][y], s [3][y], s [4][y], s [5][y], s [6][y], s [7][y], s [8][y]);
   /*  */ fprintf (f, "  p           Œ              Œ                      \n");
   y = 3; fprintf (f, "  o  c %3s %3sŒ%3s %3s%3s %3sŒ%3s %3s c %3s crave    - believe its necessary\n"   , s [0][y], s [1][y], s [2][y], s [3][y], s [4][y], s [5][y], s [6][y], s [7][y], s [8][y]);
   /*  */ fprintf (f, "  r    ‡€€€€€€€Š€€€€€€€Š€€€€€€€Š€€€€€€€†               \n");
   y = 4; fprintf (f, "  t  w %3s %3sŒ%3s %3s%3s %3sŒ%3s %3s g %3s good     - not required, but useful\n", s [0][y], s [1][y], s [2][y], s [3][y], s [4][y], s [5][y], s [6][y], s [7][y], s [8][y]);
   /*  */ fprintf (f, "  a           Œ              Œ                      \n");
   y = 5; fprintf (f, "  n  l %3s %3sŒ%3s %3s%3s %3sŒ%3s %3s l %3s like     - want if easily done\n"     , s [0][y], s [1][y], s [2][y], s [3][y], s [4][y], s [5][y], s [6][y], s [7][y], s [8][y]);
   /*  */ fprintf (f, "  c    ‡·······Š·······Š·······Š·······†               \n");
   y = 6; fprintf (f, "  e  m %3s %3sŒ%3s %3s%3s %3sŒ%3s %3s i %3s idea     - just an idea, no plan\n"   , s [0][y], s [1][y], s [2][y], s [3][y], s [4][y], s [5][y], s [6][y], s [7][y], s [8][y]);
   /*  */ fprintf (f, "              Œ              Œ                      \n");
   y = 7; fprintf (f, "     - %3s %3sŒ%3s %3s%3s %3sŒ%3s %3s - %3s backlog  - just getting on a list\n"  , s [0][y], s [1][y], s [2][y], s [3][y], s [4][y], s [5][y], s [6][y], s [7][y], s [8][y]);
   /*  */ fprintf (f, "       „€€€€€€€ˆ€€€€€€€ˆ€€€€€€€ˆ€€€€€€€…               \n");
   /*  */ fprintf (f, "    %3s  -   y   m   w   d   s   t   !  %3s            \n", t [0][1], t [1][1]);
   /*  */ fprintf (f, " IV                                         III        \n");
   y = 8; fprintf (f, "        %3s %3s %3s %3s %3s %3s %3s %3s                \n", s [0][y], s [1][y], s [2][y], s [3][y], s [4][y], s [5][y], s [6][y], s [7][y]);
   /*  */ fprintf (f, "                                           %3s total   \n", s [8][y]);
   /*  */ fprintf (f, "         b   y   m   w   d   s   t   n                 \n");
   /*  */ fprintf (f, "         a   e   o   e   a   o   o   o                 \n");
   /*  */ fprintf (f, "         c   a   n   e   y   o   d   w                 \n");
   /*  */ fprintf (f, "         k   r   t   k   s   n   a   ·                 \n");
   /*  */ fprintf (f, "         l   s   h   s       e   y   n                 \n");
   /*  */ fprintf (f, "         o       s           s       o                 \n");
   /*  */ fprintf (f, "         g                   t       w                 \n");
   /*  */ fprintf (f, "                                                       \n");
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
      /*> case 'l' : metis_inventory ();  break;                                         <*/
      /*> case 'm' : metis_rptg_matrix ();  break;                                       <*/
   default  : return rce;
   }
   return 0;
}
