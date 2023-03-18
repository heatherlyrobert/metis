/*============================----beg-of-source---============================*/
#include   "metis.h"


char
metis_yjobs_read        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(handle)-------------------------*/
   DEBUG_PROG    yLOG_note    ("database read action (YJOBS_READ)");
   rc = metis_db_read  ();
   DEBUG_PROG    yLOG_value   ("read"      , rc);
   /*---(complete)-----------------------*/
   return rc;
}

char
metis_yjobs_stats       (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        t           [LEN_LABEL] = "";
   /*---(header)-------------------------*/
   DEBUG_PROG    yLOG_note    ("database statistics action (YJOBS_STATS)");
   /*---(display)------------------------*/
   printf ("#!/usr/local/bin/metis --stats\n");
   printf ("db     å%sæ\n" , my.n_db);
   printf ("name   å%sæ\n" , g_audit.name);
   printf ("ver    å%sæ\n" , g_audit.vernum);
   strl4main (g_audit.major , t , 0, 'c', '-', LEN_LABEL);
   printf ("major  %7.7s\n", t);
   strl4main (g_audit.minor , t , 0, 'c', '-', LEN_LABEL);
   printf ("minor  %7.7s\n", t);
   strl4main (g_audit.source, t , 0, 'c', '-', LEN_LABEL);
   printf ("source %7.7s\n", t);
   strl4main (g_audit.task  , t , 0, 'c', '-', LEN_LABEL);
   printf ("task   %7.7s\n", t);
   printf ("stamp  å%sæ\n" , g_audit.heartbeat);
   /*---(complete)-----------------------*/
   return 0;
}

char
metis_yjobs_write       (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(handle)-------------------------*/
   DEBUG_PROG    yLOG_note    ("database write action (YJOBS_WRITE)");
   rc = metis_db_write ();
   DEBUG_PROG    yLOG_value   ("write"     , rc);
   /*---(complete)-----------------------*/
   return rc;
}

char
metis_yjobs_pull        (cchar *a_data)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   int         l           =    0;
   /*---(handle)-------------------------*/
   DEBUG_PROG    yLOG_note    ("data pull action (YJOBS_PULL)");
   l = strlen (a_data);
   if (a_data [l - 1] == '/')   my.source = DATA_SOURCES;
   else                         my.source = DATA_FILE;
   metis_data_refresh ();
   DEBUG_PROG    yLOG_value   ("pull"      , rc);
   /*---(complete)-----------------------*/
   return rc;
}



/*====================------------------------------------====================*/
/*===----                        main driver                           ----===*/
/*====================------------------------------------====================*/
static void  o___DRIVER__________o () { return; }

char
metis_yjobs             (cchar a_req, cchar *a_data)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   int         l           =    0;
   /*---(header)-------------------------*/
   DEBUG_PROG    yLOG_enter   (__FUNCTION__);
   DEBUG_PROG    yLOG_char    ("a_req"     , a_req);
   DEBUG_PROG    yLOG_info    ("a_data"    , a_data);
   DEBUG_PROG    yLOG_char    ("source"    , my.source);
   /*---(handle)-------------------------*/
   switch (a_req) {
   case YJOBS_READ     :
      DEBUG_PROG    yLOG_note    ("read database");
      if (!yJOBS_ifgather ())  rc = metis_yjobs_read     ();
      break;
   case YJOBS_STATS    :
      DEBUG_PROG    yLOG_note    ("called for stats");
      rc = metis_yjobs_stats ();
      break;
   case YJOBS_WRITE    :
      DEBUG_PROG    yLOG_note    ("write database");
      /*> if (my.source == DATA_DATABASE)  rc = metis_yjobs_write    ();              <*/
      rc = metis_yjobs_write    ();
      break;
   case YJOBS_PULL     :
      DEBUG_PROG    yLOG_note    ("called for pull");
      rc = metis_yjobs_pull     (a_data);
      break;
   case YJOBS_LOCALRPT : 
      DEBUG_PROG    yLOG_note    ("called for localrpt");
      l = strlen (a_data);
      if (a_data [l - 1] == '/')   my.source = DATA_SOURCES;
      else                         my.source = DATA_FILE;
      my.source = DATA_FILE;
      strlcpy (my.file, a_data, LEN_PATH);
      break;
   case YJOBS_REPORT   :
      DEBUG_PROG    yLOG_note    ("called for report");
      rc = metis_inventory ();
      /*> rc = metis_rptg_matrix ();                                                  <*/
      break;
   case YJOBS_PURGE    :
      DEBUG_PROG    yLOG_note    ("called for purge");
      rc = metis_data_purge_all ();
      break;
   }
   /*---(trouble)------------------------*/
   if (rc < 0) {
      DEBUG_PROG    yLOG_exitr   (__FUNCTION__, rc);
      return rc;
   }
   /*---(complete)-----------------------*/
   DEBUG_PROG    yLOG_exit    (__FUNCTION__);
   return 0;
}
