/*============================----beg-of-source---============================*/
#include   "metis.h"



/*====================------------------------------------====================*/
/*===----                     labels and positions                     ----===*/
/*====================------------------------------------====================*/
static void   o___LABEL___________o (void) { return; }

char         /*-> break label into coordinates -------[ ------ [gc.722.112.13]*/ /*-[01.0000.304.#]-*/ /*-[--.---.---.--]-*/
api_ymap_locator        (char a_strict, char *a_label, ushort *u, ushort *x, ushort *y, ushort *z)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =   -1;
   tTASK      *x_task      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YMAP   yLOG_senter  (__FUNCTION__);
   /*---(defaults)-----------------------*/
   DEBUG_YMAP   yLOG_snote   ("defaulting");
   if (u != NULL)  *u = 0;
   if (x != NULL)  *x = 0;
   if (y != NULL)  *y = 0;
   if (z != NULL)  *z = 0;
   /*---(defense)------------------------*/
   DEBUG_YMAP   yLOG_spoint  (a_label);
   --rce;  if (a_label == NULL) {
      DEBUG_YMAP   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YMAP   yLOG_snote   (a_label);
   /*---(find)---------------------------*/
   metis_epoch_by_name (a_label, &x_task);
   DEBUG_YMAP   yLOG_spoint  (x_task);
   --rce;  if (x_task == NULL) {
      DEBUG_YMAP   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(save-back)----------------------*/
   if (x != NULL)  *y = x_task->x;
   if (y != NULL)  *y = x_task->y;
   /*---(complete)-----------------------*/
   DEBUG_YMAP   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char         /*-> return address for coordinates -----[ ------ [gc.722.112.13]*/ /*-[01.0000.304.#]-*/ /*-[--.---.---.--]-*/
api_ymap_addressor      (char a_strict, char *a_label, ushort u, ushort x, ushort y, ushort z)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =   -1;
   tTASK      *x_task      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YMAP   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YMAP   yLOG_spoint  (a_label);
   --rce;  if (a_label == NULL) {
      DEBUG_YMAP   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   strlcpy (a_label, "", LEN_LABEL);
   /*---(find)---------------------------*/
   metis_task_by_cursor (YDLST_HEAD, &x_task);
   while (x_task != NULL) {
      if (x == x_task->x && y == x_task->y) {
         strlcpy (a_label, x_task->epoch, LEN_LABEL);
         DEBUG_YMAP   yLOG_sexit   (__FUNCTION__);
         return 0;
      }
      metis_task_by_cursor (YDLST_NEXT, &x_task);
   }
   /*---(complete)-----------------------*/
   DEBUG_YMAP   yLOG_sexitr  (__FUNCTION__, rce);
   return rce;  /* negative 1 means not recognized as an address */
}



/*====================------------------------------------====================*/
/*===----                     map loading support                      ----===*/
/*====================------------------------------------====================*/
static void   o___LOAD____________o (void) { return; }

char
api_ymap_sizer          (char a_axis, ushort *n, ushort *a, ushort *b, ushort *c, ushort *e, ushort *m, ushort *x)
{
   char        rce         =  -10;
   char        rc          =    0;
   DEBUG_YMAP   yLOG_senter  (__FUNCTION__);
   DEBUG_YMAP   yLOG_schar   (a_axis);
   --rce;  switch (a_axis) {
   case YMAP_UNIV  :
      DEBUG_YMAP   yLOG_snote   ("univ");
      DEBUG_YMAP   yLOG_sint    (NTABS);
      if (n != NULL)  *n = NTABS;
      if (a != NULL)  *a = NTABS;
      if (b != NULL)  *b = 0;
      if (c != NULL)  *c = CTAB;
      if (e != NULL)  *e = 0;
      if (m != NULL)  *m = 0;
      if (x != NULL)  *x = 0;
      break;
   case YMAP_XAXIS :
      DEBUG_YMAP   yLOG_snote   ("cols");
      DEBUG_YMAP   yLOG_sint    (NCOLS);
      if (n != NULL)  *n = NCOLS;
      if (a != NULL)  *a = WCOLS;
      if (b != NULL)  *b = BCOL;
      if (c != NULL)  *c = CCOL;
      if (e != NULL)  *e = ECOL;
      if (m != NULL)  *m = 0;
      if (x != NULL)  *x = 0;
      break;
   case YMAP_YAXIS :
      DEBUG_YMAP   yLOG_snote   ("rows");
      DEBUG_YMAP   yLOG_sint    (NROWS);
      if (n != NULL)  *n = NROWS;
      if (a != NULL)  *a = WROWS;
      if (b != NULL)  *b = BROW;
      if (c != NULL)  *c = CROW;
      if (e != NULL)  *e = EROW;
      if (m != NULL)  *m = 0;
      if (x != NULL)  *x = 0;
      break;
   case YMAP_ZAXIS :
      DEBUG_YMAP   yLOG_snote   ("depth NOT supported in metis");
      DEBUG_YMAP   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   case YMAP_WHEN  :
      DEBUG_YMAP   yLOG_snote   ("when NOT supported in metis");
      DEBUG_YMAP   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   default :
      DEBUG_YMAP   yLOG_snote   ("axis not understood");
      DEBUG_YMAP   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
      break;
   }
   DEBUG_YMAP   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
api_ymap_entry          (char a_axis, ushort a_pos, short *r_ref, uchar *r_wide, uchar *r_used)
{
   char        rce         =  -10;
   char        rc          =    0;
   --rce;  switch (a_axis) {
   case YMAP_UNIV  :
      /*> rc = TAB_entry  (a_pos, r_ref, r_wide, r_used);                             <*/
      *r_ref  = a_pos;
      *r_wide = 1;
      *r_used = YMAP_USED;
      break;
   case YMAP_XAXIS :
      /*> rc = COL_entry  (a_pos, r_ref, r_wide, r_used);                             <*/
      *r_ref  = a_pos;
      *r_wide = 1;
      *r_used = YMAP_USED;
      break;
   case YMAP_YAXIS :
      /*> rc = ROW_entry  (a_pos, r_ref, r_wide, r_used);                             <*/
      *r_ref  = a_pos;
      *r_wide = 1;
      *r_used = YMAP_USED;
      break;
   case YMAP_ZAXIS :
   case YMAP_WHEN  :
      if (r_ref  != NULL)  *r_ref  = 0;
      if (r_wide != NULL)  *r_wide = 0;
      if (r_used != NULL)  *r_used = 0;
      break;
   default :
      return rce;
      break;
   }
   return rc;
}



/*====================------------------------------------====================*/
/*===----                       content update                         ----===*/
/*====================------------------------------------====================*/
static void   o___UPDATE__________o (void) { return; }

char
api_ymap_placer         (char a_axis, ushort b, ushort c, ushort e)
{
   char        rce         =  -10;
   char        rc          =    0;
   DEBUG_YMAP   yLOG_senter  (__FUNCTION__);
   DEBUG_YMAP   yLOG_schar   (a_axis);
   --rce;  switch (a_axis) {
   case YMAP_UNIV  :
      DEBUG_YMAP   yLOG_snote   ("univ NOT ALLOWED");
      DEBUG_YMAP   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
      break;
   case YMAP_XAXIS :
      DEBUG_YMAP   yLOG_snote   ("cols");
      BCOL = b;
      CCOL = c;
      ECOL = e;
      DEBUG_YMAP   yLOG_svalue  ("b", BCOL);
      DEBUG_YMAP   yLOG_svalue  ("c", CCOL);
      DEBUG_YMAP   yLOG_svalue  ("e", ECOL);
      break;
   case YMAP_YAXIS :
      DEBUG_YMAP   yLOG_snote   ("rows");
      BROW = b;
      CROW = c;
      EROW = e;
      DEBUG_YMAP   yLOG_svalue  ("b", BROW);
      DEBUG_YMAP   yLOG_svalue  ("c", CROW);
      DEBUG_YMAP   yLOG_svalue  ("e", EROW);
      break;
   case YMAP_ZAXIS :
      DEBUG_YMAP   yLOG_snote   ("depth NOT supported in metis");
      DEBUG_YMAP   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   case YMAP_WHEN  :
      DEBUG_YMAP   yLOG_snote   ("when NOT supported in metis");
      DEBUG_YMAP   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   default :
      DEBUG_YMAP   yLOG_snote   ("axis not understood");
      DEBUG_YMAP   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
      break;
   }
   DEBUG_YMAP   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
api_ymap_done           (void)
{
   /*---(header)-------------------------*/
   DEBUG_YMAP   yLOG_senter  (__FUNCTION__);
   /*---(complete)-----------------------*/
   DEBUG_YMAP   yLOG_sexit   (__FUNCTION__);
   return 0;
}



