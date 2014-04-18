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

#include <s5pc110.h>
#include <asm/io.h>
#include <asm/arch/gpio.h>

static struct s5pc110_gpio *s5pc110_gpio;

extern u32 usb_download_filesize; 
void menu_shell(void);
void main_menu_usage(void);
char awaitkey(void);
char await_check_button();
int do_menu (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]);
char check_button(void);



U_BOOT_CMD(                                                                                        
    menu, CONFIG_SYS_MAXARGS, 0, do_menu,
	"menu - display a menu,make your choose convenient\n",                                     
    " - display a menu, help you take action more faster just press the corresponding key"         
	);                                                                                


void mydelay(long num)
{
	long i,j;
	for(i=0;i<num;i++)
		for(j=0;j<65530;j++);
}

int SDautoburning()
{
	char cmd_buf[200];
	
	printf("\n");

	strcpy(cmd_buf, "drawstring 280 330 erase.chip..........");
	run_command(cmd_buf, 0);

	mydelay(50000);

	strcpy(cmd_buf, "nand erase.chip ");
	run_command(cmd_buf, 0);

	mydelay(50000);
	mydelay(50000);
	mydelay(50000);
	
	strcpy(cmd_buf, "drawstring 280 330 seting-env..........");
		strcpy(cmd_buf, "setenv bootargs noinitrd root=/dev/mtdblock2 init=/linuxrc console=ttySAC0,115200 rootfstype=yaffs2 mem=512M");
		run_command(cmd_buf, 0);
		strcpy(cmd_buf, "setenv bootcmd 'nand read 0x30007fc0 0x100000 0x500000;bootm 0x30007fc0'; save");
		run_command(cmd_buf, 0);

	mydelay(50000);
	mydelay(50000);


	strcpy(cmd_buf, "drawstring 280 330 burning-img,please-wait-20-min.........");
	run_command(cmd_buf, 0);

	mydelay(50000);

	strcpy(cmd_buf, "sdfuse flashall");
	run_command(cmd_buf, 0);

	return 1;
}

int do_menu (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])                           
{                                                          

	char cmd_buf[200];

#ifdef CONFIG_SD_AUTO_BURN
			if(SDautoburning() == 1)
			{

				strcpy(cmd_buf, "drawstring 280 330 burning-done..........");
				run_command(cmd_buf, 0);

				mydelay(50000);
				mydelay(50000);
				strcpy(cmd_buf, "drawstring 280 330 booting-the-system-form-nand..........");
				run_command(cmd_buf, 0);

				strcpy(cmd_buf, "nand read 0x30007fc0 0x100000 0x500000");
				run_command(cmd_buf, 0);
				strcpy(cmd_buf, "bootm 0x30007fc0");
				run_command(cmd_buf, 0);

			}

			return 0;                                                   
#else
			menu_shell();                                                
			return 0;                                                   
#endif
	}
	
void menu_shell(void)
{
	char c;
	char cmd_buf[200];

	while(1)
	{
		main_menu_usage();

#ifdef DEBUG_CFB_CONSOLE_LCD 
		c = await_check_button();
		if(c != 255)
			c = c+'0';
		else
			c = 0;
#else
		 c = awaitkey();
#endif

		 printf("%c\n", c);     

		switch (c)
		{
			case '1':
			{
/*  
				printf("\n");
				printf("Download the uboot into Nand flash by DNW\n");
				strcpy(cmd_buf, "dnw 0x30000000");
				run_command(cmd_buf, 0);
					
				strcpy(cmd_buf, "nand erase  0 0x80000");
				run_command(cmd_buf, 0);
					
				sprintf(cmd_buf,"nand write 0x30000000 0 %x",usb_download_filesize);
				run_command(cmd_buf, 0);
				break;
				
				*/
				break;

			}
			
			case '2':
			{
				printf("buring img to SD...\n");

				strcpy(cmd_buf, "nand erase.chip ");
				run_command(cmd_buf, 0);

				printf("\n");

				strcpy(cmd_buf, "setenv bootargs noinitrd root=/dev/mtdblock2 init=/linuxrc console=ttySAC0,115200 rootfstype=yaffs2 mem=512M");
				run_command(cmd_buf, 0);
				strcpy(cmd_buf, "setenv bootcmd 'nand read 0x30007fc0 0x100000 0x500000;bootm 0x30007fc0'; save");
				run_command(cmd_buf, 0);


				strcpy(cmd_buf, "sdfuse flashall");
				run_command(cmd_buf, 0);

				break;
			}
				
			case '3':
			{

		     	printf("\n");
		     	strcpy(cmd_buf, "led");
		     	run_command(cmd_buf, 0);                                                                                       
		    	break;              			
			}

				
			case '4':                                                                                                              
			{           
		     	printf("\n");
		     	strcpy(cmd_buf, "uart3");
		     	run_command(cmd_buf, 0);                                                                                       
		    	break;                                                                                                      
			}
				
			case '5':
			{
				strcpy(cmd_buf, "mmc part");                                                                                       
				run_command(cmd_buf, 0);
				strcpy(cmd_buf,"fatload mmc 0:1 0x33000000 music.wav");                                                                                                                         
			    run_command(cmd_buf, 0);
                                                                                                                
		     	printf("\n");
		     	strcpy(cmd_buf, "sound");
		     	run_command(cmd_buf, 0);                                                                                       
				break;
			}  	
				
			case '6':
			{

				printf("\n");
				printf("ping the ip address 192.168.0.1\n");
				strcpy(cmd_buf, "ping 192.168.0.1");
				run_command(cmd_buf, 0);

				break;
			}
				
			case '7':                    
			{                            
			    printf("\n");
			    return;                  
			    break;                   
			}   
				
			case '8':
			{

				printf("\n");
				printf("Boot the linux (NFS)\n");
				strcpy(cmd_buf, "setenv bootargs noinitrd root=/dev/nfs nfsroot=192.168.0.136:/root/rootfs,tcp ip=192.168.0.164:192.168.0.136:192.168.0.1:255.255.255.0::eth0:off init=/linuxrc console=ttySAC0,115200");
				run_command(cmd_buf, 0);
				strcpy(cmd_buf, "setenv bootcmd 'nand read 0x30007fc0 0x100000 0x500000;bootm 0x30007fc0'; save");
				run_command(cmd_buf, 0);
				strcpy(cmd_buf, "nand read 0x30007fc0 0x100000 0x500000");
				run_command(cmd_buf, 0);
				strcpy(cmd_buf, "set ipaddr 192.168.0.123;set serverip 192.168.0.136;save");
				run_command(cmd_buf, 0);
				strcpy(cmd_buf, "bootm 0x30007fc0");
				run_command(cmd_buf, 0);

				printf("\n");
				printf("Boot the linux (YAFFS2)\n");
				strcpy(cmd_buf, "setenv bootargs noinitrd root=/dev/mtdblock2 init=/linuxrc console=ttySAC0,115200 rootfstype=yaffs2 mem=512M");
				run_command(cmd_buf, 0);
				strcpy(cmd_buf, "setenv bootcmd 'nand read 0x30007fc0 0x100000 0x500000;bootm 0x30007fc0'; save");
				run_command(cmd_buf, 0);

				strcpy(cmd_buf, "nand read 0x30007fc0 0x100000 0x500000");
				run_command(cmd_buf, 0);
				strcpy(cmd_buf, "bootm 0x30007fc0");
				run_command(cmd_buf, 0);

				break;
				}    

				case 'L':                                                                                                              
				{                                                                                                                      

				     	printf("\n");
				     	strcpy(cmd_buf, "led");
				     	run_command(cmd_buf, 0);                                                                                       
				    	break;                                                                                               
				}
				case 'U':                                                                                                              
				{                                                                                                                      

				     	printf("\n");
				     	strcpy(cmd_buf, "uart3");
				     	run_command(cmd_buf, 0);                                                                                       
				    	break;                                                                                               
				}
				case 'S':                                                                                                              
				{                                                                                                                      
				     	printf("\n");
				     	strcpy(cmd_buf, "sound");
				     	run_command(cmd_buf, 0);                                                                                       
				    	break;                                                                                               
				}
				case 'B':                    
				{                            
					printf("\n");
					strcpy(cmd_buf, "dnw 0x30000000");
					run_command(cmd_buf, 0); 
				    break;                   
				}                            
				case 'W':                    
				{                            
					printf("\n");
					strcpy(cmd_buf, "tftp 0x30000000 WindowsXP");
					run_command(cmd_buf, 0); 
				    break;                   
				}                            
				                             
                         	
				
			}
		}

	}
	void main_menu_usage(void)                                         
	{                                                    
	    	printf("\n\n");
                  
		printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
		printf("^                                                                  \n");
		printf("^  [S2] Download uboot.bin kernel.img system.img to uboot          \n");
		printf("^  [S3] LED test												   \n");
		printf("^  [S4] uart test											       \n");
		printf("^  [S5] sound test												   \n");
		printf("^  [S6] ping test					                               \n");
		printf("^-----------------------------------------------                   \n");
		printf("^  [S7] quit to command					                               \n");
		printf("^  [S8] Boot system form nand									   \n");

//		printf("^  [?] Boot the linux (NFS)                                        ^\n");
/*		printf("^                                                                  ^\n");
		printf("^ TEST FOR Webee                                                   ^\n");
		printf("^   |  |  |  |                                                     ^\n");
		printf("^   |  |  |  |                                                     ^\n");
		printf("^   V  V  V  V                                                     ^\n");
		printf("^                                                                  ^\n");
		printf("^  [U] uart3                                                       ^\n"); 
		printf("^  [L] led+key+bee                                                 ^\n"); 
		printf("^  [W] dm9000                                                      ^\n"); 
		printf("^  [B] usb_device                                                  ^\n"); 
		printf("^  [S] sound                                                       ^\n"); 
*/		printf("^                                                                  ^\n");
	   	printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
		printf("\nEnter your selection: ");   	                                                                                                     
	}                      


char check_button()
{
	char key_value = 1;
	char key_value2 = 1;
	char i;
	s5pc110_gpio = (struct s5pc110_gpio *)S5PC110_GPIO_BASE;
	for(i=0;i<4;i++)
	{
		s5p_gpio_direction_input(&s5pc110_gpio->h2, i);
		key_value = s5p_gpio_get_value(&s5pc110_gpio->h2, i);

		s5p_gpio_direction_input(&s5pc110_gpio->h3, i);
		key_value2 = s5p_gpio_get_value(&s5pc110_gpio->h3, i);
		if(0 == key_value) 
		{
			key_value = 1;
			return i+1;
			break;
		}
		if(0 == key_value2)
		{
			key_value2 = 1;
			return i+5;
			break;
		}
	}
	return 255;
}

char await_check_button()
{
	char c;
	int i;

	while(1)
	{
		if(check_button() != 255)
			return check_button();
	}
	return 0;
}


	/*keyboard scan*/
char awaitkey(void)     
{
                                                              
    int i;
                                                     
    char c;
                                  
    while (1) 
	{
                                            
		if (tstc()) /* we got a key press	*/
              
		return getc();
                                 
    }
                                                                                       
    return 0;                                               
}

