/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/

/*******************************************************************************
* File Name    : config.h
* Version      : Applilet EZ PL for RL78
* Device(s)    : R5F1056A,R5F1057A,R5F1058A
* Tool-Chain   : CC-RL
* Description  : This file implements main function.
* Creation Date: 2016/09/14
* ADD Lcd Contrlle:2021/06/08
*******************************************************************************/

#ifndef _CONFIG_H_
#define _CONFIG_H_

/* Applilet EZ PL for RL78 Append below */

#define	DEVICE_R5F1058AALA

#define	SYS_CLOCK_SRC		CLOCK_SRC_HIGHSPEED
#define	SYS_CLOCK_FREQ		12
#define	SYS_FLASHMODE		FLASHMODE_HS

/* PORT Initialize 0=Low,1=High */
#define	INIT_P0			0x00
#define	INIT_P2			0x00
#define	INIT_P3			0x00
#define	INIT_P4			0x00
#define	INIT_P5			0x00

/* PORT IN/OUT MODE 0=out,1=in */
#define	INIT_PM0		0xfC
#define	INIT_PM2		0xf0
#define	INIT_PM3		0xf7
#define	INIT_PM4		0xff
#define	INIT_PM5		0xff
#define	INIT_PM12		0xff
#define	INIT_PM13		0xff

/* PORT Pull-up resistor 1=pull-up */
#define	INIT_PU0		0x00
#define	INIT_PU2		0x00
#define	INIT_PU3		0x00
#define	INIT_PU4		0x01
#define	INIT_PU5		0x7e
#define	INIT_PU12		0x20
#define	INIT_PU13		0x00


/* A/D PORT conf */
#define	INIT_PMC0		0xfc
#define	INIT_PMC2		0xf0
#define	INIT_PMC3		0xf0
#define	INIT_PMC5		0xbf


#define	TIM_USED		(1)
#define	DGOUT_USED		(1)

#endif /*_CONFIG_H_*/
