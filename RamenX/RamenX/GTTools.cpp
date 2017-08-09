#include "Stdinc.h"

const int CharDerResnoList[0x80] =
{
	/*
		Dummy
	*/
	0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,
	0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,
	0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,
	0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,

	0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,
	0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,
	0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,
	0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0,
};

static int OrigX;
static int OrigY;
static int StepX;
static int StepY;
static int CurrX;
static int CurrY;

void GTPrintSet(int origx, int origy, int stepx, int stepy)
{
	OrigX = origx;
	OrigY = origy;
	StepX = stepx;
	StepY = stepy;
	CurrX = origx;
	CurrY = origy;
}
void GTPrint(char *line)
{
	for(char *p = line; *p; p++)
	{
		if(*p == '\n')
		{
			CurrX = OrigX;
			CurrY += StepY;
		}
		else if(*p == '\r')
		{
			CurrX -= StepX;
		}
		else
		{
			if(*p != ' ')
			{
				SimpleDrawPic(CurrX, CurrY, Der(CharDerResnoList[*p & 0x7f]));
			}
			CurrX += StepX;
		}
	}
}
void GTPrint_x(char *line)
{
	GTPrint(line);
	memFree(line);
}

void GTWaitFadeout(int resno_wallpic, double whiteLevel, int frmcnt)
{
	GTSetCurtain(whiteLevel, frmcnt);

	while(GTActWhiteLevel != whiteLevel)
	{
		SimpleDrawPic(0, 0, Pic(resno_wallpic));
		GTEndFrame();
	}
}

void GTDrawBox(int x, int y, int w, int h, int color)
{
	I2RGB(color);

	SetAlpha(Color_A / 255.0);
	SetBright(
		Color_R / 255.0,
		Color_G / 255.0,
		Color_B / 255.0
		);

	DrawPicRect(x, y, w, h, Pic(P_WHITEBOX));

	ResetBlend();
	ResetBright();
}
void GTDrawCircle(double x, double y, double r, int color)
{
	I2RGB(color);

	SetAlpha(Color_A / 255.0);
	SetBright(
		Color_R / 255.0,
		Color_G / 255.0,
		Color_B / 255.0
		);

	DrawBegin(x, y, Pic(P_WHITECIRCLE));
	DrawZoom(r / 50.0); // P_WHITECIRCLE の幅と高さは 100 
	DrawEnd();

	ResetBlend();
	ResetBright();
}

/*
	画面左上のドットの絶対座標
	絶対座標 (x, y) の画面上の座標は (x - GTCameraX, y - GTCameraY) になる。
*/
double GTCameraX;
double GTCameraY;

// ---- Print ----

PrintExtra_t PE;

typedef struct PrintInfo_st
{
	int X;
	int Y;
	char *Token;
	PrintExtra_t Extra;
}
PrintInfo_t;

void PE_Border(int color)
{
	PE.Border = 1;
	PE.BorderColor = color;
}
void PE_Reset(void)
{
	memset(&PE, 0x00, sizeof(PrintExtra_t));
	PE.Color = GetColor(255, 255, 255);
}

static int PrintFunc(PrintInfo_t *i)
{
	if(i->Extra.Border)
	{
		for(int xc = -1; xc <= 1; xc++)
		for(int yc = -1; yc <= 1; yc++)
		{
			DrawString(i->X + xc, i->Y + yc, i->Token, i->Extra.BorderColor);
		}
	}
	DrawString(i->X, i->Y, i->Token, i->Extra.Color);
	return 0;
}
static void ReleasePrintInfo(PrintInfo_t *i)
{
	memFree(i->Token);
	memFree(i);
}
static int P_BaseX;
static int P_BaseY;
static int P_YStep;
static int P_X;
static int P_Y;

void SetPrint(int x, int y, int yStep)
{
	errorCase(x < -TSLD || TSLD < x);
	errorCase(y < -TSLD || TSLD < y);
	errorCase(yStep < 0 || TSLD < yStep);

	P_BaseX = x;
	P_BaseY = y;
	P_YStep = yStep;
	P_X = 0;
	P_Y = 0;
}
void PrintRet(void)
{
	P_X = 0;
	P_Y += P_YStep;
}
void Print(char *token)
{
	Print_x(strx(token));
}
void Print_x(char *token)
{
	PrintInfo_t *i = nb(PrintInfo_t);

	i->X = P_BaseX + P_X;
	i->Y = P_BaseY + P_Y;
	i->Token = token; // bind
	i->Extra = PE;

	PrintFunc(i);
	ReleasePrintInfo(i);

	int w = GetDrawStringWidth(token, strlen(token));
	errorCase(w < 0 || TSLD < w);
	P_X += w;
}

// ----
