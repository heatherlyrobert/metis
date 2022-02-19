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
   if (s_opt == METIS_REPORT) {
      metis_rptg__header ();
      printf ("#@ x-parse   åÏ--·Ï-------------------·Ï-------------------·Ï·Ï·Ï·Ï·Ï-----------------------------------------------------------·Ï-------------------·Ï---æ\n");
      printf ("#@ titles    åseq·category·one·········category·two·········u·i·e·p·task·detail··················································source···············lineæ\n");
      printf ("\n");
   }
   for (i = 0; i < g_ntask; ++i) {
      if (s_opt != METIS_DUMP) {
         if (i %  5 == 0 && i != 0)  printf ("\n");
         if (i % 25 == 0)  printf ("#eq·category·one·········category·two·········u·i·e·p·task·detail··················································source···············line\n\n");
      }
      printf ("%3d %-20.20s %-20.20s %c %c %c %c %-60.60s %-20.20s %4d\n",
            g_tasks [i].seq, g_tasks [i].one, g_tasks [i].two,
            g_tasks [i].urg, g_tasks [i].imp, g_tasks [i].est, g_tasks [i].prg,
            g_tasks [i].txt, g_tasks [i].source, g_tasks [i].line);
   }
   if (s_opt == METIS_REPORT) {
      printf ("\n");
      printf ("# end-of-file.  done, finito, completare, whimper.\n");
   }
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
   default  : return rce;
   }
   return 0;
}
