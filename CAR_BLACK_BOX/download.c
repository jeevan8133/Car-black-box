#include <xc.h>
#include "uart.h"
#include "EEP.h"
#include "clcd.h"

void download()
{
    unsigned char less=0x00,display[17];
    display[16]='\0';

    while(1)
    {
	if(less != 0xA0) //to check with end of address
	{
	    for (int i =0 ;i<16;i++) //store it in to array
	    {
		display[i]=readEEP(less++);
	    }
	    puts(display); //display in minicom
	    puts("\n\r");
	}
	else
	    return;
    }
}

