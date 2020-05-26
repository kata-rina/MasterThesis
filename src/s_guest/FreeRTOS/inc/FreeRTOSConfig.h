#ifndef _FREERTOSCONFIG_H
#define _FREERTOSCONFIG_H

#define XPAR_PS7_SCUGIC_0_BASEADDR 0xF8F00000U

#define configUSE_PREEMPTION 1

#define configUSE_MUTEXES 1

#define configUSE_RECURSIVE_MUTEXES 1

#define configUSE_COUNTING_SEMAPHORES 1

#define configUSE_TIMERS 1

#define configUSE_IDLE_HOOK 0

#define configUSE_TICK_HOOK 0

#define configUSE_MALLOC_FAILED_HOOK 0

#define configUSE_TRACE_FACILITY 1

#define configUSE_16_BIT_TICKS 0

#define configUSE_APPLICATION_TASK_TAG 0

#define configUSE_CO_ROUTINES 0

#define configTICK_RATE_HZ (100)

#define configMAX_PRIORITIES (8)

#define configMAX_CO_ROUTINE_PRIORITIES 2

#define configMINIMAL_STACK_SIZE ( ( unsigned short ) 200*2)

#define configTOTAL_HEAP_SIZE ( ( size_t ) ( 65536*4*4*2 ) )

#define configMAX_TASK_NAME_LEN 10

#define configIDLE_SHOULD_YIELD 1

#define configTIMER_TASK_PRIORITY (configMAX_PRIORITIES - 1)

#define configTIMER_QUEUE_LENGTH 10

#define configTIMER_TASK_STACK_DEPTH ((configMINIMAL_STACK_SIZE) * 2)

#define configUSE_QUEUE_SETS 1

// #define configCHECK_FOR_STACK_OVERFLOW 2
#define configCHECK_FOR_STACK_OVERFLOW 0


#define configQUEUE_REGISTRY_SIZE 10

#define configUSE_STATS_FORMATTING_FUNCTIONS 1

#define configNUM_THREAD_LOCAL_STORAGE_POINTERS 0

#define configUSE_TICKLESS_IDLE	0
#define configTASK_RETURN_ADDRESS    NULL
#define INCLUDE_vTaskPrioritySet             1
#define INCLUDE_uxTaskPriorityGet            1
#define INCLUDE_vTaskDelete                  1
#define INCLUDE_vTaskCleanUpResources        1
#define INCLUDE_vTaskSuspend                 1
#define INCLUDE_vTaskDelayUntil              1
#define INCLUDE_vTaskDelay                   1
#define INCLUDE_eTaskGetState                1
#define INCLUDE_xTimerPendFunctionCall       1
#define INCLUDE_pcTaskGetTaskName            1
#define configMAX_API_CALL_INTERRUPT_PRIORITY (18)

#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1

#define configINTERRUPT_CONTROLLER_BASE_ADDRESS         ( 0xF8F00000)
#define configINTERRUPT_CONTROLLER_CPU_INTERFACE_OFFSET ( 0x0100 )
#define configUNIQUE_INTERRUPT_PRIORITIES                32

void vFreeRTOS_SetupTickInterrupt( void );
void vFreeRTOS_ClearTickInterrupt( void );

#define configSETUP_TICK_INTERRUPT() vFreeRTOS_SetupTickInterrupt()
#define configCLEAR_TICK_INTERRUPT()	vFreeRTOS_ClearTickInterrupt()

#endif
