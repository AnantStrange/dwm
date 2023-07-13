/* appearance */
static unsigned int borderpx = 3; /* border pixel of windows */
static unsigned int snap = 32;	  /* snap pixel */
static unsigned int gappih = 20;  /* horiz inner gap between windows */
static unsigned int gappiv = 10;  /* vert inner gap between windows */
static unsigned int gappoh = 8;	  /* horiz outer gap between windows and screen edge */
static unsigned int gappov = 20;  /* vert outer gap between windows and screen edge */
static int swallowfloating = 1;	  /* 1 means swallow floating windows by default */
static int smartgaps = 0;		  /* 1 means no outer gap when there is only one window */
static int showbar = 1;			  /* 0 means no bar */
static int topbar = 1;			  /* 0 means bottom bar */
static char *fonts[] = {"monospace:size=11", "NotoColorEmoji:pixelsize=13:antialias=true:autohint=true"};
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

typedef struct
{
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"tabbed", "-g", "1000x700", "-cr", "2", "-n", "spterm", "st", "-w", "", NULL};
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm", spcmd1}};

/* tagging */
static const char *tags[] = {"1", "2", "3", "4", "5", "6", "7"};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class    instance      title       	 tags mask    isfloating   isterminal  noswallow  monitor */
	{NULL, "spterm", NULL, SPTAG(0), 1, 1, 0, -1},
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
	{MODKEY|ShiftMask, XK_Return, spawn, {.v = (const char *[]){"st", NULL}}},

	{MODKEY, XK_Tab, shiftview, {.i = 1}},
	{MODKEY | ShiftMask, XK_Tab, shiftview, {.i = -1}},

	{MODKEY, XK_h, setmfact, {.f = -0.05}},
	{MODKEY, XK_l, setmfact, {.f = +0.05}},

	{MODKEY, XK_space, togglescratch, {.ui = 0}},

	{MODKEY, XK_b, togglebar, {0}},
	{MODKEY, XK_Return, zoom, {0}},


	{MODKEY, XK_a, defaultgaps, {0}},
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

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
/* click                event mask      button          function        argument */
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
