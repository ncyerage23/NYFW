
/*
 * NYFW -- a simple library for making games and applications on my pi
 * 
 *
 * This library will be how I make games on the pi moving forward, the code here
 * will be tested so I don't need to keep copy/pasting and creating fun new errors.
 * I'm sure later games will need something more complex, but for now this is good.
 *
 * Not sure what this file should do, though, besides just including all of the other headers, 
 * which isn't hella necessary in the long run. Maybe I'll need one big "NYFW" struct though? idk why, but maybe. 
 */


#ifndef NYFW_H
#define NYFW_H

/* ----- INCLUDES ----- */
#include "NYFW/canvas.h"
#include "NYFW/input.h"
#include "NYFW/window.h"
#include "NYFW/font.h"
#include "NYFW/colors.h"
#include "NYFW/nymg.h"


/* ----- SOME MACROS ----- */

#define MIN(x, y)	(x < y) ? x : y
#define MAX(x, y)	(x < y) ? y : x






#endif

