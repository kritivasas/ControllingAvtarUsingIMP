#ifndef _TRACKER_H_
#define _TRACKER_H_

#include <opencv/cv.h>
#include "defs.h"

// Returns a new initialized tracker object.
Tracker* init_tracker();

// for tracking.
void track_frame(Tracker *t, IplImage *cur);

#endif
