/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011, 2013 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_serial.c
* Version      : Applilet3 for RL78/G14 V2.00.00.08 [12 Apr 2013]
* Device(s)    : R5F104PJ
* Tool-Chain   : GCCRL78
* Description  : This file implements device driver for Serial module.
* Creation Date: 6/26/2013
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_serial.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"
#include <stdint.h>

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
volatile uint8_t * gp_csi11_rx_address;        /* csi11 receive buffer address */
volatile uint16_t  g_csi11_rx_length;             /* csi11 receive data length */
volatile uint16_t  g_csi11_rx_count;              /* csi11 receive data count */
volatile uint8_t * gp_csi11_tx_address;        /* csi11 send buffer address */
volatile uint16_t  g_csi11_send_length;        /* csi11 send data length */
volatile uint16_t  g_csi11_tx_count;              /* csi11 send data count */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_SAU0_Create
* Description  : This function initializes the SAU0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SAU0_Create(void)
{
    SAU0EN = 1U;    /* supply SAU0 clock */
    NOP();
    NOP();
    NOP();
    NOP();
    SPS0 = _0000_SAU_CK00_FCLK_0 | _0000_SAU_CK01_FCLK_0;
    R_CSI11_Create();
}

/***********************************************************************************************************************
* Function Name: R_CSI11_Create
* Description  : This function initializes the CSI11 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CSI11_Create(void)
{
    ST0 |= _0002_SAU_CH1_STOP_TRG_ON;    /* disable CSI11 */
    CSIMK11 = 1U;    /* disable INTCSI11 interrupt */
    CSIIF11 = 0U;    /* clear INTCSI11 interrupt flag */
    /* Set INTCSI11 low priority */
    CSIPR111 = 1U;
    CSIPR011 = 1U;
    SIR03 = _0004_SAU_SIRMN_FECTMN | _0002_SAU_SIRMN_PECTMN | _0001_SAU_SIRMN_OVCTMN;    /* clear error flag */
    SMR03 = _0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0000_SAU_CLOCK_MODE_CKS |
            _0000_SAU_TRIGGER_SOFTWARE | _0000_SAU_MODE_CSI | _0000_SAU_TRANSFER_END;
    SCR03 = _C000_SAU_RECEPTION_TRANSMISSION | _0000_SAU_TIMING_1 | _0000_SAU_MSB | _0007_SAU_LENGTH_8;
    SDR03 = _6400_CSI21_DIVISOR;
    SO0 |= _0200_SAU_CH1_CLOCK_OUTPUT_1;    /* CSI11 clock initial level */
    SO0 &= ~_0002_SAU_CH1_DATA_OUTPUT_1;    /* CSI11 SO initial level */
    SOE0 |= _0002_SAU_CH1_OUTPUT_ENABLE;    /* enable CSI11 output */
    
    /* Set SI11 pin */
    PM1 |= 0x02U;
    /* Set SO11 pin as output*/
    P1 |= 0x04U;
    PM1 &= 0xFBU;
    /* Set SCK11 pin as output */
    P1 |= 0x01U;
    PM1 &= 0xFEU;
}

/***********************************************************************************************************************
* Function Name: R_CSI11_Start
* Description  : This function starts the CSI11 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CSI11_Start(void)
{
    CSIIF11 = 0U;    /* clear INTCSI11 interrupt flag */
    CSIMK11 = 0U;    /* enable INTCSI11 */
    SO0 |= _0200_SAU_CH1_CLOCK_OUTPUT_1;    /* CSI11 clock initial level */
    SO0 &= ~_0002_SAU_CH1_DATA_OUTPUT_1;           /* CSI11 SO initial level */
    SOE0 |= _0002_SAU_CH1_OUTPUT_ENABLE;           /* enable CSI11 output */
    SS0 |= _0002_SAU_CH1_START_TRG_ON;             /* enable CSI11 */
}

/***********************************************************************************************************************
* Function Name: R_CSI11_Stop
* Description  : This function stops the CSI11 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CSI11_Stop(void)
{
    ST0 |= _0002_SAU_CH1_STOP_TRG_ON;               /* disable CSI11 */
    SOE0 &= ~_0002_SAU_CH1_OUTPUT_ENABLE;    /* disable CSI11 output */
    CSIMK11 = 1U;    /* disable INTCSI11 interrupt */
    CSIIF11 = 0U;    /* clear INTCSI11 interrupt flag */
}

/***********************************************************************************************************************
* Function Name: R_CSI11_Send_Receive
* Description  : This function sends and receives CSI11 data.
* Arguments    : tx_buf -
*                    transfer buffer pointer
*                tx_num -
*                    buffer size
*                rx_buf -
*                    receive buffer pointer
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_CSI11_Send_Receive(uint8_t * const tx_buf, uint16_t tx_num, uint8_t * const rx_buf)
{
    MD_STATUS status = MD_OK;

    if (tx_num < 1U)
    {
        status = MD_ARGERROR;
    }
    else
    {
        g_csi11_tx_count = tx_num;      
        gp_csi11_tx_address = tx_buf;     
        gp_csi11_rx_address = rx_buf;    
        CSIMK11 = 1U;                               
       SIO11 = *gp_csi11_tx_address;  
       gp_csi11_tx_address++;
       g_csi11_tx_count--;
      CSIMK11 = 0U;                   
  }
    return (status);
}

/***********************************************************************************************************************/
MD_STATUS R_CSI11_Master_Send(uint8_t adr, uint8_t * const tx_buf, uint16_t tx_num, uint8_t wait)
{
    MD_STATUS status = MD_OK;

   CSIMK11 = 1U;    /* disable INTCSI11 interrupt */

     if (tx_num < 1U)
    {
	CSIMK11 = 0U;    /* enable INTCSI11 interrupt */
        status = MD_ARGERROR;
    }
    else
    {
        STT0 = 1U;    /* send CSI start condition */
        CSIMK11 = 0U;    /* enable INTCSI11 interrupt */

        /* Wait */
        while (0U == STD0)
        {
            if (0U == (wait--))
            {
                status =  MD_ARGERROR;
                break;
            }
        }

        /* Detect start condition */
        if (MD_OK == status)
        {
           g_csi11_tx_count = tx_num;        /* send data count */
          gp_csi11_tx_address = tx_buf;     /* send buffer pointer */        
          adr &= (uint8_t)~0x01U;    /* set send mode */
          SIO11 = adr;    /* send address */
        }
    }
    return (status);
}

/***********************************************************************************************************************/
MD_STATUS R_CSI11_Master_Receive(uint8_t adr, uint8_t * const rx_buf, uint16_t rx_num, uint8_t wait)
{
    MD_STATUS status = MD_OK;

   CSIMK11 = 1U;    /* disable INTCSI11 interrupt */

     if (rx_num < 1U)
    {
	CSIMK11 = 0U;    /* enable INTCSI11 interrupt */
        status = MD_ARGERROR;
    }
    else
    {
        SS0 = 1U;     /* send CSI start condition */
        CSIMK11 = 0U;    /* enable INTCSI11 interrupt */

        /* Wait */
        while (0U == ST0)
        {
            if (0U == (wait--))
            {
                status = MD_ARGERROR;
                break;
            }
        }

        /* Detect start condition */
        if (MD_OK == status)
        {
           g_csi11_rx_length = rx_num;        /* send data count */
	   g_csi11_rx_count = 0U;
           gp_csi11_tx_address = rx_buf;     /* send buffer pointer */          
           adr |= 0x01U;    /* set receive mode */
           SIO11 = adr;    /* send address */
        }
    }
    return (status);
}