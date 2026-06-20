
/*
 * NYFW_Input -- reading input for keyboard/mouse
 *
 * TODO: write an explanation lol
 * ALSO TODO: there's some sorta issue with the static struct (this and w/ window), so that's the next
 * 	thing to fix. It works here, but when used as a static library, it does not. So yeah. At least, I think that's the issue. 
 */


/* ----- INCLUDES ----- */
#include "NYFW/input.h"
#include <poll.h>
#include <linux/input.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>


/* ----- INPUT STRUCT(S) ----- */

static NYFW_Key linux_to_nyfw[KEY_MAX + 1];	// converting linux keys to NYFW ones

typedef struct {
	int key_just_pressed[NYFW_KEY_COUNT];
	int key_held[NYFW_KEY_COUNT];
	int key_just_released[NYFW_KEY_COUNT];
} Keyboard;


typedef struct {
	int mb_just_pressed[NYFW_MB_COUNT];
	int mb_held[NYFW_MB_COUNT];
	int mb_just_released[NYFW_MB_COUNT];

	int dx;		// rel x & y of mouse
	int dy;
} Mouse;


static struct {
	int keys_on;	// flags for which devices are used
	int mouse_on;
	
	int keys_fd;	// file descriptors for mouse & keys
	int mouse_fd;

	struct pollfd fds[2];	// for poll, 2 max
	int nfds;

	Keyboard k;	// structs for keyboard & mouse related data
	Mouse m;
} inp;


/* ----- HELPERS ----- */
void clear_mouse()
{
	memset(inp.m.mb_just_pressed, 0, sizeof(inp.m.mb_just_pressed));
	memset(inp.m.mb_just_released, 0, sizeof(inp.m.mb_just_released));

	inp.m.dx = 0;
	inp.m.dy = 0;
}

void clear_keys()
{
	memset(inp.k.key_just_pressed, 0, sizeof(inp.k.key_just_pressed));
	memset(inp.k.key_just_released, 0, sizeof(inp.k.key_just_released));
}

void clear_input_data()
{
	if (inp.keys_on) 	clear_keys();
	if (inp.mouse_on) 	clear_mouse();
}


/* ----- INIT/CLOSE ----- */
int nyfw_inputInit(int flags)
{
	// checking flags
	if (flags == 0) return 0;	// no devices!
	inp.nfds = 0;

	// setting linux_to_nyfw array for later
	memset(linux_to_nyfw, 0, sizeof(linux_to_nyfw));
	linux_to_nyfw[KEY_W] = NYFW_KEY_W;
	linux_to_nyfw[KEY_A] = NYFW_KEY_A;
	linux_to_nyfw[KEY_S] = NYFW_KEY_S;
	linux_to_nyfw[KEY_D] = NYFW_KEY_D;
	
	linux_to_nyfw[KEY_SPACE] = NYFW_KEY_SPACE;
	linux_to_nyfw[KEY_ESC] = NYFW_KEY_ESC;
	
	linux_to_nyfw[KEY_UP] = NYFW_KEY_UP;
	linux_to_nyfw[KEY_DOWN] = NYFW_KEY_DOWN;
	linux_to_nyfw[KEY_LEFT] = NYFW_KEY_LEFT;
	linux_to_nyfw[KEY_RIGHT] = NYFW_KEY_RIGHT;
	
	linux_to_nyfw[KEY_X] = NYFW_KEY_X;
	linux_to_nyfw[KEY_C] = NYFW_KEY_C;

	// opening fds for devices in use
	if (flags & INPUT_KEYS) {
		inp.keys_on = 1;
		inp.keys_fd = open("/dev/input/by-id/usb-Logitech_USB_Keyboard-event-kbd", O_RDONLY | O_NONBLOCK);
		if (inp.keys_fd < 0) {
			printf("Error: keyboard input device not found\n");
			return 0;
		}

		inp.fds[inp.nfds].fd = inp.keys_fd;
		inp.fds[inp.nfds].events = POLLIN;
		inp.fds[inp.nfds].revents = 0;

		inp.nfds++;
	}
	else {
		inp.keys_on = 0;
	}

	if (flags & INPUT_MOUSE) {
		inp.mouse_on = 1;
		inp.mouse_fd = open("/dev/input/by-id/usb-Logitech_USB_Receiver-if02-event-mouse", O_RDONLY | O_NONBLOCK);
		if (inp.mouse_fd < 0) {
			printf("Error: mouse input device not found\n");
			return 0;
		}
		
		inp.fds[inp.nfds].fd = inp.mouse_fd;
		inp.fds[inp.nfds].events = POLLIN;
		inp.fds[inp.nfds].revents = 0;
		
		inp.nfds++;
	}
	else {
		inp.mouse_on = 0;
	}
	
	memset(inp.k.key_held, 0, sizeof(inp.k.key_held));
	memset(inp.m.mb_held, 0, sizeof(inp.m.mb_held));

	return 1;
}


void nyfw_inputClose()
{
	if (inp.mouse_on) close(inp.mouse_fd);
	if (inp.keys_on) close(inp.keys_fd);
}


/* ----- POLLING & READING! ----- */
void handle_event(struct input_event* ev)
{
	if (ev->type == EV_KEY) {
		
		// TODO: this is very poorly organized, so I definitely need to fix it up.
		// I changed it from KEY_MAX to KEY_MICMUTE because MAX has mouse buttons included, so yeah. 
		
		if (ev->code >= KEY_ESC && ev->code <= KEY_MICMUTE) {	// on the keyboard
			
			NYFW_Key k = linux_to_nyfw[ev->code];
			if (k == NYFW_KEY_NONE) return;	

			if (ev->value == 1) {	// key press
				inp.k.key_just_pressed[k] = 1;
				inp.k.key_held[k] = 1;
			} 
			else if (ev->value == 0) {  // key release
				inp.k.key_held[k] = 0;
				inp.k.key_just_released[k] = 1;
			}
		}

		else {
			if (ev->code == BTN_LEFT) {
				if (ev->value == 1) {
					inp.m.mb_just_pressed[0] = 1;
					inp.m.mb_held[0] = 1;
				}
				else if (ev->value == 0) {
					inp.m.mb_just_released[0] = 1;
					inp.m.mb_held[0] = 0;
				}
			}
			else if (ev->code == BTN_RIGHT) {
				if (ev->value == 1) {
					inp.m.mb_just_pressed[1] = 1;
					inp.m.mb_held[1] = 1;
				}
				else if (ev->value == 0) {
					inp.m.mb_just_released[1] = 1;
					inp.m.mb_held[1] = 0;
				}
			}

		}
	}
	else if (ev->type == EV_REL) {
		if (ev->code == REL_X)
			inp.m.dx = ev->value;
		else if (ev->code == REL_Y)
			inp.m.dy = ev->value;
	}
}


void nyfw_inputPoll()
{
	clear_input_data();

	struct input_event ev;
	
	int ret = poll(inp.fds, inp.nfds, 0);
	if (ret <= 0) return;

	for (int i = 0; i < inp.nfds; i++) {
		if (inp.fds[i].revents & POLLIN) {
			while (read(inp.fds[i].fd, &ev, sizeof(ev)) > 0) {
				handle_event(&ev);
			}
		}
	}
}



/* ----- GETTERS ----- */
int nyfw_inputKeyPressed(NYFW_Key k)
{
	return inp.k.key_just_pressed[k];
}

int nyfw_inputKeyHeld(NYFW_Key k)
{
	return inp.k.key_held[k];	
}

int nyfw_inputKeyReleased(NYFW_Key k)
{
	return inp.k.key_just_released[k];
}


int nyfw_inputMBPressed(NYFW_MouseButton m)
{
	return inp.m.mb_just_pressed[m];
}

int nyfw_inputMBHeld(NYFW_MouseButton m)
{
	return inp.m.mb_held[m];
}

int nyfw_inputMBReleased(NYFW_MouseButton m)
{
	return inp.m.mb_just_released[m];
}


int nyfw_inputMouseDX()
{
	return inp.m.dx;	
}

int nyfw_inputMouseDY()
{
	return inp.m.dy;
}




