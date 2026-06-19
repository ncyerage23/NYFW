/*
 * NYMG -- an image format
 *
 * Yeah, just definitions of the functions in nymg.h.
 * They're not quite perfect, but they should be enough for now. 
 *
 */


/* ----- INCLUDES ----- */
#include "NYFW/nymg.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>


/* ----- NYMG HEADER STRUCT ----- */
typedef struct {		// 12 bytes
	char header[2];		// 'NY'
	uint8_t version;	// should be "1"
	uint8_t type;		// 0: straight pixel data (16 bpp), 1: palette pixel data (8 bits per pixel, referring to NYFW_Palette array)
	uint32_t width;		// dimensions
	uint32_t height;
} NYMG_Header;


/* ----- FUNCTIONS ----- */
int nyfw_saveNYMG(NYFW_Canvas oc, const char* path)
{
	// opening the file to be saved.
	FILE* fp = fopen(path, "wb");	 
	if (!fp) {
		printf("fopen failed\n");
		return 0;
	}
	
	// creating the file header
	NYMG_Header out = {
		.header = {'N', 'Y'},
		.version = 1,
		.type = 0,
		.width = oc.width,
		.height = oc.height
	};
	
	fwrite(&out, sizeof(out), 1, fp);	// writing to the file

	// saving the pixel data
	if (!oc.original) {	// means there's no padding, can copy directly to file
		fwrite(
			oc.pixels,
			sizeof(uint16_t),
			oc.width * oc.height,
			fp
		);
	}
	else {			// means it's a subcanvas, need to copy by row
		for (uint32_t y = 0; y < oc.height; y++) {
			fwrite(
				oc.pixels + y * oc.stride,
				sizeof(uint16_t),
				oc.width,
				fp
			);
		}
	}

	fclose(fp);
	return 1;

}


int nyfw_loadNYMG(NYFW_Canvas* oc, const char* path)
{
	// opening the file
	FILE* fp = fopen(path, "rb");
	if (!fp) {
		printf("failed to open file\n");
		return 0;
	}

	// reading the header
	NYMG_Header head;
	if (!fread(&head, sizeof(head), 1, fp)) {
		printf("failed to read header\n");
		fclose(fp);
		return 0;
	}
	
	// checking if the values are right
	int valid = 1;
	if ( !(head.header[0] == 'N' && head.header[1] == 'Y') ) { printf("header incorrect\n"); valid = 0; }
	if ( head.version != 1 ) { printf("version incorrect: %d\n", head.version); valid = 0; }
	if ( head.type != 0 ) { printf("invalid nymg type\n"); valid = 0; }

	if (!valid) {
		printf("header values invalid. Shutting down.\n");
		fclose(fp);
		return 0;
	}
	
	// preparing the canvas
	size_t p_count = head.width * head.height;
	uint16_t* pixels = malloc(p_count * sizeof(uint16_t));
	if (!pixels) {
		printf("pixel allocation failed\n");
		fclose(fp);
		return 0;
	}

	fread(
		pixels,
		sizeof(uint16_t),
		p_count,
		fp
	);

	// constructing the canvas
	oc->width = head.width;
	oc->height = head.height;
	oc->pixels = pixels;
	oc->stride = head.width;
	oc->original = 0;

	// closing up shop
	fclose(fp);
	return 1;
}

