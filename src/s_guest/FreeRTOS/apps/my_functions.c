#include<my_functions.h>
#include <printk.h>
/* idle hook function calls hypervisor scheduler
    -> NS guest runs during FreeRTOS idle task */
// void vApplicationIdleHook( void ) {
//
//     static uint32_t off=0x00;
//
//     /** 4GPIO (LED) in FPGA fabric */
//     static uint32_t *ptr = (uint32_t *) 0x41200000;
//     *ptr = off;
//
//     /* call scheduler */
//     while(1){
//       YIELD();
//     }
// }

/* some other task to signalize that FreeRTOS is alive */
void vTask1(void *pvParameters) {
  uint32_t i = 0;
  printk("T\n");
  while(1){
    i++;
    if((i%20000) == 0){
      printk("Task\n");
    }

    // YIELD()
  }
}
