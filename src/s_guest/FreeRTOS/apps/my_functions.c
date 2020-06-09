#include<my_functions.h>
#include <printk.h>
#include <FreeRTOS.h>

static uint32_t toggle = 0x00;
uint32_t cnt = 0;
/** 4GPIO (LED) in FPGA fabric */
static uint32_t *ptr = (uint32_t *) 0x41200000;

/* idle hook function calls hypervisor scheduler
    -> NS guest runs during FreeRTOS idle task */
void vApplicationIdleHook( void ) {

    /* call scheduler */
      // printk("idle\n");
      // cnt++;
      // if(cnt == 50000){
      //   cnt = 0;
      // }
      YIELD()
}

/* some other task to signalize that FreeRTOS is alive */
void vTask1(void *pvParameters) {

  while(1){
    // toggle ^= 0xFF;
    // *ptr = toggle;
    // printk("Task1\n");
    vTaskDelay( 1000 / portTICK_RATE_MS);
  }
}
