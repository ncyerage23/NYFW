/*
 * NYFW Window -- giving the user a screen to play with
 *
 * This just sets the system to the correct mode for an application,
 * and gives the user some hooks for drawing and whatnot. It's the 
 * main app part, though it'll probably be hidden in nyfw.h.
 *
 */


#ifndef NYFW_WINDOW_H
#define NYFW_WINDOW_H

/* ----- INCLUDES ----- */
#include "NYFW/canvas.h"


/* ----- WINDOW ----- */
int nyfw_windowInit();	// initializer (and closer) for window system
void nyfw_windowClose();

NYFW_Canvas nyfw_getWindowCanvas();	// returns pointer to canvas struct representing the window's framebuffer (and relating data) (well, it's actually the backbuffer lol)

void nyfw_windowPresent();	
// updates the screen framebuffer with a virtual backbuffer




#endif
