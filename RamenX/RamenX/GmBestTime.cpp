#include "Stdinc.h"

static void AdjustLength(int length)
{
	while(S_Sd_BestTimeTable()->GetCount() < length)
	{
		S_Sd_BestTimeTable()->AddElement(TSLD);
	}
}
static int GetTime(int index)
{
	AdjustLength(index + 1);
	return S_Sd_BestTimeTable()->GetElement(index);
}
static void SetTime(int index, int frmcnt)
{
	AdjustLength(index + 1);
	S_Sd_BestTimeTable()->SetElement(index, frmcnt);
}

/*
	clearCase:
		0 == ‚½‚¾‚ÌƒNƒŠƒA
		1 == NB
		2 == NB WO
		3 == NB WO ND
*/
static int GetBTTIndex(int stageIndex, int clearCase, int playChara = Sd_PlayChara)
{
	errorCase(stageIndex < 0 || STAGE_NUM <= stageIndex);
	errorCase(clearCase < 0 || 4 <= clearCase);
	errorCase(playChara < 0 || 4 <= playChara);

	return stageIndex * 16 + clearCase * 4 + playChara;
}

int GetBestTimeFrmCnt(int stageIndex)
{
	return GetTime(GetBTTIndex(stageIndex, 0));
}
int GetBestTimeFrmCnt_NB(int stageIndex)
{
	return GetTime(GetBTTIndex(stageIndex, 1));
}
int GetBestTimeFrmCnt_NBWipedOut(int stageIndex)
{
	return GetTime(GetBTTIndex(stageIndex, 2));
}
int GetBestTimeFrmCnt_NBWipedOut_NoDamage(int stageIndex)
{
	return GetTime(GetBTTIndex(stageIndex, 3));
}

void SetBestTimeFrmCnt(int stageIndex, int frmcnt)
{
	SetTime(GetBTTIndex(stageIndex, 0), frmcnt);
}
void SetBestTimeFrmCnt_NB(int stageIndex, int frmcnt)
{
	SetTime(GetBTTIndex(stageIndex, 1), frmcnt);
}
void SetBestTimeFrmCnt_NBWipedOut(int stageIndex, int frmcnt)
{
	SetTime(GetBTTIndex(stageIndex, 2), frmcnt);
}
void SetBestTimeFrmCnt_NBWipedOut_NoDamage(int stageIndex, int frmcnt)
{
	SetTime(GetBTTIndex(stageIndex, 3), frmcnt);
}
