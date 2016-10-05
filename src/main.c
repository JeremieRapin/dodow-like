#include "FreeRTOS.h"
#include "dodow.h"
#include "BSP/bsp.h"

/* Main */
int main(void)
{
  bsp_Init();

  dodow_Init();

  /* Start the tasks */
  vTaskStartScheduler();

  return 1;
}
