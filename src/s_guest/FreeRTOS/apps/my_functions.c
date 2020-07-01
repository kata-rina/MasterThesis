#include <my_functions.h>
#include <printk.h>
#include <FreeRTOS.h>
#include <portmacro.h>
#include <string.h>
#include <zynq_spi.h>
#include <printk.h>

extern void flush_icache_and_dcache(void);

static uint32_t toggle = 0x00;
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
  printk("Stack overflow in %s\n", pcTaskName);
}

void vApplicationMallocFailedHook( void ){

  printk("Malloc failed hook\n");
}


/* some other task to signalize that FreeRTOS is alive */
void vTask1(void *pvParameters) {

  volatile uint32_t *data = (volatile uint32_t *)(0x161A80);
  uint8_t test = 0xBC;

  while(1){
    SPI1_SendData(test);
    // memcpy(data, &test, sizeof(test));

    vTaskDelay( 1000 / portTICK_RATE_MS);
  }
}

/* task for reading data received from SPI peripheral */
void vTaskReadSPIData(void *pvParameters){

  uint8_t recv;

  while(1){
    flush_icache_and_dcache();
    SPI1_ReadData(&recv);
    if (recv == 0xBC){
      *ptr = 0x11;
    }
    else{
      *ptr = 0x00;
    }
    vTaskDelay( 1000 / portTICK_RATE_MS);
  }
}
