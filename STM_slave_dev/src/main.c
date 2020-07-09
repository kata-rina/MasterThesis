#include <main.h>


int main(){

  gpio_init();
  SPI_1_Init();

  uint8_t i = 0;

  while(1){
    i++;
    SPIx_Send(i, SPI1);
  }

}
