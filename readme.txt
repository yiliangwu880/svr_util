介绍：
	linux 服务器常见可复用功能,争取量变级别提高写C++代码效率。
	静态库提供
	编译成功后，使用include,lib两个目录的内容到你的项目就可以使用了。
	部分代码和 CppUtility一样，以svr_utili为准。优先维护
	
编译方法：
	整个文件夹放到linux目录，安装cmake gcc等。
	当前目录执行：sh clearBuild.sh 完成编译

vs浏览代码：
	执行vs\run.bat,生成sln文件


目录结构：
	bin		             	 ==执行文件
	include		             ==用户用的头文件
	lib                      ==用户用的静态库
	samples					 == 使用例子
	src						 ==源码
	test					 ==测试用例
	vs                       == vs浏览工具
	
	
	
详细功能说明：
	以include目录分类：
	
	static_trick		怪异但好用的静态编程。
	time				时间，定时器相关的功能。
	BacktraceInfo.h		崩溃时堆栈日志
	。。。




