/*
* (C) Copyright 2006 by OpenMoko, Inc.
* Author: Harald Welte <laforge@openmoko.org>
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 2 of
* the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston,
* MA 02111-1307 USA
*/
#include <common.h>
#include <video_fb.h>
#include <s5pc110.h>
#include "videomodes.h"
#include <asm/arch/regs-fb.h> 
#if defined(CONFIG_VIDEO_S5PV210)
/*
* Export Graphic Device
*/
GraphicDevice smi;
#define VIDEO_MEM_SIZE 0x200000  /* 240x320x16bit = 0x25800 bytes */
extern void board_video_init(GraphicDevice *pGD);
/*******************************************************************************
*
* Init video chip with common Linux graphic modes (lilo)
*/
void *video_hw_init (void)
{
	S5PC11X_FB * const fb = S5PC11X_GetBase_FB();
	GraphicDevice *pGD = (GraphicDevice *)&smi;
	int videomode;
	unsigned long t1, hsynch, vsynch;
	char *penv;
	int tmp, i, bits_per_pixel;
	struct ctfb_res_modes *res_mode;
	struct ctfb_res_modes var_mode;
	int clkval;
	/* Search for video chip */
	printf("Video: ");
	tmp = 0;
	videomode = CFG_SYS_DEFAULT_VIDEO_MODE;
	/* get video mode via environment */
	if ((penv = getenv ("videomode")) != NULL) {
		/* deceide if it is a string */
		if (penv[0] <= '9') {
			videomode = (int) simple_strtoul (penv, NULL, 16);
			tmp = 1;
		}
	} else {
		tmp = 1;
	}
	
	if (tmp) {
		/* parameter are vesa modes */
		/* search params */
		for (i = 0; i < VESA_MODES_COUNT; i++) {
			if (vesa_modes[i].vesanr == videomode)
			break;
		}
		if (i == VESA_MODES_COUNT) {
			printf ("no VESA Mode found, switching to mode 0x%x ", CFG_SYS_DEFAULT_VIDEO_MODE);
			i = 0;
		}
		res_mode = (struct ctfb_res_modes *)&res_mode_init[vesa_modes[i].resindex];
		bits_per_pixel = vesa_modes[i].bits_per_pixel;
	} else {
		res_mode = (struct ctfb_res_modes *) &var_mode;
		bits_per_pixel = video_get_params (res_mode, penv);
	}

	/* calculate hsynch and vsynch freq (info only) */
	t1 = (res_mode->left_margin + res_mode->xres + res_mode->right_margin + res_mode->hsync_len) / 8;
	t1 *= 8;
	t1 *= res_mode->pixclock;
	t1 /= 1000;
	hsynch = 1000000000L / t1;
	t1 *= (res_mode->upper_margin + res_mode->yres + res_mode->lower_margin + res_mode->vsync_len);
	t1 /= 1000;
	vsynch = 1000000000L / t1;

	/* fill in Graphic device struct */ 
	sprintf (pGD->modeIdent, "%dx%dx%d %ldkHz %ldHz", res_mode->xres, res_mode->yres, bits_per_pixel, (hsynch /
	1000), (vsynch / 1000));
	printf ("%s\n", pGD->modeIdent);
	pGD->winSizeX = res_mode->xres;
	pGD->winSizeY = res_mode->yres;
	pGD->plnSizeX = res_mode->xres;
	pGD->plnSizeY = res_mode->yres;

	switch (bits_per_pixel) {
		case 8:
			pGD->gdfBytesPP = 1;
			pGD->gdfIndex = GDF__8BIT_INDEX;
			break;
		case 15:
			pGD->gdfBytesPP = 2;
			pGD->gdfIndex = GDF_15BIT_555RGB;
			break;
		case 16:
			pGD->gdfBytesPP = 2;
			pGD->gdfIndex = GDF_16BIT_565RGB;
			break;
		case 24:
			pGD->gdfBytesPP = 3;
			pGD->gdfIndex = GDF_24BIT_888RGB;
			break;
		case 32:
			pGD->gdfBytesPP = 4;
			pGD->gdfIndex = GDF_32BIT_X888RGB;
			break;
	}
	
#if 0
	/* statically configure settings */
	pGD->winSizeX = pGD->plnSizeX = 240;
	pGD->winSizeY = pGD->plnSizeY = 320;
	pGD->gdfBytesPP = 2;
	pGD->gdfIndex = GDF_16BIT_565RGB;
#endif
	pGD->frameAdrs = LCD_VIDEO_ADDR;
	pGD->memSize = VIDEO_MEM_SIZE;
	
	/* Clear video memory */
	memset((void *)pGD->frameAdrs, 0x00, pGD->memSize);
	board_video_init(pGD);
	t1 = res_mode->pixclock;
	t1 /= 1000;
	t1 = 1000000000L / t1;
	clkval = (CONFIG_SYS_VIDEO_VCLOCK_HZ / t1) - 1;
	
	/* 配置视频输出格式和显示使能/禁止。*/
	fb->VIDCON0 = ( S3C_VIDCON0_VIDOUT_RGB | S3C_VIDCON0_PNRMODE_RGB_P |
	S3C_VIDCON0_CLKVALUP_ALWAYS | S3C_VIDCON0_CLKVAL_F(clkval)|
	S3C_VIDCON0_VCLKEN_NORMAL | S3C_VIDCON0_CLKDIR_DIVIDED|
	S3C_VIDCON0_CLKSEL_HCLK );
	
	/* RGB I/F控制信号。*/
	fb->VIDCON1 = ( S3C_VIDCON1_IVSYNC_INVERT | S3C_VIDCON1_IHSYNC_INVERT);
	
	/* 配置视频输出时序和显示尺寸。*/
	fb->VIDTCON0 = ( S3C_VIDTCON0_VBPD(res_mode->upper_margin) |
	S3C_VIDTCON0_VFPD(res_mode->lower_margin) |
	S3C_VIDTCON0_VSPW(res_mode->vsync_len));         
	fb->VIDTCON1 = ( S3C_VIDTCON1_HBPD(res_mode->left_margin) |
	S3C_VIDTCON1_HFPD(res_mode->right_margin) |
	S3C_VIDTCON1_HSPW(res_mode->hsync_len));
	fb->VIDTCON2 = ( S3C_VIDTCON2_LINEVAL(pGD->winSizeY - 1) |
	S3C_VIDTCON2_HOZVAL(pGD->winSizeX - 1));
	
	#if defined(LCD_VIDEO_BACKGROUND)
		fb->WINCON0 = (S3C_WINCON_BPPMODE_16BPP_565 | S3C_WINCON_ENWIN_ENABLE |
		S3C_WINCON_HAWSWP_ENABLE); 
		fb->VIDOSD0A = ( S3C_VIDOSD_LEFT_X(0) | S3C_VIDOSD_TOP_Y(0));
		fb->VIDOSD0B = ( S3C_VIDOSD_RIGHT_X(pGD->winSizeX - 1) |
		S3C_VIDOSD_BOTTOM_Y(pGD->winSizeY - 1));
		/* 指定视频窗口0的大小控制寄存器。*/
		fb->VIDOSD0C = S3C_VIDOSD_SIZE(pGD->winSizeY * pGD->winSizeX);
	#endif
	/* 窗口格式设置 */
	fb->WINCON1 = (S3C_WINCON_BPPMODE_16BPP_565 | S3C_WINCON_ENWIN_ENABLE |
	S3C_WINCON_HAWSWP_ENABLE);
	
	/* 指定OSD图像的左上角像素的横向屏幕坐标。*/
	fb->VIDOSD1A = ( S3C_VIDOSD_LEFT_X(0) | S3C_VIDOSD_TOP_Y(0));
	
	/* 指定横屏右下角的OSD图像的像素坐标。*/
	fb->VIDOSD1B = ( S3C_VIDOSD_RIGHT_X(pGD->winSizeX - 1) |
	S3C_VIDOSD_BOTTOM_Y(pGD->winSizeY - 1));
	#if defined(LCD_VIDEO_BACKGROUND)
		fb->VIDOSD1C = ( S3C_VIDOSD_ALPHA0_R(LCD_VIDEO_BACKGROUND_ALPHA) |
		S3C_VIDOSD_ALPHA0_G(LCD_VIDEO_BACKGROUND_ALPHA) | S3C_VIDOSD_ALPHA0_B(LCD_VIDEO_BACKGROUND_ALPHA) );
	#endif
	
	/* 指定视频窗口1的大小控制寄存器。*/
	fb->VIDOSD1D = S3C_VIDOSD_SIZE(pGD->winSizeY * pGD->winSizeX);
	fb->SHADOWCON = S3C_WINSHMAP_CH_ENABLE(1); //Enables Channel 1
	
	#if defined(LCD_VIDEO_BACKGROUND)
		/* config Display framebuffer addr for background*/
		/* 指定窗口0的缓冲区起始地址寄存器，缓冲器0。*/
		fb->VIDW00ADD0B0 = LCD_VIDEO_BACKGROUND_ADDR;
		/* This marks the end of the frame buffer. */
		/* 指定窗口0的缓冲区，缓冲区结束地址寄存器0。*/
		fb->VIDW00ADD1B0 = (S3C_VIDW00ADD0B0 &0xffffffff) + (pGD->winSizeX+0) * pGD->winSizeY * (pGD->gdfBytesPP);
		/* 指定窗口0的缓冲区大小寄存器。*/
		fb->VIDW00ADD2= ((pGD->winSizeX * pGD->gdfBytesPP) & 0x1fff);
	#endif
	
	/* config Display framebuffer addr for console*/
	fb->VIDW01ADD0B0 = pGD->frameAdrs;
	/* This marks the end of the frame buffer. */
	fb->VIDW01ADD1B0 = (S3C_VIDW01ADD0B0 &0xffffffff) + (pGD->winSizeX+0) * pGD->winSizeY * (pGD->gdfBytesPP);
	fb->VIDW01ADD2= ((pGD->winSizeX * pGD->gdfBytesPP) & 0x1fff);
	/* Enable  Display */
	fb->VIDCON0 |= (S3C_VIDCON0_ENVID_ENABLE | S3C_VIDCON0_ENVID_F_ENABLE);  /* ENVID = 1  ENVID_F = 1*/
	fb->TRIGCON = 3;//(TRGMODE_I80 | SWTRGCMD_I80);TRIGCON = 3
	
	/* Enable  Display  */
	//VIDCON0 |= (VIDCON0_ENVID_ENABLE | VIDCON0_ENVID_F_ENABLE);   /* ENVID = 1     ENVID_F = 1*/ 
	//TRIGCON = (TRGMODE_I80 | SWTRGCMD_I80);  //TRIGCON = 3
	
	printf("Video: video_hw_init complete \n");
		return ((void*)&smi);
}

	void video_set_lut(unsigned int index,  /* color number */
	unsigned char r,     /* red */
	unsigned char g,     /* green */
	unsigned char b     /* blue */
	) {}
#endif /* CONFIG_VIDEO_S5PV210 */