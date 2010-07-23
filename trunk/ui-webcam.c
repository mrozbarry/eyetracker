#include "ui-webcam.h"
#include "ui-cvimage.h"

ui_widget *ui_newwebcam( char *name, int capture_source ) {
	ui_webcam *webcam = (ui_webcam *)malloc( sizeof( ui_webcam ) );
	webcam->capture = cvCaptureFromCAM( capture_source );
	if( !webcam->capture ) {
		printf( "Warning - webcam->capture doesn't seem to point to anything...\n" );
	}

	ui_widget *widget = ui_newwidget( name, UI_WEBCAM_TYPE, webcam );

	widget->onDestroy = &__deletewebcam;
	widget->onRedraw = &__redrawwebcam;

	// Manually redraw to get webcam->frame information (for width and height)
	cvGrabFrame( webcam->capture );
	webcam->frame = cvQueryFrame( webcam->capture );

	ui_setwidgetsize( widget, webcam->frame->width, webcam->frame->height );

	return widget;
}

void __deletewebcam( ui_widget *widget ) {
	ui_webcam *webcam = UI_CAST_WEBCAM( widget );
	if( !webcam ) return;
	cvReleaseCapture( &(webcam->capture) );
}

void __redrawwebcam( ui_widget *widget ) {
	ui_webcam *webcam = UI_CAST_WEBCAM( widget );
	if( !webcam ) return;

	if( webcam->frame && webcam->frame->imageData ) {
		SDL_Surface *convert = SDL_CreateRGBSurfaceFrom( (void *)webcam->frame->imageData,
								webcam->frame->width,
								webcam->frame->height,
								webcam->frame->depth*webcam->frame->nChannels,
								webcam->frame->widthStep,
								0xff0000, 0x00ff00, 0x0000ff, 0 );
		SDL_Surface *surface = SDL_CreateRGBSurface( 0, widget->rect.w, widget->rect.h, 24, 0, 0, 0, 0 );
		if( !convert || !surface ) printf( "SDL Error: %s\n", SDL_GetError() );
		SDL_BlitSurface( convert, NULL, surface, NULL );

		if( widget->texture ) SDL_DestroyTexture( widget->texture );
		widget->texture = SDL_CreateTextureFromSurface( 0, surface );

		SDL_FreeSurface( surface );
		SDL_FreeSurface( convert );
	}

	cvGrabFrame( webcam->capture );
	webcam->frame = cvQueryFrame( webcam->capture );
	cvFlip( webcam->frame, webcam->frame, -1 );

	widget->redraw = 1;	// Quick hack to make this redraw every frame :)
}

void ui_copywebcamframetocvimage( ui_widget *widget, ui_widget *cvimage ) {
	ui_webcam *webcam = UI_CAST_WEBCAM( widget );
	if( !webcam ) return;
	ui_cvimage *picture = UI_CAST_CVIMAGE( cvimage );
	if( !picture ) return;

	if( webcam->frame->width == picture->image->width ) {
		if( webcam->frame->height == picture->image->height) {
			if( ( webcam->frame->depth == picture->image->depth ) && ( webcam->frame->nChannels == picture->image->nChannels ) ) {
				cvCopy( webcam->frame, picture->image, 0 );
			} else {
				// If they aren't the same, it's likely greyscale...
				cvCvtColor( webcam->frame, picture->image, CV_BGR2GRAY );
			}
			cvimage->redraw = 1;
		}
	}

}

void ui_getwebcamframesize( ui_widget *widget, int *w, int *h ) {
	ui_webcam *webcam = UI_CAST_WEBCAM( widget );
	if( !webcam ) return;

	if( !(webcam->frame) ) __redrawwebcam( widget );

	*w = webcam->frame->width;
	*h = webcam->frame->height;
}
