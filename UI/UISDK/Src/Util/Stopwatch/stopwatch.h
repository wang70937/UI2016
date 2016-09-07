#pragma once

// �������ִ��ʱ��
class StopWatch
{
public:
	StopWatch()
	{
		::QueryPerformanceFrequency(&m_liPerfFreq);
		Start();
	}
	void Start()
	{
		QueryPerformanceCounter(&m_liPerfStart);
	}
	// Returns # of millisecons since Start was called;
	__int64  Now() const
	{
		LARGE_INTEGER  liPerfNow;
		QueryPerformanceCounter(&liPerfNow);
		return (((liPerfNow.QuadPart - m_liPerfStart.QuadPart) * 1000) / m_liPerfFreq.QuadPart);
	}

private:
	LARGE_INTEGER  m_liPerfFreq;    // Counts per second
	LARGE_INTEGER  m_liPerfStart;   // Starting count
};
