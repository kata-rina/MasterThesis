#include<my_functions.h>
#include<printk.h>
#include<hw_zynq.h>
#include<spi.h>
#include<FreeRTOS.h>
#include<semphr.h>

SemaphoreHandle_t xSemaphoreSPI = NULL;

void main ( void ){

	printk(" * Secure bare metal VM: running ... \n\t");

	/** Initialize hardware */
	hw_init();

	/* Initialize SPI 1 controller */
	SPI_1_Reset();
	SPI_1_ClockEnable();
	SPI_1_SignalRoute();
	SPI_1_Config();
	SPI_1_Enable();

	/* Create binary semaphore for SPI tasks and SPI ISR*/
	xSemaphoreSPI = xSemaphoreCreateBinary();

	/* Check whether semaphore was created successfully */
	if ( xSemaphoreSPI != NULL){

			/* Take the semaphore for the first time */
			xSemaphoreTake(xSemaphoreSPI, 0);

			/* create task for reading SPI data stored in system memory */
		  xTaskCreate(
		    vTaskReadSPIData,
		    (const signed char *)"READ_spi",
		    configMINIMAL_STACK_SIZE,
		    NULL,
		    2,
		    NULL);

			/* create task for sending SPI data and signalizing that
			 	 FreeRTOS is alive */
		  xTaskCreate(
		    vTaskSendSPIData,
		    (const signed char *)"SEND_spi",
		    configMINIMAL_STACK_SIZE,
		    NULL,
		    1,
		    NULL);

			// /* create task for deferred interrupt handling*/
			// xTaskCreate(
			// 	vTaskDeferredSPI_ISR,
			// 	(const signed char *)"spi_irq_task",
			// 	configMINIMAL_STACK_SIZE,
			// 	NULL,
			// 	3,
			// 	NULL);



			/* Start scheduler*/
		  vTaskStartScheduler();
	}


	/* this point should not be reached */
	printk("ERROR\n");
	while(1);
}
