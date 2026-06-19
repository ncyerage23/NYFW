/*
 * NYMG -- a brand new image format
 *
 * This module will allow users to load files with ".nymg" extensions
 * into canvases, and for users to save canvases to ".nymg" files. 
 *
 */

#ifndef NYFW_NYMG_H
#define NYFW_NYMG_H

/* ----- INCLUDES ----- */
#include "NYFW/canvas.h"


/* ----- FUNCTIONS ----- */

// creates and saves a NYFW_Canvas to a ".nymg" file at the path (1 on success, 0 on fail)
int nyfw_saveNYMG(NYFW_Canvas oc, const char* path);

// loads a ".nymg" file into canvas oc (1 on success, 0 on fail)
int nyfw_loadNYMG(NYFW_Canvas* oc, const char* path);

#endif
