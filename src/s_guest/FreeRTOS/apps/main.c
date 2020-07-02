#include<my_functions.h>
#include<printk.h>
#include<hw_zynq.h>
#include<zynq_spi.h>

void main ( void ){

	/** Initialize hardware */
	hw_init();

	printk(" * Secure bare metal VM: running ... \n\t");

	/* Initialize SPI 1 controller */
	SPI_1_Reset();
	SPI_1_ClockEnable();
	SPI_1_SignalRoute();
	SPI_1_Config();

	SPI_1_Enable();

  /* create one task */
  xTaskCreate(
    vTask1,
    (const signed char *)"TASK1",
    configMINIMAL_STACK_SIZE,
    NULL,
    1,
    NULL);

	/* create another one */
  xTaskCreate(
    vTaskReadSPIData,
    (const signed char *)"TASK2",
    configMINIMAL_STACK_SIZE,
    NULL,
    2,
    NULL);

  vTaskStartScheduler();

}
