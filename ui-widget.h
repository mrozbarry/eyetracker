/*
 * Basic widget
 *
 */

#include <SDL/SDL.h>

#ifndef UIWIDGET_
#define UIWIDGET_

typedef struct ui_event ui_event;
typedef enum ui_eventtype ui_eventtype;
typedef struct ui_widget ui_widget;

typedef void (*ui_cbWidget)( ui_widget *widget );
typedef void (*ui_cbWidgetMouse)( ui_widget *widget, int x, int y, int button, int state );

enum ui_eventtype {
	ui_event_click = 1,
	ui_event_mousein,
	ui_event_mouseout,
	ui_event_mousemove
};

struct ui_event {
	int		event_type;
	void	*callback;
};

struct ui_widget {
	void				*cwidget;	// Widget's class information
	int					wtype;		// Widget type (for cast-information)
	char				name[32];	// Name of widget (preferable to be unique)

	SDL_Rect			rect;		// Position and size of widget (position relative to parent widget)

	SDL_Texture			*texture;	// Postable texture (for posting in a window)
	int					redraw;		// Redraw onto texture

	ui_widget			*parent;	// Widget's parent
	int					children;	// Number of children
	ui_widget			**child;	// Children nodes

	int					events;		// Number of events
	ui_event			*event;		// Callback events

	ui_cbWidget			onDestroy;	// When the widget is destroyed, call this
	ui_cbWidget 		onRedraw;	// When the widget needs to be redrawn, call this

	void				*userData;	// User data to associate with this widget
};

ui_widget	*ui_newwidget( char *name, int type, void *widgetClass );
void		ui_deletewidget( ui_widget *widget );
ui_widget	*ui_newwidgetchild( ui_widget *widget, ui_widget *child );
void		ui_deletewidgetchild( ui_widget *widget, int child );
void		ui_deletewidgetchildren( ui_widget *widget );

void		ui_addwidgetevent( ui_widget *widget, int type, void *callback );

void		ui_updatewidget( ui_widget *widget );
void		ui_drawwidget( ui_widget *widget );

void		ui_setwidgetposition( ui_widget *widget, int x, int y );
void		ui_getwidgetposition( ui_widget *widget, int *x, int *y );
void		ui_setwidgetsize( ui_widget *widget, int w, int h );
void		ui_getwidgetsize( ui_widget *widget, int *w, int *h );

void		ui_dowidgetdestroy( ui_widget *widget );
void		ui_dowidgetredraw( ui_widget *widget );
void		ui_dowidgetclick( ui_widget *widget, int x, int y, int button, int state );
void		ui_dowidgetmousein( ui_widget *widget, int x, int y, int button, int state );
void		ui_dowidgetmouseout( ui_widget *widget, int x, int y, int button, int state );
void		ui_dowidgetmousemove( ui_widget *widget, int x, int y, int button, int state );

#endif /* UIWIDGET_ */

