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
#include <opencv/cv.h>
#include <opencv/highgui.h>

#ifndef UIWEBCAM_
#define UIWEBCAM_

#define UI_WEBCAM_TYPE	0x01
#define UI_CAST_WEBCAM( widget )	(widget->wtype == UI_WEBCAM_TYPE ? (ui_webcam *)widget->cwidget : NULL)

typedef struct ui_webcam {
	CvCapture	*capture;
	IplImage	*frame;
} ui_webcam;

ui_widget	*ui_newwebcam( char *name, int capture_source );
void		__deletewebcam( ui_widget *widget );
void		__redrawwebcam( ui_widget *widget );

void		ui_copywebcamframetocvimage( ui_widget *widget, ui_widget *cvimage );
void		ui_getwebcamframesize( ui_widget *widget, int *w, int *h );

#endif /* UIWEBCAM_ */
