#include <common.h>
#include <watchdog.h>
#include <command.h>
#include <image.h>
#include <malloc.h>
#include <u-boot/zlib.h>
#include <bzlib.h>
#include <environment.h>
#include <lmb.h>
#include <linux/ctype.h>
#include <asm/byteorder.h>
#include <linux/compiler.h>  

// LED
#define 	GPJ2CON 	(*(volatile unsigned long *) 0xE0200280)
#define 	GPJ2DAT		(*(volatile unsigned long *) 0xE0200284)

// KEY
#define 	GPH2CON 	(*(volatile unsigned long *) 0xE0200C40)
#define 	GPH2DAT		(*(volatile unsigned long *) 0xE0200C44)
#define 	GPH3CON 	(*(volatile unsigned long *) 0xE0200C60)
#define 	GPH3DAT		(*(volatile unsigned long *) 0xE0200C64)

//bee
#define GPD0CON		(*(volatile unsigned long *)0xE02000A0)
#define GPD0DAT		(*(volatile unsigned long *)0xE02000A4)

void buzzer_on(void)
{
	GPD0DAT |= 1<<0;
	GPJ2CON = 0x1111;
	GPH2CON = 0;
	GPH3CON = 0;
}

void buzzer_off(void)
{
	GPD0DAT &= ~(1<<0);	
}

void delay(unsigned long count)
{
	volatile unsigned long i = count;
	while (i--)
		;
}

void hareware_init(void)
{

	GPD0CON |= 1<<0;

}
void led_test(void)
{
	hareware_init();
//	while(10)				
//	{
		buzzer_on();		
		delay(0x50000);
		buzzer_off();		
		delay(0x50000);
//	}
	while(1)
	{
		if(((GPH2DAT&0xf) != 0xf)||((GPH3DAT&0xf) != 0xf))
		{
			if(!(GPH2CON&1))
			GPJ2DAT = 0x1;
			if(!(GPH2CON&(1<<1)))
			GPJ2DAT = 0x2;
			if(!(GPH2CON&(1<<2)))
			GPJ2DAT = 0x3;
			if(!(GPH2CON&(1<<3)))
			GPJ2DAT = 0x4;

			if(!(GPH3CON&1))
			GPJ2DAT = 0x5;
			if(!(GPH3CON&(1<<1)))
			GPJ2DAT = 0x6;
			if(!(GPH3CON&(1<<2)))
			GPJ2DAT = 0x7;
			if(!(GPH3CON&(1<<3)))
			GPJ2DAT = 0x8;
			
		}	
	}	
	
}

U_BOOT_CMD(                                                                                                                                                                     
    led, CONFIG_SYS_MAXARGS, 0, led_test,                                                                                                                                      
    "led bee key\n",                                                                                                                      
    " just input the 'lcd' command\n"                                                                                      
	);                                                                                
                                            
                                        

