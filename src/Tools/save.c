/**
 * file: save.c
 * date: 5th October 2016
 * author: Jeremie Rapin
 */

#include "Tools/save.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/eeprom.h"

/*!************************************************************************************************
 * \fn         void save_Init(void)
 * \brief      Initialize the save object
 * \param[in]  none
 * \return     void
 ***************************************************************************************************/
void save_Init()
{
  SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
  EEPROMInit();
}

/*!************************************************************************************************
 * \fn         void save_Cleanup(void)
 * \brief      Cleanup the save object
 * \param[in]  none
 * \return     void
 ***************************************************************************************************/
void save_Cleanup()
{

}

/*!************************************************************************************************
 * \fn         void save_SetColor(eColor_t color)
 * \brief      Save a new color
 * \param[in]  none
 * \return     void
 ***************************************************************************************************/
void save_SetColor(eColor_t color)
{
  EEPROMProgram((unsigned long *)&color, COLOR_OFFSET, COLOR_SIZE);
}

/*!************************************************************************************************
 * \fn         eColor_t save_GetColor()
 * \brief      Get the current color saved
 * \param[in]  none
 * \return     void
 ***************************************************************************************************/
eColor_t save_GetColor()
{
  unsigned long color;

  EEPROMRead(&color, COLOR_OFFSET, COLOR_SIZE);

  return (eColor_t)color;
}
