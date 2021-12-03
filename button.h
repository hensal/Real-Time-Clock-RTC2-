#include <stdint.h>
void IO_Init(void);
void op_switich_in(void);
unsigned char op_swin_1d(void);

int display1(void);

void dispset_titl(void);
int dispset_titl1(void);
int dispset_titl2(void);
void dispset_titl3(void);
void IntToStr(unsigned int intval);
void serial_uart(void); 
int set_cursor_position (uint_fast8_t row, uint_fast8_t col);
//int write_string(uint_fast8_t row, uint_fast8_t col, unsigned const char  *str);

extern unsigned char 	modef;
extern unsigned char 	upf;
extern unsigned char 	downf;
extern unsigned char 	leftf;
extern unsigned char 	rightf;
extern unsigned char 	setf;



