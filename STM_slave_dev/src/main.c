#include <main.h>
// #include <time.h>
// #include <stdio.h>
#include <LM35DZ.h>
#include <usart.h>

int main(){

  // uint8_t variable = 0x22;
  gpio_init();
  SPI_1_Init();
  // ADC1_Init();
  // USART1_Init();

  // srand(time(0));
  // uint8_t temp = 27;

  // gpio_led_state(LED4_GREEN_ID, 1);

  // uint16_t temp;
  // float f;
  uint8_t i = 0;

  while(1){
    i++;
    // temp = LM_temp(&f);
    // temp &= 0x3FF;
    // for(int i = 0; i<400000;i++);
    // uint8_t lsb = (uint8_t)(temp);
    // uint8_t msb = (uint8_t)(temp >> 8);
    // SPIx_Send(lsb, SPI1);
    // SPIx_Send(lsb, SPI1);

    // USART1_SendChar(lsb);
    // USART1_SendChar(msb);

    // for(int i = 0; i<100000;i++);
    SPIx_Send(i, SPI1);
    // if(!(i%5)){
  //     temp += (5-12%i);
  // }
  // else{
  //   temp += (5+12%i);

  // }
    // for(int i = 0; i<100000;i++);



  }


}
