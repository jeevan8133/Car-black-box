

#include <xc.h>
#include "main.h"
#include "clcd.h"
#include "ds1307.h"
#include "i2c.h"
#include "matrix_keypad.h"
#include "adc.h"
#include "pass.h"
#include "EEP.h"
#include "changepass.h"
#include "uart.h"
#include "settime.h"

                                                        //global declaration of the variable to access another file
extern unsigned char set[9];
unsigned char clock_reg[3];
unsigned char time[9];
unsigned char key;
unsigned int flag1=0,corrpass=0;
static void get_time(void)                                //for declaration the time in rtc 
{
    clock_reg[0] = read_ds1307(HOUR_ADDR);
    clock_reg[1] = read_ds1307(MIN_ADDR);
    clock_reg[2] = read_ds1307(SEC_ADDR);

    if (clock_reg[0] & 0x40)
    {
	time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
	time[1] = '0' + (clock_reg[0] & 0x0F);
    }
    else
    {
	time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
	time[1] = '0' + (clock_reg[0] & 0x0F);
    }
    time[2] = ':';
    time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
    time[4] = '0' + (clock_reg[1] & 0x0F);
    time[5] = ':';
    time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
    time[7] = '0' + (clock_reg[2] & 0x0F);
    time[8] = '\0';
}
void init_config(void)                                             //initialise the thing to use
{
    init_clcd();
    init_i2c();
    init_ds1307();
    init_matrix_keypad();
    init_adc();
    init_uart();
    TRISB0 = 0X00;
    ADCON1=0xFF;
    RB0=0;
}
void main(void)
{
    init_config();
    unsigned long key,c=0,c1=0;
    unsigned char k = '0';
    unsigned int a,sc=0;
    unsigned char add=0x00;
    unsigned char store[17],speed[4]="000",clear[9];
    unsigned char str[6][17]={"view log        ","download log    ","clear log       ","change password ","set time        ","             "};
    unsigned int option=1,j=0,option1=0,flag2=1,flag3=0,save=0,cl=1;
    store[8]  = ' ';
    store[9]  = ' ';
    store[10]  = 'O';
    store[11]  = 'N';
    store[12] = ' ';
    store[16] ='\0';
    while (1)
    {
	if(option)
	{
	    get_time();                                                 //to display the time
	    clcd_print("TIME   EVENT  SP",LINE1(0));
	    a = read_adc(CHANNEL4)/10.23;                              //for potentiometer
                                                                   //store in array
	    store[15] = (a%10)+'0';
	    store[14] = (a%100/10)+'0';
	    store[13] = (a/100)+'0';

	    key = read_switches(STATE_CHANGE);
	    if( key != ALL_RELEASED )                                   //for bouncing effect
		for(unsigned int i=1000;i--;);
	    if(key == MK_SW1)                                           //for switch 1 to increase the gear
	    {
		save = 1;
		store[10]='G';
		if ( store[11] > '4' )
		{
		    if ( k == '5' )
			k = 'R';
		    else if ( store[11] == 'N' )
			k = '1';
		}
		else
		    k++;
		store[11] = k;                                                  //store the k value
	    }
	    if(key  == MK_SW2)                                           //switch2 to decrease the gear
	    {
		save=1;
		store[10]='G';
		k--;
		if ( k > '0')
		{
		    store[11]=k;                                            //store the k value 
		}
		if(k == '0' || k == 'Q' )
		{
		    if ( k == 'Q' )
		    {
			k = '5';
			store[11] = k;
		    }
		    else
		    {
			k = '1';
			store[11]='N';
		    }
		}
	    }
	    if(key == MK_SW3)                                        //switch3 to change collision
	    {
		save=1;
		store[10]=' ';
		store[11]='C';
	    }
	    if ( key == MK_SW8)                                       //to access the password
	    {
		pass();
		for ( int i = 0; i< 2000; i++ );
		option = 0;                                           //change to sw1,sw2,sw3 are not used
		clcd_print(str[sc], LINE1(1));
		clcd_print(str[sc+1], LINE2(1));
		clcd_putch('*', LINE1(0));
		clcd_putch(' ', LINE2(0));
		key = ALL_RELEASED;                                     //to released all key
	    }
	    for( int i = 0; i< 8 ; i++ )                          //time into store array
		store[i]=time[i];
	    if(save==1)                                           //if sw1,2,3 are pressed
	    {
		if ( add == 0xA0 )                                   //ifaddress is more the change to zero
		    add= 0x00;
		for(int i=0;i<16;i++)                                //it is store the array
		{
		    writeEEP(add++,store[i]);
		}
		save=0;
	    }
	    if ( option )
		clcd_print(store,LINE2(0));
	}
	if(option == 0)                                          //it display after the password
	{
	    key = read_switches(LEVEL_CHANGE);

	    if ( key != ALL_RELEASED )                        //to avoid bouncing effect
		for ( int i = 0; i< 1000 ;i++ );

	    if (key == MK_SW8)                                  //sw8 is pressed
	    {
		c++;
	    }
	    else if (c < 300 && c != 0)                        //if it is state change
	    {
		if (sc >= 1)                                      //scroll the str decrease
		    sc--;
		clcd_print(str[sc], LINE1(1));
		clcd_print(str[sc+1], LINE2(1));
		clcd_putch('*', LINE1(0));
		clcd_putch(' ', LINE2(0));
		c = 0;
	    }
	    else if (c > 300)                                //if it level change
	    {
		if (sc == 0)                                      //if index is 0
		{
		    view();                                       //to view the eeprom
		    clcd_print(str[sc], LINE1(1));
		    clcd_putch('*', LINE1(0));
		    clcd_putch(' ', LINE2(0));
		    clcd_print(str[sc+1], LINE2(1));
		}
		else if (sc == 1)                                  //if index is 1
		{
		    if(cl)                                          //for download
		    {
			download();
		    }
		    else                                             //for clear
		    {
			puts(clear);
			puts("  CL ");
			puts(store+13);
			puts("\n\r");
		    }
		                                                     //at end of minicom display for download
		    puts(time);
		    puts("  DL ");
		    puts(store+13);
		    puts("\n\r");
		    clcd_print("DOWNLOAD DONE   ",LINE1(0));
		    clcd_print("SUCCESSFULL     ",LINE2(0));
		    for( unsigned long int i =100000;i>0;i-- );          //for display the download done
		    clcd_print(str[sc], LINE1(1));
		    clcd_putch('*', LINE1(0));
		    clcd_putch(' ', LINE2(0));
		    clcd_print(str[sc+1], LINE2(1));
		}
		else if (sc == 2)                                   //for clear the minicom
		{
		    cl = 0;                                        //for access the download else
		    clear[8]='\0';
		    add = 0x00;
		    for(int i = 0;i<9;i++)                         //To store the time
			clear[i]=time[i];
		                                                   //display the clcd clear log
		    clcd_print("CLEAR LOG SUCC  ",LINE1(0));
		    clcd_print("                ",LINE2(0));
		    for( unsigned long int i =100000;i>0;i-- );
		    clcd_print(str[sc], LINE1(1));
		    clcd_putch('*', LINE1(0));
		    clcd_putch(' ', LINE2(0));
		    clcd_print(str[sc+1], LINE2(1));
		}
		else if (sc == 3)                                   //for change password
		{
		    changepass();
		    pass();
		    clcd_print(str[sc], LINE1(1));
		    clcd_print(str[sc+1], LINE2(1));
		    clcd_putch('*', LINE1(0));
		    clcd_putch(' ', LINE2(0));
		}
		else if(sc == 4)                                       //for set time
		{
		    settime();                                              //for change time
		    unsigned char value = '0';
		                                                                   //for store the value in rtc
		    value = ( ( ( (set[0]-'0') & 15 ) << 4) | ( (set[1]-'0') & 15 ) );
		    write_ds1307(HOUR_ADDR,value);
		    value = '0';
		    value = ( ( ( (set[3]-'0') & 15 ) << 4) | ( (set[4]-'0') & 15 ) );
		    write_ds1307(MIN_ADDR,value);
		    value = '0';
		    value = ( ( ( (set[6]-'0') & 15 ) << 4) | ( (set[7]-'0') & 15 ) );
		    write_ds1307(SEC_ADDR,value);
		    option=1;                                        //change option to access sw1,2,3
		}
		c=0;
	    }
	    else
		c = 0;

	    if ( key == MK_SW9)                                  //If switch 9 is pressed
	    {
		c1++;
		if ( c1 == 200 )
		    c1 = 150; 
	    }
	    else if (c1 < 200 && c1 != 0)                      //for state change to scroll
	    {
		if (sc < 4)
		    sc++;
		clcd_print(str[sc], LINE1(1));
		clcd_print(str[sc+1], LINE2(1));
		clcd_putch('*', LINE1(0));
		clcd_putch(' ', LINE2(0));
		c1 = 0;
	    }
	    else
		c1 = 0;
	}
    }
}
