/*
	��־��
	��������ʱ�������û�ʹ�ã������û�Ҳ��log_def.h�ļ�����ͻ�� �û����Ը�������ļ��������Լ������µĺ��������µ�ʵ��
*/
#pragma once
#include <string>
#include "log_file.h"

#define L_FATAL(x, ...)  su::LogMgr::Obj().Printf(su::LL_FATAL, __FILE__, __LINE__, __FUNCTION__, x, ##__VA_ARGS__);
#define L_ERROR(x, ...)  su::LogMgr::Obj().Printf(su::LL_ERROR, __FILE__, __LINE__, __FUNCTION__, x, ##__VA_ARGS__);
#define L_WARN(x, ...)   su::LogMgr::Obj().Printf(su::LL_WARN, __FILE__, __LINE__, __FUNCTION__, x, ##__VA_ARGS__);
#define L_DEBUG(x, ...)  su::LogMgr::Obj().Printf(su::LL_DEBUG, __FILE__, __LINE__, __FUNCTION__, x, ##__VA_ARGS__);
#define L_INFO(x, ...)   su::LogMgr::Obj().Printf(su::LL_INFO, __FILE__, __LINE__, __FUNCTION__, x, ##__VA_ARGS__);

//��if���д��

//�����Ƿ�����false
#define L_COND_F(cond, ...)\
	do{\
	if(!(cond)){\
	LogMgr::Obj().PrintfCond(LL_ERROR, __FILE__, __LINE__, __FUNCTION__, "condition fail \[" #cond "]. ", ##__VA_ARGS__); \
	return false;\
	}	\
	}while(0)

//�����Ƿ�����
#define L_COND(cond, ...)\
	do{\
	if(!(cond))	\
	{\
	LogMgr::Obj().PrintfCond(LL_ERROR, __FILE__, __LINE__, __FUNCTION__, "condition fail \[" #cond "]. ", ##__VA_ARGS__); \
		return; \
	}\
	}while(0)

#define L_COND_R(cond, ret, ...)\
	do{\
	if(!(cond)){\
	LogMgr::Obj().PrintfCond(LL_ERROR, __FILE__, __LINE__, __FUNCTION__, "condition fail \[" #cond "]. ", ##__VA_ARGS__); \
	return ret;\
	}	\
	}while(0)

#define COND_R(cond, ret)\
	do{\
	if(!(cond)){\
	return ret;\
	}	\
	}while(0)

#define COND(cond)\
	do{\
	if(!(cond))	\
	return; \
	}while(0)

#define L_ASSERT(cond)\
	do{\
	if(!(cond)){\
	LogMgr::Obj().Printf(LL_ERROR, __FILE__, __LINE__, __FUNCTION__, "assert error"); \
	}	\
	}while(0)



