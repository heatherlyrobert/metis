/*============================----beg-of-source---============================*/
#ifndef METIS
#define METIS yes

/*===[[ BEG_HEADER ]]=========================================================*/

/*===[[ ONE_LINERS ]]=========================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-12345678901-12345678901-*/

#define     P_FOCUS     "PT (productivity/time mgmt)"
#define     P_NICHE     "td (todo list)"
#define     P_SUBJECT   "task management"
#define     P_PURPOSE   "task consolitation, visualization, and navigation system"

#define     P_NAMESAKE  "metis-okeanides (wise-counsel)"
#define     P_HERITAGE  "titan goddess of deep thought, wisdom, wise counsel, and cunning"
#define     P_IMAGERY   "singularly graceful, stately, and regal goddess"
#define     P_REASON    "smart task management leads to better priorities and decisions"

#define     P_ONELINE   P_NAMESAKE " " P_SUBJECT

#define     P_BASENAME  "metis"
#define     P_FULLPATH  "/usr/local/bin/metis"
#define     P_SUFFIX    "tasks"
#define     P_CONTENT   "scan-card task list"

#define     P_SYSTEM    "gnu/linux   (powerful, ubiquitous, technical, and hackable)"
#define     P_LANGUAGE  "ansi-c      (wicked, limitless, universal, and everlasting)"
#define     P_CODESIZE  "large       (appoximately 10,000 slocl)"
#define     P_DEPENDS   "none"

#define     P_AUTHOR    "heatherlyrobert"
#define     P_CREATED   "2008-06"

#define     P_VERMAJOR  "1.--, improve for more and more use and value"
#define     P_VERMINOR  "1.6-, adding central database capability"    
#define     P_VERNUM    "1.6a"
#define     P_VERTXT    "binary database integrated and unit tested"

#define     P_PRIORITY  "direct, simple, brief, vigorous, and lucid (h.w. fowler)"
#define     P_PRINCIPAL "[grow a set] and build your wings on the way down (r. bradbury)"
#define     P_REMINDER  "there are many better options, but i *own* every byte of this one"


/*
 * metis § wc8·· § write interactive-use manual for metis                                 § M1GDo1 §  · §
 *
 */


/*
 * few people regularly (except under duress) keep task lists
 *
 * people naturally keep a few notes or a short list near usage...
 *    -- meeting action items in a spreadsheet
 *    -- errand list in car
 *    -- grocery list on refrigerator
 *
 * when attempting to move to a better solution, only a few can hang on...
 *    -- project managers can delegate to a pmo
 *    -- business managers can delegate to their secretaries
 *    -- desk bound workers can use post-its or spreadsheets
 * even with these solutions, people just quit because its overwhelming and
 * simply forgetting many things can actually help.  90% of the effort is
 * tracking and closing useless things -- totally lose the big picture.
 * all these become an avalanch of minutia.
 *
 *
 */


/*===[[ SUMMARY ]]============================================================*

 *   metis is a simple, clean, and powerful todo framework allowing us to
 *   consolidate and manage our our plans and tasks into a single system.  it
 *   is also intensionally read-only system and leaves editing to vi.
 *
 */
/*===[[ PURPOSE ]]============================================================*

 *   metis is a task/todo visualizer that establishes a categorization framework
 *   which allows us to quickly capture tasks, group them into a two layer
 *   hierarchy, and identify several meaningful and standard sorting fields.
 *
 *   in this undertaking, we have incorporated pieces of several systems...
 *      - scancards  : while a dead product, it was wicked flexible and cool
 *      - covey      : past the BS was the age old urgent/important he stole ;)
 *      - gtd        : forget this, its a crack-infused rat-hole.  LOL.  ymmv.
 *      - ipod todo  : clean but not quite a fit, repeat stuff is not right
 *      - list-it    : great newton system, but really just list mgmt
 *      - zenbe      : sweet list app, my use evolved to a two layer system
 *      - focuses    : our two layer visioning system
 *
 *   we are not indending a traditional, all-in-one application, so....
 *      - no specialized database, we will use ascii text files
 *      - no captive editor, we'll just use vi so that the format stays simple
 *      - no complicated fields that require extensive error checking
 *      - no calendaring/scheduling, that's a specific application and separate
 *      - no repeat, also a scheduling thing or a daily cycle (which is unique)
 *      - no mice and menus (come on, what idiot thinks these are productive) :<
 *      - no full screen layouts, subwindows, and fancy logo crap
 *      - no tool tips and help screens
 *
 *   we are focused specifically on our own use and don't need all the hand
 *   coddling -- focusing on technically strong power users that just need
 *   structure and sweet visualization without all the contraints.  we also
 *   need a system that we can tune/adjust without jumping through hoops.
 *
 *   our expectation is that this will be a read-only, sexy, opengl visualizer
 *   weighing in at less than 1,000 source lines of code.  it will display tasks
 *   in the same format everytime and use color coding to make filtering and
 *   review as quick and efficient as possible.  it will also specialize in
 *   formats that can share the screen with programming windows.
 *
 *   metis will integrate the following rational limits/standards...
 *      - read its tasks from ~/g_hlosdo/master.tasks
 *      - show up to 48 tasks in the column and only 10 in the ticker
 *      - will provide defaults for incorrect/misunderstood task information
 *
 *   as always, there are many, stable, accepted, existing programs and
 *   utilities built by better programmers that are likely superior in speed,
 *   size, capability, and reliability; BUT, i would not have learned nearly as
 *   much using them, so i follow the adage...
 *
 *   TO TRULY LEARN> do not seek to follow in the footsteps of the men of old;
 *   seek what they sought ~ Matsuo Basho
 *
 *   the underlying idea is to use this build process to learn new programming
 *   capabilities and put additional pressure on me to improve my standards,
 *   maintenance, and coding environment.  just using programs and ideas does
 *   not lead to understanding -- so get messy ;)
 *
 *   any one who wishes to become a good writer [programmer] should endeavour,
 *   before he allows himself to be tempted by the more showy qualities, to be
 *   direct, simple, brief, vigourous, and lucid -- henry watson fowler (1908)
 *
 *   the central enemy of reliability is complexity - daniel geer
 *
 *   simplicity is prerequisite for reliability - edsger dijkstra
 *
 */
/*============================================================================*/

#include <X11/X.h>                   /* main header                           */
#include <X11/Xlib.h>                /* c-api (xlib) header                   */
#include <X11/Xutil.h>               /* c-api (xlib) header                   */
#include <X11/keysym.h>              /* for resolving keycodes/keysyms        */
#include <X11/extensions/shape.h>    /* shape extention -- funky windows    */


/*===[[ GLX HEADERS ]]========================================================*/
#include <GL/gl.h>                   /* main header for opengl                */
#include <GL/glx.h>                  /* for xlib/opengl integration           */

/*===[[ CUSTOM LIBRARIES ]]===================================================*/
#include    <yURG.h>         /* CUSTOM : heatherly urgent processing          */
#include    <yLOG.h>         /* CUSTOM : heatherly program logging            */

/*---(custom vi-keys)--------------------*/
#include    <yKEYS.h>             /* heatherly vi-keys key handling           */
#include    <yMODE.h>             /* heatherly vi-keys mode tracking          */
#include    <yMACRO.h>            /* heatherly vi-keys macro processing       */
#include    <ySRC.h>              /* heatherly vi-keys source editing         */
#include    <yCMD.h>              /* heatherly vi-keys command processing     */
#include    <yVIEW.h>             /* heatherly vi-keys view management        */
#include    <yMAP.h>              /* heatherly vi-keys location management    */
#include    <yFILE.h>             /* heatherly vi-keys content file handling  */
#include    <yVIOPENGL.h>         /* heatherly vi-keys opengl handler         */

#include    <yJOBS.h>             /* heatherly job execution and control      */
#include    <yREGEX.h>       /* CUSTOM  heatherly regular expressions         */
#include    <ySORT.h>        /* CUSTOM  heatherly sort and search             */


#include    <ySTR.h>         /* CUSTOM : heatherly string handling            */
#include    <yX11.h>         /* heatherly xlib/glx setup            */
#include    <yFONT.h>        /* heatherly text display for opengl   */
#include    <yGLTEX.h>       /* heatherly opengl texture support              */
#include    <yCOLOR.h>       /* heatherly opengl color support                */

#include    <yDLST_solo.h>   /* heatherly                                     */

#include   <stdio.h>
#include   <stdlib.h>                  /* getenv()                            */
#include   <string.h>
#include   <sys/time.h>
#include   <sys/stat.h>
#include   <unistd.h>
#include   <time.h>
#include   <signal.h>                  /* signal(), sigalarm()                */
#include   <dirent.h>



typedef   unsigned int  uint;
typedef     struct      dirent      tDIRENT;
typedef     struct      stat        tSTAT;




extern int       g_mtall;                   /* menu texture height            */
extern int       g_mwide;                   /* menu texture width             */
extern uint      g_mtex;                    /* menu texture                   */
extern uint      g_mfbo;                    /* menu fbo                       */
extern uint      g_mdep;                    /* menu depth                     */

extern      char          unit_answer [LEN_FULL];



extern     char      g_print     [LEN_RECD];



/*---(statistics)-----------*/
#define     METIS_URG      'u'
#define     METIS_IMP      'i'
#define     METIS_EST      'e'
#define     METIS_PRG      'p'
#define     METIS_SHR      's'
/*---(other sorts)----------*/
#define     METIS_ORIG     'o'
#define     METIS_DATE     'd'
/*---(validations)----------*/
#define     METIS_URGS     my.urgs
#define     METIS_IMPS     my.imps
#define     METIS_ESTS     my.ests
#define     METIS_PRGS     my.prgs
#define     METIS_SHRS     my.shrs
#define     METIS_STATS    "uieps"
#define     METIS_SORTS    "uiepsod"
/*---(btress)---------------*/
#define     B_MAJOR        'M'
#define     B_MINOR        'm'
#define     B_SOURCE       's'
#define     B_TASK         't'
#define     B_UNIQUE       'u'
/*---(warnings)-------------*/
#define     WARN_NODATA    32000
#define     WARN_SPACER    32001
#define     WARN_FILTER    32002
/*---(done)-----------------*/



typedef     struct   cMAJOR   tMAJOR;
typedef     struct   cMINOR   tMINOR;
typedef     struct   cTASK    tTASK;
typedef     struct   cSOURCE  tSOURCE;

struct      cMAJOR {
   /*---(master data)-------*/
   uchar       name        [LEN_LABEL];     /* major category                 */
   /*---(children)----------*/
   tMINOR     *head;
   tMINOR     *tail;
   ushort      count;
   /*---(btree)-------------*/
   tSORT      *ysort;
   /*---(done)--------------*/
};

struct      cMINOR {
   /*---(parent)------------*/
   tMAJOR     *major;
   /*---(master data)-------*/
   uchar       name        [LEN_TITLE];     /* minor category                 */
   /*---(in major)----------*/
   tMINOR     *prev;
   tMINOR     *next;
   /*---(children)----------*/
   tTASK      *head;
   tTASK      *tail;
   ushort      count;
   /*---(btree)-------------*/
   tSORT      *ysort;
   /*---(done)--------------*/
};

struct      cSOURCE {
   /*---(master data)-------*/
   uchar       path        [LEN_PATH];      /* data source                    */
   ushort      ref;                         /* pointer for re-linking task    */
   /*---(children)----------*/
   tTASK      *head;
   tTASK      *tail;
   ushort      count;
   /*---(btree)-------------*/
   tSORT      *ysort;
   /*---(done)--------------*/
};

struct      cTASK  {
   /*---(parent)------------*/
   tMINOR     *minor;
   ushort      seq;                         /* original order (to unsort)     */
   ushort      ref;                         /* pointer for re-linking source  */
   /*---(master data)-------*/
   uchar       urg;                         /* urgency code                   */
   uchar       imp;                         /* importance code                */
   uchar       est;                         /* estimated work                 */
   uchar       prg;                         /* progress flag                  */
   uchar       shr;                         /* sharing flag                   */
   uchar       txt         [LEN_HUND];      /* text of task                   */
   uchar       epoch       [LEN_TERSE];     /* date/unique id of task         */
   uchar       days;                        /* days until closed              */
   /*---(in minor)----------*/
   tTASK      *m_prev;
   tTASK      *m_next;
   /*---(source)------------*/
   tSOURCE    *source;
   tTASK      *s_prev;
   tTASK      *s_next;
   short       line;                        /* source line in file             */
   /*---(filtering)---------*/
   uchar       show;                        /* filtering mark                  */
   uchar       note;                        /* none (-), regex (r)             */
   uchar       key         [LEN_HUND];
   uchar       srch        [LEN_FULL];
   /*---(btree)-------------*/
   tSORT      *ysort;
   tSORT      *unique;
   /*---(done)--------------*/
};




/*---(task filtering)--------------------*/
extern char  g_recd [LEN_RECD];

#define     FILE_MASTER    "/home/member/g_hlosdo/metis.tasks"

#define     DATA_NONE      '-'
#define     DATA_SOURCES   's'
#define     DATA_MASTER    'm'
#define     DATA_CUSTOM    'c'
#define     DATA_PIPE      'p'
#define     DATA_ALL       "smcp"

#define     F_DB           "/var/lib/metis/metis.db"
#define     F_WORLD        "/var/lib/metis/world.txt"

typedef struct cAUDIT  tAUDIT;
struct cAUDIT {
   char        name        [LEN_LABEL];
   char        vernum      [LEN_SHORT];
   ushort      major;
   ushort      minor;
   ushort      source;
   ushort      task;
};
extern      tAUDIT      g_audit;

typedef     struct cMY     tMY;
struct cMY {
   /*---(yJOBS)----------------*/
   char        run_as;                      /* khronos, eos, heracles, ...    */
   char        run_mode;                    /* verify, install, audit, ...    */
   char        run_file    [LEN_PATH];      /* file to act on                 */
   int         run_uid;                     /* uid of person who launched     */
   long        runtime;
   char        quick;                       /* generate metis source line     */
   /*---(files)--------------------------*/
   char        n_db        [LEN_RECD];      /* name of database file          */
   FILE       *f_db;                        /* shared database of tasks       */
   /*---(counts)-------------------------*/
   ushort      nmajor;
   ushort      nminor;
   ushort      nsource;
   ushort      ntask;
   /*---(program wide)-------------------*/
   char        daemon;                      /* daemon vs foreground mode      */
   char        quit;                        /* stop the program               */
   char        trouble;                     /* flag bad keys                  */
   char        keys        [LEN_LABEL];     /* batch/menu keys to execute     */
   char        search      [LEN_RECD];      /* global search string           */
   int         fixed;                       /* fixed font                     */
   int         pretty;                      /* pretty font                    */
   char        lines;                       /* show some helpful debug stats  */
   char        png;                         /* png save control flag          */
   /*---(data source)--------------------*/
   char        source;                      /* data sourcing location         */
   char        file        [LEN_RECD];      /* file for reading tasks         */
   /*---(validity)-----------------------*/
   char        urgs        [LEN_LABEL];     /* all valid urgent codes         */
   char        imps        [LEN_LABEL];     /* all valid importance codes     */
   char        ests        [LEN_LABEL];     /* all valid estimate codes       */
   char        prgs        [LEN_LABEL];     /* all valid status flags         */
   char        shrs        [LEN_LABEL];     /* all valid status flags         */
   /*---(filtering)----------------------*/
   int         nact;                        /* number of active tasks         */
   uchar       curg;                        /* current urgency filter         */
   uchar       cimp;                        /* current importance filter      */
   uchar       cest;                        /* current estimating filter      */
   uchar       cprg;                        /* current progress filter        */
   uchar       cshr;                        /* current progress filter        */
   uchar       ctxt        [LEN_HUND];      /* current text filter            */
   uchar       sort;                        /* sorting request                */
   /*---(window)-------------------------*/
   char        format;                   /* display style                     */
   char        breaks;                   /* style of breaks                   */
   char        x_skip;                   /* off limits col for display        */
   char        y_skip;                   /* off limits row for display        */
   char        sighup;                   /* force a refresh/redraw            */
   char        sigusr2;                  /* cause a font jumble               */
   /*---(univ/tabs)----------------------*/
   ushort      ntabs;                    /* number of tabs                    */
   ushort      ctab;                     /* number of tabs                    */
   /*---(xpos/cols)----------------------*/
   ushort      wcols;                    /* number of cols in window          */
   ushort      tcols;                    /* number of cols on texture         */
   ushort      ncols;                    /* number of cols of data            */
   ushort      bcol;                     /* beginning of screen               */
   ushort      ccol;                     /* current col                       */
   ushort      ecol;                     /* ending of screen                  */
   /*---(ypos/rows)----------------------*/
   ushort      wrows;                    /* number of rows in window          */
   ushort      trows;                    /* number of rows on texture         */
   ushort      nrows;                    /* number of rows of data            */
   ushort      brow;                     /* beginning of screen               */
   ushort      crow;                     /* current row                       */
   ushort      erow;                     /* ending of screen                  */
   /*---(movement)-----------------------*/
   char        action;                   /* moving (0 = no, 1 = yes)          */
   char        update;                   /* xevent (0 = no, 1 = yes)          */
   double      incr;                     /* incemental offset each cycle      */
   double      move;                     /* position offset due to moving     */
   double      play;
   double      mspeed;
   double      change;
   /*---(win/tex)------------------------*/
   char        win_title   [LEN_DESC];   /* window title                      */
   int         s_wide;                   /* screen width                      */
   int         s_tall;                   /* screen height                     */
   int         c_wide;                   /* column width                      */
   int         c_offset;                 /* column spacing offset             */
   int         c_over;                   /* extra columns shown in window     */
   int         r_tall;                   /* row height                        */
   int         r_offset;                 /* row spacing offset                */
   int         r_over;                   /* extra rows shown in window        */
   int         w_left;                   /* window left                       */
   int         w_wide;                   /* window width                      */
   int         w_topp;                   /* window top                        */
   int         w_tall;                   /* window height                     */
   int         w_ftall;                  /* window height + status bar        */
   int         t_wide;                   /* texture width                     */
   int         t_tall;                   /* texture height                    */
   int         m_offset;                 /* menu space allowed (horz)         */
   /*---(opengl objects)-----------------*/
   uint        g_tex;                    /* task texture                      */
   uint        g_fbo;                    /* task fbo                          */
   uint        g_dep;                    /* task depth                        */
   /*---(done)---------------------------*/
};
extern tMY   my;


/*---(tabs)---------------------------*/
#define     NTABS       my.ntabs
#define     CTAB        my.ctab
/*---(columns)------------------------*/
#define     WCOLS       my.wcols
#define     TCOLS       my.tcols
#define     NCOLS       my.ncols
#define     BCOL        my.bcol
#define     CCOL        my.ccol
#define     ECOL        my.ecol
/*---(rows)---------------------------*/
#define     WROWS       my.wrows
#define     TROWS       my.trows
#define     NROWS       my.nrows
#define     BROW        my.brow
#define     CROW        my.crow
#define     EROW        my.erow


/*---(debugging)-------------------------*/


extern uchar g_mode;
extern uchar g_major;
extern uchar g_minor;

extern float  g_alpha;

extern int   max_disp;

#define   FORMAT_TICKER       't'
#define   FORMAT_BASELINE     'b'
#define   FORMAT_RSHORT       'r'
#define   FORMAT_RLONG        'R'
#define   FORMAT_LSHORT       'l'
#define   FORMAT_LLONG        'L'
#define   FORMAT_STREAMER     'S'
#define   FORMAT_WIDE         'w'
#define   FORMAT_PROJECT      'p'
#define   FORMAT_EXTRA        'x'

#define   FORMAT_COLUMNS      "rlRLS"
#define   FORMAT_RIGHTS       "rR"
#define   FORMAT_LONGS        "RL"
#define   FORMAT_TICKERS      "tb"
#define   FORMAT_LARGES       "wpx"

#define   FORMAT_ALL          "tbrRlLSwpx"
#define   FORMAT_VERTS        "rRlLSwpx"
#define   FORMAT_HORZS        "tb"


#define   MAX_COLS    15
#define   MAX_ROWS    100
#define   STOP        0.0000

extern short g_map [MAX_COLS][MAX_ROWS];



extern float     step;

/*---(prototypes)------------------------*/
int        main              (int a_argc, char *a_argv []);






/*---(task texture)-------------------*/
char       OPENGL_resize            (uint, uint);

char*       OPENGL__unit            (char *a_question, int a_num);

void       prog_catch        (int);
char       prog_signals      (void);




/*===[[ metis_data.c ]]=======================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(program)--------------*/
char        metis_data_purge_all    (void);
char        metis_data_init         (void);
char        metis_data_vikeys       (void);
/*---(parse)----------------*/
char        metis_data_catinfo      (char a_cat, char a_sub, char *a_clabel, char *a_slabel, char *a_desc);
char        metis_data_stats        (tTASK *a_task, char *a_stats);
char        metis_data_header       (char *a_recd, tMAJOR **r_major, tMINOR **r_minor);
char        metis_data_parsing      (tMINOR *a_minor, tSOURCE *a_source, int a_line, char *a_recd);
/*---(source)---------------*/
char        metis_data_file         (tMINOR *a_minor, tSOURCE *a_source, char a_type);
char        metis_data_directory    (tMAJOR *a_major, char *a_home);
char        metis_data_project      (void);
/*---(other)----------------*/
char        metis_data_read         (char *a_filename);
char        DATA__master            (void);
char        DATA__custom            (void);
/*---(driver)---------------*/
char        metis_data_refresh      (void);


char*       DATA__unit              (char *a_question, int a_num);



char        metis_format_init       (void);
char        metis_format_vikeys     (void);
char        format_column           (void);
char        format_ticker           (void);
char        format_streamer         (void);
char        format_projects         (void);
char        format_wideview         (void);
char        format_extra            (void);
char        metis_format_refresh    (void);


/*---(preinit)--------------*/
char        PROG_urgents            (int a_argc, char *a_argv []);
/*---(startup)--------------*/
char        PROG_reset_yjobs        (void);
char        PROG__init              (void);
char        PROG__args              (int a_argc, char *a_argv []);
char        PROG__begin             (void);
char        PROG_startup            (int a_argc, char *a_argv []);
/*---(execution)------------*/
char        PROG_dawn               (void);
char        PROG_dusk               (void);
char        PROG_dispatch           (void);
/*---(shutdown)-------------*/
char        PROG__end               (void);
char        PROG_shutdown           (void);
/*---(unittest)-------------*/
char        PROG__unit_quiet        (void);
char        PROG__unit_loud         (void);
char        PROG__unit_end          (void);
/*---(done)-----------------*/

char        metis_filter_init       (void);
char        metis_filter_vikeys     (void);
char        metis_filter_clear      (void);
char        metis_filter_set        (void);
char        metis_filter_search     (char *a_search);
char        metis_filter_key        (tTASK *a_task);
char        metis_filter_sort       (void);
char*       FILTER__unit            (char *a_question, int a_num);


char*       FORMAT__unit            (char *a_question, int a_num);



/*===[[ METIS_YMAP.C ]]=======================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(label)----------------*/
char        api_ymap_locator        (char a_strict, char *a_label, ushort *u, ushort *x, ushort *y, ushort *z);
char        api_ymap_addressor      (char a_strict, char *a_label, ushort u, ushort x, ushort y, ushort z);
/*---(load)-----------------*/
char        api_ymap_sizer          (char a_axis, ushort *n, ushort *a, ushort *b, ushort *c, ushort *e, ushort *m, ushort *x);
char        api_ymap_entry          (char a_axis, ushort a_pos, short *r_ref, uchar *r_wide, uchar *r_used);
/*---(update)---------------*/
char        api_ymap_placer         (char a_axis, ushort b, ushort c, ushort e);
char        api_ymap_done           (void);
/*---(done)-----------------*/



/*===[[ METIS_YVIKEYS.C ]]====================================================*/
char        api_yvikeys_init        (void);
char        api_yvikeys_mapper      (char a_req);
char        api_yvikeys_sort        (char *a_how);
char        api_yvikeys_filter      (char *a_which, char *a_string);
char        api_yvikeys_window      (char *a_format);
char        api_yvikeys_refresh     (void);
char        api_yvikeys_png         (void);


char        metis_reporter          (void);






/*===[[ metis_shared.c ]]=====================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(memory)---------------*/
char        metis_shared_new        (char a_abbr, void **r_new, char a_force, char *a_wiper (void *));
char        metis_shared_free       (char a_abbr, void **a_old);;



/*===[[ metis_major.c ]]======================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(support)--------------*/
char        metis_major_wipe        (tMAJOR *a_dst);
/*---(memory)---------------*/
char        metis_major_new         (char *a_name, char a_force, tMAJOR **r_new);
char        metis_major_free        (tMAJOR **r_old);;
/*---(hooking)--------------*/
char        metis_major_hook        (tMAJOR *a_major, tMINOR *a_minor);
char        metis_major_unhook      (tMINOR *a_minor);
/*---(search)---------------*/
int         metis_major_count       (void);
char        metis_major_by_name     (uchar *a_name, tMAJOR **r_major);
char        metis_major_by_index    (int n, tMAJOR **r_major);
char        metis_major_by_cursor   (char a_dir, tMAJOR **r_major);
char*       metis_major_entry       (int n);
/*---(program)--------------*/
char        metis_major_init        (void);
char        metis_major_cleanse     (void);
char        metis_major_purge_tasks (tMAJOR *a_major);
/*---(done)-----------------*/



/*===[[ metis_minor.c ]]======================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(support)--------------*/
char        metis_minor_wipe        (tMINOR *a_dst);
/*---(memory)---------------*/
char        metis_minor_new         (tMAJOR *a_major, char *a_name, char a_force, tMINOR **r_new);
char        metis_minor_free        (tMINOR **r_old);;
/*---(search)---------------*/
int         metis_minor_count       (void);
char        metis_minor_by_name     (uchar *a_name, tMINOR **r_minor);
char        metis_minor_by_index    (int n, tMINOR **r_minor);
char        metis_minor_by_cursor   (char a_dir, tMINOR **r_minor);
char*       metis_minor_entry       (int n);
/*---(program)--------------*/
char        metis_minor_init        (void);
char        metis_minor_cleanse     (void);
char        metis_minor_purge_tasks (tMINOR *a_minor);
/*---(done)-----------------*/



/*===[[ metis_source.c ]]=====================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(support)--------------*/
char        metis_source_wipe       (tSOURCE *a_dst);
/*---(memory)---------------*/
char        metis_source_new        (char *a_path, char a_force, tSOURCE **r_new);
char        metis_source_free       (tSOURCE **r_old);;
/*---(hooking)--------------*/
char        metis_source_hook       (tSOURCE *a_source, tTASK *a_task);
char        metis_source_unhook     (tTASK *a_task);
/*---(search)---------------*/
int         metis_source_count      (void);
char        metis_source_by_name    (uchar *a_name, tSOURCE **r_source);
char        metis_source_by_index   (int n, tSOURCE **r_source);
char        metis_source_by_cursor  (char a_dir, tSOURCE **r_source);
char*       metis_source_entry      (int n);
/*---(program)--------------*/
char        metis_source_init       (void);
char        metis_source_cleanse    (void);
/*---(done)-----------------*/



/*===[[ metis_task.c ]]=======================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(support)--------------*/
char        metis_task_wipe         (tTASK *a_dst);
/*---(memory)---------------*/
char        metis_task_new          (tMINOR *a_minor, char a_force, tTASK **r_new);
char        metis_task_free         (tTASK **r_old);;
/*---(search)---------------*/
int         metis_task_count        (void);
char        metis_task_by_index     (int n, tTASK **r_task);
char        metis_task_by_cursor    (char a_dir, tTASK **r_task);
int         metis_epoch_count       (void);
char        metis_epoch_by_index    (int n, tTASK **r_task);
char        metis_epoch_by_cursor   (char a_dir, tTASK **r_task);
char        metis_epoch_by_name     (uchar *a_name, tMINOR **r_minor);
int         metis_task_by_regex     (char *a_regex, tTASK **r_task);
char*       metis_task_entry        (int n);
char        metis_task_by_aindex    (int n, tTASK**r_task);
/*---(program)--------------*/
char        metis_task_init         (void);
char        metis_task_purge_all    (void);
/*---(done)-----------------*/



/*===[[ metis_opengl.c ]]=====================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(support)--------------*/
char        metis_opengl_color      (char *a_valid, char a_color, float a_alpha);
char        metis_opengl_font_load  (void);
char        metis_opengl_font_close (void);
/*---(program)--------------*/
char        metis_opengl_init       (void);
char        metis_opengl_wrap       (void);
/*---(show)-----------------*/
char        metis_opengl__panel     (float a_wtop, float a_wlef, float a_wbot, float a_wrig, float a_ttop, float a_tlef, float a_tbot, float a_trig);
char        metis_opengl__tickers   (float *a_xcur, float *a_ycur);
char        metis_opengl__columns   (float *a_xcur, float *a_ycur);
char        metis_opengl__larges    (float *a_xcur, float *a_ycur);
char        metis_opengl__current   (float a_xcur, float a_ycur);
char        metis_opengl_show       (void);
/*---(place)----------------*/
char        metis_opengl_blank      (short a_warn);
/*---(draw)-----------------*/
char        metis_opengl__base      (char  a_urg);
char        metis_opengl__urg       (uchar a_urg, int z);
char        metis_opengl__imp       (uchar a_imp, int z);
char        metis_opengl__est       (uchar a_est, int z);
char        metis_opengl__prg       (uchar a_prg, int z);
char        metis_opengl__age       (uchar a_prg, uchar *a_epoch, uchar a_days, short a_line, int z);
char        metis_opengl__bullets   (void);
char        metis_opengl__text      (int a_index, char *a_major, char *a_minor, char *a_text);
char        metis_opengl__cats      (char a_urg, char a_imp, char a_est);
char        metis_opengl__borders   (void);
char        metis_opengl__card      (int a_index);
char        metis_opengl__prep      (void);
char        metis_opengl_draw       (void);
/*---(mask)-----------------*/
char        metis_opengl_mask       (void);
/*---(done)-----------------*/


char        metis_place_clear       (void);
char        metis_place_assign      (void);
char        metis_place_show        (void);
short       metis_place_get         (short a_col, short a_row);
char*       metis_place__unit       (char *a_question, int a_num);



char        metis_refresh           (void);
char        metis_refresh_full      (void);



/*===[[ metis_db.c ]]=========================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(support)--------------*/
char        metis_db_cli            (char *a_name, char a_loud);
char        metis_db_init           (void);
char        metis_db_verify         (uchar *a_name);
/*---(file)-----------------*/
char        metis_db__read_head     (char *a_name, ushort *a_var);
char        metis_db__write_head    (char *a_name, ushort a_var);
char        metis_db__open          (char a_mode, short *a_nmajor, short *a_nminor, short *a_nsource, short *a_ntask);
char        metis_db__close         (void);
/*---(write)----------------*/
char        metis_db__write_sources (void);
char        metis_db__write_task    (tMINOR *x_minor);
char        metis_db__write_minor   (tMAJOR *x_major);
char        metis_db_write          (void);
/*---(read)-----------------*/
char        metis_db__read_sources  (int n);
char        metis_db__read_task     (tMINOR *a_minor, ushort n);
char        metis_db__read_minor    (tMAJOR *a_major, ushort n);
char        metis_db_read           (void);
/*---(unittest)-------------*/
char*       metis_db__unit          (char *a_question);
/*---(done)-----------------*/




#endif
/*============================----end-of-source---============================*/
