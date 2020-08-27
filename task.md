


# 加静态运行宏 static _run( i = 3)_
   


mem_pool 加功能：
{
	宏定义可选：每个对象内存前面留uint32内存用来存放控制信息。 检查重复 delete 内存，高概率检查delete野地址(uint32存放特定控制信息，随机数字吧)。
}

C++11 时间
C++11 RANDOM

日志分TRACE，普通文件。 TRACE输出文件包含所有的日志，普通的，按是否屏蔽一些级别来输出。
日志文件，过大就创建新的。

列表找最大或最小值，返回索引。 注意最小值可能由相同的多个

参考 delay_handler.h ，写异步调用，保存上下文
实现 守护进程 参考 man daemon

#define DEBUG_ASSERT(pointer, info, ...)	\
	if(!pointer)	\
	{	\
		OptErrorLog(info);	\
		return __VA_ARGS__;	\
	}
参考__VA_ARGS__写法，弄个日志返回宏

SingleProgress 加更容易复用层。

fun()


	//for(const MfAddr &addr : vec_mf_addr) 这种应该更好
	//考虑删掉 FOR_IT FOR_IT_CONST
	
定时器是项目的常见错误根源，写个管理定时器的类，待统计定时器信息。
{
	哪里定义。
	当前运行有多少个定时器。
}

找个好的开源 scope guard





实时排行榜：
方法1：
{	
	mutimap<score, id>.  排序
	vector 存ID, 名次查找ID
	obj id:
	{
		score  . score查找map元素
	}
}
方法2：
{
	obj
	{
		id
		score
	}
	vector<obj>. 
	名次查找ID
	分数变化， 二分查找删除插入，并移动影响的元素。
	
}