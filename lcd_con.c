#include <stdio.h>
#include <string.h>
#include "iodefine.h"
#include <stdint.h>
//#include "common.h"
//#include "config.h"
//#include "system.h"
#include "sfr.h"
#include "lcd_con.h"

#include "LCM_driver.h"
#include "r_cg_serial.h"
#include "LCM_driver_config.h"
#include"lcd_con.h"
/**********************************************************************
*** define　(文字定義）
**********************************************************************/
void dispset_count(void)
{
      delay_msec(5);	
}
//****************************************
//**　LCD初期化 Initialization
//
void lcd_init(void)
{
	P7 = 0x00;		//E=0,RS=0
//	
	P7 = 0x03;
	P7 = 0x23;		//E=1,RS=0
	delay_micro(100);
	P7 = 0x03;		//E=0,RS=0
	delay_msec(5);
//
	P7 = 0x23;		//E=1,RS=0
	delay_micro(100);
	P7 = 0x03;		//E=0,RS=0
	delay_msec(5);
//
	P7 = 0x23;		//E=1,RS=0
	delay_micro(100);
	P7 = 0x03;		//E=0,RS=0
	delay_msec(5);
//
	P7 = 0x02;		//4bitﾃﾞｰﾀ長設定
	P7 = 0x22;		//E=1,RS=0
	delay_micro(100);
	P7 = 0x02;		//E=0,RS=0
	delay_msec(5);
//** ここから4ﾋﾞｯﾄﾃﾞｰﾀ長
	lcd_cout(0x02);		                          //for 4-bit lcd intialization
	delay_msec(5);
	lcd_cout(0x28);		
	delay_msec(5);
	lcd_cout(0x0C);                                    
	delay_msec(5);    
	lcd_cout(0x06);                                       //auto-increment
	delay_msec(5); 
	lcd_cout(0x01);
	delay_msec(5); 				
}

//*****************************************
//** 1行目ｱﾄﾞﾚｽｾｯﾄ
//** col_1 = 列
//**
void lcd_l1(unsigned char col1)
{
	unsigned char posit1;
//
	posit1 = col1 & 0x0F;	//列の最大値＝16
	posit1 = posit1 + 0x80;	//1行目ﾋﾞｯﾄ追加
	lcd_cout(posit1);
	delay_msec(5);
}

//*****************************************
//** 2行目ｱﾄﾞﾚｽｾｯﾄ
//**
void lcd_l2(unsigned char col2)
{
	unsigned char posit2;

	posit2 = col2 & 0x4F;	//列の最大値＝16
	posit2 = posit2 + 0xC0;	//2行目ﾋﾞｯﾄ追加
	lcd_cout(posit2);
	delay_msec(5);
}
//*****************************************
//** ｷｬﾗｸﾀLCD　ｺﾝﾄﾛｰﾙｺｰﾄﾞ出力
//
void lcd_cout(unsigned char ccod)
{
	unsigned char ccod_msb;
	unsigned char ccod_lsb;

	ccod_msb = ccod / 0x10;
	ccod_lsb = ccod & 0x0F;
//
	P7 = ccod_msb;		//E=0,RS=0 + MSB
	delay_micro(2);
	P7 = ccod_msb | 0x20;	//E=1,RS=0
	delay_micro(2);
	P7 = ccod_msb;		//E=0,RS=0
//
	delay_micro(2);
//
	P7 = ccod_lsb;		//E=0,RS=0 + LSB
	delay_micro(2);
	P7 = ccod_lsb | 0x20;	//E=1,RS=0
	delay_micro(2);
	P7 = ccod_lsb;		//E=0,RS=0
	delay_micro(50);	
}
//****************************************
//** ｷｬﾗｸﾀLCD　ﾃﾞｰﾀｺｰﾄﾞ出力
//
int lcd_dout(unsigned char dcod)
{
	unsigned char dcod_msb;
	unsigned char dcod_lsb;

	dcod_msb = dcod / 0x10;
	dcod_lsb = dcod & 0x0F;
//
	P7 = dcod_msb | 0x10;	//E=0,RS=1 + MSB
	delay_micro(2);
	P7 = dcod_msb | 0x30;	//E=1,RS=1
	delay_micro(2);
	P7 = dcod_msb | 0x10;	//E=0,RS=1
//
	delay_micro(2);
//
	P7 = dcod_lsb | 0x10;	//E=0,RS=1 + LSB
	delay_msec(2);
	P7 = dcod_lsb | 0x30;	//E=1,RS=1
	delay_micro(2);
	P7 = dcod_lsb | 0x10;	//E=0,RS=1
	delay_micro(50);     
	return 0;
}

//******************************************
void r_LCM_clear(void)
{
    lcd_cout(_0x00_LCM_COMMAND_CLEAR_DISPLAY);
}

//******************************************
void r_LCM_send_string(uint8_t * const str, lcm_position_t pos)
{
    int i;
    uint8_t *p;
    
   lcd_cout(_0x80_LCM_COMMAND_SET_DDRAM_ADDRESS | pos);

    for (i = 0, p = str; *p != '\0'; i++, p++)
    {
        if (i >= LCM_CONFIG_MAX_CHAR_PER_LINE)
        {
            /* The string length reached the maximum. */
            break;
        }
       lcd_dout(*p);
    }
}

//*****************************************
//** ﾏｲｸﾛ秒遅延
void delay_micro(unsigned int icnt)
{
	unsigned int del_cnt;
	for (del_cnt=0;del_cnt<icnt;del_cnt++){	
		        NOP();
 			NOP();
			NOP();
			NOP();
			NOP();					
	              }
                  }
//******************************************
//** ﾐﾘ秒遅延
void delay_msec(unsigned int icnt)
{
	unsigned int del_cnt;
	for (del_cnt=0;del_cnt<icnt;del_cnt++) {
//		wdt_rst();      
               WDTE = 0xAC;
		delay_micro(1000);		 
	}
}
