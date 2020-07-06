#include "FreeRTOSConfig.h"
#include "zynq_ttc.h"
#include "gic.h"
#include "portmacro.h"

void vFreeRTOS_SetupTickInterrupt( void ){

  ttc_init(TTC0,TTCx_2,INTERVAL);

	interrupt_enable(TTC0_TTCx_2_INTERRUPT,TRUE);
	interrupt_target_set(TTC0_TTCx_2_INTERRUPT,0,1);
  interrupt_priority_set(TTC0_TTCx_2_INTERRUPT, 31);

	vFreeRTOS_ClearTickInterrupt();

	/* Set tick every 500 ms */
	ttc_request(TTC0, TTCx_2, 1000000);
	ttc_enable(TTC0, TTCx_2);

}



void vFreeRTOS_ClearTickInterrupt( void ){

  ttc_interrupt_clear(TTC0_TTCx_2_INTERRUPT);

}
