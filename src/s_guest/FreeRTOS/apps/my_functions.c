#include <my_functions.h>
#include <printk.h>
#include <FreeRTOS.h>
#include <portmacro.h>
#include <string.h>

static uint32_t toggle = 0x00;
uint32_t cnt = 0;
/** 4GPIO (LED) in FPGA fabric */
static uint32_t *ptr = (uint32_t *) 0x41200000;

/* idle hook function calls hypervisor scheduler
    -> NS guest runs during FreeRTOS idle task */
void vApplicationIdleHook( void ) {

    /* call scheduler */
      YIELD();
      asm volatile (	"dsb		\n"
      					      "isb		\n" );
}

void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                    signed char *pcTaskName)
{
  *ptr = 0xAA;
  printk("Stack overflow in %s\n", pcTaskName);
}

void vApplicationMallocFailedHook( void ){
  *ptr = 0xEE;

  printk("Malloc failed hook\n");
}

/* some other task to signalize that FreeRTOS is alive */
void vTask1(void *pvParameters) {

  volatile uint32_t *data = (volatile uint32_t *)(0x161A80);
  uint32_t nmr = 25;

  while(1){
    toggle ^= 0xFF;
    *ptr = toggle;
    cnt++;
    if(cnt == 5000){
      cnt = 0;

    memcpy(data, &nmr, sizeof(nmr));
    }
    vTaskDelay( 1000 / portTICK_RATE_MS);
  }
}
