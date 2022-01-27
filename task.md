
# 标准输出日志

template<class ... Args>
void  WriteFormatLogSTemplate(const char* file, int line, const char* fun, LOG_PRIORITY priority, Args&& ... args)
{
	std::ostringstream s;
	std::initializer_list<int> { (s << std::forward<Args>(args), 0)... }; //例如vector<int> v = { (3,1), (3,2) }; 和 {1,2} 返回结果一样; 里面的3只是执行，不返回给任何表达式使用
																			//如果不用完美转发，这样写 std::initializer_list<int> { (cout << args, 0)... }; 
    WriteFormatLogS(file, line, fun, priority, s.str().c_str());
}


# 定时器，测试
三消项目的timer（时间轮询算法） ,libevent, svr_util . 那个效率高

# 参考三消项目，app启动器
main:
	Reg(libevent::Ins(), id);
	su::app.Start();
	while(app.Loop());
	app.exit();

静态库实现zone,gate等。

STATIC_REG(Reg(ZoneSvr::Ins(), id););
class ZoneSvr ： public BaseSvr
{
	virtual void Init() override{}
}




C++11 时间

日志分TRACE，普通文件。 TRACE输出文件包含所有的日志，普通的，按是否屏蔽一些级别来输出。
日志文件，过大就创建新的。

列表找最大或最小值，返回索引。 注意最小值可能由相同的多个

参考 delay_handler.h ，写异步调用，保存上下文
实现 守护进程 参考 man daemon

 参考 SnowFlake算法 重新做一个ID生成器

# 实时排行榜：
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

# app管理器
 单例每个mgr,
 必须有event mgr
 启动进程mgr.
