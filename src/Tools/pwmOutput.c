/**
 * file: dodow.c
 * date: 5th October 2016
 * author: Jeremie Rapin
 */

#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "Tools/pwmOutput.h"

/* Define for PWM */
#define GPIO_PF1_M1PWM5         0x00050405
#define GPIO_PF2_M1PWM6         0x00050805
#define GPIO_PF3_M1PWM7         0x00050C05

#define LOW_VALUE       1

/*!************************************************************************************************
 * \fn         void pwmOutput_Init()
 * \brief      init pwm Output object
 * \param[in]  none
 * \return     void
 ***************************************************************************************************/
void pwmOutput_Init()
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);  //The Tiva Launchpad has two modules (0 and 1). Module 1 covers the LED pins

  //Configure PF1,PF2,PF3 Pins as PWM
  GPIOPinConfigure(GPIO_PF1_M1PWM5);
  GPIOPinConfigure(GPIO_PF2_M1PWM6);
  GPIOPinConfigure(GPIO_PF3_M1PWM7);
  GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

  //Configure PWM Options
  //PWM_GEN_2 Covers M1PWM4 and M1PWM5
  //PWM_GEN_3 Covers M1PWM6 and M1PWM7 See page 207 4/11/13 DriverLib doc
  PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
  PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

  //Set the Period (expressed in clock ticks)
  PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, 320);
  PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, 320);

  //Set PWM duty-50% (Period /2)
  PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 1);
  PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 1);
  PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 1);

  // Enable the PWM generator
  PWMGenEnable(PWM1_BASE, PWM_GEN_2);
  PWMGenEnable(PWM1_BASE, PWM_GEN_3);

  // Turn on the Output pins
  pwmOutput_Start();
}

/*!************************************************************************************************
 * \fn         void pwmOutput_Cleanup()
 * \brief      cleanup pwm Output object
 * \param[in]  none
 * \return     void
 ***************************************************************************************************/
void pwmOutput_Cleanup()
{

}

/*!************************************************************************************************
 * \fn         void pwmOutput_Stop()
 * \brief      clear all output
 * \param[in]  eColor_t
 * \param[in]  int
 * \return     void
 ***************************************************************************************************/
void pwmOutput_Stop()
{
  PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT | PWM_OUT_6_BIT | PWM_OUT_7_BIT, false);
}

/*!************************************************************************************************
 * \fn         void pwmOutput_Start()
 * \brief      set all output
 * \param[in]  eColor_t
 * \param[in]  int
 * \return     void
 ***************************************************************************************************/
void pwmOutput_Start()
{
  pwmOutput_Set(COLOR_WHITE, 1);
  PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT | PWM_OUT_6_BIT | PWM_OUT_7_BIT, true);
}

/*!************************************************************************************************
 * \fn         void pwmOutput_Set(eColor_t color, int value)
 * \brief      cleanup pwm Output object
 * \param[in]  eColor_t
 * \param[in]  int
 * \return     void
 ***************************************************************************************************/
void pwmOutput_Set(eColor_t color, int value)
{
  switch (color)
  {
    case (COLOR_CLEAR_BLUE):
    {
      PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, LOW_VALUE);
      PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, value);
      PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, value);
    }
    break;

    case (COLOR_YELLOW):
    {
      PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, value);
      PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, LOW_VALUE);
      PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, value);
    }
    break;

    case (COLOR_ROSE):
    {
      PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, value);
      PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, value);
      PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, LOW_VALUE);
    }
    break;

    case (COLOR_RED):
    {
      PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, value);
      PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, LOW_VALUE);
      PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, LOW_VALUE);
    }
    break;

    case (COLOR_BLUE):
    {
      PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, LOW_VALUE);
      PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, value);
      PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, LOW_VALUE);
    }
    break;

    case (COLOR_GREEN):
    {
      PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, LOW_VALUE);
      PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, LOW_VALUE);
      PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, value);
    }
    break;

    default:
    case (COLOR_WHITE):
    {
      PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, value);
      PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, value);
      PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, value);
    }
    break;
  }
}
