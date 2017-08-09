#include "Stdinc.h"

singleton(SELoadedList, autoList<int>,    makeList(SE_MAX, 0))   // 0 or 'not 0'
singleton(SEVolumeList, autoList<double>, makeList(SE_MAX, 0.5)) // 0.0 - 1.0 (default: 0.5)

// Dupl >

#define DUPL_MAX 64

static int DuplSETable[SE_MAX][DUPL_MAX];
static int DuplSEIndexes[SE_MAX];

static void DuplSE(int resno, int h)
{
	DuplSETable[resno][0] = h;

	for(int index = 1; index < DUPL_MAX; index++)
	{
		DuplSETable[resno][index] = DuplSound(h);
	}
}
static void UnduplSE(int resno)
{
	for(int index = 1; index < DUPL_MAX; index++) // 注意: 先頭はオリジナル
	{
		UnloadSound(DuplSETable[resno][index]);
	}
}
static void DuplSESetVolume(int resno, double volume)
{
	for(int index = 0; index < DUPL_MAX; index++)
	{
		SetVolume(DuplSETable[resno][index], volume);
	}
}

// < Dupl

int SE_Load(int resno, autoList<uchar> *image)
{
	int h = LoadSound(image);

	S_SELoadedList()->SetElement(resno, 1);
	DuplSE(resno, h);
	return h;
}
void SE_Unload(int resno, int h)
{
	UnduplSE(resno);
	UnloadSound(h);
}
Resource_t *SE_Res(void)
{
	static Resource_t *res;

	if(!res)
		res = CreateResource("Data3.dat", "..\\..\\SoundEffect.txt", SE_MAX, SE_Load, SE_Unload);

	return res;
}

static singleton(RequestQueue, autoQueue<int>, new autoQueue<int>(256))
static int SleepFrame;

static int EchoFrame(void);
static void UnloadAllEcho(void);

void ReleaseSoundEffects(void)
{
	S_RequestQueue()->Clear();
	SleepFrame = 1;

	UnloadAllEcho();
	UnloadAllHandle(SE_Res()); // 再生中の音は普通に止まるようだ。
}
int GetSERequestCount(void)
{
	return S_RequestQueue()->GetCount();
}

int SEFrame(void) // ret: ? 音を処理した。
{
	if(!m_countDown(SleepFrame))
	{
		if(EchoFrame())
		{
			goto proced;
		}
		if(S_RequestQueue()->GetCount())
		{
			int resno = S_RequestQueue()->Dequeue();
			int h = GetHandle(SE_Res(), resno); // DuplSE* ロードのため必要なんです。

			if(S_SELoadedList()->GetElement(resno))
			{
				DuplSESetVolume(resno, Sd.SEMasterVolume * S_SEVolumeList()->GetElement(resno));
				S_RequestQueue()->GetList()->AddElement(resno);
				S_SELoadedList()->SetElement(resno, 0);
			}
			else
				SoundPlay(DuplSETable[resno][DuplSEIndexes[resno] = (DuplSEIndexes[resno] + 1) % DUPL_MAX]);

			goto proced;
		}
	}
	return 0;

proced:
	SleepFrame = 1;
	return 1;
}

void SETouch(int resno)
{
	GetHandle(SE_Res(), resno);
}
void SEPlay(int resno)
{
	errorCase(resno < 0 || SE_MAX <= resno);

	/*
		同時に再生し過ぎチェック

		但し、短い間隔で何度も再生することは、ここでは防げない。
	*/
	{
		autoList<int> *rqSEList = S_RequestQueue()->GetList();
		int sameSECnt = 0;

		for(int index = 0; index < rqSEList->GetCount(); index++)
		{
			if(resno == rqSEList->GetElement(index))
			{
				sameSECnt++;
			}
		}
		if(2 <= sameSECnt) // ? 同時に再生し過ぎ。
		{
			return;
		}
	}
	S_RequestQueue()->GetList()->AddElement(resno);
}
void UpdateSEVolume(void)
{
	for(int resno = 0; resno < SE_MAX; resno++)
	{
		if(IsHandleLoaded(SE_Res(), resno))
		{
			DuplSESetVolume(resno, Sd.SEMasterVolume * S_SEVolumeList()->GetElement(resno));
		}
	}
//	UnloadAllEcho();
}

// Echo >

#define ECHO_MAX 8
#define ECHO_CYCLE 10 // ! 30 Hz @ SEFrame()

static int EchoSETable[SE_MAX][ECHO_MAX];
static int EchoSELoadedList[SE_MAX];

static singleton(EchoResnoQueue, autoQueue<int>, new autoQueue<int>(ECHO_MAX))
static singleton(EchoIndexQueue, autoQueue<int>, new autoQueue<int>(ECHO_MAX))
static int EchoSleepFrame;

void SEPlayEcho(int resno)
{
	for(int index = -1; index < ECHO_MAX; index++) // -1: Load
	{
		S_EchoResnoQueue()->GetList()->AddElement(resno);
		S_EchoIndexQueue()->GetList()->AddElement(index);
	}
}
void SEStopEcho(void)
{
	S_EchoResnoQueue()->Clear();
	S_EchoIndexQueue()->Clear();
}
static int EchoLoad(int resno) // ret: ? ロードした。
{
	errorCase(resno < 0 || SE_MAX <= resno);

	if(!EchoSELoadedList[resno])
	{
		for(int index = 0; index < ECHO_MAX; index++)
		{
			int h = DuplSound(GetHandle(SE_Res(), resno));

			SetVolume(h,
				Sd.SEMasterVolume *
				S_SEVolumeList()->GetElement(resno) *
				((double)(ECHO_MAX - index) / ECHO_MAX)
				);
			EchoSETable[resno][index] = h;
		}
		EchoSELoadedList[resno] = 1;
		return 1;
	}
	return 0;
}
static int EchoFrame(void) // ret: ? 音を処理した。
{
	if(!m_countDown(EchoSleepFrame))
	{
		if(S_EchoResnoQueue()->GetCount())
		{
redequeue:
			int resno = S_EchoResnoQueue()->Dequeue();
			int index = S_EchoIndexQueue()->Dequeue();

			if(index == -1)
			{
				if(EchoLoad(resno))
					return 1;

				goto redequeue;
			}
			errorCase(resno < 0 || SE_MAX <= resno);
			errorCase(index < 0 || ECHO_MAX <= index);
			errorCase(EchoLoad(resno));

			SoundPlay(EchoSETable[resno][index]);
			EchoSleepFrame = ECHO_CYCLE;
			return 1;
		}
	}
	return 0;
}
static void UnloadEcho(int resno)
{
	S_EchoResnoQueue()->Clear();
	S_EchoIndexQueue()->Clear();
	EchoSleepFrame = 0;

	errorCase(resno < 0 || SE_MAX <= resno);

	if(EchoSELoadedList[resno])
	{
		for(int index = 0; index < ECHO_MAX; index++)
		{
			UnloadSound(EchoSETable[resno][index]);
		}
		EchoSELoadedList[resno] = 0;
	}
}
static void UnloadAllEcho(void)
{
	for(int resno = 0; resno < SE_MAX; resno++)
	{
		UnloadEcho(resno);
	}
}

// < Echo
