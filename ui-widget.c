
#include "ui-widget.h"

ui_widget *ui_newwidget( char *name, int type, void *widgetClass ) {
	ui_widget *widget = (ui_widget *)malloc( sizeof( ui_widget ) );

	widget->wtype = type;
	widget->cwidget = widgetClass;
	sprintf( widget->name, "%s", name );

	widget->parent = NULL;
	widget->children = 0;
	widget->child = (ui_widget **)malloc( sizeof( ui_widget * ) );

	widget->events = 0;
	widget->event = (ui_event *)malloc( sizeof( ui_event ) );

	widget->texture = NULL;

	widget->onDestroy = NULL;
	widget->onRedraw = NULL;

	widget->redraw = 0;

	widget->userData = NULL;

	return widget;
}

void ui_deletewidget( ui_widget *widget ) {
	if( !widget ) return;

	if( widget->onDestroy ) widget->onDestroy( widget );

	ui_deletewidgetchildren( widget );

	if( widget->cwidget ) free( widget->cwidget );
	free( widget->event );

	free( widget );
}

ui_widget *ui_newwidgetchild( ui_widget *widget, ui_widget *child ) {
	if( !widget ) return NULL;

	int id = widget->children;
	widget->child = (ui_widget **)realloc( widget->child, sizeof( ui_widget *) * ++widget->children );
	widget->child[id] = child;
	child->parent = widget;
	return child;
}

void ui_deletewidgetchild( ui_widget *widget, int child ) {
	if( !widget ) return;
	if( widget->children == 0 ) return;
	int i = 0;
	ui_deletewidget( widget->child[ child] );
	for( i = child; i < (widget->children-1); i++ ) {
		widget->child[i] = widget->child[i+1];
	}
	widget->child = (ui_widget **)realloc( widget->child, sizeof( ui_widget *) * --widget->children );
}

void ui_deletewidgetchildren( ui_widget *widget ) {
	if( !widget ) return;
	if( widget->children == 0 ) return;
	int i = 0;
	for( i = 0; i < widget->children; i++ ) {
		ui_deletewidget( widget->child[i] );
	}
}

void ui_addwidgetevent( ui_widget *widget, int type, void *callback ) {
	if( !widget ) return;
	if(  callback == NULL ) return;
	int id = widget->events;
	widget->event = (ui_event *)realloc( widget->event, sizeof( ui_event ) * ++widget->events );
	widget->event[id].event_type = type;
	widget->event[id].callback = callback;
}

void ui_updatewidget( ui_widget *widget ) {
	if( !widget ) return;
	ui_dowidgetredraw( widget );
}

void ui_drawwidget( ui_widget *widget ) {
	if( !widget ) return;
	if( !widget->texture ) return;
	SDL_RenderCopy( widget->texture, NULL, (const SDL_Rect *)&(widget->rect) );
}

void ui_setwidgetposition( ui_widget *widget, int x, int y ) {
	if( !widget ) return;
	widget->rect.x = x;
	widget->rect.y = y;
}

void ui_getwidgetposition( ui_widget *widget, int *x, int *y ) {
	if( !widget ) return;
	*x = widget->rect.x;
	*y = widget->rect.y;
}

void ui_setwidgetsize( ui_widget *widget, int w, int h ) {
	if( !widget ) return;
	widget->rect.w = w;
	widget->rect.h = h;

	widget->redraw = 1;
}

void ui_getwidgetsize( ui_widget *widget, int *w, int *h ) {
	if( !widget ) return;
	*w = widget->rect.w;
	*h = widget->rect.h;
}

void ui_dowidgetdestroy( ui_widget *widget ) {
	if( !widget ) return;
	if( widget->onDestroy ) widget->onDestroy( widget );
}

void ui_dowidgetredraw( ui_widget *widget ) {
	if( !widget ) return;
	if( widget->redraw ) {
		widget->redraw = 0;
		if( widget->onRedraw ) widget->onRedraw( widget );
	}

}

void ui_dowidgetclick( ui_widget *widget, int x, int y, int button, int state ) {
	ui_cbWidgetMouse click = NULL;
	int i = 0;
	for( i = 0; i < widget->events; i++ ) {
		if( widget->event[i].event_type == ui_event_click ) {
			click = widget->event[i].callback;
			click( widget, x, y, button, state );
		}
	}
}

void ui_dowidgetmousein( ui_widget *widget, int x, int y, int button, int state ) {
	ui_cbWidgetMouse mousein = NULL;
	int i = 0;
	for( i = 0; i < widget->events; i++ ) {
		if( widget->event[i].event_type == ui_event_mousein ) {
			mousein = widget->event[i].callback;
			mousein( widget, x, y, button, state );
		}
	}
}

void ui_dowidgetmouseout( ui_widget *widget, int x, int y, int button, int state ) {
	ui_cbWidgetMouse mouseout = NULL;
	int i = 0;
	for( i = 0; i < widget->events; i++ ) {
		if( widget->event[i].event_type == ui_event_mouseout ) {
			mouseout = widget->event[i].callback;
			mouseout( widget, x, y, button, state );
		}
	}
}

void ui_dowidgetmousemove( ui_widget *widget, int x, int y, int button, int state ) {
	ui_cbWidgetMouse mousemove = NULL;
	int i = 0;
	for( i = 0; i < widget->events; i++ ) {
		if( widget->event[i].event_type == ui_event_mousemove ) {
			mousemove = widget->event[i].callback;
			mousemove( widget, x, y, button, state );
		}
	}
}
