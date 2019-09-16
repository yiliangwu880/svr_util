
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