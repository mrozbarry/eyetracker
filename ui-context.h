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

#include <SDL/SDL.h>
#include "ui-widget.h"

#ifndef UICONTEXT_
#define UICONTEXT_

#define UI_MAXWIDGETSPERLIST	100

typedef struct ui_widgetlist {
	int					length;							// Number of widgets matched
	ui_widget			*widget[UI_MAXWIDGETSPERLIST];	// Pointers to widgets (used static array to avoid deallocations)
} ui_widgetlist;

typedef struct ui_ctx {
	SDL_Window		*window;

	int				widgets;
	ui_widget		**widget;

	ui_widget		*selected;
	ui_widget		*hovered;

	Uint32			set_tag;
	ui_widget		*tag;
} ui_ctx;

ui_ctx		*ui_newcontext( char *name, int x, int y, int w, int h, Uint32 flags );
void		ui_deletecontext( ui_ctx *context );
void		ui_setcontextvisible( ui_ctx *context, int visible );
void		ui_getcontextvisible( ui_ctx *context, int *visible );
void		ui_getcontextsize( ui_ctx *context, int *w, int *h );

ui_widget	*ui_newcontextwidget( ui_ctx *context, ui_widget *widget );

int			ui_iscontextfocused( ui_ctx *context, SDL_Window *focus );
void		ui_applycontextevent( ui_ctx *context, SDL_Event *e );
void		ui_updatecontext( ui_ctx *context );

int			ui_findcontextwidgets( ui_ctx *context, const char *name, ui_widgetlist *list );

#endif /* UICONTEXT_ */
