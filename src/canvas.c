
/*
 * NYFW_Canvas -- some fun functions for using canvases
 *
 * This is NOT a drawing library or anything like that. It's simply a definition of
 * a canvas (in the header) and some necessary helper functions for it. Drawing/gfx
 * will come later, and elsewhere. 
 * 
 *
 * DEV NOTES: I think this works, but I can't test it until I do the window
 * stuff. Also, I need to finish the header (I didn't include anything lol). 
 * But, it will probably work. Who knows?
 *
 */  


/* ----- INCLUDES ----- */
#include "NYFW/canvas.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define CMIN(x, y)	(x < y) ? x : y
#define CMAX(x, y)	(x < y) ? y : x


/* ----- FUNCTIONS! ----- */

NYFW_Canvas nyfw_canvas(uint16_t* pixels, size_t width, size_t height, size_t stride)
{
	NYFW_Canvas out = {
		.pixels = pixels,
		.width = width,
		.height = height,
		.stride = stride,
		.original = 0,
	};
	return out;
}


int nyfw_clipRect(NYFW_Canvas oc, NYFW_Rect* r)
{
	if (oc.pixels == NULL) return 0;
	if (r->w <= 0 || r->h <= 0) return 0;

	r->x = CMAX(0, r->x);
	r->y = CMAX(0, r->y);
	r->w = (r->x + r->w < oc.width) ? r->w : oc.width - r->x;
	r->h = (r->y + r->h < oc.height) ? r->h : oc.height - r->y;
	
	return 1;
}


int nyfw_canvInBounds(NYFW_Canvas oc, int x, int y)
{
	if (oc.pixels == NULL) 			return 0;
	if (x < 0 || y < 0) 			return 0;
	if (x >= oc.width || y >= oc.height)	return 0;
	return 1;
}


NYFW_Canvas nyfw_subcanvas(NYFW_Canvas oc, NYFW_Rect* r)
{
	NYFW_Rect nr = *r;

	if (!nyfw_clipRect(oc, &nr)) return CANV_NULL;

	oc.pixels = &CANV_PIXEL(oc, nr.x, nr.y);
	oc.width = nr.w;
	oc.height = nr.h;
	oc.original = 1;	// subcanvases have 1 here, og canvases have 0
	return oc;
}


void nyfw_canvasBlit(NYFW_Canvas src, NYFW_Rect* srcRect, NYFW_Canvas dst, NYFW_Rect* dstRect)
{
	/* ----- BOUNDS FOR COPYING ----- */
	int sx, sy, dx, dy;	// start pixel coords for source and destination canvases
	int w, h;		// width and height of space being copied
	int sw, sh, dw, dh;
		
	if (srcRect == NULL) {
		sx = 0;
		sy = 0;
		sw = src.width;
		sh = src.height;
	} else {
		NYFW_Rect nr = *srcRect;
		if (!nyfw_clipRect(src, &nr)) return;
		sx = nr.x;
		sy = nr.y;
		sw = nr.w;
		sh = nr.h;
	}

	if (dstRect == NULL) {
		dx = 0;
		dy = 0;
		dw = dst.width;
		dh = dst.height;
	} else {
		NYFW_Rect nr = *dstRect;
		if (!nyfw_clipRect(dst, &nr)) return;
		dx = nr.x;
		dy = nr.y;
		dw = nr.w;
		dh = nr.h;
	}

	w = CMIN(sw, dw);
	h = CMIN(sh, dh);

	/* ----- COPYING THE CANVAS ----- */
	for (int x = 0; x < w; x++)
		for (int y = 0; y < h; y++)
			CANV_PIXEL(dst, x+dx, y+dy) = CANV_PIXEL(src, x+sx, y+sy);
}


void nyfw_canvasClear(NYFW_Canvas oc)
{
	memset(oc.pixels, 0, 2 * oc.height * oc.stride);
}


void nyfw_canvasFill(NYFW_Canvas oc, uint16_t color, NYFW_Rect* r)
{
	int x, y, w, h;

	if (r == NULL) {
		x = 0;
		y = 0;
		w = oc.width;
		h = oc.height;
	} else {
		NYFW_Rect nr = *r;
		nyfw_clipRect(oc, &nr);
		x = nr.x;
		y = nr.y;
		w = nr.w;
		h = nr.h;
	}

	for (int j = 0; j < h; j++)
		for (int i = 0; i < w; i++)
			CANV_PIXEL(oc, x+i, y+j) = color;
}



uint16_t* nyfw_canvasPixel(NYFW_Canvas oc, int x, int y)
{
	if (nyfw_canvInBounds(oc, x, y)) return &CANV_PIXEL(oc, x, y);
	return NULL;
}


NYFW_Canvas nyfw_canvasScaleUp(NYFW_Canvas oc, int scale)
{
	NYFW_Canvas out;
	int out_width = oc.width * scale;
	int out_height = oc.height * scale;
	uint16_t* out_pixels = malloc(sizeof(uint16_t) * out_width * out_height);
	if (!out_pixels) return CANV_NULL;

	out = nyfw_canvas(out_pixels, out_width, out_height, out_width);
		
	for (int i = 0; i < out.width; i++)
		for (int j = 0; j < out.height; j++)
			nyfw_canvSetPixel(out, i, j, nyfw_canvGetPixel(oc, i/scale, j/scale));

	return out;
}





