/*
	brief: ���ӣ���ʱ��
	��������ʹ�õײ��ṩ�Ķ�ʱ��. ��Ϊapi���𣬴���ʹ�����׳�����Դй©�Ѹ��١�
	д�˸�Ӧ�ò㶨ʱ����ʹ�ü򵥣��������׸��١� ��Ҫ�ײ�һ����ʱ���ص���������

etc:
�麯���÷���
		class MyTimer : public Timer
		{
		public:
			//timeout��ص�����. timeout�ص�ǰ��S_WAIT_START_TIMER ״̬
			virtual void OnTimer(void *para)
			{
				UNIT_ASSERT((void *)11 == para);
			};
		};

		main(){
			MyTimer t1;
			t1.StartTimer(1, (void *)11);
		}

std::bind�÷���

		void testFreeCb()
		{

		}

		main(){
			Timer t12;
			t1.StartTimer(1, testFreeCb);
		}
*/

#pragma once
#include "typedef.h"
#include "singleton.h"
#include "su_cd.h"
#include <time.h>
#include <vector>
#include <map>
#include <functional>

namespace su
{

	class Timer;

	namespace inner
	{
		//��ʱ����������
		struct CtrlData
		{
			time_t start_sec;
			uint32 interval;
			bool is_loop;  //true��ʾѭ����ʱ��
			Timer *pTimer;
			CtrlData()
				:start_sec(0)
				, interval(0)
				, is_loop(false)
				, pTimer(nullptr)
			{}
		};
	}
	/*
		��ʱ����ע��ص�������timeout���ú�����
        ע�⣺��ʱ������������̫�࣬����֤���ܶ�����ġ�
	*/
	class TimeCallBack : public SingletonBase<TimeCallBack>
	{
		friend class Timer;
        typedef std::vector<inner::CtrlData> VecData;
		typedef std::multimap<time_t, inner::CtrlData> TimeMapData;   //���ھ���ʱ�� map ����   ��Ҫ�Ż���Ƶ������ɾ�������ڴ���Ƭ
	public:
        //���timeout�¼���ִ�лص�����һ��ѭ���������������
		void checkTimeOut();

        //�����ж�ʱ�¼�
		void clear();

        //��ȡ�ȴ����ڵĶ�ʱ������
        uint32 GetTimeNum();

    private:
		//�漰ָ��ӿ�˽�л��������ӷ���.
		bool CreateTimer(Timer *pTimer, uint32 interval, bool is_loop= false);
		bool DelTimer(Timer *pTimer); //deattach and stop timer, ���汣֤Timerָ��ɾ��������Ұ��


	private:
        static const uint32 MAX_TIMER_NUMBER = 1000; //һ����̶�ʱ����1000������Ʋ��á�
		TimeMapData m_time2data;
	};

	typedef std::function<void(void)> TimerCB;
	//���������������ٶ�ʱ������֤��й¶��Դ, ����ص������ڵ�Timer
	class Timer
	{
		friend class TimeCallBack;
	public:
		Timer();
		virtual ~Timer();

		//timeout��ص�����. timeout�ص�ǰ��S_WAIT_START_TIMER ״̬
		virtual void OnTimer(void *para) {}; //��ͳ��ʽ��дʵ��

		//fun ����timer,��ʱ�ص� Timer::OnTimer
		//para is_loop true��ʾѭ����ʱ��
		//return, true�ɹ���ʼ��ʱ����false �Ѿ���ʼ�ˣ�����Ҫ�趨(������stop,��start)
		bool StartTimer(uint32 interval, void *para = nullptr, bool is_loop = false);
		//@para const TimerCB &cb,  ��std::bind�󶨵ĺ���
		bool StartTimer(uint32 interval, const TimerCB &cb, bool is_loop = false);
		//ֹͣ���ڽ��еĶ�ʱ����
		//return, false ����Ҫֹͣ. true �ɹ�������ֹͣ
		bool StopTimer();

	private:
		void OnTimerCB();

	private:
		enum State
		{
			S_WAIT_START_TIMER,
			S_WAIT_TIME_OUT,
		};
		State m_state;
		TimerCB m_cb; //��std::bind��ʽ�󶨵Ļص�����
	};
	

}//namespace su