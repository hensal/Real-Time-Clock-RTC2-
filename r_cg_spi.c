#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "r_cg_serial.h"
#include <stdint.h>

extern volatile uint8_t G_CSI11_SendingData; //spi busy flag
extern volatile uint8_t G_CSI11_ReceivingData; //spi busy flag

uint8_t *SPI_CS_Port[] = {
    (uint8_t *)&P1,          //CSI        
    (uint8_t *)&P7,          // LCD-CS        
    (uint8_t *)&P3,          // LCD      
    (uint8_t *)&P14         // RTC   
};

uint8_t SPI_CS_Pin[] = {
    1,    //CSI   
    5,   // LCD-CS      
    0,   // LCD      
    7    // RTC   
};

void IO_Reset(void)
{
    int i = 0;
    
    //#warning RESET-IO must be inverted for actual HW
    P14 |= (1<<0); // Assert P130 (#RESET-IO)
    for (i=0;i<12000;i++);
    P14 &= ~(1<<0);  // Deassert P130 (#RESET-IO)
    for (i=0;i<12000;i++);
}

void SPI_Init(void)
{
    IO_Reset();
    
    R_CSI11_Start();
}

void SPI_CS_Start(uint8_t aDevice)
{
	*SPI_CS_Port[aDevice] &= ~(1<<SPI_CS_Pin[aDevice]);
}

void SPI_CS_End(uint8_t aDevice)
{
	*SPI_CS_Port[aDevice] |= (1<<SPI_CS_Pin[aDevice]);
}

void SPI_Send(uint8_t aDevice, uint8_t *aData, uint32_t aLength)
{
    uint8_t noRXData;
	G_CSI11_SendingData = 1;
	G_CSI11_ReceivingData = 0;
	
	SPI_CS_Start(aDevice);

    R_CSI11_Send_Receive(aData, aLength, &noRXData);
    while(G_CSI11_SendingData);
    
    SPI_CS_End(aDevice);
}

uint8_t SPI_SendReceive(uint8_t aDevice, uint8_t *aTXData, uint32_t aTXLength, uint8_t *aRXData)
{
    uint16_t timeout;
	G_CSI11_SendingData = 1;
	G_CSI11_ReceivingData = 1;
	
	SPI_CS_Start(aDevice);
   
    
    R_CSI11_Send_Receive(aTXData, aTXLength, aRXData);
    
    timeout = 0;
    while(timeout < 1000)
    {
        if(!(G_CSI11_SendingData || G_CSI11_ReceivingData))
            break;
        
        timeout++;
    }
    
	SPI_CS_End(aDevice);
    
    if(timeout >= 1000)
        return 0;
    else
        return 1;
}

