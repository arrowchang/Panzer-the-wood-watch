#ifndef __DISPLAY2_MAPPING_H
#define __DISPLAY2_MAPPING_H

#include "Display1_Hardware.h"
#include "myConfig.h"
#include "assistant.h"

typedef enum{
	NORTH = (unsigned char)0x01,
	SOUTH = (unsigned char)0x02,
	WEST = (unsigned char)0x10,
	EAST = (unsigned char)0x20,
	
	NW = NORTH | WEST,
	NE = NORTH | EAST,
	SW = SOUTH | WEST,
	SE = SOUTH | EAST,
	
	N_S = NORTH | SOUTH,//cannot be parameter,only for logic
	W_E = WEST | EAST//cannot be parameter,only for logic
	
}Orientation_TypeDef;

/*
LSB
	|
	|		CooY
	|   ^_ _ _ _
	|		|_|_|_|_| (LED Matrix) 
	|	  |_|_|_|_|
	|   |_|_|_|_|
MSB   |_|_|_|_| >CooX
	    o
BG[i]	BG[0] -> BG[X_LEN-1]

*/

typedef struct {
	unsigned char Decoded[4];
	unsigned char NullEast;
	
	signed char CooX;
	signed char CooY;
	
}liveElement;

void decodeCharacter(unsigned char *decodeArr,unsigned char *compactCode);
void generateLayer(unsigned char *layer,liveElement* ele);

unsigned char  getBorder(Orientation_TypeDef ori,unsigned char *decodeArr,unsigned char arrLen);
void shiftArr(Orientation_TypeDef ori,unsigned char *arr,unsigned char arrLen,unsigned char shiftVals);

unsigned char getRandomNum(unsigned char _range);



#endif


