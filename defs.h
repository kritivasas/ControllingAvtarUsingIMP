#ifndef _DEFS_H_
#define _DEFS_H_

#include "opencv/cv.h"
#include "opencv/highgui.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>

#define WID 400
#define HGT 300

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
#define RFOOT 15

char marker_name[NMARKERS][50] = {
	"left wrist",
	"left elbow",
	"left shoulder",
	"chest",
	"right shoulder",
	"right elbow",
	"right wrist",
	"stomach",
	"neck",
	"head",
	"left hip",
	"left knee",
	"left foot",
	"right hip",
	"right knee",
	"right foot",
};

//-----------------------------------------------------------------------------
// Some Helpful Macros
#define ABS(a)	   (((a) < 0) ? -(a) : (a));

//-----------------------------------------------------------------------------
// Model's parameters (TODO: Make a BodyModel Object out of these).

int mark = 0; // number of joints initalized.
CvPoint marker2d[NMARKERS]; // 2D positions of the markers.
double luarm; // length of upper arm
double llarm; // length of upper arm
double lst = 90, lsp = 0; // shoulder angles

//-----------------------------------------------------------------------------
// Tracker Object
typedef struct Tracker Tracker;

struct Tracker
{
  // Constant data initialized at start, but not changed
  IplImage* bkg_img;        // background image
  CvHistogram *huarm;        // HS(V) Color histogram for upper arm
  CvHistogram *hlarm;        // HS(V) Color histogram for lower arm

  // Features/Data that are updated every frame
  IplImage* prev_img;       // previous image
  IplImage* silh;           // silhouette obtained through background sub.
  int nFrames;              // no. of frames processed.
  IplImage **HSplanes;      // HS(V) planes of current image;

};

#endif
