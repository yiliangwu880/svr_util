/*
	brief: ʱ�����

*/

#pragma once
#include "typedef.h"
#include "singleton.h"
#include "su_cd.h"
#include <time.h>
#include <vector>
#include <map>

namespace su
{
    //���������е��������£� ��ʾʵ�����֣����� 2015 1 ����2015��1��
    //time_t��  ��������ʱ��1970 �� 1 �� 1 �� 00:00:00 ��ʼ����������� �����й�ʱ���� 1970-01-01 08:00:00
	//ʹ�÷������ö�ʱ��ÿ�����SysTime::obj().refresh()������ʱ�䲻ˢ�¡��ô�����ÿ�ε���ˢ��Ч�ʸ�һ�㣬 �������й����У�SysTime::obj()���ᷢ���仯������ʱ��仯�����bug
	//ʹ�����ӣ�
	/*
		main_loop()
		{
			SysTime::obj().refresh();ˢ�µ�ǰϵͳʱ��
			const tm &t = SysTime::obj().getTm();
		}

	*/
	class SysTime : public SingletonBase<SysTime>
	{
        friend class SingletonBase<SysTime>;
        SysTime();

	public:
        //para1: int y,  ��   
        //para2: int m,  ��
		static int GetDaysInMonth(int y,int m);
        
        //�����µ���������
        //para1: int y_idx,  �ڼ��꣬0��ʼ��
        //para2: int m,  ��
		static int GetMonthPeriodCnt(int y_idx, int m);

        //������
		static bool IsLeapYear(int y);

		//��ȡtime_t���������ʱ
		static time_t GetCurDayStart(time_t sec);
		//��ȡtime_t�����ܵĵ�һ�����ʱ, ʧ�ܷ���0
		static time_t GetCurWeekStart(time_t sec);
		//��ȡtime_t�����µĵ�һ�����ʱ, ʧ�ܷ���0
		static time_t GetCurMonStart(time_t sec);
		//��ȡtime_t������ĵ�һ�����ʱ, ʧ�ܷ���0
		static time_t GetCurYearStart(time_t sec);

		//!����"%Y-%m-%d-%H-%M-%S"��ʾ��ʱ��ת����time_t
		static bool TimeStrToTime_t(const char* szTimeString, time_t& ret_time);
		//!����"%Y-%m-%d-%H-%M-%S"��ʾ��ʱ��ת����tm��ʾ�Ľṹ
		static bool TimeStringToTm(const char* szTimeString, tm& ret_tm);
		//!����tTime��ʾ��ʱ��ת������"%Y-%m-%d %H-%M-%S"��ʾ
		static bool TimeToTimeStr(time_t t, std::string &str);
	public:
		//ˢ�µ�ǰ����ʱ��
		void Refresh();
		//����ϵͳ�ĵ�ǰ����ʱ�������
        inline time_t Sec() const{return m_sec;};
		//���ص�ǰ������ʽ
		const tm &CurTm() const;
		bool IsLeapYear() const;
		//���ص�ǰ���ж�����
		int GetDaysInMonth() const;
		//�����µ�����������1970..��ʼ����. 1��ʼ
		int GetMonthPeriodCnt() const;
		//�����������������1970-01-04��ʼ����
		int GetDayPeriodCnt() const;
		//�������ڵ�����������1970-01-04��ʼ����
		int GetWeekPeriodCnt() const;
        std::string GetTimeStr() const;

        //������
        /////////////////////////////////////////
        //����ϵͳʱ���ƫ�ơ� ������ƫ�ơ� ��������1����ʾ��ȡϵͳʱ��==��ʵʱ��+1��
        void SetTimeOffset(time_t offset);
        //����ƫ��
        void AddTimerOffset(time_t offset);
        //����ʽ"%Y-%m-%d-%H-%M-%S"��ʾ��ʱ�� ����Ϊ��ǰʱ��
        //���ֱ���Ϊ4λ����2λ
        bool SetTimeByStr(const char *pTime);
        //����ƫ��
        void AddMonthOffset(unsigned int month);
        //����ƫ��
        void AddYearOffset(unsigned int year);
        /////////////////////////////////////////

	private:
		time_t m_sec;
		tm m_tm;
		CurCycleNum m_day_period_cnt;
        CurCycleNum m_week_period_cnt;
        time_t m_offset;
	};

	// ��ʱ������������ʼ����ͣ���ָ�����ȡ����ʱ��
	class TimerCnt
    {
    public:
        enum State{
            TIMER_S_NULL,
            TIMER_S_RUN,
            TIMER_S_PAUSE,
        };
	public:
		TimerCnt()
			:m_start(0)
			,m_pause(0)
			,m_state(TIMER_S_NULL)
		{
		}
        State state();
		void start();
		void pause();
		void resume();
		//�������е�ʱ�䵫������
		time_t peerCurrentTimer() const;
		//�������е�ʱ��
		time_t end();
	private:
		time_t m_start;
		time_t m_pause;
        State m_state;
	};
}//namespace su