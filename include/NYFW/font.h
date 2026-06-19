/* NYFW Fonts -- drawing text 
 *
 * Just drawing a string of text to a canvas. 
 * Pretty simple stuff. 
 *
 *
 */


#ifndef NYFW_FONT_H
#define NYFW_FONT_H

/* ----- INCLUDES ----- */
#include "NYFW/canvas.h"
#include <stdint.h>


/* ----- TEXT RENDERING ----- */
int nyfw_fontInit(const char* font_path);	// loads font file to memory
void nyfw_fontClose();

void nyfw_fontDrawString(NYFW_Canvas oc, int x, int y, const char* s, uint16_t color);				// draws string at regular size (8x16)
void nyfw_fontDrawStringS(NYFW_Canvas oc, int x, int y, const char* s, uint16_t color, int scale);		// draws string scaled up (now 8*scale x 16*scale)

#endif
