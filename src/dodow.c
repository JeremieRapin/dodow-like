/**
 * file: dodow.c
 * date: 5th October 2016
 * author: Jeremie Rapin
 */

#include "dodow.h"
#include "button.h"
#include "Tools/save.h"
#include "Tools/pwmOutput.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

typedef enum
{
  DODOW_EVENT_INCREASE,
  DODOW_EVENT_STEP,
  DODOW_EVENT_DECREASE,
  DODOW_EVENT_STOP,
} eDodowEvent_t;

typedef enum
{
  DODOW_ASYNC_CHANGE_COLOR,
  DODOW_ASYNC_START_STOP,
} eDodowAsync_t;

typedef struct
{
  eColor_t color;
  int step;
  int inc;
  int maxInc;
  int power;
  eDodowEvent_t current;
  TimerHandle_t timer;
  TaskHandle_t taskId;
  QueueHandle_t asyncQueue;
} sDodow_t;

sDodow_t sDodow;
#define this (&sDodow)

#define dodowQUEUE_SIZE (10)

#define TIMER_DURATION  8
#define STEP_COUNTER    (4 * TIMER_DURATION) // A step occurs each 15 seconds. So 4 step by minutes

#define BASE_TIME               130
#define ONE_SECOND_IN_BASE_TIME (BASE_TIME * 10)
#define INCREASE_STEP           (4 * ONE_SECOND_IN_BASE_TIME) / 100
#define REST_STEP               (3 * ONE_SECOND_IN_BASE_TIME) / 100
#define DECREASE_STEP           (8 * ONE_SECOND_IN_BASE_TIME) / 100

/* Declare private function */
static void dodow_ChangeColor();
static void dodow_StartStop();

static void dodow_ChangeColorButton();
static void dodow_StartStopButton();

static void dodow_Start();
static void dodow_Stop();

static void dodow_Task();
static void dodow_AsyncFromISR(eDodowAsync_t event);

static void dodow_BaseTime(TimerHandle_t pxTimer);

/*!************************************************************************************************
 * \fn         void dodow_Init()
 * \brief      init dodow object
 * \param[in]  none
 * \return     void
 ***************************************************************************************************/
void dodow_Init()
{
  save_Init();
  button_Init();

  button_AddCallback(BUTTON_SW1, dodow_ChangeColorButton);
  button_AddCallback(BUTTON_SW2, dodow_StartStopButton);

  this->color = save_GetColor();

  /* Create the event action queue */
  this->asyncQueue = xQueueCreate(dodowQUEUE_SIZE, sizeof(eDodowAsync_t));

  /* fork on an independent thread */
  xTaskCreate(dodow_Task, "dodow", dodow_TASK_SIZE, 0, dodow_TASK_PRIORITY, &(this->taskId));

  this->timer = xTimerCreate("dodowBaseTime", BASE_TIME, pdTRUE, NULL, dodow_BaseTime);
  xTimerStart(this->timer, 0);

  this->inc     = 0;
  this->maxInc  = INCREASE_STEP;
  this->current = DODOW_EVENT_INCREASE;
}

/*!************************************************************************************************
 * \fn         void dodow_Cleanup()
 * \brief      cleanup dodow object
 * \param[in]  none
 * \return     void
 ***************************************************************************************************/
void dodow_Cleanup()
{

}

/*!************************************************************************************************
 * \fn         void dodow_Task()
 * \brief      main task
 * \param[in]  none
 * \return     void
 ***************************************************************************************************/
static void dodow_Task()
{
  eDodowAsync_t eAsync;

  /* run task */
  for (;;)
  {
    /* Wait until there is something to do. */
    xQueueReceive(this->asyncQueue, &eAsync, portMAX_DELAY);

    /* Perform a different action for each event type. */
    switch (eAsync)
    {
      case DODOW_ASYNC_CHANGE_COLOR:
        dodow_ChangeColor();
      break;

      case DODOW_ASYNC_START_STOP:
        dodow_StartStop();
      break;

      default:
      break;
    }
  }
}

/*!************************************************************************************************
 * \fn         void dodow_BaseTime()
 * \brief      BaseTime of dodow
 * \param[in]  none
 * \return     void
 ***************************************************************************************************/
static void dodow_BaseTime(TimerHandle_t pxTimer)
{
  /* Handle tick between each steps */
  this->inc++;
  if(this->inc > this->maxInc)
  {
    this->current++;
    if(this->current >= DODOW_EVENT_STOP)
    {
      if (this->step > STEP_COUNTER)
      {
        this->current = DODOW_EVENT_STOP;
      }
      else
      {
        this->current = DODOW_EVENT_INCREASE;
      }
      this->step++;
    }
    this->inc = 1;
  }

  /* Handle action to do on step */
  switch (this->current)
  {
    case DODOW_EVENT_INCREASE:
      this->maxInc = INCREASE_STEP;
      pwmOutput_Set(this->color, this->power++);
    break;

    case DODOW_EVENT_STEP:
      this->maxInc = REST_STEP;

      if((this->inc == 1) || (this->inc == (this->maxInc - 1)))
      {
        pwmOutput_Set(this->color, (this->power / 2));
        this->power = DECREASE_STEP;
      }
      else
      {
        this->power++;
        pwmOutput_Set(this->color, this->power);
      }
    break;

    case DODOW_EVENT_DECREASE:
      this->maxInc = DECREASE_STEP;
      pwmOutput_Set(this->color, this->power--);
    break;

    case DODOW_EVENT_STOP:
      dodow_Stop();
      break;

    default:
      break;
  }
}

/*!************************************************************************************************
 * \fn         static void dodow_AsyncFromISR(eDodowAsync_t event)
 * \brief      event from isr
 * \param[in]  event
 * \return     void
 ***************************************************************************************************/
static void dodow_AsyncFromISR(eDodowAsync_t event)
{
  xQueueSendFromISR(this->asyncQueue, &event, 0);
}

/*!************************************************************************************************
 * \fn         static void dodow_Start()
 * \brief      start application
 * \param[in]  none
 * \return     void
 ***************************************************************************************************/
static void dodow_Start()
{
  xTimerStart(this->timer, 0);
  this->current = DODOW_EVENT_INCREASE;
  this->inc = 0;
  this->power = 0;
  this->maxInc = INCREASE_STEP;
  this->step = 0;
  pwmOutput_Start();
}

/*!************************************************************************************************
 * \fn         static void dodow_Stop()
 * \brief      stop application
 * \param[in]  none
 * \return     void
 ***************************************************************************************************/
static void dodow_Stop()
{
  xTimerStop(this->timer, 0);
  this->current = DODOW_EVENT_STOP;
  pwmOutput_Stop();
}

/*!************************************************************************************************
 * \fn         static void dodow_ChangeColorButton()
 * \brief      change color request from button
 * \param[in]  none
 * \return     void
 ***************************************************************************************************/
static void dodow_ChangeColorButton()
{
  dodow_AsyncFromISR(DODOW_ASYNC_CHANGE_COLOR);
}

/*!************************************************************************************************
 * \fn         static void dodow_StartStopButton()
 * \brief      start/stop request from button
 * \param[in]  none
 * \return     void
 ***************************************************************************************************/
static void dodow_StartStopButton()
{
  dodow_AsyncFromISR(DODOW_ASYNC_START_STOP);
}

/*!************************************************************************************************
 * \fn         static void dodow_ChangeColor()
 * \brief      change color
 * \param[in]  none
 * \return     void
 ***************************************************************************************************/
static void dodow_ChangeColor()
{
  if (this->color < COLOR_END)
  {
    this->color++;
  }
  else
  {
    this->color = COLOR_CLEAR_BLUE;
  }
  save_SetColor(this->color);
}

/*!************************************************************************************************
 * \fn         static void dodow_StartStop()
 * \brief      start stop application
 * \param[in]  none
 * \return     void
 ***************************************************************************************************/
static void dodow_StartStop()
{
  if(this->current == DODOW_EVENT_STOP)
  {
    dodow_Start();
  }
  else
  {
    dodow_Stop();
  }
}
