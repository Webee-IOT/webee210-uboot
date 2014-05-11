###参与webee210-uboot的开发

如果你想参与本__webee210-uboot__的开发,请用力hit下面的链接:

[webee210-uboot开发指南](https://github.com/iZobs/webee210-uboot/blob/master/Develop-doc.md)

###关于
这是网蜂webee210v2的uboot程序,它使用于webee210V2开发板。
###UPDATE
__2014-5-1__
- uboot支持LCD logo和文字显示
- uboot支持从SD卡烧写镜像文件命令sdfuse
- uboot支持按键扫描功能
- 修改uboot的内存配置，让其支持双通道的256MB+256MB的内存

###TODO
- 让uboot支持从SD读取文件*.txt功能
- 让uboot支持android的引导启动
###编译

         @ make distclean
         @ make webee210_config
         @ make

###烧uboot到你的sd卡
__1.在linux环境下__
在linux 终端下使用工程目录下的烧写脚本`sd-burn.sh`,参数为你SD卡的路径,如:

	sudo ./sd-burn.sh /dev/mmcblk0

__2.在windows环境下__

	coming soon.........

###uboot SD烧写镜像文件
在SD卡里建一个文件夹，名字为webee210，把uboot，内核，文件系统分别命名为：
`uboot.bin`,`kernel.img`,`system.img`。然后存放于该目录。如下:

	|-- webee210
	|   |-- kernel.img
	|   |-- system.img
	|   |-- uboot.bin
