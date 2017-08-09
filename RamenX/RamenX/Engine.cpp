#include "Stdinc.h"

int FrameCount; // 405日くらいでカンスト
int LFrmCnt; // ローカルカウンタ、シーン毎に初期化する。

int WindowIsActive;

int Hz;
int VSyncFlag; // ? グラボで同期する。
int SyncWaitMillis; // 0 でも CPU 使用率が天井に張り付くことはない。
int SyncNextTimeMillis;
int FRDLevel;
int MaxFRDLevel;
int FRDCount;

void SetVSyncFlag(int flag)
{
	SetDrawScreen(DX_SCREEN_BACK);
	SetWaitVSyncFlag(flag ? true : false);

	VSyncFlag = flag;
}
static void CheckHz(void)
{
	static int hzcnt;
	static int lasttimems;
	int timems = GetNowCount() / 500;

	hzcnt++;

	if(lasttimems != timems)
	{
		Hz = hzcnt * 2;
		hzcnt = 0;
		lasttimems = timems;

		if(4 <= abs(60 - Hz))
		{
			FRDLevel += 5;
			MaxFRDLevel = m_max(MaxFRDLevel, FRDLevel);
		}
		else
			m_countDown(FRDLevel);

		const int FRD_TSLD = 30;

		if(FRD_TSLD <= FRDLevel)
		{
			FRDLevel = FRD_TSLD;
			FRDCount++;
		}
	}
}
static void SyncFrame(void)
{
	if(!VSyncFlag)
	{
		int timems = GetNowCount();

		if(SyncNextTimeMillis < timems)
		{
			SyncNextTimeMillis = timems;
		}
		else
		{
			do
			{
				Sleep(SyncWaitMillis);
			}
			while(GetNowCount() < SyncNextTimeMillis);
		}
		SyncNextTimeMillis += FrameCount % 3 == 0 ? 17 : 16;
	}
}
void EndFrame(void)
{
	if(!SEFrame())
	{
		MusicFrame();
	}

	// Core >

	ScreenFlip();
	SyncFrame();

	if(CheckHitKey(KEY_INPUT_ESCAPE) == 1 || ProcessMessage() == -1)
	{
		EndProc();
	}

	// < Core

	FrameCount++;
	errorCase(TSLD < FrameCount); // まさかのカンスト
	LFrmCnt++;

	WindowIsActive = IsWindowActive();

	CheckHz();

	PadFrame();
	KeyFrame();
}
