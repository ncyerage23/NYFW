/*
 * NYFW Canvas -- a 2D array of pixels
 *
 * Used for representing the screen, but also for virtual screens
 * and things. Will add image loading and other fun stuff to this.
 *
 */

#ifndef NYFW_CANVAS_H
#define NYFW_CANVAS_H

/* ----- INCLUDES ----- */
#include <stdint.h>
#include <stddef.h>


/* ----- STRUCTS & MACROS ----- */
	// The Canvas struct. Holds an array of pixels, along with some info about them.
	// Width/height are the virtual width and height of the 2D canvas (it's stored in 1D)
	// Stride corresponds to pixels per row. This is the same as width, unless the canvas is a
	// subcanvas (shown later).
	// The "original" value is set to 0 for all new canvases, 1 for subcanvases. 
typedef struct {
	uint16_t* pixels;
	size_t width;
	size_t height;
	size_t stride;
	int original;
} NYFW_Canvas;	

	// Two macros for canvases. CANV_PIXEL outputs the pixel at a given (x,y), though it's not
	// safe if the coordinates are out of bounds. 
	// CANV_NULL is a NULL representation of a canvas, for returns in later functions. 
#define CANV_PIXEL(oc, x, y)	(oc).pixels[(y)*(oc).stride + (x)]
#define CANV_NULL		((NYFW_Canvas){0})
#define CANV_IN_BOUNDS(oc, x, y)	( 0 <= (x) && (x) < (oc).width && 0 <= (y) && (y) < (oc).height ) ? 1 : 0

	// A representation of a rectangle, for use in blitting and subcanvas operations. 
	// Just a nice wrapper for that data. 
typedef struct {
	int x, y;
	int w, h;
} NYFW_Rect;


/* ----- CONSTRUCTORS ----- */
	// Returns a canvas struct made from the given data. 
NYFW_Canvas nyfw_canvas(uint16_t* pixels, size_t width, size_t height, size_t stride);

	// Returns a canvas representing a section of a larger one (clipped to oc's bounds)
NYFW_Canvas nyfw_subcanvas(NYFW_Canvas oc, NYFW_Rect* r);


/* ----- CLIPPING & BOUNDS CHECKING ----- */
	// clips r within oc's bounds. Returns 1 on success and 0 on fail
int nyfw_clipRect(NYFW_Canvas oc, NYFW_Rect* r);		

	// checks if point (x,y) is in canvas's bounds (1 for true, 0 for false)
int nyfw_canvInBounds(NYFW_Canvas oc, int x, int y);


/* ----- DRAWING TO CANVAS ----- */
	// copying (without scaling) from one canvas to another.
	// If srcRect is NULL, we copy all of src to dst. 
	// If dstRect is NULL, all (w/in srcRect) is copied to dst starting at (0,0)
void nyfw_canvasBlit(NYFW_Canvas src, NYFW_Rect* srcRect, NYFW_Canvas dst, NYFW_Rect* dstRect);

	// Fill operations. canvasClear sets all to black -- using memset for speed, if the canvas's original value is 0
	// canvasFill sets the canvas to a specified color
void nyfw_canvasClear(NYFW_Canvas oc);					// sets whole canvas to black (only works with original canvases, subcanvases will be weird)
void nyfw_canvasFill(NYFW_Canvas oc, uint16_t color, NYFW_Rect* r);	// fills w/ specified color 

	// Returns a pointer to the pixel at (x,y), NULL if out of bounds. 
	// A safe version of the CANV_PIXEL macro. 
uint16_t* nyfw_canvasPixel(NYFW_Canvas oc, int x, int y);	



static inline void nyfw_canvSetPixel(NYFW_Canvas oc, int x, int y, uint16_t color)
{
	if (CANV_IN_BOUNDS(oc, x, y)) CANV_PIXEL(oc, x, y) = color;
}

static inline uint16_t nyfw_canvGetPixel(NYFW_Canvas oc, int x, int y)
{
	if (CANV_IN_BOUNDS(oc, x, y)) return CANV_PIXEL(oc, x, y);
	return 0;
}

static inline int nyfw_inRect(NYFW_Rect r, int x, int y)
{
	if (x < r.x || y < r.y) return 0;
	if (r.x + r.w <= x || r.y + r.h <= y) return 0;
	return 1;
}



// scales up a canvas, returning a larger one. Single scale value 
NYFW_Canvas nyfw_canvasScaleUp(NYFW_Canvas oc, int scale);

#endif
