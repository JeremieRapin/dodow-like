/**
 * file: button.c
 * date: 5th October 2016
 * author: Jeremie Rapin
 */

#include "button.h"
#include "string.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"

typedef struct
{
  pfctButtonCallback_t callbacks[BUTTON_COUNTER];
}sButton_t;

sButton_t sButton;
#define this (&sButton)

static void button_interrupt();

static void button_sw1Pressed();

static void button_sw2Pressed();

/*!************************************************************************************************
 * \fn         void button_Init()
 * \brief      init button object
 * \param[in]  none
 * \return     void
 ***************************************************************************************************/
void button_Init()
{
  memset(this->callbacks, 0, (sizeof(pfctButtonCallback_t) * BUTTON_COUNTER));

  //PORTF GPIO_PIN_0 is locked due to JTAG use. The following unlocks the same.
  HWREG(GPIO_PORTF_BASE + 0x520) = 0x4C4F434B;
  HWREG(GPIO_PORTF_BASE + 0x524)|= 0x01;

  // Configure PF4 as input
  GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, (GPIO_PIN_0 | GPIO_PIN_4));
  // Enable weak pullup resistor for PF4
  GPIOPadConfigSet(GPIO_PORTF_BASE, (GPIO_PIN_0 | GPIO_PIN_4), GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

  // Disable interrupt for PF4 (in case it was enabled)
  GPIOPinIntDisable(GPIO_PORTF_BASE, (GPIO_PIN_0 | GPIO_PIN_4));
  // Clear pending interrupts for PF4
  GPIOPinIntClear(GPIO_PORTF_BASE, (GPIO_PIN_0 | GPIO_PIN_4));
  // Register our handler function for port F
  GPIOPortIntRegister(GPIO_PORTF_BASE, button_interrupt);
  // Configure PF4 for falling edge trigger
  GPIOIntTypeSet(GPIO_PORTF_BASE, (GPIO_PIN_0 | GPIO_PIN_4), GPIO_RISING_EDGE);
  // Enable interrupt for PF4
  GPIOPinIntEnable(GPIO_PORTF_BASE, (GPIO_PIN_0 | GPIO_PIN_4));
}

/*!************************************************************************************************
 * \fn         void button_Cleanup()
 * \brief      cleanup button object
 * \param[in]  none
 * \return     void
 ***************************************************************************************************/
void button_Cleanup()
{

}

/*!************************************************************************************************
 * \fn         void button_AddCallback(eButton_t id, pfctButtonCallback_t callback)
 * \brief      Add callback for a button
 * \param[in]  none
 * \return     void
 ***************************************************************************************************/
void button_AddCallback(eButton_t id, pfctButtonCallback_t callback)
{
  if(id < BUTTON_COUNTER)
  {
    this->callbacks[id] = callback;
  }
}

/*!************************************************************************************************
 * \fn         void button_interrupt(void)
 * \brief      Interrupt
 * \param[in]  none
 * \return     void
 ***************************************************************************************************/
 static void button_interrupt()
 {
   int value = GPIOPinIntStatus(GPIO_PORTF_BASE, false);

   if (value & GPIO_PIN_0)
   {
     GPIOPinIntClear(GPIO_PORTF_BASE, GPIO_PIN_0);
     button_sw2Pressed();
   }

   if (value & GPIO_PIN_4)
   {
     GPIOPinIntClear(GPIO_PORTF_BASE, GPIO_PIN_4);
     button_sw1Pressed();
   }
 }

 /*!************************************************************************************************
  * \fn         void button_sw1Pressed()
  * \brief      action on sw1
  * \param[in]  none
  * \return     void
  ***************************************************************************************************/
 static void button_sw1Pressed()
 {
   if(this->callbacks[BUTTON_SW1])
   {
     this->callbacks[BUTTON_SW1]();
   }
 }

/*!************************************************************************************************
 * \fn         void button_sw2Pressed()
 * \brief      action on sw2
 * \param[in]  none
 * \return     void
 ***************************************************************************************************/
 static void button_sw2Pressed()
 {
   if(this->callbacks[BUTTON_SW2])
   {
     this->callbacks[BUTTON_SW2]();
   }
 }
