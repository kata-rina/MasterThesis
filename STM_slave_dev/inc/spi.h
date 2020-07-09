#ifndef SPI_H
#define SPI_H

#include <stm32f4xx_spi.h>
#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>


// We configure STM32F407 as a slave, Zedboard is a master device
// GPIO alternate functions
// for GPIOB PB9 => NSS, PB13 => SCLK, PB14 => MISO, PB15 => MOSI
// Choose SPI => currenty is cosen SPI2 interface
#define SPIx                           SPI2
#define SPIx_CLK                       RCC_APB1Periph_SPI2
#define SPIx_CLK_INIT                  RCC_AHB1PeriphClockCmd
#define SPIx_IRQn                      SPI2_IRQn
#define SPIx_IRQHANDLER                SPI2_IRQHandler

// SPI2 Clock pin
#define SPIx_SCK_PIN                   GPIO_Pin_13
#define SPIx_SCK_GPIO_PORT             GPIOB
#define SPIx_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOB
#define SPIx_SCK_SOURCE                GPIO_PinSource13
#define SPIx_SCK_AF                    GPIO_AF_SPI2

// SPI2 MISO Pin
#define SPIx_MISO_PIN                  GPIO_Pin_14
#define SPIx_MISO_GPIO_PORT            GPIOB
#define SPIx_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define SPIx_MISO_SOURCE               GPIO_PinSource14
#define SPIx_MISO_AF                   GPIO_AF_SPI2

// SPI2 MOSI Pin
#define SPIx_MOSI_PIN                  GPIO_Pin_15
#define SPIx_MOSI_GPIO_PORT            GPIOB
#define SPIx_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define SPIx_MOSI_SOURCE               GPIO_PinSource15
#define SPIx_MOSI_AF                   GPIO_AF_SPI2

// SPI2 slave select
#define SPIx_SS_PIN                   GPIO_Pin_9
#define SPIx_SS_GPIO_PORT             GPIOB
#define SPIx_SS_GPIO_CLK              RCC_AHB1Periph_GPIOB
#define SPIx_SS_SOURCE                GPIO_PinSource9
#define SPIx_SS_AF                    GPIO_AF_SPI2

void SPIx_Send(uint8_t data, SPI_TypeDef* SPI_x);
uint8_t SPIx_Read(SPI_TypeDef* SPI_x);
void SPI_1_Init(void);


#endif
