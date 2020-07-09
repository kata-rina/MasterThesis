#include <spi.h>
#include <gpio.h>

/*-----------------------------------------------------------------------------*/
/* Function configurates and initializes SPI Peripheral
   - STM32F4 is slave device in this case
   - input argument = none
   - return value = none */
void SPI_1_Init(void){

  SPI_InitTypeDef SPIx_InitStruct;
  GPIO_InitTypeDef GPIO_Spi;

  gpio_led_state(LED3_ORANGE_ID, 1);

  /* Peripheral Clock Enabe ---------------------------------------------------*/
  // Enable SPI Clock
  // SPIx_CLK_INIT(RCC_AHB1Periph_GPIOB, ENABLE);
  // Enable GPIO clock for MOSI, MISO and NSS GPIO
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  // RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  /* GPIO Configuration for SPI -----------------------------------------------*/
  // Connect the pin to the desired peripherals' Alternate Function
  // GPIO_PinAFConfig(SPIx_SCK_GPIO_PORT, SPIx_SCK_SOURCE, SPIx_SCK_AF);
  // GPIO_PinAFConfig(SPIx_MISO_GPIO_PORT, SPIx_MISO_SOURCE, SPIx_MISO_AF);
  // GPIO_PinAFConfig(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_SOURCE, SPIx_MOSI_AF);
  // GPIO_PinAFConfig(SPIx_SS_GPIO_PORT,SPIx_SS_SOURCE, SPIx_SS_AF);
  // Configuration of desired pins
  GPIO_Spi.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Spi.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Spi.GPIO_OType = GPIO_OType_PP;
  GPIO_Spi.GPIO_PuPd = GPIO_PuPd_UP;

  // SPI SCK Pin
  GPIO_Spi.GPIO_Pin = GPIO_Pin_5;
  GPIO_Init(GPIOA, &GPIO_Spi);

  // SPI MOSI Pin
  GPIO_Spi.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOA, &GPIO_Spi);

  // SPI MISO Pin
  GPIO_Spi.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOA, &GPIO_Spi);


  // SPI SS pin
  GPIO_Spi.GPIO_Pin = GPIO_Pin_4;
  GPIO_Spi.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Spi.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Spi.GPIO_OType = GPIO_OType_PP;


  GPIO_Init(GPIOA, &GPIO_Spi);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

  /* SPI Configuration --------------------------------------------------------*/

  // reset to default configuration
  SPI_I2S_DeInit(SPI1);

  SPIx_InitStruct.SPI_Mode = SPI_Mode_Slave;
  SPIx_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPIx_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPIx_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
  SPIx_InitStruct.SPI_DataSize = SPI_DataSize_8b;
  SPIx_InitStruct.SPI_CPOL = SPI_CPOL_High;
  SPIx_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
  SPIx_InitStruct.SPI_NSS = SPI_NSS_Soft;

  SPI_Init( SPI1, &SPIx_InitStruct);

  SPI_BiDirectionalLineConfig(SPI1, SPI_Direction_1Line_Rx | SPI_Direction_1Line_Tx );
  // enable SPI
  SPI_Cmd( SPI1, ENABLE);

  return;
}


void SPIx_Send(uint8_t data, SPI_TypeDef* SPI_x){

    uint16_t recv;
    //check nss
    // while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4 ));
    if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4 )){

      if (SPI_I2S_GetFlagStatus(SPI_x, SPI_I2S_FLAG_RXNE )){
        recv = SPI_I2S_ReceiveData(SPI_x);
        if ((recv) == 0xbc){
          gpio_led_state(LED6_BLUE_ID, 1);
        }
        else{
          gpio_led_state(LED6_BLUE_ID, 0);
        }
          gpio_led_state(LED5_RED_ID, 0);
        // while(!SPI_I2S_GetFlagStatus(SPI_x, SPI_I2S_FLAG_TXE));
          gpio_led_state(LED5_RED_ID, 1);
          SPI_I2S_SendData( SPI_x, (uint16_t) 0xBC );

    }
}
  return;
}


uint8_t SPIx_Read(SPI_TypeDef* SPI_x){

    uint8_t data;

    //check nss
    if (!GPIO_ReadInputDataBit(GPIOB, SPIx_SS_PIN )){
      data = SPI_I2S_ReceiveData(SPI_x);

    }
    return data;
}
