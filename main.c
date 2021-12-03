#include "sfr.h"
#include "common.h"
#include "config.h"
#include "system.h"
#include "timer.h"
#include "watchdogtimer.h"
#include "panel.h"
#include "lcd_con.h"
#include "button.h"
#include <stdint.h>
#include <stdbool.h>

#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "r_cg_userdefine.h"

#include "r_cg_macrodriver.h"
#include "r_cg_port.h"
#include "r_cg_rtc.h"
#include "r_cg_serial.h"
#include "Config_PORT.h"
#include "Config_RTC.h"
#include "LCM_driver.h"
#include "r_cg_userdefine.h"

void IO_Init(void);	
void op_switich_in(void);		
void op_switich_mdoef(void);
void op_switich_modef(void);
void op_switich_upf(void);
void op_switich_downf(void);
int op_switich_leftf(void);
void op_switich_rightf(void);
void op_switich_setf(void);
void op_switich_setff(void);
int display1(void);
void R_Systeminit(void);
void serial_uart(void); 
//int write_string(uint_fast8_t row, uint_fast8_t col, unsigned const char  *str);
void op_switich_upf1(void);

unsigned char 	modef 	= 0;		
unsigned char 	upf 	= 0;
unsigned char 	downf 	= 0;
unsigned char 	leftf 	= 0;
unsigned char 	rightf 	= 0;
unsigned char 	setf 	= 0;

unsigned char dpcnt[4];
//************************************************************************
//* Function Name: main
//* Description  : This function implements main function.?i???C?Åg?O?Åh?j
//* Arguments    : None
//* Return Value : None
//*************************************************************************
void main(void);
void main_loop(void);

void main(void) {
	
        /* Hardware Initiate */   
	hdwinit(); 	
	 
	/* Panel Initiate */
	panel_init0();	
	
	/*LCD Initiate */
	lcd_init();
	
	/*LCD display */
	// display1();	
	
	/* Main Loop */
	main_loop();		
       
	{
        if (0U == PORF)
        {
            /* Initialize RTC only at Power-On-Reset. */
            R_Config_RTC_Create();
            r_rtc_init_current_time();            
            PORF = 1U;
            R_Config_RTC_Start();
        }
        else
        {
            /* Setting for INTRTC. */
            /* Supplies input clock */
            RTCEN = 1U;
            /* Set INTRTC Low priority */
            RTCPR1 = 1U;
            RTCPR0 = 1U;
            /* Clear INTRTC interrupt flag */
            RTCIF = 0U;
            /* Enable INTRTC interrupt */
            RTCMK = 0U;
        }         

        /* Wait an interrupt. */
        while (1U)
        {
            if (r_rtc_is_constperiod_flag_on())
            {
                RTCMK = 1U;
                r_rtc_display_current_time();
                r_rtc_clear_constperiod_flag();
                RTCMK = 0U;
            }
	        panel();		
		
		WDT_Reset();		
		
		dispset_count();
		
		op_switich_in();
		
	        op_switich_mdoef();
		
		op_switich_upf();
		
		op_switich_downf();
		
		op_switich_leftf();
		
		op_switich_rightf();
		
		op_switich_setf();		 
        }
	
    }
}

void main_loop(void)
{   
   /*while(1U) {   
	   
	        //serial_uart();			
	        
		panel();		
		
		WDT_Reset();		
		
		dispset_count();
		
		op_switich_in();
		
	        op_switich_mdoef();
		
		op_switich_upf();
		
		op_switich_downf();
		
		op_switich_leftf();
		
		op_switich_rightf();
		
		op_switich_setf();		 
	}   	 */     	
}
