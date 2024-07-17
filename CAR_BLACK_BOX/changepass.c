#include <xc.h>
#include "changepass.h"
#include "matrix_keypad.h"
#include "clcd.h"
#include "pass.h"

extern unsigned char str1[9];
void changepass()
{
    unsigned char key=0,input='0',input2='0',pass1[9]="00000000",pass2[9]="00000000";
    unsigned int j=0,k=0,j1=0,k1=0,flag=0,blink=0,delay=0,flag1=0;

    CLEAR_DISP_SCREEN;
    while(1)
    {
	key=read_switches(STATE_CHANGE);
	if(input < '8')//to read the switch upto 8 input
	{
	    clcd_print("change Password  ", LINE1(0));
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
		pass1[j]='1';
		j++;
		clcd_print("*", LINE2(k));
		blink = 0;
		k++;
	    }
	    if(key == MK_SW9) //switch 2 is pressed store 0 as str and display *
	    {
		input++;
		pass1[j]='0';
		j++;
		clcd_print("*", LINE2(k));
		k++;
	    }
	}
	else if(input2 < '8')//to read the switch upto 8 input
	{
	    if ( input == '8' )
	    {
		clcd_print("               ", LINE2(0));
		blink = 0;
		input = '9';
	    }
	    clcd_print("confirm Password  ", LINE1(0));
	    if(blink++ < 500) //to blink the cursour
	    {
		clcd_print(" ",LINE2(k1));
	    }
	    else //to blink the _ input want to given
	    {
		if(blink == 1000)//for upto 1000 then set as 0
		    blink = 0;
		clcd_print("_",LINE2(k1));
	    }
	    if(key == MK_SW8) //switch 1 is pressed store 1 in str and display *
	    {
		input2++;
		pass2[j1]='1';
		j1++;
		clcd_print("*", LINE2(k1));
		k1++;
		blink = 0;
	    }
	    if(key == MK_SW9) //switch 2 is pressed store 0 as str and display *
	    {
		input2++;
		pass2[j1]='0';
		j1++;
		clcd_print("*", LINE2(k1));
		k1++;
		blink = 0;
	    }
	}

	if(input2 == '8') //if input 2 is 8
	{
	    flag1 = 1;
	    input2 ='9';
	}

	if(flag1) //to check the both input is match
	{
	    for(int i=0;i<8;i++)
	    {
		if(pass1[i] != pass2[i]) //if not match change the input value and break the loop
		{
		    flag=1;
		    clcd_print(" TRY AGAIN      ",LINE2(0));
		    input = '0';
		    input2 = '0';
		    k1=0;
		    k=0;
		    blink = 0;
		    flag1 = 0;
		    for ( unsigned int i = 1000;i--;);
		    clcd_print("                ",LINE2(0));
		    break;
		}
	    }
	    if(flag != 1) //if match store to str1 and return to enter password
	    {
		for( int i= 0; i<8;i++ )
		    str1[i] = pass1[i];
		clcd_print("   SUCCESS      ",LINE2(0));
		if(delay++ == 500)
		{
		    return;
		}
	    }

	}
    }
}

