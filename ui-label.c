
#include "ui-label.h"

ui_widget *ui_newlabel( char *name, TTF_Font *font, char *text, int autosize, Uint8 r, Uint8 g, Uint8 b ) {
	ui_label *label = malloc( sizeof( ui_label ) );
	label->font = NULL;
	label->text = NULL;

	label->autosize = autosize;

	ui_widget *widget = ui_newwidget( name, UI_LABEL_TYPE, label );

	widget->onDestroy = &__deletelabel;
	widget->onRedraw = &__redrawlabel;

	ui_setlabelfont( widget, font );
	ui_setlabelcolour( widget, r, g, b );
	ui_setlabeltext( widget, text );

	return widget;
}

void __deletelabel( ui_widget *widget ) {
	ui_label *label = UI_CAST_LABEL(  widget );
	if( !label ) return;
	if( label->text ) free( label->text );
	if( label->font ) TTF_CloseFont( label->font );
}

void __redrawlabel( ui_widget *widget ) {
	ui_label *label = UI_CAST_LABEL(  widget );
	if( !label ) return;
	if( !label->text ) return;
	if( !label->font ) return;

	SDL_Surface *textdrawing = TTF_RenderText_Blended( label->font, label->text, (SDL_Color){ label->r, label->g, label->b } );

	if( label->autosize ) {
		int w = 0, h = 0;
		ui_getwidgetsize( widget, &w, &h );
		if( ( textdrawing->w != w ) || ( textdrawing->h != h ) ) ui_setwidgetsize( widget, textdrawing->w, textdrawing->h );
	}

	SDL_Surface *surface = SDL_CreateRGBSurface( 0, widget->rect.w, widget->rect.h, 24, 0, 0, 0, 0 );
	SDL_BlitSurface( textdrawing, NULL, surface, NULL );

	widget->texture = SDL_CreateTextureFromSurface( 0, surface );

	SDL_FreeSurface( textdrawing );
	SDL_FreeSurface( surface );
}

void ui_setlabeltext( ui_widget *widget, char *text ) {
	ui_label *label = UI_CAST_LABEL(  widget );
	if( !label ) return;

	int length = strlen( text ) + 1;
	if( !label->text ) { label->text = (char *)malloc( sizeof( char ) * length ); } else { label->text = (char *)realloc( label->text, sizeof( char ) * length ); }

	memcpy( label->text, text, length - 1 );
	label->text[ length-1 ] = '\0';

	widget->redraw = 1;
}

void ui_setlabeltextf( ui_widget *widget, char *format, ... ) {
	va_list args;
	va_start( args, format );
	char str[500];
	sprintf( str, format, args );
	ui_setlabeltext( widget, str );
}

void ui_getlabeltext( ui_widget *widget, char **text ) {
	ui_label *label = UI_CAST_LABEL(  widget );
	if( !label ) return;

	*text = label->text;
}

void ui_setlabelcolour( ui_widget *widget, Uint8 r, Uint8 g, Uint8 b ) {
	ui_label *label = UI_CAST_LABEL(  widget );
	if( !label ) return;

	label->r = r;
	label->g = g;
	label->b = b;

	widget->redraw = 1;
}

void ui_getlabelcolour( ui_widget *widget, Uint8 *r, Uint8 *g, Uint8 *b ) {
	ui_label *label = UI_CAST_LABEL(  widget );
	if( !label ) return;

	*r = label->r;
	*g = label->g;
	*b = label->b;
}

void ui_setlabelfont( ui_widget *widget, TTF_Font *font ) {
	ui_label *label = UI_CAST_LABEL(  widget );
	if( !label ) return;

	label->font = font;

	widget->redraw = 1;
}

void ui_getlabelfont( ui_widget *widget, TTF_Font **font ) {
	ui_label *label = UI_CAST_LABEL(  widget );
	if( !label ) return;

	*font = label->font;
}
