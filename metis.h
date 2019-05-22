/*============================----beg-of-source---============================*/

/*===[[ BEG_HEADER ]]=========================================================*/

/*===[[ ONE_LINERS ]]=========================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-12345678901-12345678901-*/

#define     P_FOCUS     "PT (productivity/time mgmt)"
#define     P_NICHE     "td (todo list)"
#define     P_PURPOSE   "task consolitation, visualization, and navigation system"

#define     P_NAMESAKE  "metis"
#define     P_HERITAGE  "titan goddess of wisdom, wise counsel, cunning, prudence, and deep thought"
#define     P_IMAGERY   "singularly graceful, stately, and regal goddess"

#define     P_SYSTEM    "gnu/linux   (powerful, ubiquitous, technical, and hackable)"
#define     P_LANGUAGE  "ansi-c      (wicked, limitless, universal, and everlasting)"
#define     P_CODESIZE  "large       (appoximately 10,000 slocl)"

#define     P_AUTHOR    "heatherlyrobert"
#define     P_CREATED   "2008-06"
#define     P_DEPENDS   "none"

#define     P_VERMAJOR  "1.--, improve for more and more use and value"
#define     P_VERMINOR  "1.1-, stabilize and add full yURG debugging"
#define     P_VERNUM    "1.1j"
#define     P_VERTXT    "using yvikeys to draw/process, floating command/search bar now appears"

#define     P_PRIORITY  "direct, simple, brief, vigorous, and lucid (h.w. fowler)"
#define     P_PRINCIPAL "[grow a set] and build your wings on the way down (r. bradbury)"
#define     P_REMINDER  "there are many better options, but i *own* every byte of this one"



/*===[[ METIS BACKLOG ]]======================================================*
 *  metis  -----  tbd
 *
 */


/*
 * few people regularly (except under duress) keep task lists
 *
 * people naturally keep focused lists of tasks near usage...
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
 *
 * first, distributing the lists to their sources keeps the scary volume
 * of tasks hidden.  switch focuses, switch lists.  easy and big picture.
 *
 * second, i think covey had it right with the urgent/important matrix from
 * eisenhower or truman.  it allows the mind to cut the list down.
 *
 * third, put a category of effort on tasks so that you can pick off smaller
 * ones when you have little time, or big ones when your are able to focus.
 *
 * fourth, skip on tracking dates and actual effort.  this is a obviously cool
 * thing that is a total time trap.
 *
 * fifth, do not "imagine" possible tasks for some future scenario.  it adds
 * a lot of frightening, speculative bs to the lists.  focus on real tasks.
 *
 * sixth, do not calendar/schedule.  this requires a lot of time control which
 * most people do not have.  review the list, do what you can, move on.
 *
 * seventh, do frequent sweeps to delete completed tasks.  do not save history
 * that you will never use.  its hard to delete accomplishments, but do it.
 *
 *
 */

/*
 * major cat      project name
 *
 * minor cat      source file or other grouping
 *
 * urgency        (t)oday     (s)oonest   (d)ays      (w)eeks
 *                (m)onths    (q)uarters  (y)ears     (�) tbd
 *
 * importance     (a)bsolute  (n)eed      (w)ant
 *                (l)like     (m)ight     (i)dea      (�) tbd
 *
 * description    actual task
 *
 * estimate       (!) 5m      (s) 15m     (m) 30m     (1) 1h
 *                (2) 2hs     (4) 4hrs    (8) hours   (�) tbd     (+) bigger
 *
 * status         (<) focus   (o) wip     (>) closing
 *                (#) done    (x) cancel  (�) backlog 
 *
 *
 * source file example (today, need, 1hr, and focus) categories are auto
 *  metis  tn1<  create a github project and get changes uploaded
 *
 *
 * task file example (same) categories must be explicit
 *  metis  metis_prog.c
 *     tn1<  create a github project and get changes uploaded
 *
 *
 *
 *
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
#include    <yVIKEYS.h>      /* CUSTOM : heatherly vi_keys standard           */
#include    <ySTR.h>         /* CUSTOM : heatherly string handling            */
#include    <yX11.h>         /* heatherly xlib/glx setup            */
#include    <yFONT.h>        /* heatherly text display for opengl   */
#include    <yGLTEX.h>       /* heatherly opengl texture support              */

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

/*---(title)-----------------------------*/
extern char      win_title[100];

/*---(sizes)-----------------------------*/
int       win_w;                            /* window width                   */
int       win_h;                            /* window height                  */
int       tex_w;                            /* texture width                  */
int       tex_h;                            /* texture height                 */

/*---(opengl objects)--------------------*/
extern uint      g_tex;                     /* task texture                   */
extern uint      g_fbo;                     /* task fbo                       */
extern uint      g_dep;                     /* task depth                     */

extern int       g_mtall;                   /* menu texture height            */
extern int       g_mwide;                   /* menu texture width             */
extern uint      g_mtex;                    /* menu texture                   */
extern uint      g_mfbo;                    /* menu fbo                       */
extern uint      g_mdep;                    /* menu depth                     */

extern      char          unit_answer [LEN_FULL];




/*---(task data structure)------------*/
typedef     struct cCARD   tCARD;
#define     MAX_CARDS  1000            /* how many we can read                */
struct      cCARD
{
   /*---(master data)--------------------*/
   char        one         [LEN_LABEL];/* major category                      */
   char        two         [LEN_LABEL];/* minor category                      */
   char        urg;                    /* urgency code                        */
   char        imp;                    /* importance code                     */
   char        est;                    /* estimated work                      */
   char        flg;                    /* status flag                         */
   char        txt         [LEN_HUND]; /* text of task                        */
   /*---(source data)--------------------*/
   short       seq;                    /* original order (to unsort)          */
   short       line;                   /* source line in file                 */
   /*---(filtering)----------------------*/
   char        act;                    /* active (y/n)                        */
   char        key         [LEN_HUND];
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

#define     DATA_SOURCES   's'
#define     DATA_MASTER    'm'
#define     DATA_CUSTOM    'c'
#define     DATA_PIPE      'p'


typedef     struct cMY     tMY;
struct cMY {
   /*---(program wide)-------------------*/
   char        quit;                        /* stop the program               */
   char        trouble;                     /* flag bad keys                  */
   char        keys        [LEN_LABEL];     /* batch/menu keys to execute     */
   char        search      [LEN_RECD];      /* global search string           */
   int         fixed;                       /* fixed font                     */
   int         pretty;                      /* pretty font                    */
   /*---(data source)--------------------*/
   char        source;                      /* data sourcing location         */
   char        file        [LEN_RECD];      /* file for reading tasks         */
   /*---(validity)-----------------------*/
   char        urgs        [LEN_LABEL];     /* all valid urgent codes         */
   char        imps        [LEN_LABEL];     /* all valid importance codes     */
   char        ests        [LEN_LABEL];     /* all valid estimate codes       */
   char        flgs        [LEN_LABEL];     /* all valid status flags         */
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
   /*---(window)-------------------------*/
   char        format;                   /* display style                       */
   char        sighup;                   /* force a refresh/redraw              */
   char        sigusr2;                  /* cause a font jumble                 */
   /*---(rows)---------------------------*/
   int         nrows;                    /* number of rows of data              */
   int         wrows;                    /* number of rows in window            */
   int         trows;                    /* number of rows on texture           */
   int         crow;                     /* current topmost visible row         */
   int         prow;                     /* previous topmost visible row        */
   /*---(columns)------------------------*/
   int         wcols;                    /* number of cols in window            */
   int         tcols;                    /* number of cols on texture           */
   int         ncols;                    /* number of cols                      */
   int         ccol;                     /* current leftmost visible column     */
   int         pcol;                     /* previous leftmost visible column    */
   /*---(movement)-----------------------*/
   char        action;                   /* moving (0 = no, 1 = yes)            */
   char        update;                   /* xevent (0 = no, 1 = yes)            */
   double      incr;                     /* incemental offset each cycle        */
   double      move;                     /* position offset due to moving       */
   double      play;
   double      mspeed;
   double      change;
};
extern tMY   my;


/*---(debugging)-------------------------*/


extern uchar g_mode;
extern uchar g_major;
extern uchar g_minor;

extern float  g_alpha;

extern int   max_disp;

#define   FORMAT_TICKER       't'
#define   FORMAT_BASELINE     'b'
#define   FORMAT_COLUMN       'c'
#define   FORMAT_LONG         'l'
#define   FORMAT_WIDE         'w'
#define   FORMAT_PROJECT      'p'
#define   FORMAT_EXTRA        'x'

#define   STREAMER    if (my.format == 's')
#define   TICKER      if (my.format == 't')
#define   BASELINE    if (my.format == 'b')
#define   COLUMN      if (my.format == 'c')
#define   LONG        if (my.format == 'l')
#define   WIDEVIEW    if (my.format == 'w')
#define   PROJECT     if (my.format == 'p')
#define   EXTRA       if (my.format == 'x')

#define   MAX_COLS     20
#define   STOP        0.0000

extern float     step;

/*---(prototypes)------------------------*/
int        main              (int argc, char *argv[]);

char       PROG_event        (void);

char       font_load         (void);
char       font_change       (void);
char       font_delete       (void);

char             /* [G-----] output a formatted report of tasks --------------*/
task_list          (void);

char             /* [G-----] output a formatted structure of tasks -----------*/
task_structure     (void);


/*---(program-level)------------------*/
char       OPENGL_init              (void);
char       OPENGL_wrap              (void);
/*---(task texture)-------------------*/
char       OPENGL_show              (void);
char       OPENGL_draw              (void);
char       OPENGL_resize            (uint, uint);
char       OPENGL_menu_start        (void);
char       OPENGL_menu_cont         (void);
char       OPENGL_mask              (void);

void       prog_catch        (int);
char       prog_signals      (void);

/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        DATA_init               (void);
char        DATA__header            (char *a_recd);
char        DATA__stats             (char *a_stats);
char        DATA__detail            (char *a_recd, int a_line);
char        DATA__read              (char *a_filename);
char        DATA__master            (void);
char        DATA__custom            (void);
char        DATA__sources           (void);
char        DATA__blankcard         (void);
char        DATA_refresh            (void);
char*       DATA__unit              (char *a_question, int a_num);

char        PROG_init               (void);
char        PROG_args               (int argc, char *argv[]);
char        PROG_begin              (void);
char        PROG_final              (void);
char        PROG_wrap               (void);
char        PROG_end                (void);
char        PROG__unit_quiet        (void);
char        PROG__unit_loud         (void);
char        PROG__unit_end          (void);

char        FILTER_clear            (void);
char        FILTER_refresh          (void);
char*       FILTER__unit            (char *a_question, int a_num);
char        SORT_stats              (void);
char        SORT_unsort             (void);
char        SORT_refresh            (void);


char*       FORMAT__unit            (char *a_question, int a_num);

char        OPENGL__clearall        (void);

/*============================----end-of-source---============================*/
