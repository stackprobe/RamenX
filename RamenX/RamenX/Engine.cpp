#include "Stdinc.h"

int FrameCount; // 405�����炢�ŃJ���X�g
int LFrmCnt; // ���[�J���J�E���^�A�V�[�����ɏ���������B

int WindowIsActive;

int Hz;
int VSyncFlag; // ? �O���{�œ�������B
int SyncWaitMillis; // 0 �ł� CPU �g�p�����V��ɒ���t�����Ƃ͂Ȃ��B
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
	errorCase(TSLD < FrameCount); // �܂����̃J���X�g
	LFrmCnt++;

	WindowIsActive = IsWindowActive();

	CheckHz();

	PadFrame();
	KeyFrame();
}
