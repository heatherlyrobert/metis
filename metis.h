/*============================----beg-of-source---============================*/

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
#define     P_VERMINOR  "1.5-, move to yJOBS interface and butch-up"
#define     P_VERNUM    "1.5b"
#define     P_VERTXT    "major and minor now build using btrees and unit tested"

#define     P_PRIORITY  "direct, simple, brief, vigorous, and lucid (h.w. fowler)"
#define     P_PRINCIPAL "[grow a set] and build your wings on the way down (r. bradbury)"
#define     P_REMINDER  "there are many better options, but i *own* every byte of this one"


/*
 * 12345 § 12345 § 12345678901-12345678901-12345678901-12345678901-12345678901-12345678901- § ---beg---- § ---end---- §
 * metis § wv8·· § write interactive-use manual for metis                                   § 1645134601 § ·········· §
 *
 */

/*  ,x··0D··:put =strftime('%s')¦···0kdd··i * metis § ····· § tbd                                                                                  § ¥··$a § ·········· §¥··,y         */

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
#include   <unistd.h>
#include   <time.h>
#include   <signal.h>                  /* signal(), sigalarm()                */
#include   <dirent.h>



typedef   unsigned int  uint;
typedef     struct      dirent      tDIRENT;




extern int       g_mtall;                   /* menu texture height            */
extern int       g_mwide;                   /* menu texture width             */
extern uint      g_mtex;                    /* menu texture                   */
extern uint      g_mfbo;                    /* menu fbo                       */
extern uint      g_mdep;                    /* menu depth                     */

extern      char          unit_answer [LEN_FULL];



extern     char      g_print     [LEN_RECD];



#define    B_MAJOR   'M'
#define    B_MINOR   'm'
#define    B_SOURCE  's'
#define    B_TASK    't'

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
   /*---(parent)------------*/
   tMAJOR     *major;
   /*---(master data)-------*/
   uchar       name        [LEN_HUND];      /* data source                    */
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
   /*---(master data)-------*/
   char        urg;                         /* urgency code                        */
   char        imp;                         /* importance code                     */
   char        est;                         /* estimated work                      */
   char        prg;                         /* progress flag                       */
   char        shr;                         /* sharing flag                        */
   char        txt         [LEN_HUND];      /* text of task                        */
   int         beg;                         /* created on epoch                    */
   int         end;                         /* completed on epoch                  */
   /*---(in minor)----------*/
   tMINOR     *prev;
   tMINOR     *next;
   /*---(source)------------*/
   tSOURCE    *source;
   short       seq;                         /* original order (to unsort)          */
   short       line;                        /* source line in file                 */
   /*---(btree)-------------*/
   tSORT      *ysort;
   /*---(done)--------------*/
};




/*---(task data structure)------------*/
typedef     struct cCARD   tCARD;
#define     MAX_CARDS  1000            /* how many we can read                */
struct      cCARD
{
   /*---(master data)--------------------*/
   char        one         [LEN_LABEL];     /* major category                      */
   char        two         [LEN_LABEL];     /* minor category                      */
   char        urg;                         /* urgency code                        */
   char        imp;                         /* importance code                     */
   char        est;                         /* estimated work                      */
   char        prg;                         /* progress flag                       */
   char        shr;                         /* sharing flag                        */
   char        txt         [LEN_HUND];      /* text of task                        */
   int         beg;                         /* created on epoch                    */
   int         end;                         /* completed on epoch                  */
   /*> char        beg         [LEN_TERSE];     /+ created on epoch                    +/   <* 
    *> char        end         [LEN_TERSE];     /+ completed on epoch                  +/   <*/
   /*---(source data)--------------------*/
   short       seq;                         /* original order (to unsort)          */
   char        source      [LEN_HUND];      /* text of task                        */
   short       line;                        /* source line in file                 */
   /*---(filtering)----------------------*/
   char        act;                         /* active (y/n)                        */
   char        key         [LEN_RECD];
   /*---(visualization)------------------*/
   short       pos;
   short       col;
   short       row;
   /*---(for unit testing)---------------*/
   short       x;
   short       y;
   /*---(done)---------------------------*/
};
extern tCARD g_tasks [MAX_CARDS];
extern int   g_ntask;                   /* number of tasks                     */



/*---(task filtering)--------------------*/
extern char  g_recd [LEN_RECD];
extern char      one [20];

#define     FILE_MASTER    "/home/member/g_hlosdo/metis.tasks"

#define     DATA_NONE      '-'
#define     DATA_SOURCES   's'
#define     DATA_MASTER    'm'
#define     DATA_CUSTOM    'c'
#define     DATA_PIPE      'p'
#define     DATA_ALL       "smcp"


typedef     struct cMY     tMY;
struct cMY {
   /*---(yJOBS)----------------*/
   char        run_as;                      /* khronos, eos, heracles, ...    */
   char        run_mode;                    /* verify, install, audit, ...    */
   char        run_file    [LEN_PATH];      /* file to act on                 */
   int         run_uid;                     /* uid of person who launched     */
   long        runtime;
   /*---(counts)-------------------------*/
   short       nmajor;
   short       nminor;
   short       ntask;
   /*---(program wide)-------------------*/
   char        daemon;                      /* daemon vs foreground mode      */
   char        quit;                        /* stop the program               */
   char        trouble;                     /* flag bad keys                  */
   char        keys        [LEN_LABEL];     /* batch/menu keys to execute     */
   char        search      [LEN_RECD];      /* global search string           */
   int         fixed;                       /* fixed font                     */
   int         pretty;                      /* pretty font                    */
   char        lines;                       /* show some helpful debug stats  */
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
   char        curg;                        /* current urgency filter         */
   char        cimp;                        /* current importance filter      */
   char        cest;                        /* current estimating filter      */
   char        cflg;                        /* current status filter          */
   char        cone        [LEN_LABEL];     /* current cat one filter         */
   char        ctwo        [LEN_LABEL];     /* current cat two filter         */
   char        ctxt        [LEN_HUND];      /* current text filter            */
   char        sort;                        /* sorting request                */
   char        order;                       /* sorting order request          */
   /*---(window)-------------------------*/
   char        format;                   /* display style                     */
   char        sighup;                   /* force a refresh/redraw            */
   char        sigusr2;                  /* cause a font jumble               */
   /*---(univ/tabs)----------------------*/
   ushort      ntabs;                    /* number of tabs                    */
   ushort      ctab;                     /* number of tabs                    */
   /*---(xpos/cols)----------------------*/
   ushort      wcols;                    /* number of cols in window          */
   ushort      tcols;                    /* number of cols on texture         */
   ushort      ncols;                    /* number of cols                    */
   ushort      bcol;                     /* beginning of screen               */
   ushort      ccol;                     /* current col                       */
   ushort      ecol;                     /* ending of screen                  */
   /*---(ypos/rows)----------------------*/
   ushort      nrows;                    /* number of rows of data            */
   ushort      wrows;                    /* number of rows in window          */
   ushort      trows;                    /* number of rows on texture         */
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

#define   FORMAT_ALL          "tbrlRLSwpx"
#define   FORMAT_HORZ         "tbwpx"


#define   MAX_COLS    15
#define   MAX_ROWS    100
#define   STOP        0.0000

extern float     step;

/*---(prototypes)------------------------*/
int        main              (int a_argc, char *a_argv []);


char       font_load         (void);
char       font_delete       (void);

char             /* [G-----] output a formatted report of tasks --------------*/
task_list          (void);

char             /* [G-----] output a formatted structure of tasks -----------*/
task_structure     (void);

char       task_dump          (void);

/*---(program-level)------------------*/
char       OPENGL_init              (void);
char       OPENGL_wrap              (void);
/*---(task texture)-------------------*/
char       OPENGL_show              (void);
char       OPENGL_draw              (void);
char       OPENGL_resize            (uint, uint);
char       OPENGL_mask              (void);

char        OPENGL__clearall        (void);
char*       OPENGL__unit            (char *a_question, int a_num);

void       prog_catch        (int);
char       prog_signals      (void);

/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        DATA_catinfo            (char a_type, char a_abbr, char *a_label, char *a_desc);
char        DATA_init               (void);
char        DATA__header            (char *a_recd);
char        DATA__stats             (char *a_stats);
char        DATA__detail            (char *a_source, int a_line, char *a_recd);
char        DATA__read              (char *a_filename);
char        DATA__master            (void);
char        DATA__custom            (void);
char        DATA__sources           (char *a_suffix);
char        DATA__blankcard         (void);
char        DATA_refresh            (void);
int         DATA_cursor             (char a_type);
char*       DATA__unit              (char *a_question, int a_num);

char        FORMAT_init             (void);
int         format_check            (int a_col, int a_row);
char        format_column           (void);
char        format_ticker           (void);
char        format_streamer         (void);
char        format_projects         (void);
char        format_wideview         (void);
char        format_extra            (void);
char        FORMAT_refresh          (void);


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

char        FILTER_init             (void);
char        FILTER_clear            (void);
char        FILTER_refresh          (void);
char*       FILTER__unit            (char *a_question, int a_num);
char        SORT_stats              (char a_type);
char        SORT_names              (void);
char        SORT_unsort             (void);
char        SORT_refresh            (void);


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
char        metis_major_wrap        (void);
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
char        metis_minor_wrap        (void);
/*---(done)-----------------*/



/*---(hooking)--------------*/
/*> char        metis_major_hook        (tMAJOR *a_major, tMINOR *a_minor);           <*/
/*> char        metis_major_unhook      (tMINOR *a_minor);                            <*/


/*============================----end-of-source---============================*/
