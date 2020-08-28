/************************************************************************/
/*
Author:         Yiliangwu
*/
/************************************************************************/

#pragma once

#include "typedef.h"
#include <vector>
#include "easy_code.h"
#include <random>

typedef  unsigned short U16;
/* ---------------------------------------------------------------------------
    RANDOM NUMBER GENERATOR
    This library provide the functions to generate random number sequence,
    all the number in this sequence will not repeat within a round
	
	
   ------------------------------------------------------------------------ */
class PrimeRandom
{
public:	
	//参数U16 maxNum表示 能随机的总数。从0开始。 比如参数=2，只能随机出0,1
	explicit PrimeRandom(U16 maxNum);
	U16 GetRandom();
private:
	void Init(U16 maxNum);
	U16 oRandom( U16 range );
	void RandomMode();
	void GetRightNext();
	void GetLeftNext();
	U16 GetNext(bool isLeft);

private:
	U16 m_modeStartIndex;
	U16 m_step;
	U16 m_mode;
	U16 m_right;
	U16 m_left;
	U16 m_start;
	U16 m_base;

	static const U16 PrimeNumbersTable[];
	static const std::size_t REMAIN_RANG = 6;
	static const U16 MAX_PRIME;
	static const U16 PRIME_LEN;
};


/************************************************************************/
/* 
规则：权重随机，比如[0]=10, [1]=90, [2]=0. 随机1的几率是90% 不会随机2
*/
/************************************************************************/
class RandomWeight
{
private:
	std::vector<uint32> m_sumWeight;
	uint32 m_sum;

public:
	//@vecWeight 权重比例
	RandomWeight(const std::vector<uint32> &vecWeight);
	//根据权重随机索引。 索引为 RandomWeight函数的参数vecWeight的索引
	uint32 rand();	
	//@vecWeight 为随机权重数组
	//return 索引，从0开始
	static uint32 rand(const std::vector<uint32> &vecWeight);
};

//建议不用这个，用 RandomC11
struct Random
{
	static void Init();
	//返回 闭区间[min,max]内随机一个数
	static uint32 RandUint32(uint32 min, uint32 max);
	//返回 闭区间[min,max]内随机一个数
	static int RandInt(int min, int max);

	//随机小数
	static double RandDecimal();
	static uint32 rand32();
    // 生成[low, high]之间的无重复的cnt个随机数
    //如果区间跨度大，效率会低。建议用PrimeRandom类
	static bool GetIntervalNoRepeatedRandNum(U16 low, U16 high, U16 cnt, U16 *num);
};

struct RandomC11
{
	//返回 闭区间[min,max]内随机一个数
	template<class IntType>
	static IntType Rand(IntType min, IntType max);

	//返回 闭区间[min,max]内随机一个数
	static double Double(double min, double max);

private:
	static std::default_random_engine & GetEgnine();
};

template<class IntType>
IntType RandomC11::Rand(IntType min, IntType max)
{
	if (min > max)
	{
		std::swap(min, max);
	}
	std::uniform_int_distribution<IntType> distribution(min, max);
	return distribution(GetEgnine());
}
