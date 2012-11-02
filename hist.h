#ifndef _HIST_H_
#define _HIST_H_

#include <opencv/cv.h>

// Helper function.
// A rectangular mask is constructed around p1 & p2.
void drawMask(CvArr* mask,CvPoint p1,CvPoint p2);

// Helper function.
// Returns HS planes corresponding to passed image.
void calcHSplanes(IplImage* src, IplImage *planes[]);

// Returns HS histogram corresponding to passed HS planes and mask.
CvHistogram* calcHist(IplImage *planes[], CvArr *mask);

#endif
