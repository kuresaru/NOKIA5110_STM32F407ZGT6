#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_spi.h"
#include "common.h"

#define NOKIA5110_RST_SET   GPIOC->BSRRL = GPIO_Pin_0
#define NOKIA5110_RST_CLR   GPIOC->BSRRH = GPIO_Pin_0
#define NOKIA5110_CS_SET    GPIOC->BSRRL = GPIO_Pin_1
#define NOKIA5110_CS_CLR    GPIOC->BSRRH = GPIO_Pin_1
#define NOKIA5110_DC_SET    GPIOC->BSRRL = GPIO_Pin_2
#define NOKIA5110_DC_CLR    GPIOC->BSRRH = GPIO_Pin_2

void NOKIA5110_Init() {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_12;//SPI3 SCK MOSI
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;//RST CS DC
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    SPI_InitTypeDef SPI_InitStructure;
    SPI_StructInit(&SPI_InitStructure);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
    SPI_Init(SPI3, &SPI_InitStructure);
    SPI_Cmd(SPI3, ENABLE);

    NOKIA5110_RST_SET;
    NOKIA5110_CS_SET;
    Delay_us(2);
    NOKIA5110_RST_CLR;
    Delay_us(2);
    NOKIA5110_RST_SET;

    NOKIA5110_WriteCmd(0x21);
    NOKIA5110_WriteCmd(0xC8);
    NOKIA5110_WriteCmd(0x06);
    NOKIA5110_WriteCmd(0x13);
    NOKIA5110_WriteCmd(0x20);
    NOKIA5110_WriteCmd(0x0C);
}

void SPI3_Write(u8 dat) {
    while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_BSY) != RESET);
    SPI_I2S_SendData(SPI3, dat);
    while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_BSY) != RESET);
}

void NOKIA5110_WriteCmd(u8 dat) {
    NOKIA5110_CS_CLR;
    NOKIA5110_DC_CLR;
    SPI3_Write(dat);
    NOKIA5110_CS_SET;
}

void NOKIA5110_WriteData(u8 dat) {
    NOKIA5110_CS_CLR;
    NOKIA5110_DC_SET;
    SPI3_Write(dat);
    NOKIA5110_CS_SET;
}

void NOKIA5110_SetPos(u8 x, u8 y) {
    NOKIA5110_WriteCmd(0x40 | y);
    NOKIA5110_WriteCmd(0x80 | x);
}
