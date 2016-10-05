/**
 * file: bsp.c
 * date: 5th October 2016
 * author: Jeremie Rapin
 */

#include "BSP/bsp.h"
#include "button.h"
#include "Tools/pwmOutput.h"
#include "driverlib/pin_map.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"

/*!************************************************************************************************
 * \fn         void bsp_Init(void)
 * \brief      Initialize the bsp
 * \param[in]  none
 * \return     void
 ***************************************************************************************************/
void bsp_Init()
{
  //Set the clock
  SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

  //Configure PWM Clock to match system
  SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

  // Enable the peripherals used by this program.
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

  button_Init();

  pwmOutput_Init();
}
