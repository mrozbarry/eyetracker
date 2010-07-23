
#include "ui-cvimage.h"

ui_widget *ui_newcvimage( char *name, int w, int h, int depth, int channels ) {
	ui_cvimage *image = (ui_cvimage *)malloc( sizeof( ui_cvimage ) );
	image->image = cvCreateImage( cvSize( w, h ), depth, channels );

	image->gammaLutMatrix = cvCreateMatHeader( 1, 256, CV_8UC1 );
	image->briConLutMatrix = cvCreateMatHeader( 1, 256, CV_8UC1 );

	ui_widget *widget = ui_newwidget( name, UI_CVIMAGE_TYPE, image );
	widget->onDestroy = &__deletecvimage;
	widget->onRedraw = &__redrawcvimage;

	ui_setwidgetsize( widget, w, h );

	return widget;
}

ui_widget *ui_newcvimagefrom( char *name, IplImage *source, int depth, int channels ) {
	ui_widget *widget = ui_newcvimage( name, source->width, source->height, source->depth, source->nChannels );

	ui_copycvimagefrom( widget, source );

	return widget;
}

ui_widget *ui_newcvimageload( char *name, char *path ) {
	ui_cvimage *image = (ui_cvimage *)malloc( sizeof( ui_cvimage ) );
	image->image = cvLoadImage( (const char *)path, CV_LOAD_IMAGE_UNCHANGED );

	ui_widget *widget = ui_newwidget( name, UI_CVIMAGE_TYPE, image );
	widget->onDestroy = &__deletecvimage;
	widget->onRedraw = &__redrawcvimage;

	ui_setwidgetsize( widget, image->image->width, image->image->height );

	return widget;
}

void __deletecvimage( ui_widget *widget ) {
	ui_cvimage *image = UI_CAST_CVIMAGE( widget );
	if( !image ) return;

	image->gammaLutMatrix = cvCreateMatHeader( 1, 256, CV_8UC1 );
	image->briConLutMatrix = cvCreateMatHeader( 1, 256, CV_8UC1 );

	cvReleaseMatHeader( &image->gammaLutMatrix );
	cvReleaseMatHeader( &image->briConLutMatrix );

	if( image->image ) cvReleaseImage( &image->image );
}

void __redrawcvimage( ui_widget *widget ) {
	ui_cvimage *image = UI_CAST_CVIMAGE( widget );
	if( !image ) return;
	IplImage *tconv = NULL;

	int bpp = image->image->depth * image->image->nChannels;
	if( bpp < 24 ) {
		tconv = cvCreateImage( cvGetSize( image->image ), 8, 3 );
		cvCvtColor( image->image, tconv, CV_GRAY2RGB );
		cvReleaseImage( &tconv );
	}

	SDL_Surface *tmp = SDL_CreateRGBSurfaceFrom( (void *)image->image->imageData,
							image->image->width,
							image->image->height,
							image->image->depth*image->image->nChannels,
							image->image->widthStep,
							0xff0000, 0x00ff00, 0x0000ff, 0 );
	SDL_Surface *surface = SDL_CreateRGBSurface( 0, image->image->width, image->image->height, 24, 0, 0, 0, 0 );
	SDL_BlitSurface( tmp, NULL, surface, NULL );
	if( widget->texture ) SDL_DestroyTexture( widget->texture );
	widget->texture = SDL_CreateTextureFromSurface( 0, surface );
	SDL_FreeSurface( surface );
	SDL_FreeSurface( tmp );
}

// The following two functions were taken from the EyeWriter project's source code, and have been slightly modified
void ui_applycvimagegamma( ui_widget *widget, float gamma ) {
	ui_cvimage *image = UI_CAST_CVIMAGE( widget );
	if( !image ) return;

	double minval;
	double maxval;

	if( image->image ) {
		cvMinMaxLoc( image->image, &minval, &maxval, NULL, NULL, NULL );
	}

	float spread = (float)(maxval - minval);

	int i = 0;
	for( i = 0; i < 256; i++ ){
		float pct = (float)( i - minval ) / spread;
		if( pct < 0 ) pct = 0;
		if( pct > 1 ) pct = 1;
		image->gammaLut[i] = (int)( 255.0 * powf( pct, gamma ) );
	}
	cvSetData( image->gammaLutMatrix, image->gammaLut, 0 );

	cvLUT( image->image, image->image, image->gammaLutMatrix);
}

void ui_applycvimagebc( ui_widget *widget, float brightness, float contrast ) {
	ui_cvimage *image = UI_CAST_CVIMAGE( widget );
	if( !image ) return;

	int i;

	if( contrast > 0 ) {
		double delta = 127. * contrast;
		double a = 255. / ( 255. - delta * 2 );
		double b = a * ( brightness * 100 - delta );
		for( i = 0; i < 256; i++ ) {
			int v = cvRound( a * i + b );
			if( v < 0 ) v = 0;
			if( v > 255 ) v = 255;
			image->briConLut[i] = (uchar)v;
		}
	} else {
		double delta = -128. * contrast;
		double a = ( 256. - delta * 2 ) / 255.;
		double b = a * brightness * 100. + delta;
		for( i = 0; i < 256; i++ ) {
			int v = cvRound( a * i + b);
			if( v < 0 ) v = 0;
			if( v > 255 ) v = 255;
			image->briConLut[i] = (uchar)v;
		}
	}

	cvSetData( image->briConLutMatrix, image->briConLut, 0 );

	cvLUT( image->image, image->image, image->briConLutMatrix);

}

void ui_getcvimagepicture( ui_widget *widget, IplImage **picture ) {
	ui_cvimage *image = UI_CAST_CVIMAGE( widget );
	if( !image ) return;

	*picture = image->image;
}

void ui_copycvimagefrom( ui_widget *widget, IplImage *source ) {
	ui_cvimage *image = UI_CAST_CVIMAGE( widget );
	if( !image ) return;

	cvConvertImage( source, image->image, 0 );
}

void ui_dilatecvimage( ui_widget *widget, int passes ) {
	ui_cvimage *image = UI_CAST_CVIMAGE( widget );
	if( !image ) return;

	cvDilate( image->image, image->image, NULL, passes );
}

void ui_convertcvimagethreshold( ui_widget *widget, double threshold, double maxvalue, int type ) {
	ui_cvimage *image = UI_CAST_CVIMAGE( widget );
	if( !image ) return;

	cvThreshold( image->image, image->image, threshold, maxvalue, type );
}

void ui_flipcvimage( ui_widget *widget, int flipmode ) {
	ui_cvimage *image = UI_CAST_CVIMAGE( widget );
	if( !image ) return;

	cvFlip( image->image, image->image, flipmode );
}
