/*
author: yiliangwu880
可以从 git@github.com:yiliangwu880/svr_util.git 获取更多的参考。
延时，下个系统调用再执行函数
应用场景：比如PlayerMgr 拥有player, Player::Fun 里面需要调用 PlayerMgr::Ins().delPlayer(); 由于当前调用位置在Player方法，不能马上调用，避免野Player操作。
code example:
	
	auto f=[](){  //注意延时后，你的传入对象生存期是否有效！！
		PlayerMgr::Ins().delPlayer();
	};
	PostFunMgr::Ins().Post(f); //延时执行函数

	某个栈浅的地方(接近main函数)或者30MS TIMER，执行.
	PostFunMgr::Ins().Excute();

*/

#pragma once

#include <vector>
#include <list>
#include <set>
#include "typedef.h"
#include <functional>
#include <tuple>
#include "../log_file.h"
#include "static_reg.h"

namespace su
{
	using PostFun = std::function<void(void)>;
	class PostFunMgr
	{
		std::vector<PostFun> m_vec;
		std::vector<PostFun> m_tmpVec;//避免每次构造
	public:
		static PostFunMgr& Ins()
		{
			static PostFunMgr obj;
			return obj;
		}

		void Post(PostFun f);//注意：延时调用容易引用野对象。用户需要正确控制对象的生存期。
		void Excute();
	};

} //end namespace su


//end file
