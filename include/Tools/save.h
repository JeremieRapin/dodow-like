/**
 * file: save.h
 * date: 5th October 2016
 * author: Jeremie Rapin
 */

 #ifndef _SAVE_H_
   #define _SAVE_H_

#include "Object/color.h"
#include <stdint.h>

#define COLOR_OFFSET 0
#define COLOR_SIZE   4

void save_Init();
void save_Cleanup();

void save_SetColor(eColor_t color);
eColor_t save_GetColor();

#endif // _SAVE_H_
