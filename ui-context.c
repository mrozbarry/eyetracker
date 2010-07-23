
#include "ui-context.h"

ui_ctx *ui_newcontext( char *name, int x, int y, int w, int h, Uint32 flags ) {
	ui_ctx *context = (ui_ctx *)malloc( sizeof( ui_ctx ) );
	context->window = SDL_CreateWindow( (const char *)name, x, y, w, h, flags );
	if( !context->window ) {
		free( context );
		return NULL;
	}

    if( SDL_CreateRenderer( context->window, -1, SDL_RENDERER_PRESENTFLIP2 ) != 0 ) {
    	printf( "Could not create renderer: %s\n", SDL_GetError() );
    	SDL_DestroyWindow( context->window );
    	free( context );
    	return NULL;
    }

	context->widgets = 0;
	context->widget = (ui_widget **)malloc( sizeof( ui_widget *) );

	context->selected = NULL;
	context->hovered = NULL;

	return context;
}

void ui_deletecontext( ui_ctx *context ) {
	int i = 0;
	for( i = 0; i < context->widgets; i++ ) ui_deletewidget( context->widget[i] );
	SDL_DestroyWindow( context->window );
	free( context->widget );
	free( context );
}

void ui_setcontextvisible( ui_ctx *context, int visible ) {
	if( visible ) {
		SDL_ShowWindow( context->window );
	} else {
		SDL_HideWindow( context->window );
	}
}

void ui_getcontextvisible( ui_ctx *context, int *visible ) {
	*visible = (SDL_GetWindowFlags( context->window ) & SDL_WINDOW_SHOWN);
}

void ui_getcontextsize( ui_ctx *context, int *w, int *h ) {
	SDL_GetWindowSize( context->window, w, h );
}

ui_widget *ui_newcontextwidget( ui_ctx *context, ui_widget *widget ) {
	int id = context->widgets;
	context->widget = (ui_widget **)realloc( context->widget, sizeof( ui_widget * ) * ++context->widgets );
	context->widget[id] = widget;
	return widget;
}

int ui_iscontextfocused( ui_ctx *context, SDL_Window *focus ) {
	if( context->window == focus ) return 1;
	return 0;
}

void ui_applycontextevent( ui_ctx *context, SDL_Event *e ) {
	int localx = 0, localy = 0;
	int i = 0;
	int hovered = 0;
	switch( e->type ) {
	case SDL_MOUSEBUTTONUP:
	case SDL_MOUSEBUTTONDOWN:
		if( context->hovered != NULL ) {
			localx = e->motion.x - context->hovered->rect.x;
			localy = e->motion.y - context->hovered->rect.y;
			ui_dowidgetclick( context->hovered, localx, localy, e->button.button, e->button.type );
		}
		break;
	case SDL_MOUSEMOTION:
		for( i = 0; i < context->widgets; i++ ) {
			if( ( e->motion.x >= context->widget[i]->rect.x ) &&  ( e->motion.x <= (context->widget[i]->rect.x+context->widget[i]->rect.w) ) ) {
				if( ( e->motion.y >= context->widget[i]->rect.y ) &&  ( e->motion.y <= (context->widget[i]->rect.y+context->widget[i]->rect.h) ) ) {
					if( context->hovered != context->widget[i] ) {
						if( context->hovered != NULL ) {
							localx = e->motion.x - context->hovered->rect.x;
							localy = e->motion.y - context->hovered->rect.y;
							ui_dowidgetmouseout( context->hovered, localx, localy, e->motion.state, 0 );
						}
						localx = e->motion.x - context->widget[i]->rect.x;
						localy = e->motion.y - context->widget[i]->rect.y;
						ui_dowidgetmousein( context->widget[i], localx, localy, e->motion.state, 0 );
					} else {
						localx = e->motion.x - context->widget[i]->rect.x;
						localy = e->motion.y - context->widget[i]->rect.y;
						ui_dowidgetmousemove( context->widget[i], localx, localy, e->motion.state, 0 );
					}
					context->hovered = context->widget[i];
					hovered = 1;
				}
			}
		}
		if( hovered == 0 ) context->hovered = NULL;
		break;
	}
}

void ui_updatecontext( ui_ctx *context ) {
	SDL_SelectRenderer( context->window );
	SDL_SetRenderDrawColor( 0, 0, 0, 255 );
	SDL_RenderClear();

	int i = 0;
	for( i = 0; i < context->widgets; i++ ) {
		ui_updatewidget( context->widget[i] );
		ui_drawwidget( context->widget[i] );
	}
	SDL_RenderPresent();
}

int ui_findcontextwidgets( ui_ctx *context, const char *name, ui_widgetlist *list ) {
	int i = 0;
	for( i = 0; i < context->widgets; i++ ) {
		//
	}
	return 0;
}
