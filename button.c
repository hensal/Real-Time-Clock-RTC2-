#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "iodefine.h"
#include "lcd_con.h"
#include "sfr.h"
#include "button.h"

#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "r_cg_userdefine.h"

unsigned char before_opsw_id;

extern unsigned char TITL13[32] = "My name is:               ";

extern unsigned char Line111[32] = "Hello,ggggg ";
extern unsigned char Line112[32] = "9   ";

extern unsigned char TITL99[32] = "Try your best 2021";
extern unsigned char TITL98[32] = "work hard   2053";

extern unsigned char TITL199[32] = "welcome  2021";
extern unsigned char TITL198[32] = "nepal walfare 2053";

extern unsigned char Line1991[32] = "Hello  ";
extern unsigned char Line1112[32] = "16563     ";

extern unsigned char Data0[32] = "'0804-0810:04*01";
extern unsigned char Data1[32] = "Hello  ";

extern unsigned char Data2[32] = "t=                 ";
int counter = 0;
int count = 0;
int first = 1;
int second = 2;
int temp = '2';	
int temp1 = '5';
int temp2 = '7';
int temp3 = '0';
int temp4 = '4';
int temp5 = '8';

//**********************************************************************
typedef int bool;
#define true 1
#define false 0
bool blink_enabled = false; 

enum{
    LCD1,
    LCD2
}LCD_NUM;

bool SwitchLCD_flag = false;	 
//**********************************************************************

uint8_t g_uart0_rx_buffer = 0U;                     /* UART0 receive buffer */
uint8_t g_uart0_rx_error = 0U;                      /* UART0 receive error status */
MD_STATUS g_uart0_tx_end = 0U;                      /* UART0 transmission end */  

//**********************************************************************
int set_cursor_position (uint_fast8_t row, uint_fast8_t col)                                                 
	       {		 	 
		  if (row)
		                {
			            col |= 0x40;				    
				    } 
				    col |= 0x80;
				    lcd_cout (col); 	
				    return 0;
  }        	       
//**********************************************************************
/*int display1(void)
{
	unsigned char n;
	unsigned char dsp_buf;
		
	lcd_l1(0);				
	        for (n=0 ;n<16 ;n++) {
		         dsp_buf = Data0[n];		
		         lcd_dout(dsp_buf);		
	           }  
		   delay_msec(5);
		   lcd_l2(0x80);
		   for (n=0 ;n<16 ;n++) {
		         dsp_buf = Data2[n];		
		         lcd_dout(dsp_buf);	
		   } 
		   delay_msec(5); 
		   lcd_l2(0x82);
		   lcd_dout(temp);
		    lcd_l2(0x83);
		   lcd_dout(temp1);
		    lcd_l2(0x84);
		   lcd_dout(temp2);
		    lcd_l2(0x85);
                   lcd_dout(temp3);
		    lcd_l2(0x86);
		   lcd_dout(temp4);			   
	return 0;
}*/

//**********************************************************************
void op_switich_mdoef(void)                      //change the screen
{	
	if (modef == 1) {	                                            
		     delay_msec(100);                                                                    
	             modef=0;			     
        switch(count) {
	             case 0:     // LCD_NUM = LCD1;
	                                lcd_cout(0x01); 
	                               dispset_titl1(); 
				       break;
		     case 1:      // LCD_NUM = LCD2;
				       lcd_cout(0x01);                                     
				       dispset_titl2();  
		                       break;
		     //case 2:      lcd_cout(0x01); dispset_titl3();  break;
		     //case 2:      lcd_cout(0x01); display1();   break;		   
                 }  
	            count = (count+1)%3;
	}
}

//**********************************************************************
void op_switich_upf(void) 
{	  	
	if (upf == 1 && blink_enabled == true) {	    	     
		if (second == 2) {
			temp++;
			if(temp > 0x39) temp = 0x30 ;
			lcd_dout(temp);	        			 
		}				
		if (second == 3) {			    
			temp1++;
			if(temp1 > 0x39) temp1 = 0x30 ;
			if(temp1 == 0x30) temp = temp +1;
			lcd_dout(temp1);   
		}
		if (second == 4) {			    
			temp2++;
			if(temp2 > 0x39) temp2 = 0x30 ;
			lcd_dout(temp2);   
		}
		if (second == 5) {			    
			temp3++;
			if(temp3 > 0x39) temp3 = 0x30 ;
			lcd_dout(temp3);   
		}
		if (second == 6) {			    
			temp4++;
			if(temp4 > 0x39) temp4 = 0x30 ;
			lcd_dout(temp4);   
		}
		set_cursor_position (1, second);				 
		upf = 0;
	}
}

//**********************************************************************
void op_switich_downf(void)
{		
	if (downf == 1 && blink_enabled == true) {
		if (second == 2) {
			temp--;
			if(temp < 0x30) temp = 0x39 ;
			lcd_dout(temp);
	        }
		if (second == 3) {
			temp1--;
			if(temp1 < 0x30) temp1 = 0x39 ;			
			lcd_dout(temp1);
	        }	
		if (second == 4) {
			temp2--;
			if(temp2 < 0x30) temp2 = 0x39 ;
			lcd_dout(temp2);
	        }
		if (second == 5) {			    
			temp3--;
			if(temp3 > 0x39) temp3 = 0x30 ;
			lcd_dout(temp3);   
		}
		if (second == 6) {			    
			temp4--;
			if(temp4 > 0x39) temp4 = 0x30 ;
			lcd_dout(temp4);   
		}
		set_cursor_position (1, second);		  
		downf = 0;
	}
}

//**********************************************************************
void op_switich_setf(void)
{		
	if (setf == 1) {	                         	                // Button was pressed.		
             	if (blink_enabled == true) {			// Turn off blinking.     
             		lcd_cout(0x0c);
			blink_enabled = false;
			second = 0;
		}
    		else {
			second = 2;
             		set_cursor_position(1, second);		// Turn on blinking.
             		lcd_cout(0x0f);
             		blink_enabled = true;
             	}
             	setf = 0;
        } 
}

//**********************************************************************
void op_switich_in(void)
{
	unsigned char	opsw_id;
	
	opsw_id = op_swin_1d();		   

	if (opsw_id != before_opsw_id){
		if ((opsw_id & 0x01) == 0){
			if ((before_opsw_id & 0x01) != 0) modef = 1;
		};
		if ((opsw_id & 0x02) == 0 && (before_opsw_id & 0x02) != 0) upf = 1;
		if ((opsw_id & 0x04) == 0 && (before_opsw_id & 0x04) != 0) downf = 1;
		if ((opsw_id & 0x08) == 0 && (before_opsw_id & 0x08) != 0) leftf = 1;
		if ((opsw_id & 0x10) == 0 && (before_opsw_id & 0x10) != 0) rightf = 1;
		if ((opsw_id & 0x20) == 0){
			if ((before_opsw_id & 0x20) != 0) setf = 1;
		}
	}	
	before_opsw_id = opsw_id;		
}	

//**********************************************************************
unsigned char op_swin_1d(void)
{
	unsigned char swid;
	
	P0 = P0 | 0x20;		       //op_switch common on
	delay_micro(100);	               //100microsec delay
	swid = P2 & 0x3F;
	P0 = P0 & 0xDF;		       //op_switch common off 	
	return swid;
}

//**********************************************************************
int dispset_titl1(void)
{
	unsigned char n;
	unsigned char dsp_buf;

	lcd_l1(0);				
	for (n=0 ;n<16 ;n++) {
		dsp_buf = TITL13[n];		
		lcd_dout(dsp_buf);		     
	}	
	delay_msec(5);
	lcd_l2(0x82);				
	       lcd_dout(temp5);	       
	       return 0;
}

//**********************************************************************
int dispset_titl2(void)
{
	unsigned char n;
	unsigned char dsp_buf;

	lcd_l1(0);				
	for (n=0 ;n<16 ;n++) {
		dsp_buf = TITL99[n];		
		lcd_dout(dsp_buf);		
	}	
	lcd_l2(0);				
	for (n=0 ;n<16 ;n++) {
		dsp_buf = TITL98[n];		
		lcd_dout(dsp_buf);	
	}
	return 0;
}

//**********************************************************************
void dispset_titl3(void)
{
	unsigned char n;
	unsigned char dsp_buf;

	lcd_l1(0);				
	for (n=0 ;n<16 ;n++) {
		 dsp_buf = TITL199[n];		
		 lcd_dout(dsp_buf);		
	}	
	lcd_l2(0);				
	for (n=0 ;n<16 ;n++) {
		dsp_buf = TITL198[n];		
		lcd_dout(dsp_buf);		
	}
}

//**********************************************************************

//**********************************************************************
void op_switich_rightf(void)
{	
	if (rightf == 1 &&  blink_enabled == true) {		
		if( second>1 && second <=6 ) {		
			first = 1;
			second = second+1;	
			if(second	>6) second =2;
		        set_cursor_position (first,second);	
		}	
		        rightf = 0;	
     	} 
}

//**********************************************************************
void op_switich_leftf(void)
{	
	if (leftf == 1 && blink_enabled == true){
		if ( second>1 && second <=6){		
			first =1;
			second = second -1;
			if(second	<2) second =6;
		        set_cursor_position (first, second);	
		}
	                leftf =0;
	}
}
