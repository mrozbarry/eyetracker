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

#include "ui-toggle.h"

ui_widget *ui_newtoggle( char *name, TTF_Font *font, char *label, int value, Uint8 r, Uint8 g, Uint8 b ) {
	ui_toggle *toggle = (ui_toggle *)malloc( sizeof( ui_toggle ) );
	toggle->value = value;
	toggle->font = font;
	toggle->label = NULL;
	toggle->colour.r = r;
	toggle->colour.g = g;
	toggle->colour.b = b;

	ui_widget *widget = ui_newwidget( name, UI_TOGGLE_TYPE, toggle );
	if( !widget ) {
		free( toggle );
		return NULL;
	}

	widget->onDestroy = &__deletetoggle;
	widget->onRedraw = &__redrawtoggle;

	ui_addwidgetevent( widget, ui_event_click, &__onclicktoggle );

	ui_settogglelabel( widget, label );

	widget->redraw = 1;

	return widget;
}

void __deletetoggle( ui_widget *widget ) {
	ui_toggle *toggle = UI_CAST_TOGGLE( widget );
	if( !toggle ) return;

	if( toggle->font ) TTF_CloseFont( toggle->font );
	if( toggle->label ) free( toggle->label );
}

void __redrawtoggle( ui_widget *widget ) {
	ui_toggle *toggle = UI_CAST_TOGGLE( widget );
	if( !toggle ) return;

	if( !toggle->font || !toggle->label ) return;

	SDL_Surface *drawtext = TTF_RenderText_Solid( toggle->font, toggle->label, toggle->colour );
	SDL_Surface *surface = SDL_CreateRGBSurface( 0, drawtext->w + 20, drawtext->h, 24, 0, 0, 0, 0 );

	SDL_Rect text;
	text.x = 20; text.y = 0;
	text.w = drawtext->w; text.h = drawtext->h;
	SDL_BlitSurface( drawtext, NULL, surface, &text );

	SDL_Rect outer, inner;

	outer.x = 0; outer.y = 0;
	outer.w = 10; outer.h = drawtext->h;
	inner.x = 1; inner.y = 1;
	inner.w = 8; inner.h = drawtext->h - 2;

	SDL_FillRect( surface, &outer, SDL_MapRGB( surface->format, toggle->colour.r, toggle->colour.g, toggle->colour.b ) );
	int r, g, b;
	r = toggle->value == 0 ? 0 : toggle->colour.r;
	g = toggle->value == 0 ? 0 : toggle->colour.g;
	b = toggle->value == 0 ? 0 : toggle->colour.b;
	SDL_FillRect( surface, &inner, SDL_MapRGB( surface->format, r, g, b ) );

	if( widget->texture ) SDL_DestroyTexture( widget->texture );
	widget->texture = SDL_CreateTextureFromSurface( 0, surface );

	SDL_FreeSurface( surface );
	SDL_FreeSurface( drawtext );
}

void __onclicktoggle( ui_widget *widget, int x, int y, int button, int state ) {
	ui_toggle *toggle = UI_CAST_TOGGLE( widget );
	if( !toggle ) return;

	if( ( state & SDL_BUTTON_LMASK ) ) ui_settogglevalue( widget, !toggle->value );
}

void ui_settogglevalue( ui_widget *widget, int value ) {
	ui_toggle *toggle = UI_CAST_TOGGLE( widget );
	if( !toggle ) return;

	toggle->value = value == 0 ? 0 : 1;

	widget->redraw = 1;
}

void ui_gettogglevalue( ui_widget *widget, int *value ) {
	ui_toggle *toggle = UI_CAST_TOGGLE( widget );
	if( !toggle ) return;

	*value = toggle->value;
}

void ui_settogglefont( ui_widget *widget, TTF_Font *font ) {
	ui_toggle *toggle = UI_CAST_TOGGLE( widget );
	if( !toggle ) return;

	toggle->font = font;

	if( !toggle->font || !toggle->label ) return;

	int w = 0, h = 0;
	TTF_SizeText( toggle->font, toggle->label, &w, &h );

	ui_setwidgetsize( widget, w + 20, h );
}

void ui_gettogglefont( ui_widget *widget, TTF_Font **font ) {
	ui_toggle *toggle = UI_CAST_TOGGLE( widget );
	if( !toggle ) return;

	*font = toggle->font;
}

void ui_settogglelabel( ui_widget *widget, char *text ) {
	ui_toggle *toggle = UI_CAST_TOGGLE( widget );
	if( !toggle ) return;

	int length = strlen( text );
	if( toggle->label ) { free( toggle->label ); toggle->label = NULL; }
	if( length < 1 ) return;
	toggle->label = (char *)malloc( sizeof( char ) * (length+1) );
	memcpy( toggle->label, text, length );
	toggle->label[length] = '\0';

	if( !toggle->font || !toggle->label ) return;

	int w = 0, h = 0;
	TTF_SizeText( toggle->font, toggle->label, &w, &h );

	ui_setwidgetsize( widget, w + 20, h );
}

void ui_gettogglelabel( ui_widget *widget, char **text ) {
	ui_toggle *toggle = UI_CAST_TOGGLE( widget );
	if( !toggle ) return;

	*text = toggle->label;
}

void ui_settogglecolour( ui_widget *widget, Uint8 r, Uint8 g, Uint8 b ) {
	ui_toggle *toggle = UI_CAST_TOGGLE( widget );
	if( !toggle ) return;

	toggle->colour.r = r;
	toggle->colour.g = g;
	toggle->colour.b = b;
}

void ui_gettogglecolour( ui_widget *widget, Uint8 *r, Uint8 *g, Uint8 *b ) {
	ui_toggle *toggle = UI_CAST_TOGGLE( widget );
	if( !toggle ) return;

	*r = (Uint8)toggle->colour.r;
	*g = (Uint8)toggle->colour.g;
	*b = (Uint8)toggle->colour.b;
}
