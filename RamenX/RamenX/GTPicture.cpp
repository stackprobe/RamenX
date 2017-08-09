#include "Stdinc.h"

// -- Curtain --

double GTActWhiteLevel; // (-1.0) - 0.0 - 1.0 == çï - ìßñæ - îí
int GTCurtainSleep;

singleton(GTWhiteLevelQueue, autoQueue<double>, new autoQueue<double>(1024))

void GTDrawCurtain(double whiteLevel)
{
	if(whiteLevel == 0.0)
	{
		return;
	}
	if(whiteLevel < 0.0)
	{
		SetBright(0.0, 0.0, 0.0);
		whiteLevel *= -1.0;
	}
	if(whiteLevel < 1.0)
	{
		SetAlpha(whiteLevel);
	}
	DrawPicRect(0, 0, 800, 600, Pic(P_WHITEBOX));

	ResetBlend();
	ResetBright();
}
void GTCurtainFrame(void)
{
	if(m_countDown(GTCurtainSleep))
	{
		return;
	}
	GTActWhiteLevel = S_GTWhiteLevelQueue()->Dequeue(GTActWhiteLevel);
	GTDrawCurtain(GTActWhiteLevel);
}
void GTEarlyCurtain(void)
{
	GTCurtainFrame();
	GTCurtainSleep = 1;
}

void GTSetCurtain(double startWhiteLevel, double destWhiteLevel, int frmcnt)
{
	m_range(startWhiteLevel, -1.0, 1.0);
	m_range(destWhiteLevel,  -1.0, 1.0);

	errorCase(frmcnt < 0 || TSLD < frmcnt);

	S_GTWhiteLevelQueue()->Clear();

	if(frmcnt)
	{
		S_GTWhiteLevelQueue()->GetList()->AddElement(startWhiteLevel);

		for(int count = 1; count < frmcnt; count++)
		{
			double wl = startWhiteLevel + (destWhiteLevel - startWhiteLevel) * ((double)count / frmcnt);

			S_GTWhiteLevelQueue()->GetList()->AddElement(wl);
		}
	}
	S_GTWhiteLevelQueue()->GetList()->AddElement(destWhiteLevel);
}
void GTSetCurtain(double whiteLevel, int frmcnt)
{
	GTSetCurtain(GTActWhiteLevel, whiteLevel, frmcnt);
}
void GTSetCurtain(double whiteLevel)
{
	GTSetCurtain(whiteLevel, 0);
}

// -- Effect --

taskGroup_t *GTEffectList;
int GTEffectSleep;

taskGroup_t *GTGetEffectList(void)
{
	if(!GTEffectList)
		GTEffectList = CreateTaskGroup();

	return GTEffectList;
}
void GTEffectFrame(void)
{
	if(m_countDown(GTEffectSleep))
	{
		return;
	}
	ExecuteTaskGroup(GTGetEffectList());
}
void GTEarlyEffect(void)
{
	GTEffectFrame();
	GTEffectSleep = 1;
}

// ----

void GTEndFrame(void)
{
	GTEffectFrame();
	GTCurtainFrame();

	EndFrame(); // Core
}
