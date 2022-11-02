#pragma once

class Timer
{
public:

	static Timer* GetInstance();

	void TimerPlay(bool TimeResetFlag = true);
	//�^�Ȃ�i�߂Ă�

	float frame = 0.f;
	float titleEaseFrame = 0.f;
	float time;

private:
	Timer(const Timer& timer) = delete;
	Timer& operator=(const Timer& timer) = delete;
	Timer();
	~Timer();
};

