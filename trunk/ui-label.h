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

#ifndef UILABEL_
#define UILABEL_

#define UI_LABEL_TYPE	0x02
#define UI_CAST_LABEL( widget )	(widget->wtype == UI_LABEL_TYPE ? (ui_label *)widget->cwidget : NULL)

typedef struct ui_label {
	char		*text;
	TTF_Font	*font;
	int			autosize;
	Uint8		r, g, b;
} ui_label;

ui_widget	*ui_newlabel( char *name, TTF_Font *font, char *text, int autosize, Uint8 r, Uint8 g, Uint8 b );
void		__deletelabel( ui_widget *widget );
void		__redrawlabel( ui_widget *widget );

void		ui_setlabeltext( ui_widget *widget, char *text );
void		ui_getlabeltext( ui_widget *widget, char **text );

void		ui_setlabelcolour( ui_widget *widget, Uint8 r, Uint8 g, Uint8 b );
void		ui_getlabelcolour( ui_widget *widget, Uint8 *r, Uint8 *g, Uint8 *b );

void		ui_setlabelfont( ui_widget *widget, TTF_Font *font );
void		ui_getlabelfont( ui_widget *widget, TTF_Font **font );

#endif /* UILABEL_ */
