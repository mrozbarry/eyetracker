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

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#ifndef FOUR_PI
#	define FOUR_PI 12.56637061435917295385
#endif

#include "ui-context.h"
#include "ui-webcam.h"
#include "ui-label.h"
#include "ui-cvimage.h"
#include "ui-slider.h"

// -- Structures/Enums
typedef enum trackermode {
	TM_IDLE = 0,								// Display webcam, and tracking points if applicable
	TM_SET_TEMPLATES,							// Setup tracking points
	TM_SET_CALIBRATION,							// Set calibration points
	TM_TRACK,									// Test tracking
	TM_QUIT,									// Exit program
	TM_LASTMODE									// Size of array needed to hold above elements
} trackermode;

typedef enum uicontexts {
	context_setup = 0,
	context_config,
	context_showgrey,
	context_main,
	context_EOA
} uicontexts;

#define uicvpt	CvPoint2D32f

typedef struct cvblob {
	float		area, length;
	SDL_Rect	boundingRect;
	uicvpt		centroid;
	int			hole;
	int			points;
	uicvpt		*point;
} cvblob;

// -- Function Prototypes
void sortBlobs( CvSeq *blob[], int num );

void onchange_gamma( ui_widget *widget, float value );
void onchange_brightness( ui_widget *widget, float value );
void onchange_contrast( ui_widget *widget, float value );
void onchange_threshold( ui_widget *widget, float value );

void load_lastvalues( void );
void save_currentvalues( void );

// -- Global variables
SDL_Window		*wnd;									// Main SDL Window
IplImage		*frame;									// Camera frame being processed
int				mode = TM_IDLE;							// Program Mode
uicvpt			eyepoint, norm_eyepoint;
ui_widget		*lblgamma, *lblbrightness, *lblcontrast, *lblthreshold;
float			gbc[3] = { 1.0, 0.0, 0.0 };
float			threshold = 75.0f;

#define RESX		1024
#define RESY		768

// -- Program Entry Point
int main( int argc, char *argv[] ) {
	SDL_Event	event;
	ui_ctx		*context[context_EOA];
	SDL_Window	*window = NULL;


	if( SDL_Init( SDL_INIT_VIDEO|SDL_INIT_TIMER ) < 0 ) {
		printf( "Unable to initialize SDL:  %s\n", SDL_GetError() );
        return 1;
    }
    if( TTF_Init( ) < 0 ) {
		printf( "Unable to initialize SDL_ttf:  %s\n", TTF_GetError() );
        return 1;
    }

	// -- Setup the UI to default startups
    context[context_setup] = ui_newcontext( "Eye Tracker Setup", 1, SDL_WINDOWPOS_CENTERED, RESX, RESY, 0 );
    context[context_config] = ui_newcontext( "Configure", RESX + 10, SDL_WINDOWPOS_CENTERED, 200, RESY, SDL_WINDOW_RESIZABLE );
    context[context_showgrey] = ui_newcontext( "Tracking Image", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0 );
    context[context_main] = ui_newcontext( "Eye Tracker", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, RESX, RESY, 0 );

	ui_setcontextvisible( context[context_setup], 1 );
	ui_setcontextvisible( context[context_config], 1 );
	ui_setcontextvisible( context[context_showgrey], 1 );
	ui_setcontextvisible( context[context_main], 0 );

    ui_widget *camera = ui_newcontextwidget( context[context_setup], ui_newwebcam( "webcam", -1 ) );
    ui_setwidgetposition( camera, (RESX-640)/2, (RESY-480)/2 );
    int w, h; ui_getwebcamframesize( camera, &w, &h );
    ui_widget *camtransform = ui_newcontextwidget( context[context_showgrey], ui_newcvimage( "webcam0", w, h, 8, 1 ) );
    ui_setwidgetposition( camera, 0, 0 );

	lblgamma = ui_newcontextwidget( context[context_config], ui_newlabel( "lblGamma", TTF_OpenFont( "WHITRABT.ttf", 12 ), "Gamma(0.0)", 1, 255, 255, 255 ) );
	ui_setwidgetposition( lblgamma, 5, 20 );
    ui_widget *gamma = ui_newcontextwidget( context[context_config], ui_newslider( "gamma", 0.0, 5.0, 0.0, 190, &onchange_gamma ) );
	ui_setwidgetposition( gamma, 5, 40 );
	ui_setslidervalue( gamma, gbc[0] );

	lblbrightness = ui_newcontextwidget( context[context_config], ui_newlabel( "lblBrightness", TTF_OpenFont( "WHITRABT.ttf", 12 ), "Brightness(0.0)", 1, 255, 255, 255 ) );
	ui_setwidgetposition( lblbrightness, 5, 70 );
    ui_widget *brightness = ui_newcontextwidget( context[context_config], ui_newslider( "brightness", -1.0, 1.0, 0.0, 190, &onchange_brightness ) );
	ui_setwidgetposition( brightness, 5, 90 );
	ui_setslidervalue( brightness, gbc[1] );

	lblcontrast = ui_newcontextwidget( context[context_config], ui_newlabel( "lblContrast", TTF_OpenFont( "WHITRABT.ttf", 12 ), "Contrast(0.0)", 1, 255, 255, 255 ) );
	ui_setwidgetposition( lblcontrast, 5, 120 );
    ui_widget *contrast = ui_newcontextwidget( context[context_config], ui_newslider( "contrast", -1.0, 1.0, 0.0, 190, &onchange_contrast ) );
	ui_setwidgetposition( contrast, 5, 140 );
	ui_setslidervalue( contrast, gbc[2] );

	lblthreshold = ui_newcontextwidget( context[context_config], ui_newlabel( "lblThreshold", TTF_OpenFont( "WHITRABT.ttf", 12 ), "Threshold(0.0)", 1, 255, 255, 255 ) );
	ui_setwidgetposition( lblthreshold, 5, 190 );
    ui_widget *wthreshold = ui_newcontextwidget( context[context_config], ui_newslider( "threshold", 0.0, 255.0, 0.0, 190, &onchange_threshold ) );
	ui_setwidgetposition( wthreshold, 5, 210 );
	ui_setslidervalue( wthreshold, threshold );

	IplImage *imgtransform;
	CvMemStorage *storage = cvCreateMemStorage(0);
	CvMoments myMoments; // = (CvMoments *)malloc( sizeof( CvMoments * ) );
	int i = 0, j = 0;
	while( mode != TM_QUIT ) {
		// **** This part of the code was taken from the EyeWriter Project and chunks of OpenFrameworks
		ui_copywebcamframetocvimage( camera, camtransform );
		ui_dilatecvimage( camtransform, 2 );
		ui_applycvimagegamma( camtransform, gbc[0] );
		ui_applycvimagebc( camtransform, gbc[1], gbc[2] );
		ui_convertcvimagethreshold( camtransform, (double)threshold, 255, CV_THRESH_BINARY );
		ui_getcvimagepicture( camtransform, &imgtransform );
		CvSeq *contours = NULL;
		int nc = cvFindContours( imgtransform, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint( 0, 0 ) );
		if( nc ) {
			CvSeq *_blob[nc];
			int _blobs = 0;
			CvSeq *list = contours;
			while( list ) {
				float area = fabs( cvContourArea( list, CV_WHOLE_SEQ, 0 ) );
				if( (area > 300) && (area < 10000 ) ) _blob[_blobs++] = list;
				list = list->h_next;
			} _blobs--;
			if( _blobs > 1 ) sortBlobs( _blob, _blobs );
			int numblobs = _blobs > 100 ? 100 : _blobs;
			cvblob blobs[numblobs];
			IplImage *frame = UI_CAST_WEBCAM( camera )->frame;
			printf( "\n----------\n" );
			for( i = 0; i < numblobs; i++ ) {
				float _area = cvContourArea( _blob[i], CV_WHOLE_SEQ, 0 );
				CvRect rect	= cvBoundingRect( _blob[i], 0 );
				cvMoments( _blob[i], &myMoments, 0 );

				printf( "%d. Area: %.2f\n", i+1, fabs( _area ) );

				blobs[i].area = fabs(_area);
				blobs[i].hole = _area < 0 ? 1 : 0;
				blobs[i].length = cvArcLength( _blob[i], CV_WHOLE_SEQ, -1 );
				blobs[i].boundingRect.x = rect.x;
				blobs[i].boundingRect.y = rect.y;
				blobs[i].boundingRect.w = rect.width;
				blobs[i].boundingRect.h = rect.height;
				blobs[i].centroid.x = (myMoments.m10 / myMoments.m00);
				blobs[i].centroid.y = (myMoments.m01 / myMoments.m00);
				blobs[i].points = _blob[i]->total;
				blobs[i].point = (uicvpt *)malloc( sizeof( uicvpt ) * blobs[i].points );


				//cvRectangle(frame, cvPoint( rect.x, rect.y ), cvPoint( rect.x+rect.width, rect.y+rect.height ), CV_RGB( 0, 255, 0 ), 1, 8, 0 );

				uicvpt pt;
				CvSeqReader reader;
				cvStartReadSeq( _blob[i], &reader, 0 );
				blobs[i].point = (uicvpt *)realloc( blobs[i].point, sizeof( uicvpt ) * ++blobs[i].points );
				for( j = 0; j < _blob[i]->total; j++ ) {
					CV_READ_SEQ_ELEM( pt, reader );
					blobs[i].point[j].x = pt.x;
					blobs[i].point[j].y = pt.y;
				}

				float ratio = blobs[i].boundingRect.w < blobs[i].boundingRect.h ?
								(float)blobs[i].boundingRect.w / (float)blobs[i].boundingRect.h :
								(float)blobs[i].boundingRect.h / (float)blobs[i].boundingRect.w;
				float compactness = ( ( (float)blobs[i].length*(float)blobs[i].length/(float)blobs[i].area)/(float)FOUR_PI);
				printf( " -> Ratio: %.3f Compactness: %.1f\n", ratio, compactness );
				if( compactness > 5 ) continue;
				if( ratio > 0.5f ) {
					eyepoint = blobs[i].centroid;
					norm_eyepoint.x = eyepoint.x / frame->width;
					norm_eyepoint.y = eyepoint.y / frame->height;

					cvRectangle(frame, cvPoint( rect.x, rect.y ), cvPoint( rect.x+rect.width, rect.y+rect.height ), CV_RGB( 255, 0, 0 ), 1, 8, 0 );

					break; // Yay, we're finally done!
				}
			}
		}
		cvClearMemStorage( storage );
		// ******* End of borrowed code

		// Update UI
		for( i = 0; i < context_EOA; i++ ) ui_updatecontext( context[i] );

		while( SDL_PollEvent(  &event ) ) {
			switch( event.type ) {
			case SDL_QUIT:
				mode = TM_QUIT;
				break;
			case SDL_KEYDOWN:
				if( event.key.keysym.sym == SDLK_ESCAPE ) mode = TM_QUIT;
				break;
			case SDL_MOUSEMOTION:
				window = SDL_GetWindowFromID( event.motion.windowID );
				for( i = 0; i < context_EOA; i++ ) {
					if( ui_iscontextfocused( context[i], window ) ) ui_applycontextevent( context[i], &event );
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				window = SDL_GetWindowFromID( event.button.windowID );
				for( i = 0; i < context_EOA; i++ ) {
					if( ui_iscontextfocused( context[i], window ) ) ui_applycontextevent( context[i], &event );
				}
				break;
			}
		}

		SDL_Delay(1);
	}

	for( i = 0; i < context_EOA; i++ ) ui_deletecontext( context[i] );


	//cleanObjects();
	cvReleaseMemStorage( &storage );

	TTF_Quit();
	SDL_Quit();

	return 0;
}

void sortBlobs( CvSeq *arr[], int num ) {
	float a, b;
	CvSeq *tmp = NULL;
	int i, j;
	for( i = 0; i < num; i++ ) {
		for( j = 0; j < num; j++ ) {
			if( j == i ) continue;
			a = fabs( cvContourArea( arr[i], CV_WHOLE_SEQ, 0 ) );
			b = fabs( cvContourArea( arr[j], CV_WHOLE_SEQ, 0 ) );
			if( a > b ) {
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
	}
}

void onchange_gamma( ui_widget *widget, float value ) {
	char tmpText[50];

	gbc[0] = value;

	sprintf( tmpText, "Gamma(%.1f)", gbc[0] );
	ui_setlabeltext( lblgamma, tmpText );
}

void onchange_brightness( ui_widget *widget, float value ) {
	char tmpText[50];

	gbc[1] = value;

	sprintf( tmpText, "Brightness(%.1f)", gbc[1] );
	ui_setlabeltext( lblbrightness, tmpText );
}

void onchange_contrast( ui_widget *widget, float value ) {
	char tmpText[50];

	gbc[2] = value;

	sprintf( tmpText, "Contrast(%.1f)", gbc[2] );
	ui_setlabeltext( lblcontrast, tmpText );
}

void onchange_threshold( ui_widget *widget, float value ) {
	char tmpText[50];

	threshold = value;

	sprintf( tmpText, "Threshold(%.1f)", threshold );
	ui_setlabeltext( lblthreshold, tmpText );
}

void load_lastvalues( void ) {
}

void save_currentvalues( void ) {
}
