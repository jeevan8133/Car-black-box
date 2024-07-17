#include <xc.h>
#include "clcd.h"
#include "EEP.h"
#include "view.h"
#include "matrix_keypad.h"

void view(void)
{
    unsigned char display[17] ,display1[17],read=0x00,key,flag=1;
    unsigned int count=0,count1=0;
    display[16]='\0';
    display1[16]='\0';
    while(1)
    {
	key=read_switches(LEVEL_CHANGE);
	for ( int i = 0; i< 1500;i++); //for bouncing effect
	if(flag == 1) //for scroll down
	{
	    for (int i =0 ;i<16;i++) //to store in display array
	    {
		if(read == 0xA0) //if read address is match then store it in first position
		    read = 0x00;
		display[i]=readEEP(read);
		display1[i]=readEEP(read+0x10);
		read++;
	    }
	    //to print in clcd
	    clcd_print(display,LINE1(0));
	    clcd_print(display1,LINE2(0));
	    flag=0;
	}
	if(flag == 2) //for scroll up
	{
	    for (int i =15 ;i>=0;i--) //to store in displayarray
	    {
		read--;
		display1[i]=readEEP(read);
		display[i]=readEEP(read+0x10);
		if(read == 0x00) //address is match then change to last
		    read = 0xA0;
	    }
	    //to print in clcd
	    clcd_print(display,LINE1(0));
	    clcd_print(display1,LINE2(0));
	    flag=0;
	}
	if(key == MK_SW8) //sw8 is pressed
	{
	    count++;
	    if ( count == 200 )
		count = 150;
	}
	else if(count < 200 && count != 0) //for state change to scroll down
	{
	    count=0;
	    flag=1;
	}
	else 
	    count=0;

	if(key == MK_SW9) //if sw9 is pressed
	{
	    count1++;
	}
	else if(count1 < 500 && count1 != 0) //for state change to scroll up
	{
	    count1=0;
	    flag=2;
	}
	else if (count1 > 500) //for level change to return main
	{
	    return;
	}
	else
	    count1=0;
    }
}
