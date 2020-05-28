#include<my_functions.h>
#include<printk.h>

void main ( void ){

  /** Initialize hardware */
	hw_init();

	printk(" * Secure FreeRTOS VM: running ... \n\t");

  /* create one task */
  xTaskCreate(
    vTask1,
    (const signed char *)"TASK1",
    configMINIMAL_STACK_SIZE,
    NULL,
    2,
    NULL);

  vTaskStartScheduler();

}
