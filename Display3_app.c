/**
  ****************************************************************************** 
  * @author   eastwood.work
  * @version  /
  * @date     /
  * @brief    Applications lib. to operate pixel by functions, leave alone the 
							lower layer.
  */ 
/* Includes ------------------------------------------------------------------*/

#include "display3_app.h"

extern bool BAT_TWINKLE_FLAG;
unsigned char ClockFontOfTheDay=0;//save a random number of day.define the 

bool TIME_DSP_REFRESH = FALSE;
/* dice/majong font compact code of '0' ~ '9'*/
unsigned char const Num_0[LEN0][2]={{0x00,0x00}};
unsigned char const Num_1[LEN1][2]={{0x04,0x00},{0x20,0x00}};
unsigned char const Num_2[LEN2][2]={{0x42,0x00},{0x60,0x00}};
unsigned char const Num_3[LEN3][2]={{0x21,0x04},{0x46,0x00},{0x62,0x00},
																		{0x70,0x00},{0x22,0x02}};
unsigned char const Num_4[LEN4][2]={{0x66,0x00},{0x62,0x04},{0x47,0x00},
																		{0x62,0x02},{0x62,0x01}};
unsigned char const Num_5[LEN5][2]={{0x66,0x01},{0x72,0x02},{0x46,0x06},
																		{0x22,0x07},{0x47,0x04},{0x64,0x05}};
unsigned char const Num_6[LEN6][2]={{0x67,0x04},{0x66,0x06},{0x76,0x02},
																		{0x27,0x06},{0x56,0x06},{0x66,0x05}};
unsigned char const Num_7[LEN7][2]={{0x27,0x07},{0x76,0x05},{0x67,0x06}};
unsigned char const Num_8[LEN8][2]={{0x77,0x06},{0x67,0x07},{0x76,0x07},
																		{0x57,0x07}};
unsigned char const Num_9[LEN9][2]={{0x77,0x07}};

/*Severing for index. To easy locate a specific number's corresponding code*/
unsigned char const *Num[10]={Num_0[0],Num_1[0],Num_2[0],Num_3[0],Num_4[0],
															Num_5[0],Num_6[0],Num_7[0],Num_8[0],Num_9[0]};
unsigned char const Len[10]={LEN0,LEN1,LEN2,LEN3,LEN4,LEN5,LEN6,LEN7,LEN8,LEN9};
/*Numerial code mainly for Second display*/
unsigned char const Digtal[10][2]={
{0x60,0x60},//0
{0x00,0x7c},//1
{0x74,0x5C},//2
{0x54,0x7c},//3
{0x1C,0x70},//4
{0x5c,0x74},//5
{0x7c,0x70},//6
{0x04,0x7c},//7
{0x7c,0x7c},//8
{0x1c,0x7c},//9
};

extern unsigned char BG[MATRIX_X_LEN];

/**
  * @brief  Lighten the dot @(cooX,cooY)
  * @param  cooX/cooY elements of the coordinate of LED matrix
  * @retval None
	*	@note		
	* @date		2016-01-14
*/	
void lightenDot(unsigned char *layer,unsigned char cooX,unsigned char cooY,FunctionalState _en){
	unsigned char mask;
	mask = 0x80>>cooY;
	
	if(_en){
		*(layer + cooX) |= mask;
	}else{
		*(layer + cooX) &= (~mask);
	}
	
}


/**
  * @brief  Twinkle the dot @(cooX,cooY)
  * @param  cooX/cooY elements of the coordinate of LED matrix
  * @retval None
	*	@note		None
	* @date		2016-01-14 2016-01-15
*/	
void twinkleDot(unsigned char *layer,unsigned char cooX,unsigned char cooY){
	
	if(BAT_TWINKLE_FLAG){
		BAT_TWINKLE_FLAG = FALSE;
		*(layer + cooX) ^= (0x80>>cooY);
		
	}
	
}


/**
  * @brief  Display a numerical number on the whole matrix. (for Second diaplay)
  * @param  tens tens of the Number
						unit	Unit of the Number
						*layer the Layer to diaplay
  * @retval None
	*	@note		None
	* @date		016-01-14 2016-01-15
*/	
void writeDigtal(unsigned char tens,unsigned char unit,unsigned char *layer){
	//static unsigned char t=10,t1=10;
	unsigned char i;
	
	for(i=0;i<2;i++){
		*(layer + 1 + i) = Digtal[tens][i];
		*(layer + 4 + i) = Digtal[unit][i];
	}
	
}

/**
  * @brief  Adjust the display("Trim" the blank Rows/Colums)
  * @param  *decodeArr: the decoded code of dice font. Points to a LEN=4 Array
						ori:  the value	can be any of the Orientation_TypeDef elements.
  * @retval None
	*	@note		each orientation has a different method to adjust.
	* @date		016-01-15
  */
static void numDspAdjust(unsigned char *decodeArr,Orientation_TypeDef ori){
	unsigned char shiftVals;	
	
	shiftVals = getBorder(ori,decodeArr,3);
	shiftArr(ori,decodeArr,3,shiftVals);
}

/**
  * @brief  Get pointer of a number's compact code.
  * @param  _num: specifies the number
						**p: return the pointer.
  * @retval None
	*	@note		The return value is based on getRandomNum() if a number has
						more than one group of compact code.
	* @date		2016-01-14 2016-01-22: Add unsigned char font
  */
static void getNumPointer(unsigned char **p,unsigned char _num,unsigned char font){
	*p = Num[_num] + (font<<1);
	
}
/**
  * @brief  Write a dice font number on a given position.
  * @param  num: the number to be displayed.
						ori: specifies the position on Matrix. Value could be one of the follwing:
									NW, NW corner of Matrix (occupied 3x3 cells)
									NE, NE corner of Matrix (occupied 3x3 cells)
									SW, SW corner of Matrix (occupied 3x3 cells)
									SE, SE corner of Matrix (occupied 3x3 cells)
						dspLayer: specifies the array of Matrix Layer. Value can be:
									BG,
									LAYER0,
									LAYER1,...,etc.
						font:	specifies a group of compact code (2 Bytes), to shift the pointer
						randomFontEnable: Random choosen a font if enabled.
						
  * @retval None
	*	@note		Based on the hardware and led matrix reflection.
	* @date		2016-01-22
  */
void numDsp_Write(unsigned char num,Orientation_TypeDef ori,unsigned char *dspLayer,unsigned char font,FunctionalState randomFontEnable){
	unsigned char *p,clr,shift;
	unsigned char t[4];
	
	if(randomFontEnable){	
		font = getRandomNum(Len[num]);
	}
	getNumPointer(&p,num,font);
	decodeCharacter(t,p);

	numDspAdjust(t,ori);
	
	if((ori & EAST) == EAST){
		dspLayer += MATRIX_X_LEN - 3;
	}
	
	if((ori & NORTH) == NORTH){
		clr = CLR_LOW_NIBBLE;
	}else{
		clr = CLR_HIGH_NIBBLE;
	}
	
	clearArray(dspLayer,3,clr);
	
	mergeArray(dspLayer,t,3,NO_SHIFT);
	
}

/**
  * @brief  Pckage a profile of clock updating.
  * @param  _num1: for NW Corner
						_num2: for NE Corner
						_num3: for SW Corner
						_num4: for SE Corner
  * @retval None
	*	@note		
	* @date		2016-01-22 
						2016-01-29:change function name --> clockDspUpdate
						2016-05-29:change function name --> TimeDspUpdate
						2016-06-01: add randomFontEnable parameter
  */
void TimeDspUpdate(unsigned char _num1,unsigned char _num2,unsigned char _num3,unsigned char _num4,FunctionalState randomFontEnable){
	static unsigned char t1 = 10,t2 = 10,t3 = 10,t4 = 10;
	
	if(TIME_DSP_REFRESH){
		TIME_DSP_REFRESH = FALSE;
		CLR_DSPLayer(DEFAULT_LAYER);
		t1 = t2 = t3 = t4 = 10;		
	}
	
	if(_num1 != t1){
		t1 = _num1;
		numDsp_Write(_num1,NW,BG,ClockFontOfTheDay%Len[_num1],randomFontEnable);
	}
	
	if(_num2 != t2){
		t2 = _num2;
		numDsp_Write(_num2,NE,BG,ClockFontOfTheDay%Len[_num2],randomFontEnable);
	}
	
	if(_num3 != t3){
		t3 = _num3;
		numDsp_Write(_num3,SW,BG,ClockFontOfTheDay%Len[_num3],randomFontEnable);
	}
	
	if(_num4 != t4){
		t4 = _num4;
		numDsp_Write(_num4,SE,BG,ClockFontOfTheDay%Len[_num4],randomFontEnable);
	}
	
}