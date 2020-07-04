#include <my_functions.h>
#include <printk.h>
#include <FreeRTOS.h>
#include<semphr.h>
#include <portmacro.h>
#include <string.h>
#include <spi.h>
#include <printk.h>

extern void flush_icache_and_dcache(void);
extern SemaphoreHandle_t xSemaphoreSPI;


static uint32_t toggle = 0x00;
/** 4GPIO (LED) in FPGA fabric */
static uint32_t *ptr = (uint32_t *) 0x41200000;

/* idle hook function calls hypervisor scheduler
    -> NS guest runs during FreeRTOS idle task */
void vApplicationIdleHook( void ) {

    /* call scheduler */
      // YIELD();
      while(1);
      // asm volatile (	"dsb		\n"
      					      // "isb		\n" );

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
void vTaskSendSPIData(void *pvParameters) {

  volatile uint32_t *data = (volatile uint32_t *)(0x161A80);
  while(1){
    SPI1_SendData(0xAC);
    SPI1_SendData(0xBC);

    // memcpy(data, &test, sizeof(test));

    vTaskDelay( 1000 / portTICK_RATE_MS);
  }
}


/* task for reading data received from SPI peripheral */
void vTaskReadSPIData(void *pvParameters){

  volatile uint8_t recv;

  while(1){

    xSemaphoreTake(xSemaphoreSPI, portMAX_DELAY);
    flush_icache_and_dcache();
    SPI1_ReadData(&recv);
    printk("Received: %x\n", recv);
    xSemaphoreGive(xSemaphoreSPI);

    // xSemaphoreGive(xSemaphoreSPI);
    vTaskDelay( 1000 / portTICK_RATE_MS);
  }
}
