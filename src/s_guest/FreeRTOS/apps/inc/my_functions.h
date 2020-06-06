#ifndef MY_FUNCTIONS_H
#define MY_FUNCTIONS_H

#include<FreeRTOS.h>
#include<task.h>
#include<queue.h>
#include<semphr.h>

#include<hw_zynq.h>
#include<printk.h>

void vApplicationIdleHook( void );
void vTask1( void *pvParameters);

#endif
