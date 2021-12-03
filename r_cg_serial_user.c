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
* File Name    : r_cg_serial_user.c
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
extern volatile uint8_t * gp_csi11_rx_address;         /* csi11 receive buffer address */
extern volatile uint16_t  g_csi11_rx_length;           /* csi11 receive data length */
extern volatile uint16_t  g_csi11_rx_count;            /* csi11 receive data count */
extern volatile uint8_t * gp_csi11_tx_address;         /* csi11 send buffer address */
extern volatile uint16_t  g_csi11_send_length;         /* csi11 send data length */
extern volatile uint16_t  g_csi11_tx_count;            /* csi11 send data count */
/* Start user code for global. Do not edit comment generated here */
volatile uint8_t G_CSI11_SendingData = 0;
volatile uint8_t G_CSI11_ReceivingData = 0;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_csi11_interrupt
* Description  : This function is INTCSI11 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_csi11_interrupt(void)
{
    uint8_t err_type;

    err_type = (uint8_t)(SSR03 & _0001_SAU_OVERRUN_ERROR);
    SIR03 = (uint16_t)err_type;

    if (1U == err_type)
    {
        r_csi11_callback_error(err_type);    /* overrun error occurs */
    }
    else
    {
        if (g_csi11_tx_count > 0U)
        {
            *gp_csi11_rx_address = SIO11;
            gp_csi11_rx_address++;
            SIO11 = *gp_csi11_tx_address;
            gp_csi11_tx_address++;
            g_csi11_tx_count--;
        }
        else 
        {
            if (0U == g_csi11_tx_count)
            {
                *gp_csi11_rx_address = SIO11;
            }

            r_csi11_callback_sendend();    /* complete send */
            r_csi11_callback_receiveend();    /* complete receive */
        }
    }
}

/***********************************************************************************************************************
* Function Name: r_csi11_callback_receiveend
* Description  : This function is a callback function when CSI11 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_csi11_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
    G_CSI11_ReceivingData = 0;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_csi11_callback_error
* Description  : This function is a callback function when CSI11 reception error occurs.
* Arguments    : err_type -
*                    error type value
* Return Value : None
***********************************************************************************************************************/
static void r_csi11_callback_error(uint8_t err_type)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_csi11_callback_sendend
* Description  : This function is a callback function when CSI11 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_csi11_callback_sendend(void)
{
    /* Start user code. Do not edit comment generated here */
    G_CSI11_SendingData = 0;
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
