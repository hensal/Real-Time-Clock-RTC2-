
#ifndef _RDKRL78_SPI_H
#define _RDKRL78_SPI_H

#define SPI        1
#define SPI_LCD         5
#define SPI_LCD_POWER       0
#define SPI_RTC   7

void IO_Reset(void);
void SPI_Init(void);
void SPI_Send(uint8_t aDevice, uint8_t *aData, uint32_t aLength);
uint8_t SPI_SendReceive(uint8_t aDevice, uint8_t *aTXData, uint32_t aTXLength, uint8_t *aRXData);

#endif     