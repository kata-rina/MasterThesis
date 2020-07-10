#include <main.h>


int main(){

  gpio_init();
  SPI_1_Init();


  while(1){
    /* Communicate via SPI - polling method*/
    SPIx_Read(SPI1);
    SPIx_Send(SPI1);
  }

}
