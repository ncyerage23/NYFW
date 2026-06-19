
/*
 * NYFW_Window -- stuff for opening and working w/ the pi's framebuffer
 *
 * This is pretty simple, just accessing the file in dev/fb0 and mapping its
 * memory (using it as a NYFW_Canvas, defined elsewhere) for simplicity and 
 * whatnot.
 *
 */


/* ----- INCLUDES ----- */
#include "NYFW/window.h"
#include <linux/kd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* ----- WINDOW STRUCT ----- */
static struct {
	int fb;			// file descriptor for the framebuffer
	
	NYFW_Canvas fb_canvas;	// canvas for framebuffer
	NYFW_Canvas bb_canvas;	// same for backbuffer
	
	uint32_t screensize;

	struct termios raw;	// for terminal mode switching
	struct termios og;
} win;

/* 
 * this is the struct where we'll hold all of the data for the window file.
 * Later, I could have the window_init thing return a similar struct, but since
 * there would be no reason to open another window (it wouldn't really be possible
 * anyway) this just makes a lot more sense. 
 */


// maybe getter/setter? idk


/* ----- FUNCTIONS! ----- */
int nyfw_windowInit()
{
	// opening the buffer
	win.fb = open("/dev/fb0", O_RDWR);
	if (win.fb < 0) {
		printf("Error: fb didn't open\n");
		return 0;
	}
	

	// get fixed & variable screen info
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	
	ioctl(win.fb, FBIOGET_FSCREENINFO, &finfo);
	ioctl(win.fb, FBIOGET_VSCREENINFO, &vinfo);


	// set vars & stuff
	int width 		= vinfo.xres;
	int height 		= vinfo.yres;
	uint32_t bpp 		= vinfo.bits_per_pixel / 8;
	uint32_t line_length 	= finfo.line_length;
	size_t stride 		= line_length / sizeof(uint16_t);
	win.screensize 		= line_length * height;


	// map and allocate memory for fb and bb 
	uint16_t* fb_pixels = (uint16_t*)mmap(0, win.screensize, PROT_READ | PROT_WRITE, MAP_SHARED, win.fb, 0);
	if ((intptr_t)fb_pixels == -1) {
		printf("mmap failed :(\n");
		return 0;
	}

	uint16_t* bb_pixels = (uint16_t*)malloc(win.screensize);
	if (bb_pixels == NULL) {
		printf("backbuffer allocation failed :(\n");
		return 0;
	}


	// creating the canvases!
	win.fb_canvas = nyfw_canvas(fb_pixels, width, height, stride);
	win.bb_canvas = nyfw_canvas(bb_pixels, width, height, stride);


	// set to graphics mode
	ioctl(0, KDSETMODE, KD_GRAPHICS);
	
	// switch to raw terminal mode
	tcgetattr(STDIN_FILENO, &win.og);		// saving original terminal mode
	
	win.raw = win.og;
	cfmakeraw(&win.raw);				// switch to raw terminal mode
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &win.raw);	// set it

	return 1;
}


void nyfw_windowClose()
{
	ioctl(0, KDSETMODE, KD_TEXT);			// go back to text mode
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &win.og);	// reset to original terminal mode
	
	munmap(win.fb_canvas.pixels, win.screensize);	// freeing the buffers
	free(win.bb_canvas.pixels);		
	
	close(win.fb);
}


NYFW_Canvas nyfw_getWindowCanvas()
{
	return win.bb_canvas;
}


void nyfw_windowPresent()
{
	// nyfw_canvasBlit(win.bb_canvas, NULL, win.fb_canvas, NULL);
	// this ^ is slow asf unfortunately, so I'll do it w/ memcpy here for now
	
	memcpy(
		win.fb_canvas.pixels,
		win.bb_canvas.pixels,
		win.screensize
	);
}





