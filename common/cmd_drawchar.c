#include <common.h>
#include <watchdog.h>
#include <command.h>
#include <video.h>
#include <image.h>
#include <asm/byteorder.h>
#include <asm/io.h>

int video_point_x=0;
int video_point_y=0;

int do_drawstring(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
//	video_puts("Hello world");
	if(argc < 4)
	{
		printf("argc is %d \n",argc);
		printf("Usage: drawchar [xpoint] [ypoint] [string]\n");
	}
	else	
	{
		video_point_x = simple_strtol(argv[1],NULL,10);
		video_point_y = simple_strtol(argv[2],NULL,10);
		printf("video_point_x is %d,video_point_y is %d\n",video_point_x,video_point_y);
		video_draw_string(video_point_x,video_point_y,argv[3]);
		return 1;
	}
}



U_BOOT_CMD(                                                                                                                                                                     
    drawstring,4, 1, do_drawstring,                                                                                                                                      
    "Usage: drawchar [xpoint] [ypoint] [string]\n",                                                                                                                      
    " just put the string on the lcd\n"                                                                                      
	);                                                                                
                                            
                                        

