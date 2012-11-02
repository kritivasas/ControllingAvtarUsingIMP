#include "defs.h"
#include "hist.h"

void drawMask(CvArr* mask,CvPoint p1,CvPoint p2)
{
	cvZero(mask);
	int dx = ABS(p1.x - p2.x) ;
	int dy = ABS(p1.y - p2.y) ;
	int d = sqrt(dx*dx + dy*dy);
	int w = d/10;
	int dwx = 0, dwy = 0;

	if(dx>dy) dwy = w;
	else dwx = w;

	CvPoint pts[4];
	pts[0] = cvPoint(
		p1.x - dwx,
		p1.y - dwy
	);
	pts[1] = cvPoint(
		p1.x + dwx,
		p1.y + dwy
	);
	pts[2] = cvPoint(
		p2.x + dwx,
		p2.y + dwy
	);
	pts[3] = cvPoint(
		p2.x - dwx,
		p2.y - dwy
	);

	cvFillConvexPoly(mask, pts, 4, CV_RGB(255,255,255), 8, 0);
	return;
}

void calcHSplanes(IplImage* src, IplImage *planes[])
{
        //
        IplImage* hsv = cvCreateImage( cvGetSize(src), 8, 3 ); 
        cvCvtColor( src, hsv, CV_BGR2HSV );

        IplImage* h_plane  = cvCreateImage( cvGetSize(src), 8, 1 );
        IplImage* s_plane  = cvCreateImage( cvGetSize(src), 8, 1 );
        IplImage* v_plane  = cvCreateImage( cvGetSize(src), 8, 1 );
        cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );
        planes[0] = h_plane;
	planes[1] = s_plane;

	cvReleaseImage(&hsv);
	cvReleaseImage(&v_plane);

	return;
}

CvHistogram* calcHist(IplImage *planes[], CvArr *mask)
{
        // Build the histogram and compute its contents.
        //
        int h_bins = 30, s_bins = 32; 
        CvHistogram* hist;
        {
          int    hist_size[] = { h_bins, s_bins };
          float  h_ranges[]  = { 0, 180 };          // hue is [0,180]
          float  s_ranges[]  = { 0, 255 }; 
          float* ranges[]    = { h_ranges, s_ranges };
          hist = cvCreateHist( 
            2, 
            hist_size, 
            CV_HIST_ARRAY, 
            ranges, 
            1 
          );
        }
        cvCalcHist(planes, hist, 0, mask);

	return hist;
}
