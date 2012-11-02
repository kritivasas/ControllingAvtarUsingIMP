// Include header files
#include "tracker.c"
#include <stdio.h>

Tracker* init(CvCapture* cp);
void init_mouse_callback(int event, int x, int y, int flags, void* param);
void draw_figure(IplImage *cur2);

// Main function, defines the entry point for the program.
int main( int argc, char** argv )
{
	CvCapture* cp = cvCreateFileCapture( argv[1] );
	Tracker *T;
	IplImage *cur;

	// Call the initialization module
	T = init(cp);		printf("Initialization over %d markers initalized\n", mark);

	cur = cvQueryFrame(cp);
	IplImage* cur2 = cvCloneImage(cur);

	while(1) {
		track_frame(T, cur);
		draw_figure(cur2);
		cvShowImage ( "result", cur2 );
		char c;
		c = cvWaitKey(3);
		if(c==27) break;

		cur  = cvQueryFrame(cp);
		if(!cur) break;
		cvCopy( cur,cur2,0 );
	}

        cvReleaseCapture( &cp );
	cvDestroyWindow("result");
	return 0;
}

Tracker* init(CvCapture* cp)
{
	IplImage* ic = cvQueryFrame(cp);
	IplImage* temp = cvCloneImage(ic);
	Tracker *T;

	T = init_tracker();
	T->bkg_img = cvCloneImage(ic);

	cvNamedWindow("Initialize", CV_WINDOW_AUTOSIZE);

	cvSetMouseCallback(
		"Initialize",
		init_mouse_callback,
		(void*) temp
	);

	while( 1 ) {
	//	ic = cvQueryFrame(cp);
		cvCopy(ic,temp,0);
		cvShowImage( "Initialize", temp );
		char c = cvWaitKey(15);
		if( c==27 ) {
			break;
		}
	}

	double dx = ABS((double)(marker2d[LSHOULDER].x - marker2d[LELBOW].x));
	double dy = ABS((double)(marker2d[LSHOULDER].y - marker2d[LELBOW].y));
	luarm = sqrt(dx*dx + dy*dy);
	dx = ABS((double)(marker2d[LELBOW].x - marker2d[LWRIST].x));
	dy = ABS((double)(marker2d[LELBOW].y - marker2d[LWRIST].y));
	llarm = sqrt(dx*dx + dy*dy);

	T->prev_img = cvCloneImage(ic);

        CvMat* mask = cvCreateMat( ic->height, (ic)->width, CV_8UC1); 
	calcHSplanes(ic, T->HSplanes);
	drawMask(mask,marker2d[LSHOULDER],marker2d[LELBOW]);
	T->huarm = calcHist(T->HSplanes, mask);

	cvDestroyWindow("Initialize");
	cvReleaseImage(&temp);
	return T;
}

void init_mouse_callback( int event, int x, int y, int flags, void* param) {
	IplImage* image = (IplImage*) param;
	int i;
	switch( event ) {
		case CV_EVENT_LBUTTONUP: {
			marker2d[mark]=cvPoint(x,y);
			cvCircle(image,marker2d[mark],4,cvScalar(0xff,0x00,0x00,0xff),1,8,4);
			if(mark<10) {
		//		printf("%s initialized\n",marker_name[i]);
				mark++;
			}
		}
		break;
	}
}

void draw_figure(IplImage *cur2)
{
	int i = 0;
	for(i=0;i<mark;i++) {
		cvCircle(cur2,marker2d[i],4,cvScalar(0xff,0x00,0x00,0xff),1,8,4);
	}
	cvCircle(cur2,marker2d[LSHOULDER],luarm,cvScalar(0xff,0x00,0x00,0xff),1,8,4);
	cvCircle(cur2,marker2d[LELBOW],luarm,cvScalar(0xff,0x00,0x00,0xff),1,8,4);
	cvLine(cur2, marker2d[LWRIST], marker2d[LELBOW], CV_RGB(255,0,0), 1, 8, 0);
	cvLine(cur2, marker2d[LELBOW], marker2d[LSHOULDER], CV_RGB(255,0,0), 1, 8, 0);
	cvLine(cur2, marker2d[LSHOULDER], marker2d[CHEST], CV_RGB(255,0,0), 1, 8, 0);
	cvLine(cur2, marker2d[CHEST], marker2d[RSHOULDER], CV_RGB(255,0,0), 1, 8, 0);
	cvLine(cur2, marker2d[RSHOULDER], marker2d[RELBOW], CV_RGB(255,0,0), 1, 8, 0);
	cvLine(cur2, marker2d[RELBOW], marker2d[RWRIST], CV_RGB(255,0,0), 1, 8, 0);
	return;
}
