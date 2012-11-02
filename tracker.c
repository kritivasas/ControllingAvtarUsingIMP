#include "tracker.h"
#include <stdio.h>
#include "hist.c"
#include "senderprog.h"

#define MAX_CORNERS 100
#define MATCH_THRESH 0.94

Tracker* init_tracker()
{
  Tracker *T = (Tracker *)malloc(sizeof(Tracker));
  if(T == NULL) return NULL;

  T->bkg_img = NULL;
  T->huarm = NULL;
  T->hlarm = NULL;

  T->prev_img = NULL;
  T->silh = NULL;
  T->nFrames = 0;
  T->HSplanes = (IplImage **)malloc(2*sizeof(IplImage *));
  T->HSplanes[0] = NULL;
  T->HSplanes[1] = NULL;

  return T;
}

static void getSilh(Tracker *T, IplImage* cur)
{
  // do background subtraction to obtain silhouette and store it
  IplImage *silh, *imgA, *imgB;

  imgA = cvCreateImage(cvSize(cur->width, cur->height), 8, 1);
  cvCvtColor(T->bkg_img, imgA, CV_RGB2GRAY);
  imgB = cvCreateImage(cvSize(cur->width, cur->height), 8, 1);
  cvCvtColor(cur, imgB, CV_RGB2GRAY);
  silh = cvCreateImage(cvSize(cur->width, cur->height), 8, 1);
  cvAbsDiff(imgA, imgB, silh);
  cvThreshold(silh, silh, 15, 255, CV_THRESH_BINARY);

  cvReleaseImage(&imgA);
  cvReleaseImage(&imgB);
  cvReleaseImage(&T->silh);

  T->silh = silh;
  return;
}

static void setHSplanes(Tracker *T, IplImage* cur)
{
  cvReleaseImage(&T->HSplanes[0]);
  cvReleaseImage(&T->HSplanes[1]);
  calcHSplanes(cur, T->HSplanes);
  return;
}

static void update_features(Tracker *T, IplImage* cur)
{
  // comment out features you dont't want to use
  //getSilh(T, cur);
  T->nFrames++;
  setHSplanes(T, cur);
  return;
}

static float calcAngle(int joint, CvPoint p, float len)
{
	float a, dy;
	dy = (p.y - marker2d[joint].y);
	a = dy/len;
	if (a > 1) a = 1;
	a = asin(a);
	if(p.x > marker2d[joint].x) {
		if(dy > 0) a = 3.1416 - a;
		else a = (-a) - 3.1416;
	}
	return a;
}

static void search_pose(int joint1, int joint2, float del, float length, Tracker *T, IplImage *cur)
{
        CvMat* mask = cvCreateMat( cur->height, (cur)->width, CV_8UC1); 
	CvHistogram *h1,*h2,*h3;
	float len = length, mcur;

	CvPoint p3= cvPoint(marker2d[joint2].x,marker2d[joint2].y);
	drawMask(mask,marker2d[joint1],marker2d[joint2]);
	h3 = calcHist(T->HSplanes,mask);
	mcur = cvCompareHist(T->huarm,h3,CV_COMP_CORREL);

	while ( del > 1 ) {
		CvPoint p1,p2;
		float lc = calcAngle(joint1, p3, len);

		float theta=3.14*del/180;
		p1 = cvPoint((int)(marker2d[joint1].x-len*cos(lc-theta)) , (int)(marker2d[joint1].y+len*sin(lc-theta)));
		p2 = cvPoint((int)(marker2d[joint1].x-len*cos(lc+theta)) , (int)(marker2d[joint1].y+len*sin(lc+theta)));
		double m1,m2;

		drawMask(mask,marker2d[joint1],p1);
		h1 = calcHist(T->HSplanes,mask);
		m1 = cvCompareHist(T->huarm,h1,CV_COMP_CORREL);
		drawMask(mask,marker2d[joint1],p2);
		h2 = calcHist(T->HSplanes,mask);
		m2 = cvCompareHist(T->huarm,h2,CV_COMP_CORREL);

		if(m1>mcur) {
			mcur = m1;
			p3 = p1;
		}
		if(m2>mcur) {
			mcur = m2;
			p3 = p2;
		}
			 
		if(mcur>MATCH_THRESH) {
			break;
		}	
		
		del = del/4;

		cvReleaseHist(&h1);
		cvReleaseHist(&h2);
		cvReleaseHist(&h3);
	}
	marker2d[joint2] = p3;
	cvReleaseMat(&mask);
	return;
}

static void update_llarm(Tracker *T, IplImage *cur)
{
	search_pose(LELBOW, LWRIST, 5, llarm, T, cur);
	return;
}

static void update_luarm(Tracker *T, IplImage *cur)
{
	search_pose(LSHOULDER, LELBOW, 5, luarm, T, cur);
	update_llarm(T, cur);
	return;
}

static void update_rlarm(Tracker *T, IplImage *cur)
{
	search_pose(RELBOW, RWRIST, 5, llarm, T, cur);
	return;
}

static void update_ruarm(Tracker *T, IplImage *cur)
{
	search_pose(RSHOULDER, RELBOW, 5, luarm, T, cur);
	update_rlarm(T, cur);
	return;
}

static void update_torso(Tracker *T, IplImage *cur)
{
	update_luarm(T, cur);
	update_ruarm(T, cur);
	return;
}

// Print all the pose parameters, to be used for debugging
static void print_pose_params(Tracker *T)
{
#if 0
  printf("Frame No: %d\n", T->nFrames);
  printf("LS:(%d,%d) LE:(%d,%d)\n", marker2d[LSHOULDER].x,
         marker2d[LSHOULDER].y, marker2d[LELBOW].x, marker2d[LELBOW].y);
  printf("\n");
#endif
}

static void send_angles()
{
	float ls, le, rs, re;
	ls = 57.29*calcAngle(LSHOULDER, marker2d[LELBOW], luarm);
	le = 57.29*calcAngle(LELBOW, marker2d[LWRIST], llarm);
	rs = 57.29*calcAngle(RSHOULDER, marker2d[RELBOW], luarm);
	re = 57.29*calcAngle(RELBOW, marker2d[RWRIST], llarm);

/*	ls = 90 - ls;
	le = 90 - le -ls;
	rs = rs - 90;
	re = re - 90 - rs;
*/
	const char addr[15] = "127.0.0.1";
	char msg[100];
	sprintf(msg,"%.0f %.0f %.0f %.0f",ls, le, rs, re);
	printf("%s\n", msg);
	//send_data(addr, msg);
	return;
}


// track motion in this frame
void track_frame(Tracker *T, IplImage *cur)
{
  update_features(T, cur);
  update_torso(T, cur);
  if(T->nFrames%20==0) send_angles();
  print_pose_params(T);
  return;
}
