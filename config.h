/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* patches */
static const unsigned int gappx     = 6;        /* gap pixel between windows */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { 
										"JetBrainsMono-regular:size=10", 
										"JoyPixels:pixelsize=12:antialias=true:autohint=true",
										"FontAwesome:size=10:pixelsize=10",
									  };
static const char dmenufont[]       = "JetBrainsMono:size=10";
static const char col1[] = "#2E5266";
static const char col2[] = "#6E8898";
static const char col3[] = "#9FB1BC";
static const char col4[] = "#D3D0CB";
static const char col5[] = "#E2C044";
static const char darkgray[]       = "#2e3440";
static const char darkgray2[]	    = "#4e5157";
static const char lightgray[]      = "#d8dee9";
static const char light[]	    = "#eeeeee";
static const char red[]        	    = "#bf616a";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { lightgray, darkgray, darkgray }, // lightgray, darkgray, darkgray
	[SchemeSel]  = { light, darkgray2,  col3  }, // light darkgray2, red
};

/* tagging */
static const char *tags[] = { "Main", "Code", "Browse", "Work", "Meet", "Music" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "firefox",   NULL,       NULL,       0,      0,           -1 },
	{ "Spotify",   NULL,	   NULL,       1 << 5, 	    0,		 -1 },
	{ "discord",   NULL,	   NULL,       1 << 4,	    0, 		 -1 },
	{ "Alacritty", NULL, 	   "Alacritty - Main",       1 << 1, 	    0, 		 -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "",         tile },    /* first entry is default */
	{ "< >",      NULL },    /* no layout function means floating behavior */
	{ "[ ]",      monocle },
};

/* key definitions */
/*metal = Mod4Masks*/
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/usr/local/bin/st", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", darkgray, "-nf", light, "-sb", light, "-sf", darkgray2, NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *alaPrim[] = { "alacritty", "--title", "Alacritty - Main" };
static const char *firefox[] = { "firefox", NULL };
static const char *firefoxPrivate[] = { "firefox", "--private-window" };
static const char *discord[] = { "discord", NULL };
static const char *spotify[] = { "spotify", NULL };
static const char *upvol[]   = { "/usr/bin/pactl", "set-sink-volume", "1", "+5%",     NULL };
static const char *maxvol[]   = { "/usr/bin/pactl", "set-sink-volume", "1", "100%",     NULL };
static const char *downvol[] = { "/usr/bin/pactl", "set-sink-volume", "1", "-5%",     NULL };
static const char *mutevol[] = { "/usr/bin/pactl", "set-sink-mute",   "1", "toggle",  NULL };

static const char *play_pause[] = { "playerctl", "play-pause" };
static const char *previous[] = { "playerctl", "previous" };
static const char *next[] = { "playerctl", "next" };

static const char *decbright[] = { "xbacklight", "-dec", "5" };
static const char *incbright[] = { "xbacklight", "-inc", "5" };

static const char *F11[] = { "xdotool", "key", "F11" };

static Key keys[] = {
	/* modifier                     key        function        argument */
	/* Lyd */
	{ 0,                       XF86XK_AudioLowerVolume, spawn, {.v = downvol } },
	{ 0,                       XF86XK_AudioMute, spawn, {.v = mutevol } },
	{ 0,                       XF86XK_AudioRaiseVolume, spawn, {.v = upvol   } },
	{ MODKEY,                       XF86XK_AudioRaiseVolume, spawn, {.v = maxvol   } },

	{ 0, 238, spawn, {.v = F11} },
	{ 0, XF86XK_MonBrightnessDown, spawn, {.v = decbright} },

	{ 0,                       XF86XK_AudioPrev, spawn, {.v = previous   } },
	{ 0,                       XF86XK_AudioPlay, spawn, {.v = play_pause   } },
	{ 0,                       XF86XK_AudioNext, spawn, {.v = next   } },

	/* Genveje */
	{ MODKEY,                       XK_space,  spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_t,	   spawn,          {.v = alaPrim } },
	{ MODKEY,                       XK_b,      spawn,          {.v = firefox } },
	{ MODKEY|ShiftMask,				XK_b,	   spawn,		   {.v = firefoxPrivate}},
	{ MODKEY, 						XK_dead_diaeresis, spawn,  {.v = discord } },
	{ MODKEY,						XK_apostrophe, spawn,	   {.v = spotify } },


	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,	               	XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,             		XK_0,      view,           {.ui = ~0 } },
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
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
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
