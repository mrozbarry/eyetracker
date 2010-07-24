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

#include "ui-slider.h"

ui_widget *ui_newslider( char *name, float min, float max, float value, int width, void *onchange ) {
	ui_slider *slider = (ui_slider *)malloc( sizeof( ui_slider ) );
	slider->min = min;
	slider->max = max;
	slider->value = value;
	slider->onchange = onchange;

	ui_widget *widget = ui_newwidget( name, UI_SLIDER_TYPE, slider );
	if( !widget ) {
		free( slider );
		return NULL;
	}

	widget->onDestroy = &__deleteslider;
	widget->onRedraw = &__redrawslider;

	ui_setwidgetsize( widget, width, 20 );
	ui_addwidgetevent( widget, ui_event_click, &__onclickslider );
	ui_addwidgetevent( widget, ui_event_mousemove, &__onmousemoveslider );

	widget->redraw = 1;

	return widget;
}

void __deleteslider( ui_widget *widget ) {
	// nothing for now
}

void __redrawslider( ui_widget *widget ) {
	ui_slider *slider = UI_CAST_SLIDER(  widget );
	if( !slider ) return;

	SDL_Surface *surface = SDL_CreateRGBSurface( 0, widget->rect.w, widget->rect.h, 24, 0, 0, 0, 0 );

	SDL_Rect bar;
	bar.x = 2; bar.y = (widget->rect.h/2)-2;
	bar.w = widget->rect.w-4; bar.h = 2;
	SDL_FillRect( surface, &bar, SDL_MapRGB( surface->format, 150, 150, 150 ) );
	bar.x = 2; bar.y = (widget->rect.h/2);
	bar.w = widget->rect.w-4; bar.h = 2;
	SDL_FillRect( surface, &bar, SDL_MapRGB( surface->format, 100, 100, 100 ) );
	bar.x = ( slider->value / slider->max ) * ( widget->rect.w - 4 ); bar.y = 0;
	bar.w = 4; bar.h = widget->rect.h;
	SDL_FillRect( surface, &bar, SDL_MapRGB( surface->format, 75, 75, 75 ) );
	SDL_DrawRect( surface, &bar, SDL_MapRGB( surface->format, 255, 255, 255 ) );

	if( widget->texture ) SDL_DestroyTexture( widget->texture );
	widget->texture = SDL_CreateTextureFromSurface( 0, surface );
	SDL_FreeSurface( surface );
}

void __onclickslider( ui_widget *widget, int x, int y, int button, int state ) {
	ui_slider *slider = UI_CAST_SLIDER(  widget );
	if( !slider ) return;

	if( button == SDL_BUTTON_LEFT ) {
		float value = ( (float)x / (float)widget->rect.w ) * slider->max;
		ui_setslidervalue( widget, value );
	}
}

void __onmousemoveslider( ui_widget *widget, int x, int y, int button, int state ) {
	ui_slider *slider = UI_CAST_SLIDER(  widget );
	if( !slider ) return;

	if( button & SDL_BUTTON_LMASK ) {
		float value = ( (float)x / (float)widget->rect.w ) * slider->max;
		ui_setslidervalue( widget, value );
	}
}

void __onkeyslider( ui_widget *widget, SDL_keysym sym, int state ) {
	// left/right to change the value?
}

void ui_setslidervalue( ui_widget *widget, float value ) {
	ui_slider *slider = UI_CAST_SLIDER(  widget );
	if( !slider ) return;

	slider->value = value;
	if( slider->value > slider->max ) slider->value = slider->max;
	if( slider->onchange ) slider->onchange( widget, value );
	widget->redraw = 1;
}

void ui_getslidervalue( ui_widget *widget, float *value ) {
	ui_slider *slider = UI_CAST_SLIDER(  widget );
	if( !slider ) return;

	*value = slider->value;
}

void ui_setsliderrange( ui_widget *widget, float min, float max ) {
	ui_slider *slider = UI_CAST_SLIDER(  widget );
	if( !slider ) return;

	slider->min = min;
	slider->max = max;
	slider->vsize = max - min;
	if( slider->value > slider->max ) slider->value = slider->max;
	if( slider->value < slider->min ) slider->value = slider->min;
	widget->redraw = 1;
}

void ui_getsliderrange( ui_widget *widget, float *min, float *max ) {
	ui_slider *slider = UI_CAST_SLIDER(  widget );
	if( !slider ) return;

	*min = slider->min;
	*max = slider->max;
}
