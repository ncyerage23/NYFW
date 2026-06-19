/*
 * Test for Canvas!! 
 *
 * Just testing "blit" right now, but I'll expand this later. 
 * Not gonna even include anything but the canvas module. 
 *
 */

#include "NYFW/canvas.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define W1		25
#define H1		30

#define W2		75
#define H2		50

int main()
{
	uint16_t* p1 = (uint16_t*)malloc(sizeof(uint16_t) * W1 * H1);
	uint16_t* p2 = (uint16_t*)malloc(sizeof(uint16_t) * W2 * H2);
	
	NYFW_Canvas c1 = nyfw_canvas(p1, W1, H1, W1);
	NYFW_Canvas c2 = nyfw_canvas(p2, W2, H2, W2);



	// THIS WORKS!! 
	// I'm just testing the blit, which I fixed because the old one was super weird. 
	// Pretty happy with this, though.
	
	nyfw_canvasClear(c1);
	nyfw_canvasClear(c2);
	nyfw_canvasFill(c1, 0xffff, NULL);

	NYFW_Rect r1 = {5, 5, 10, 5};
	NYFW_Rect r2 = {20, 25, 30, 20};
	
	nyfw_canvasBlit(c1, &r2, c2, NULL);

	uint16_t a = CANV_PIXEL(c1, 5, 5);
	uint16_t b = CANV_PIXEL(c2, 0, 0);
	uint16_t c = CANV_PIXEL(c2, 10, 4);
	uint16_t d = CANV_PIXEL(c2, 4, 4);

	printf("a: %X\nb: %X\nc: %X\nd: %X\n", a, b, c, d);
}
