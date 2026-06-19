/*
 * NYFW Input -- managing input devices 
 *
 * This guy will (obviously) track input from keyboard/mouse (and more later)
 * It's pretty simple for now, though I'll need to do some expanding. 
 *
 */


#ifndef NYFW_INPUT_H
#define NYFW_INPUT_H

/* ----- INCLUDES ----- */


/* ----- INPUT ----- */
#define INPUT_KEYS	1
#define INPUT_MOUSE	2

typedef enum {		// all the keys you can read (for now at least)
	NYFW_KEY_NONE,

	NYFW_KEY_W,
	NYFW_KEY_A,
	NYFW_KEY_S,
	NYFW_KEY_D,

	NYFW_KEY_SPACE,
	NYFW_KEY_ESC,

	NYFW_KEY_UP,
	NYFW_KEY_DOWN,
	NYFW_KEY_LEFT,
	NYFW_KEY_RIGHT,

	NYFW_KEY_X,
	NYFW_KEY_C,

	NYFW_KEY_COUNT
} NYFW_Key;

typedef enum {
	NYFW_MB_L,
	NYFW_MB_R,
	NYFW_MB_COUNT
} NYFW_MouseButton;

int nyfw_inputInit(int flags);		// initialize (& close) input data/system
void nyfw_inputClose();

void nyfw_inputPoll();		// read input devices & save what we find

// getters for keyboard and mouse input, return 0/1 (except the last two)
int nyfw_inputKeyPressed(NYFW_Key k);
int nyfw_inputKeyHeld(NYFW_Key k);
int nyfw_inputKeyReleased(NYFW_Key k);

int nyfw_inputMBPressed(NYFW_MouseButton m);
int nyfw_inputMBHeld(NYFW_MouseButton m);
int nyfw_inputMBReleased(NYFW_MouseButton m);

int nyfw_inputMouseDX();
int nyfw_inputMouseDY();


#endif
