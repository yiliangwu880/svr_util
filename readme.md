# 介绍：
	linux C++服务器常见可复用功能
	目的提高C++代码效率。
	静态库提供
	编译成功后，使用include,lib两个目录的内容到你的项目就可以使用了。
	
# 编译方法：
	整个文件夹放到linux目录，安装cmake gcc等。
	当前目录执行：sh clearBuild.sh 完成编译

典型使用例子参考: https://github.com/yiliangwu880/simple_web_svr.git

目录结构：
	src						 ==源码
	include		             ==用户用的头文件
	bin		             	 ==执行文件
	lib                      ==用户用的静态库
	samples					 ==使用例子
	test					 ==测试用例
	vs                       ==vs浏览工具
	
	
	
详细功能说明：
	以include目录分类：
	
	static_trick		怪异但好用的静态编程。
	time				时间，定时器相关的功能。
	BacktraceInfo.h		崩溃时堆栈日志
	。。。




