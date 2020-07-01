#include<my_functions.h>
#include<printk.h>
#include<hw_zynq.h>
#include<zynq_spi.h>

void led_blink( void * pvParameters );

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

void led_blink( void * parameters ){

	// static uint32_t toggle;
	/** 4GPIO (LED) in FPGA fabric */
	// static uint32_t *ptr = (uint32_t *) 0x41200000;
	uint32_t i = 0;
	for( ;; ){
			// printk("led blink\n");
			YIELD();
		}
}
