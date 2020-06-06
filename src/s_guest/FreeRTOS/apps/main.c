#include<my_functions.h>
#include<printk.h>
#include<hw_zynq.h>

void led_blink( void * pvParameters );

void main ( void ){

	/** Initialize hardware */
	// hw_init();

	printk(" * Secure bare metal VM: running AA ... \n\t");


	// led_blink((void*)0);

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
