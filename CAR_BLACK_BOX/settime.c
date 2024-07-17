#include <xc.h>
#include "matrix_keypad.h"
#include "clcd.h"

unsigned char set[9]="00:00:00";
void settime(void)
{
    unsigned int delay=0,key,flag=1,blink=0;
    int k=0,l=0,m=0;
    clcd_print("                ",LINE2(0));
    clcd_print(set,LINE2(0));
    clcd_print("   SET _ TIME   ",LINE1(0));
    while(1)
    {
	if(flag == 1)
	{
	    if(blink++ < 1000) //for " " is occur
	    {
		clcd_print("  ",LINE2(6));
	    }
	    else // value is occur
	    {
		if(blink == 2000)
		{
		    blink=0;
		}
		clcd_print(set,LINE2(0));
	    }

	}
	if(flag == 2)
	{
	    if(blink++ < 1000) //for " " is occur
	    {
		clcd_print("  ",LINE2(3));
	    }
	    else //  value is occur
	    {
		if(blink == 2000)
		{
		    blink=0;
		}
		clcd_print(set,LINE2(0));
	    }

	}
	if(flag == 3)
	{
	    if(blink++ < 1000) //for " " is occur
	    {
		clcd_print("  ",LINE2(0));
	    }
	    else //value is occur
	    {
		if(blink == 2000)
		{
		    blink=0;
		}
		clcd_print(set,LINE2(0));
	    }

	}

	key = read_switches(STATE_CHANGE);
	if(key == MK_SW11) //for sw11
	{
	    if(flag == 1) //increment the second
	    {
		k++;
		if(k==59)
		    k=0;
		set[7] = (k%10)+48;
		set[6] = (k/10)+48;
	    }
	    if(flag == 2) //increment the minute
	    {
		l++;
		if(l == 59)
		    l = 0;
		set[4] = (l%10)+'0';
		set[3] = (l/10)+'0';
	    }
	    if(flag == 3) //for increment the hour
	    {
		m++;
		if(m == 24)
		    m = 0;
		set[1] = (m%10)+'0';
		set[0] = (m/10)+'0';
	    }
	}
	if(key == MK_SW12) //sw12 to change the flag
	{
	    clcd_print(set,LINE2(0));
	    flag++;
	    blink = 0;
	    if(flag == 4)
		flag = 1;
	}
	if(key == MK_SW9)
	    return;
    }
}
