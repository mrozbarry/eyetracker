/*
 *		Eyetracker - Copyright 2010 by Alex Barry ( alex DOT barry AT gmail DOT com )
 *		Eye Tracking Using OpenCV and Rendered using SDL
 *		LGPL License
 *
 *		This software is distributed "as-is" without any warranty of any kind.
 *		You are granted permission to:
 *			- Redistribute/Change this softare..
 *				- As long as you make a reference to this project, and clearly mark what has been changed
 *			- Use portions of this software...
 *				- As long as you give myself and other authors of this software credit
 *		Please read the included LICENSE file for more information.
 */

#include "ui-widget.h"
#include <SDL/SDL_ttf.h>

#ifndef UITOGGLE_
#define UITOGGLE_

#define UI_TOGGLE_TYPE	0x05
#define UI_CAST_TOGGLE( widget )	(widget->wtype == UI_TOGGLE_TYPE ? (ui_toggle *)widget->cwidget : NULL)

typedef struct ui_toggle {
	int			value;		// 1 = on, 0 = off
	TTF_Font	*font;
	char		*label;
	SDL_Color	colour;
} ui_toggle;

ui_widget	*ui_newtoggle( char *name, TTF_Font *font, char *label, int value, Uint8 r, Uint8 g, Uint8 b );
void		__deletetoggle( ui_widget *widget );
void		__redrawtoggle( ui_widget *widget );
void		__onclicktoggle( ui_widget *widget, int x, int y, int button, int state );

void		ui_settogglevalue( ui_widget *widget, int value );
void		ui_gettogglevalue( ui_widget *widget, int *value );

void		ui_settogglefont( ui_widget *widget, TTF_Font *font );
void		ui_gettogglefont( ui_widget *widget, TTF_Font **font );

void		ui_settogglelabel( ui_widget *widget, char *text );
void		ui_gettogglelabel( ui_widget *widget, char **text );

void		ui_settogglecolour( ui_widget *widget, Uint8 r, Uint8 g, Uint8 b );
void		ui_gettogglecolour( ui_widget *widget, Uint8 *r, Uint8 *g, Uint8 *b );

#endif /* UITOGGLE_ */
