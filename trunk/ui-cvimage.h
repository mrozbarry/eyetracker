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

#ifndef UICVIMAGE_
#define UICVIMAGE_

#define UI_CVIMAGE_FROMSOURCE	-1

#define UI_CVIMAGE_TYPE	0x03
#define UI_CAST_CVIMAGE( widget )	(widget->wtype == UI_CVIMAGE_TYPE ? (ui_cvimage *)widget->cwidget : NULL)

typedef struct ui_cvimage {
	IplImage	*image;
	uchar		gammaLut[256];
	CvMat		*gammaLutMatrix;
	uchar		briConLut[256];
	CvMat		*briConLutMatrix;
} ui_cvimage;

ui_widget	*ui_newcvimage( char *name, int w, int h, int depth, int channels );
ui_widget	*ui_newcvimagefrom( char *name, IplImage *source, int depth, int channels );
ui_widget	*ui_newcvimageload( char *name, char *path );
void		__deletecvimage( ui_widget *widget );
void		__redrawcvimage( ui_widget *widget );

void		ui_applycvimagegamma( ui_widget *widget, float gamma );
void		ui_applycvimagebc( ui_widget *widget, float brightness, float contrast );

void		ui_getcvimagepicture( ui_widget *widget, IplImage **picture );

void		ui_copycvimagefrom( ui_widget *widget, IplImage *source );
void		ui_dilatecvimage( ui_widget *widget, int passes );
void		ui_convertcvimagethreshold( ui_widget *widget, double threshold, double maxvalue, int type );
void		ui_flipcvimage( ui_widget *widget, int flipmode );

#endif /* UICVIMAGE_ */
