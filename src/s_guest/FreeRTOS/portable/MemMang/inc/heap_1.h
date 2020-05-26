#ifndef HEAP_1_H
#define HEAP_1_H

#include<stdlib.h>

#include "FreeRTOS.h"
#include "task.h"

void *pvPortMalloc( size_t xWantedSize );
void vPortFree( void *pv );
void vPortInitialiseBlocks( void );
size_t xPortGetFreeHeapSize( void );

#endif
