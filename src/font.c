/* FONT RENDERING! */


/* ----- INCLUDES ----- */
#include "NYFW/font.h"
#include <stdio.h>
#include <string.h>



/* ----- DEFINES ----- */
#define GLYPH_WIDTH	8	// bits in glyph width (one uint8_t)
#define GLYPH_HEIGHT	16	// cols in glyph (bytes)
#define GLYPH_COUNT	256	// yeah


/* ----- GLOBALS ----- */
FILE* font;
uint8_t cur_glyph[GLYPH_HEIGHT];

// struct for the header of the psf file
typedef struct {
	uint16_t magic_bytes;	// always 36 04
	uint8_t font_modes;	// flags, 0x02 will be set (doesn't matter)
	uint8_t glyph_size;	// should be 16 (in bytes, this is glyph_height)
} PSF_Header;

PSF_Header ph;


/* ----- INIT/DESTROY ----- */
int nyfw_fontInit(const char* font_path)
{
	font = fopen(font_path, "rb");
	if (font == NULL) {
		perror("Error: font file did not open");
		return 0;
	}

	fread(&ph, sizeof(PSF_Header), 1, font);

	if (ph.magic_bytes != 0x0436) {
		perror("Error: psf magic bytes are incorrect");
		return 0;
	}

	if (ph.font_modes & 1) {
		perror("Error: file has 512 glyphs (want 256)");
		fclose(font);
		return 0;
	}

	if ( !((ph.font_modes >> 1) & 1) ) {
		perror("Error: file has no unicode table");
		fclose(font);
		return 0;
	}

	return 1;
}


void nyfw_fontClose()
{
	fclose(font);
}


/* ----- HELPER ----- */
int load_glyph(char c)
{
	if (c < 32 || c > 255) return 0;	// not a printable character
	
	long int location = ph.glyph_size * c;	// find correct glyph (+4 for header)
	fseek(font, location + 4, SEEK_SET);	
	
	fread(&cur_glyph, sizeof(cur_glyph), 1, font);	// load glyph data into array

	return 1;
}


/* ----- NORMAL SIZE CHAR DRAWING ----- */

// these don't check bounds, I can so I should (later) (!!!!!)

void draw_char(NYFW_Canvas oc, int x, int y, char c, uint16_t color)
{
	if (!load_glyph(c)) return;

	for (int i = 0; i < GLYPH_HEIGHT; i++) {
		int j = GLYPH_WIDTH - 1;
		uint8_t slice = cur_glyph[i];

		while (j >= 0) {
			if (slice & 1) {
				CANV_PIXEL(oc, x+j, y+i) = color;
			}

			j--;
			slice = slice >> 1;
		}
	}
}


void nyfw_fontDrawString(NYFW_Canvas oc, int x, int y, const char* s, uint16_t color) {
	int slen = strlen(s);	

	for (int i = 0; i < slen; i++)
		draw_char(oc, x + GLYPH_WIDTH * i, y, s[i], color);
}


/* ----- SCALED STRING DRAWING ----- */
#define PSF_PIXEL(row, x)	(((row) >> (GLYPH_WIDTH - 1 - (x))) & 1)	// helper macro to find the pixel, instead of shifting each time

void draw_char_scaled(NYFW_Canvas oc, int x, int y, char c, uint16_t color, int scale)
{
	if (!load_glyph(c)) return;

	int sw = GLYPH_WIDTH * scale;	// scaled dimensions
	int sh = GLYPH_HEIGHT * scale;

	float scale_x = (float)GLYPH_WIDTH /  (float)sw;
	float scale_y = (float)GLYPH_HEIGHT / (float)sh;

	for (int j = 0; j < sh; j++) {
		int src_y = j * scale_y;
		uint8_t row = cur_glyph[src_y];
		
		for (int i = 0; i < sw; i++) {
			int src_x = i * scale_x;
			if (PSF_PIXEL(row, src_x))
				CANV_PIXEL(oc, x+i, y+j) = color;
		}
	}

}


void nyfw_fontDrawStringS(NYFW_Canvas oc, int x, int y, const char* s, uint16_t color, int scale)
{
	int slen = strlen(s);
	int jump = GLYPH_WIDTH * scale;

	for (int i = 0; i < slen; i++)
		draw_char_scaled(oc, x + jump * i, y, s[i], color, scale);
	
}















