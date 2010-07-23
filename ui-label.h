
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
void		ui_setlabeltextf( ui_widget *widget, char *format, ... );
void		ui_getlabeltext( ui_widget *widget, char **text );

void		ui_setlabelcolour( ui_widget *widget, Uint8 r, Uint8 g, Uint8 b );
void		ui_getlabelcolour( ui_widget *widget, Uint8 *r, Uint8 *g, Uint8 *b );

void		ui_setlabelfont( ui_widget *widget, TTF_Font *font );
void		ui_getlabelfont( ui_widget *widget, TTF_Font **font );

#endif /* UILABEL_ */
