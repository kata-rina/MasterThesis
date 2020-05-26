#include<my_functions.h>
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

  static uint32_t on=0xFF;

  /** 4GPIO (LED) in FPGA fabric */
  static uint32_t *ptr = (uint32_t *) 0x41200000;

  while(1){
    *ptr = on;
    // YIELD()
  }
}
