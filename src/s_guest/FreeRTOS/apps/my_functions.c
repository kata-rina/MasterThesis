#include <my_functions.h>
#include <printk.h>
#include <FreeRTOS.h>
#include <semphr.h>
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
      YIELD();
      // while(1);
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




/* Task for requesting data from SPI slave device */
void vTaskSendSPIData(void *pvParameters) {

  /* Delay at the start because there is some UNRESOLVED PROBLEM with Linux as
     non-secure guest - apparently Linux tries to write to secure memory while
     booting and it corrupts some part of the system and SPI won't work. */
  vTaskDelay( 2000 / portTICK_RATE_MS);

  /* After Linux is booted and running it's ok to safely configure and use
     SPI peripheral for communication with external device */
  SPI_1_Reset();
  SPI_1_ClockEnable();
  SPI_1_SignalRoute();
  SPI_1_Config();
  SPI_1_Enable();
  
  flush_icache_and_dcache();


  while(1){

    /* Assert slave and send data as trigger for slave device */
    SPI1_SendData(0xBC);

    /* Give proccessor to other tasks */
    vTaskDelay( 1000 / portTICK_RATE_MS);
    // SPI1_SendData(0xAC);
    // vTaskDelay( 1000 / portTICK_RATE_MS);

  }
}



/* Task for reading data received from SPI peripheral */
void vTaskReadSPIData(void *pvParameters){

  volatile uint8_t recv;

  /* Make sure that Linux boots first then continue with task execution. */
  vTaskDelay( 2000 / portTICK_RATE_MS);

  while(1){

    /* Take the semaphore to ensure coherency */
    xSemaphoreTake(xSemaphoreSPI, portMAX_DELAY);
    // flush_icache_and_dcache();

    /* Read data from SPI */
    SPI1_ReadData(&recv);
    printk("Received: %x\n", recv);
    vTaskDelay( 1000 / portTICK_RATE_MS);
  }
}
