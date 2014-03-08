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
#include "wb_lcd.h"



U_BOOT_CMD(                                                                                                                                                                     
    lcd, CONFIG_SYS_MAXARGS, 0, WB_lcd_init_and_display,                                                                                                                                      
    "lcd - display a photo\n",                                                                                                                      
    " just input the 'lcd' command\n"                                                                                      
	);                                                                                
                                            
                                        

