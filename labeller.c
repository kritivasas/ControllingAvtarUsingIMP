// Include header files
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

CvPoint marker2d[15];
int mark;
float luarm,llarm,ruarm,rlarm;

#define ABS(a)	   (((a) < 0) ? -(a) : (a));

#define NMARKERS 16
#define LWRIST 0
#define LELBOW 1
#define LSHOULDER 2
#define CHEST 3
#define RSHOULDER 4
#define RELBOW 5
#define RWRIST 6
#define STOMACH 7
#define NECK 8
#define HEAD 9
#define LULEG 10
#define LKNEE 11
#define LFOOT 12
#define RULEG 13
#define RKNEE 14

void init_mouse_callback(int event, int x, int y, int flags, void* param);
void label(IplImage* img);
float calcAngle(int joint1, int joint2, float len);
void print_pose();

// Main function, defines the entry point for the program.
int main( int argc, char** argv )
{
	CvCapture* cp = cvCreateFileCapture( argv[1] );
	IplImage *cur;
        int nFrame = 0;

	cur = cvQueryFrame(cp);

	while(1) {
		char c;
		c = cvWaitKey(3);
		if(c==27) break;

		cur  = cvQueryFrame(cp);
		nFrame++;
		if(!cur) break;
		if(nFrame%20==0) label(cur);
	}

        cvReleaseCapture( &cp );
	return 0;
}

void label(IplImage* img)
{
	cvNamedWindow("Label", CV_WINDOW_AUTOSIZE);

	cvSetMouseCallback(
		"Label",
		init_mouse_callback,
		(void*) img
	);
	mark = 0;
	while( 1 ) {
		cvShowImage( "Label", img );
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
	dx = ABS((double)(marker2d[RSHOULDER].x - marker2d[RELBOW].x));
	dy = ABS((double)(marker2d[RSHOULDER].y - marker2d[RELBOW].y));
	ruarm = sqrt(dx*dx + dy*dy);
	dx = ABS((double)(marker2d[RELBOW].x - marker2d[RWRIST].x));
	dy = ABS((double)(marker2d[RELBOW].y - marker2d[RWRIST].y));
	rlarm = sqrt(dx*dx + dy*dy);

	print_pose();

	cvDestroyWindow("Label");
	return;
}

void init_mouse_callback( int event, int x, int y, int flags, void* param) {
	IplImage* image = (IplImage*) param;
	int i;
	switch( event ) {
		case CV_EVENT_LBUTTONUP: {
			marker2d[mark]=cvPoint(x,y);
			cvCircle(image,marker2d[mark],4,cvScalar(0xff,0x00,0x00,0xff),1,8,4);
			if(mark<10) {
				mark++;
			}
		}
		break;
	}
}

float calcAngle(int joint1, int joint2, float len)
{
	float a, dy;
	dy = (marker2d[joint2].y - marker2d[joint1].y);
	a = dy/len;
	if (a > 1) a = 1;
	a = asin(a);
	if(marker2d[joint2].x > marker2d[joint1].x) {
		if(dy > 0) a = 3.1416 - a;
		else a = (-a) - 3.1416;
	}
	return a;
}


void print_pose()
{
	float ls, le, rs, re;
	ls = 57.29*calcAngle(LSHOULDER, LELBOW, luarm);
	le = 57.29*calcAngle(LELBOW, LWRIST, llarm);
	rs = 57.29*calcAngle(RSHOULDER, RELBOW, luarm);
	re = 57.29*calcAngle(RELBOW, RWRIST, llarm);

/*	ls = 90 - ls;
	le = 90 - le -ls;
	rs = rs - 90;
	re = re - 90 - rs;
*/
	const char addr[15] = "127.0.0.1";
	char msg[100];
	sprintf(msg,"%.0f %.0f %.0f %.0f",ls, le, rs, re);
	printf("%s\n", msg);
	return;
}
