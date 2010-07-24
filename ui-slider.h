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

#ifndef UISLIDER_
#define UISLIDER_

#define UI_SLIDER_TYPE	0x04
#define UI_CAST_SLIDER( widget )	(widget->wtype == UI_SLIDER_TYPE ? (ui_slider *)widget->cwidget : NULL)

typedef void (*ui_slideronchangeCB)( ui_widget *widget, float value );

typedef struct ui_slider {
	float					min, max;
	float					vsize;
	float					value;
	ui_slideronchangeCB		onchange;
} ui_slider;

ui_widget	*ui_newslider( char *name, float min, float max, float value, int width, void *onchange );
void		__deleteslider( ui_widget *widget );
void		__redrawslider( ui_widget *widget );
void		__onclickslider( ui_widget *widget, int x, int y, int button, int state );
void		__onmousemoveslider( ui_widget *widget, int x, int y, int button, int state );
void		__onkeyslider( ui_widget *widget, SDL_keysym sym, int state );

void		ui_setslidervalue( ui_widget *widget, float value );
void		ui_getslidervalue( ui_widget *widget, float *value );

void		ui_setsliderrange( ui_widget *widget, float min, float max );
void		ui_getsliderrange( ui_widget *widget, float *min, float *max );

#endif /* UISLIDER_ */
