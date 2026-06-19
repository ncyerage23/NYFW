
/* 
 * TEST EVERYTHING FOR NYFW!!
 * 
 * Here's the compile command 
 * 	gcc tests/main.c src/nyfw_window.c src/nyfw_canvas.c src/nyfw_input.c -Iinclude -o test
 *
 * I had a weird issue with clearing, but I fixed it. Memset does things in bytes. duh. 
 *
 */

#include "NYFW/nyfw.h"
#include <unistd.h>


// lil player
int px = 100;
int py = 100;
int pw = 50;

// mouse!
int mx = 10;
int my = 10;


int main()
{
	if (!nyfw_windowInit()) return 0;
	if (!nyfw_inputInit(INPUT_MOUSE | INPUT_KEYS)) return 0;
	if (!init_font()) return 0;

	NYFW_Canvas test = nyfw_getWindowCanvas();
	nyfw_canvasClear(test);
	
	while (1) {
		nyfw_inputPoll();
		
		// update
		if (nyfw_inputKeyPressed(NYFW_KEY_ESC)) break;

		if (nyfw_inputKeyHeld(NYFW_KEY_UP)) py -= 5;
		if (nyfw_inputKeyHeld(NYFW_KEY_DOWN)) py += 5;
		if (nyfw_inputKeyHeld(NYFW_KEY_LEFT)) px -= 5;
		if (nyfw_inputKeyHeld(NYFW_KEY_RIGHT)) px += 5;

		mx += nyfw_inputMouseDX();
		my += nyfw_inputMouseDY();

		// draw
		nyfw_canvasClear(test);
		draw_string(test, 50, 50, "hello?", 0xffff);
		
		for (int i = px; i < px + pw; i++)
			for (int j = py; j < py + pw; j++)
				nyfw_canvSetPixel(test, i, j, 0xf800);
			
		nyfw_canvSetPixel(test, mx, my, 0xffff);

		// present
		nyfw_windowPresent();
		//usleep(50000);
	}

	
	close_font();
	nyfw_inputClose();
	nyfw_windowClose();
	return 1;
}

