#include<my_functions.h>
#include <printk.h>
#include <FreeRTOS.h>

static uint32_t toggle = 0x00;
/** 4GPIO (LED) in FPGA fabric */
static uint32_t *ptr = (uint32_t *) 0x41200000;

/* idle hook function calls hypervisor scheduler
    -> NS guest runs during FreeRTOS idle task */
void vApplicationIdleHook( void ) {

    /* call scheduler */
    while(1){
      YIELD();
    }
}

/* some other task to signalize that FreeRTOS is alive */
void vTask1(void *pvParameters) {
  uint32_t i = 0;

  while(1){
    i++;
    if((i%200000) == 0){
      vTaskDelay( 10000 / portTICK_RATE_MS);
      toggle ^= 0xFF;
      *ptr = toggle;
      i=0;
    }
  }
}
