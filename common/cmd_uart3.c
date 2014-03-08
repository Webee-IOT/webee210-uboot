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
#define GPA0CON 		( *((volatile unsigned long *)0xE0200000) )		
#define GPA1CON 		( *((volatile unsigned long *)0xE0200020) )

// UART相关寄存器
#define ULCON0 			( *((volatile unsigned long *)0xE2900C00))		
#define UCON0 			( *((volatile unsigned long *)0xE2900C04) )
#define UFCON0 			( *((volatile unsigned long *)0xE2900C08) )
#define UMCON0 			( *((volatile unsigned long *)0xE2900C0C) )
#define UTRSTAT0 		( *((volatile unsigned long *)0xE2900C10) )
#define UERSTAT0 		( *((volatile unsigned long *)0xE2900C14) )
#define UFSTAT0 		( *((volatile unsigned long *)0xE2900C18) )
#define UMSTAT0 		( *((volatile unsigned long *)0xE2900C1C) )
#define UTXH0 			( *((volatile unsigned long *)0xE2900C20) )
#define URXH0 			( *((volatile unsigned long *)0xE2900C24) )
#define UBRDIV0 		( *((volatile unsigned long *)0xE2900C28) )
#define UDIVSLOT0 		( *((volatile unsigned long *)0xE2900C2C) )
#define UINTP 			( *((volatile unsigned long *)0xE2900C30) )
#define UINTSP 			( *((volatile unsigned long *)0xE2900C34) )
#define UINTM 			( *((volatile unsigned long *)0xE2900C38) )

#define UART_UBRDIV_VAL3		35
#define UART_UDIVSLOT_VAL3	0x1
static void hareware_init(void)
{
		// 1 配置引脚用于RX/TX功能
	GPA0CON = 0x22222222;
	GPA1CON = 0x2222;

	// 2 设置数据格式等
	// 使能fifo
	UFCON0 = 0x1;
	// 无流控
	UMCON0 = 0x0;
	// 数据位:8, 无校验, 停止位: 1
	ULCON0 = 0x3;
	// 时钟：PCLK，禁止中断，使能UART发送、接收
	UCON0  = 0x5;  				

	// 3 设置波特率
	UBRDIV0 = UART_UBRDIV_VAL3;
	UDIVSLOT0 = UART_UDIVSLOT_VAL3;
}
/*
char getc(void)
{
	// 如果RX FIFO空，等待
	while (!(UTRSTAT0 & (1<<0)));
	// 取数据
	return URXH0;                   	
}
*/
static void delay(unsigned int count)
{

	while(count--);
}
// 发送一个字符
void putc3(char c)
{
	// 如果TX FIFO满，等待
	while (!(UTRSTAT0 & (1<<2)));
	// 写数据
	UTXH0 = c;                      	
}
void uart3_test(void)
{
	hareware_init();
	while(1)
	{
		putc3('w');
		putc3('e');
		putc3('b');
		putc3('e');
		putc3('e');
		putc3('\r');
		putc3('\n');
//		delay(0x10000000);	
	}
}
U_BOOT_CMD(                                                                                                                                                                     
    uart3, CONFIG_SYS_MAXARGS, 0, uart3_test,                                                                                                                                      
    "led bee key\n",                                                                                                                      
    " just input the 'lcd' command\n"                                                                                      
);                                                                                
                                            
                                        

