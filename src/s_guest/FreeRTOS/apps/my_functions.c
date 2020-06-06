#include<my_functions.h>
#include <printk.h>
#include <FreeRTOS.h>

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
      vTaskDelay( 100 / portTICK_RATE_MS);
      i=0;
    }

    // YIELD()
  }
}
