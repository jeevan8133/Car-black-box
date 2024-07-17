#include <xc.h>
#include "pass.h"
#include "matrix_keypad.h"
#include "clcd.h"
unsigned char str1[9]="00000000";
int pass(void)
{
    unsigned char key;
    unsigned char input='0',try='5';
    unsigned int i=0,j=0,flag=1,m=0,t=0,k=0,blink=0,delay=0,y=0;
    unsigned char str2[9]="00000000";
    clcd_print("Enter Password  ", LINE1(0));//clcd line 1 is print
    clcd_print("                ",LINE2(0));
    while(1)
    {
	if(try > '0') //try to access the 5 time of wrong password
	{
	    key = read_switches(STATE_CHANGE);//read the input using switches
	    if(input < '8')//to read the switch upto 8 input
	    {
		if(blink++ < 500) //to blink the cursour
		{
		    clcd_print(" ",LINE2(k));
		}
		else //to blink the _ input want to given
		{
		    if(blink == 1000)//for upto 1000 then set as 0
			blink = 0;
		    clcd_print("_",LINE2(k));
		}
		if(key == MK_SW8) //switch 1 is pressed store 1 in str and display * 
		{
		    input++;
		    str2[j]='1';
		    j++;
		    clcd_print("*", LINE2(k));
		    k++;
		}
		if(key == MK_SW9) //switch 2 is pressed store 0 as str and display *
		{
		    input++;
		    str2[j]='0';
		    j++;
		    clcd_print("*", LINE2(k));
		    k++;
		}
	    }
	    else //input is 8
	    {
		for( int i = 0; i<8; i++ )//compare the input and correct password
		{
		    if ( str1[i] != str2[i] ) //not match change flag as 0
		    {
			flag = 0;
		    }
		}
		if ( flag ) //str are match
		{
		    try='0';
		}
		else //str not match
		{
		    if ( try > '1' ) //to initimate the attempt
		    {
			if ( t++ < 1000 ) //this block display on clcd for certain time
			{
			    clcd_print("TRY AGAIN",LINE2(0));
			    clcd_putch(try-1,LINE2(10));
			    clcd_print("LEFT",LINE2(12));
			}
			else 
			{
			    if ( m++ == 500 )//this for change the value of str,reduce try initimate led
			    {
				flag = 1;
				j=0;
				k=0;
				input = '0';
				t = 0;
				m=0;
				RB0 = 0;
				try--;
			    }
			    clcd_print("                  ",LINE2(0));//after certain time clcd print blank
			}
			if(delay++ == 100) //forblink led for certain time
			{
			    RB0=!RB0;
			    delay=0;
			}
		    }
		    else//try attend the last
		    {
			try = '0';
			flag = 0;
		    }
		}
	    }
	}
	else //try attend last time
	{
	    if ( flag ) //flag is led is off print this 
	    {
		clcd_print("    SUCCESS    ",LINE2(0));
		RB0 = 0;
		for(long int i=10000;i>0;i--);
		break;
	    }
	    else //flag is 0 led is on print this
	    {
		clcd_print("    FAILURE    ",LINE2(0));
		RB0 = 1;
	    }
	}
	for(int i = 1000; i--; );
    }
}
