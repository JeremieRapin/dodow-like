/**
 * file: button.h
 * date: 5th October 2016
 * author: Jeremie Rapin
 */

#ifndef _BUTTON_H_
  #define _BUTTON_H_

typedef enum
{
  BUTTON_SW1,
  BUTTON_SW2,
  BUTTON_COUNTER,
}eButton_t;

typedef void (*pfctButtonCallback_t)();

void button_Init();
void button_Cleanup();

void button_AddCallback(eButton_t id, pfctButtonCallback_t callback);

#endif // _BUTTON_H_
