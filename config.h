/* alt-tab configuration */
static const unsigned int tabModKey 		= 0x40;	/* if this key is hold the alt-tab functionality stays acitve. This key must be the same as key that is used to active functin altTabStart `*/
static const unsigned int tabCycleKey 		= 0x17;	/* if this key is hit the alt-tab program moves one position forward in clients stack. This key must be the same as key that is used to active functin altTabStart */
static const unsigned int tabPosY 			= 0;	/* tab position on Y axis, 0 = bottom, 1 = center, 2 = top */
static const unsigned int tabPosX 			= 2;	/* tab position on X axis, 0 = left, 1 = center, 2 = right */
static const unsigned int maxWTab 			= 600;	/* tab menu width */
static const unsigned int maxHTab 			= 200;	/* tab menu height */



/* appearance */
static unsigned int borderpx = 3; /* border pixel of windows */
static unsigned int snap = 32;	  /* snap pixel */
static unsigned int gappih = 20;  /* horiz inner gap between windows */
static unsigned int gappiv = 10;  /* vert inner gap between windows */
static unsigned int gappoh = 8;	  /* horiz outer gap between windows and screen edge */
static unsigned int gappov = 20;  /* vert outer gap between windows and screen edge */
static int swallowfloating = 0;	  /* 1 means swallow floating windows by default */
static int smartgaps = 1;		  /* 1 means no outer gap when there is only one window */
static int showbar = 0;			  /* 0 means no bar */

static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
 

static int topbar = 1;			  /* 0 means bottom bar */
static char *fonts[] = {"monospace:size=9", "NotoColorEmoji:pixelsize=12:antialias=true:autohint=true"};
static char normbgcolor[] = "#222222";
static char normbordercolor[] = "#444444";
static char normfgcolor[] = "#bbbbbb";
static char selfgcolor[] = "#eeeeee";
static char selbordercolor[] = "#770000";
static char selbgcolor[] = "#005577";
static char *colors[][3] = {
	/*               fg           bg           border   */
	[SchemeNorm] = {normfgcolor, normbgcolor, normbordercolor},
	[SchemeSel] = {selfgcolor, selbgcolor, selbordercolor},
};

static const char *const autostart[] = {
	"brave-nightly", NULL,
	"nm-applet", NULL,
	"blueman-applet", NULL,
	"jamesdsp","-t", NULL,
    "sxhkd", NULL,
    "thunar","--daemon ", NULL,
    "dunst", NULL,
    "/usr/lib/polkit-gnome/polkit-gnome-authentication-agent-1", NULL,
    "xbindkeys", NULL,
    "dbus-update-activation-environment","--systemd","DBUS_SESSION_BUS_ADDRESS","DISPLAY XAUTHORITY", NULL,
    "picom" , NULL,
    NULL /* terminate */
};

typedef struct
{
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"tabbed", "-g", "1000x700", "-cr", "2", "-n", "spterm", "st", "-w", "", NULL};
const char *spcmd2[] = {"st","-g","120x30","-n","spaudio","-e","pavucontrol", NULL};
const char *spcmd3[] = {"st","-g","150x40","-n","sptorrent","-e","transmission-remote-gtk", NULL};
const char *spcmd4[] = {"st","-g","150x40","-n","spfile","-e","ranger","/home/anant", NULL};
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm", spcmd1},
	{"spaudio",spcmd2},
	{"sptorrent",spcmd3},
	{"spfile",spcmd4},
};

/* tagging */
static const char *tags[] = {"1", "2", "3", "4", "5", "6", "7"};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class    instance      title       	 tags mask    isfloating   isterminal  noswallow  monitor */
	{NULL, "spterm", NULL, SPTAG(0), 1, 0, 1, -1},
	{NULL, "spaudio", NULL, SPTAG(1), 1, 1, 0, -1},
	{NULL, "sptorrent", NULL, SPTAG(2), 1, 1, 0, -1},
	{NULL, "spfile", NULL, SPTAG(3), 1, 1, 0, -1},
};

/* layout(s) */
static float mfact = 0.55;			 /* factor of master area size [0.05..0.95] */
static int nmaster = 1;				 /* number of clients in master area */
static int resizehints = 0;			 /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
#define FORCE_VSPLIT 1				 /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{"[]=", tile},	 /* Default: Master on left, slaves on right */
	{"TTT", bstack}, /* Master on top, slaves on bottom */

	{"[@]", spiral},   /* Fibonacci spiral */
	{"[\\]", dwindle}, /* Decreasing in size right and leftward */

	{"[D]", deck},	  /* Master on left, slaves in monocle-like mode on right */
	{"[M]", monocle}, /* All windows on top of eachother */

	{"|M|", centeredmaster},		 /* Master in middle, slaves on sides */
	{">M>", centeredfloatingmaster}, /* Same but master floats */

	{"><>", NULL}, /* no layout function means floating behavior */
	{NULL, NULL},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                          \
	{MODKEY, KEY, view, {.ui = 1 << TAG}},                         \
		{MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}}, \
		{MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},          \
		{MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},
#define STACKKEYS(MOD, ACTION)                  \
	{MOD, XK_j, ACTION##stack, {.i = INC(+1)}}, \
		{MOD, XK_k, ACTION##stack, {.i = INC(-1)}}, // { MOD,  XK_v,   ACTION##stack,  {.i = 0 } },

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
	{"color0", STRING, &normbordercolor},
	{"color3", STRING, &selbordercolor},
	{"color0", STRING, &normbgcolor},
	{"color4", STRING, &normfgcolor},
	{"color0", STRING, &selfgcolor},
	{"color4", STRING, &selbgcolor},
	{"borderpx", INTEGER, &borderpx},
	{"snap", INTEGER, &snap},
	{"showbar", INTEGER, &showbar},
	{"topbar", INTEGER, &topbar},
	{"nmaster", INTEGER, &nmaster},
	{"resizehints", INTEGER, &resizehints},
	{"mfact", FLOAT, &mfact},
	{"gappih", INTEGER, &gappih},
	{"gappiv", INTEGER, &gappiv},
	{"gappoh", INTEGER, &gappoh},
	{"gappov", INTEGER, &gappov},
	{"swallowfloating", INTEGER, &swallowfloating},
	{"smartgaps", INTEGER, &smartgaps},
};

#include <X11/XF86keysym.h>
#include "shiftview.c"

static const Key keys[] = {
	/* modifier                     key        function        argument */
	STACKKEYS(MODKEY, focus)
	STACKKEYS(MODKEY | ShiftMask, push)
	TAGKEYS(XK_1, 0)
	TAGKEYS(XK_2, 1)
	TAGKEYS(XK_3, 2)
	TAGKEYS(XK_4, 3)
	TAGKEYS(XK_5, 4)
	TAGKEYS(XK_6, 5)
	TAGKEYS(XK_7, 6)


	{ Mod1Mask,             		XK_Tab,    altTabStart,	   {1} },
	{ Mod1Mask|ShiftMask,           XK_Tab,    altTabStart,	   {-1} },




	{MODKEY, XK_0, view, {.ui = ~0}},
	{MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},

	{MODKEY, XK_q, killclient, {0}},

	{MODKEY, XK_t, setlayout, {.v = &layouts[0]}},			   /* tile */
	{MODKEY | ShiftMask, XK_t, setlayout, {.v = &layouts[1]}}, /* bstack */
	{MODKEY | ShiftMask, XK_s, setlayout, {.v = &layouts[2]}}, /* spiral */
	{MODKEY, XK_d, setlayout, {.v = &layouts[3]}},			   /* dwindle */
	{MODKEY | ShiftMask, XK_d, setlayout, {.v = &layouts[4]}}, /* deck */
	{MODKEY, XK_m, setlayout, {.v = &layouts[5]}},			   /* monocle */
	{MODKEY, XK_c, setlayout, {.v = &layouts[6]}},			   /* centeredmaster */
	{MODKEY | ShiftMask, XK_c, setlayout, {.v = &layouts[7]}}, /* centeredfloatingmaster */
	{MODKEY | ShiftMask, XK_f, setlayout, {.v = &layouts[8]}},
	{MODKEY | ShiftMask, XK_space, togglefloating, {0}},
	{MODKEY, XK_s, togglesticky, {0}},
	{MODKEY, XK_f, togglefullscr, {0}},

	{MODKEY | ShiftMask, XK_a, togglegaps, {0}},
	{MODKEY | ShiftMask, XK_apostrophe, togglesmartgaps, {0}},

	{MODKEY, XK_i, incnmaster, {.i = +1}},
	{MODKEY, XK_o, incnmaster, {.i = -1}},

	{MODKEY, XK_p, spawn, {.v = (const char *[]){"dmenu_run", NULL}}},
        //{MODKEY|ShiftMask, XK_Return, spawn, {.v = (const char *[]){"st", NULL}}},
	{MODKEY|ShiftMask, XK_Return, spawn, {.v = (const char *[]){"tabbed","-cr", "2", "st", "-w", "", NULL}}},

	{MODKEY, XK_Tab, shiftview, {.i = 1}},
	{MODKEY | ShiftMask, XK_Tab, shiftview, {.i = -1}},

	{MODKEY, XK_h, setmfact, {.f = -0.05}},
	{MODKEY, XK_l, setmfact, {.f = +0.05}},

	{MODKEY, XK_space, togglescratch, {.ui = 0}},
	{MODKEY, XK_a, togglescratch, {.ui = 1}},
	{ControlMask, XK_t, togglescratch, {.ui = 2}},
	{Mod1Mask, XK_f, togglescratch, {.ui = 3}},

	{MODKEY, XK_b, togglebar, {0}},
	{MODKEY, XK_Return, zoom, {0}},


	{MODKEY, XK_g, defaultgaps, {0}},
	{MODKEY,				XK_equal,	incrgaps,       {.i = +2 } }, 
	{MODKEY,				XK_minus,	incrgaps,       {.i = -2 } }, 

	{MODKEY|ShiftMask,    XK_equal,      incrogaps,      {.i = +2 } }, 
	{MODKEY|ShiftMask,    XK_minus,      incrogaps,      {.i = -2 } }, 

	// {ShiftMask,  XK_equal,      incrigaps,      {.i = +2 } },
	// {ShiftMask,  XK_minus,      incrigaps,      {.i = -2 } },

	/* { MODKEY,                       XK_y,      incrihgaps,     {.i = +1 } }, */
	/* { MODKEY,                       XK_o,      incrihgaps,     {.i = -1 } }, */

	/* { MODKEY|ControlMask,           XK_y,      incrivgaps,     {.i = +1 } }, */
	/* { MODKEY|ControlMask,           XK_o,      incrivgaps,     {.i = -1 } }, */

	/* { MODKEY|Mod4Mask,              XK_y,      incrohgaps,     {.i = +1 } }, */
	/* { MODKEY|Mod4Mask,              XK_o,      incrohgaps,     {.i = -1 } }, */

	/* { MODKEY|ShiftMask,             XK_y,      incrovgaps,     {.i = +1 } }, */
	/* { MODKEY|ShiftMask,             XK_o,      incrovgaps,     {.i = -1 } }, */

	{ MODKEY,                       XK_Down,   moveresize,     {.v = "0x 25y 0w 0h" } },
	{ MODKEY,                       XK_Up,     moveresize,     {.v = "0x -25y 0w 0h" } },
	{ MODKEY,                       XK_Right,  moveresize,     {.v = "25x 0y 0w 0h" } },
	{ MODKEY,                       XK_Left,   moveresize,     {.v = "-25x 0y 0w 0h" } },
	{ MODKEY|ShiftMask,             XK_Down,   moveresize,     {.v = "0x 0y 0w 25h" } },
	{ MODKEY|ShiftMask,             XK_Up,     moveresize,     {.v = "0x 0y 0w -25h" } },
	{ MODKEY|ShiftMask,             XK_Right,  moveresize,     {.v = "0x 0y 25w 0h" } },
	{ MODKEY|ShiftMask,             XK_Left,   moveresize,     {.v = "0x 0y -25w 0h" } },
	{ MODKEY|ControlMask,           XK_Up,     moveresizeedge, {.v = "t"} },
	{ MODKEY|ControlMask,           XK_Down,   moveresizeedge, {.v = "b"} },
	{ MODKEY|ControlMask,           XK_Left,   moveresizeedge, {.v = "l"} },
	{ MODKEY|ControlMask,           XK_Right,  moveresizeedge, {.v = "r"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Up,     moveresizeedge, {.v = "T"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Down,   moveresizeedge, {.v = "B"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Left,   moveresizeedge, {.v = "L"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Right,  moveresizeedge, {.v = "R"} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
/* click                event mask      button          function        argument */

	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
 

#ifndef __OpenBSD__
	{ClkWinTitle, 0, Button2, zoom, {0}},
	{ClkStatusText, 0, Button1, sigdwmblocks, {.i = 1}},
	{ClkStatusText, 0, Button2, sigdwmblocks, {.i = 2}},
	{ClkStatusText, 0, Button3, sigdwmblocks, {.i = 3}},
	{ClkStatusText, 0, Button4, sigdwmblocks, {.i = 4}},
	{ClkStatusText, 0, Button5, sigdwmblocks, {.i = 5}},
	{ClkStatusText, ShiftMask, Button1, sigdwmblocks, {.i = 6}},
#endif

	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },

};
