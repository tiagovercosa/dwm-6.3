/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 5;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 5;       /* vert inner gap between windows */
static const unsigned int gappoh    = 5;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 5;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
/* static const char *fonts[]          = { "monospace:size=10" }; */
static const char *fonts[]          = { "monospace:size=10:antialias=true:autohint=true",
                                        "Hack:size=8:antialias=true:autohint=true",
                                        "JoyPixels:size=10:antialias=true:autohint=true" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class				instance    title    tags mask     isfloating  isterminal  noswallow   monitor */
	{ "st",       			NULL,       NULL,    0,             0,			1,          0,		   -1 },
	{ "Gimp",     			NULL,       NULL,    1 << 3,        0,          0,          1,		   -1 },
	{ "firefox",  			NULL,  "About Mozilla Firefox",  0,   1,		0,			1,         -1 },
	{ "firefox",  			NULL,  "Library",    0,			    1,          1,          1,         -1 },
	{ "Arandr",   			NULL,		NULL,    0,             1,          0,          1,         -1 },
	{ "TelegramDesktop",    NULL,		NULL,    1 << 7,		1,          0,          1,         -1 },
	{ "Deezer",             NULL,		NULL,    1 << 8,		1,          0,          1,         -1 },
	{ "Bitwarden",          NULL,     	NULL,    0,				1,          0,          1,         -1 },
	{ "st-256color",	    NULL,     	"qalc",  0,  	    	1,          1,          1,         -1 },
	{ "Qalculate-gtk",	    NULL,     	NULL,	 0,  	    	1,          0,          1,         -1 },
	{ "Arandr",             NULL,     	NULL,    0,         	1,          0,          1,         -1 },
	{ "Lxappearance",       NULL,     	NULL,    0,         	1,          0,          1,         -1 },
	{ "qt5ct",              NULL,     	NULL,    0,         	1,          0,          1,         -1 },
	{ "Pavucontrol",        NULL,     	NULL,    0,         	1,          0,          1,         -1 },
	{ "Pcmanfm",			NULL,  "Execute File",    0,        1,          0,          1,         -1 },
	{ "Nvidia-settings",    NULL,  "NVIDIA X Server Settings",  0,      1,      0,      1,         -1 },
	{ NULL,					NULL,  "Event Tester", 0,			0,          0,          1,         -1 }, /* xev */
};

#include <X11/XF86keysym.h>

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-b", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *tabtermcmd[]  = { "tabbed", "-r 2", "st", "-w", "''", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,			            XK_Return, spawn,          {.v = termcmd } },
	{ ControlMask, 					XK_Return, spawn,          {.v = tabtermcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY|ShiftMask,             XK_j,      rotatestack,    {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      rotatestack,    {.i = -1 } },
	{ MODKEY,                       XK_Left,   rotatetags,     {.i = -1 } },
	{ MODKEY,                       XK_Right,  rotatetags,     {.i = +1 } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_u,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.01} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.01} },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY|Mod1Mask,              XK_u,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_u,      incrgaps,       {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_i,      incrigaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_o,      incrogaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_6,      incrihgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_7,      incrivgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_8,      incrohgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_9,      incrovgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_0,      togglegaps,     {0} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,			            XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ControlMask,			XK_comma,  cyclelayout,    {.i = -1 } },
	{ MODKEY|ControlMask,           XK_period, cyclelayout,    {.i = +1 } },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_e,      quit,           {0} },
	{ MODKEY|ShiftMask,				XK_r,      quit,           {1} }, 

	/* Dmenu scripts launched with ALT + CTRL + KEY */
  	{ Mod1Mask|ControlMask,         XK_q,      spawn,          SHCMD("~/.local/bin/dmenu-logout.sh") },
  	{ Mod1Mask|ControlMask,         XK_m,      spawn,          SHCMD("~/.local/bin/dmenu-monitor.sh") },

  	/* Change the volume */
  	{ 0,              XF86XK_AudioLowerVolume, spawn,          SHCMD("pactl set-sink-volume 0 -5%; pkill -RTMIN+10 dwmblocks")},
  	{ 0,              XF86XK_AudioMute,        spawn,          SHCMD("pactl set-sink-mute 0 toggle; pkill -RTMIN+10 dwmblocks")},
  	{ 0,              XF86XK_AudioRaiseVolume, spawn,          SHCMD("pactl set-sink-volume 0 +5%; pkill -RTMIN+10 dwmblocks")},

  	/* Change the bright */
  	{ 0,               XF86XK_MonBrightnessUp,     spawn,	   SHCMD("xbacklight -inc 10; pkill -RTMIN+10 dwmblocks")},
  	{ 0,               XF86XK_MonBrightnessDown,   spawn,  	   SHCMD("xbacklight -dec 10; pkill -RTMIN+10 dwmblocks")},

  	/* Multimedia controls */
  	{ 0,               XF86XK_AudioNext,	   spawn,          SHCMD("playerctl next")},
  	{ 0,               XF86XK_AudioPrev,   	   spawn,          SHCMD("playerctl previous")},
  	{ 0,               XF86XK_AudioPlay,   	   spawn,          SHCMD("playerctl play-pause")},
  	{ 0,               XF86XK_AudioStop,   	   spawn,          SHCMD("playerctl stop")},

  	 /* lockscreen with i3lock */
  	{ MODKEY|ControlMask,			XK_l,      spawn,          SHCMD("i3lock-fancy")},
	
	/* Open App's*/
	{	MODKEY|ShiftMask,			XK_f,	   spawn,		   SHCMD("firefox")},
	{	MODKEY,						XK_c,	   spawn,		   SHCMD("st qalc")},

	/* Screenshot */
  	{MODKEY,           XK_Print,           spawn,          SHCMD("flameshot gui")},
  	{0,                XK_Print,           spawn,          SHCMD("flameshot full -c")},
  	{ShiftMask,        XK_Print,           spawn,          SHCMD("flameshot full -p /home/tiago/Pictures/screenshots")}
  	/* {ControlMask,      XK_Print,           spawn,          SHCMD("scrot -u /home/tiago/Pictures/screenshots/`date +%Y-%m-%d_%H:%M:%S`.png")} */
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

