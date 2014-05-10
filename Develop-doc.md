###webee210-uboot的开发指南

如果想参与本项目的开发,请确保你已经看过[网蜂开发者社区简介](https://github.com/Webee-IOT/Document/blob/master/%E7%BD%91%E8%9C%82%E5%BC%80%E5%8F%91%E8%80%85%E7%A4%BE%E5%8C%BA%E7%AE%80%E4%BB%8B.md),并且已经拥有了上面提到的`github`和`gmail`.你只需要从[webee210v2-uboot](https://github.com/iZobs/webee210v2-uboot)fork这个项目,从__自己主页__git clone 下来，并且加入这个邮件列表就可以了:[webee210-uboot-google-groups](https://groups.google.com/d/forum/webee210-uboot).该项目有两个分支一个为`master`,另一个为
`dev`.`dev`分支为开发分支,有TODO列表正在开发的功能,bug也比较多.`master`是稳定版,拥有`TODO`列表已经完成的功能,你可以在这条分支上debug.

例如下面的TODO:

###TODO

- 让uboot通过按按钮S1实现从SD烧入uboot,linux 内核,文件系统.                   
- 让uboot支持从SD卡直接启动linux内核,不需要烧入nand flash.       
- 在uboot集成裸机的所有测试程序,通过`uboot command line`调用,模块包括:
	+ LDE灯
	+ 按键
	+ 蜂鸣器
	+ LCD
	+ PWM
	+ nand flash 测试
	+ i2c
	+ i2s音频
	

###讨论

发送E-mail到`webee210-uboot@googlegroups.com`.注意邮件的主题的格式,用[]做标签.如讨论BUG,发送email的主题如下: 

    send:webee210-uboot@googlegroups.com
	topic:[Bug]uboot的菜单页面出现乱码


###git使用tips

- [如何同步fork源仓库的代码](https://help.github.com/articles/syncing-a-fork)
- [git分支管理策略](http://www.ruanyifeng.com/blog/2012/07/git.html)
- [git权威指南](http://www.worldhello.net/gotgit/)


###项目维护人              

__izobs : ivincentlin@gmail.com__
