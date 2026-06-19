/* 
 * Colors -- a simple color palette for NYFW
 *
 *
 * I'm gonna use an Enum for now, but later I think I'll use
 * binary files, or something like that. Not sure yet though. 
 *
 * This palette is stolen from PICO-8, because theirs doesn't seem too bad. 
 * I'm not sure these are right, though. 
 *
 */


#ifndef NYFW_COLORS_H
#define NYFW_COLORS_H

// fix this bc it's weird
#define CLEAR		0
#define	BLACK		NYFW_Palette[0]
#define	DBLUE		NYFW_Palette[1]
#define MAROON		NYFW_Palette[2]
#define DGREEN		NYFW_Palette[3]
#define BROWN		NYFW_Palette[4]
#define DBROWN		NYFW_Palette[5]
#define GRAY		NYFW_Palette[6]
#define WHITE		NYFW_Palette[7]
#define RED		NYFW_Palette[8]
#define ORANGE		NYFW_Palette[9]
#define YELLOW		NYFW_Palette[10]
#define LGREEN		NYFW_Palette[11]
#define LBLUE		NYFW_Palette[12]
#define PURPLE		NYFW_Palette[13]
#define PINK		NYFW_Palette[14]
#define FLESH		NYFW_Palette[15]


uint16_t NYFW_Palette[] = {
	0x0000,
	0x216a,
	0x792a,
	0x042a,
	0xaa87,
	0x62aa,
	0xc618,
	0xff9c,
	0xf809,
	0xfd00,
	0xff45,
	0x0707,
	0x2d7f,
	0x83b3,
	0xfbb4,
	0xfe55
};


#endif
