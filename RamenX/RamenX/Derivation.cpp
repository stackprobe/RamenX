#include "Stdinc.h"

typedef struct DerInfo_st
{
	int ParentPic;
	int X;
	int Y;
	int W;
	int H;
	int Handle; // HANDLE_CLOSED == 未ロード
}
DerInfo_t;

static int DerListInited;
static DerInfo_t DerList[D_MAX];
static int DerIndex;

static void AddDerInfo(int parent_pic, int x, int y, int w, int h)
{
	errorCase(D_MAX <= DerIndex);

	DerInfo_t *i = DerList + DerIndex;

	i->ParentPic = parent_pic;
	i->X = x;
	i->Y = y;
	i->W = w;
	i->H = h;
	i->Handle = HANDLE_CLOSED;

	DerIndex++;
}
static void AddAnimation(int parent_pic, int xnum, int ynum, int w, int h, int repcnt = 1)
{
	for(int y = 0; y < ynum; y++)
	for(int x = 0; x < xnum; x++)
	for(int c = 0; c < repcnt; c++)
	{
		AddDerInfo(parent_pic, x * w, y * h, w, h);
	}
}
static void CreateDerList(void)
{
#if 0 // 廃止
#define ADDJOUGEDIV(resno) \
	CheckPicSize(Pic(resno)); \
	AddDerInfo(resno, 0,           0, Pic_W, 600); \
	AddDerInfo(resno, 0, Pic_H - 600, Pic_W, 600)

#define ADDCENTERDIV(resno, x_size, y_size) \
	CheckPicSize(Pic(resno)); \
	AddDerInfo(resno, (Pic_W - x_size) / 2, (Pic_H - y_size) / 2, x_size, y_size)
#else
#define ADDJOUGEDIV(resno) \
	AddDerInfo(resno, 0, 0, 10, 10); \
	AddDerInfo(resno, 0, 0, 10, 10)

#define ADDCENTERDIV(resno, x_size, y_size) \
	AddDerInfo(resno, 0, 0, 10, 10)
#endif

	int stock_pic_w = Pic_W;
	int stock_pic_h = Pic_H;

	ADDJOUGEDIV(P_DUMMY);
	ADDJOUGEDIV(P_DUMMY);
	ADDJOUGEDIV(P_DUMMY);
	ADDJOUGEDIV(P_DUMMY);
	ADDJOUGEDIV(P_DUMMY);
	ADDJOUGEDIV(P_DUMMY);
	ADDJOUGEDIV(P_DUMMY);
	ADDJOUGEDIV(P_DUMMY);
	ADDJOUGEDIV(P_DUMMY);
	ADDCENTERDIV(P_DUMMY, 600, 600);
	ADDJOUGEDIV(P_DUMMY);
	ADDJOUGEDIV(P_DUMMY);
	ADDJOUGEDIV(P_DUMMY);
	ADDJOUGEDIV(P_DUMMY);
	ADDCENTERDIV(P_DUMMY, 1900, 600);
	ADDCENTERDIV(P_DUMMY, 800, 30);
	ADDJOUGEDIV(P_DUMMY);
	ADDJOUGEDIV(P_DUMMY);
	ADDJOUGEDIV(P_DUMMY);
	ADDJOUGEDIV(P_DUMMY);

	Pic_W = stock_pic_w;
	Pic_H = stock_pic_h;

#undef ADDJOUGEDIV

	/*
		中心が良くわからん。
		高速回転させると軸がブレる。
	*/
#if 0 // old
	AddDerInfo(P_ETAMA6, 0 + 32 * 0, 1, 31, 31);
	AddDerInfo(P_ETAMA6, 0 + 32 * 1, 1, 31, 31);
	AddDerInfo(P_ETAMA6, 0 + 32 * 2, 1, 31, 31);
	AddDerInfo(P_ETAMA6, 0 + 32 * 3, 1, 31, 31);
	AddDerInfo(P_ETAMA6, 0 + 32 * 4, 1, 31, 31);
	AddDerInfo(P_ETAMA6, 0 + 32 * 5, 1, 31, 31);
	AddDerInfo(P_ETAMA6, 0 + 32 * 6, 1, 31, 31);
	AddDerInfo(P_ETAMA6, 0 + 32 * 7, 1, 31, 31);
#else
	AddDerInfo(P_TAMA, 190, 30 + 64 * 7, 28, 28);
	AddDerInfo(P_TAMA, 190, 30 + 64 * 0, 28, 28);
	AddDerInfo(P_TAMA, 190, 30 + 64 * 6, 28, 28);
	AddDerInfo(P_TAMA, 190, 30 + 64 * 5, 28, 28);
	AddDerInfo(P_TAMA, 190, 30 + 64 * 4, 28, 28);
	AddDerInfo(P_TAMA, 190, 30 + 64 * 3, 28, 28);
	AddDerInfo(P_TAMA, 190, 30 + 64 * 1, 28, 28);
	AddDerInfo(P_TAMA, 190, 30 + 64 * 7, 28, 28);
#endif

	AddAnimation(P_EXPLOSION, 5, 2, 192, 192);
	AddAnimation(P_HANABI, 5, 2, 192, 192);
	AddAnimation(P_REDRING, 5, 2, 192, 192);
	AddAnimation(P_STARRING, 5, 4, 192, 192);

	for(int n = 0; n < 10; n++)
	{
		AddDerInfo(P_MARISA_MASTERSPARK_00 + n, 128, 0, 128, 256);
	}

	AddDerInfo(P_ETAMA2, 32, 64, 16, 16);
	AddDerInfo(P_ETAMA2, 48, 64, 16, 16);
	AddDerInfo(P_ETAMA2, 80, 64, 16, 16);

	{
		const int ORIG_X = 0;
		const int ORIG_Y = 20;
		const int SPAN_X = 220;
		const int SPAN_Y = 210;
		const int MARGIN_XY = 5;
		int c = 0;

		for(int n = 0; n < 2; n++)
		for(int x = 0; x < 4; x++)
		for(int y = 0; y < 4; y++)
		{
			int x1 = ORIG_X + SPAN_X * x;
			int y1 = ORIG_Y + SPAN_Y * y;
			int x2 = ORIG_X + SPAN_X * x;
			int y2 = ORIG_Y + SPAN_Y * y;

			x1 += MikoPosTbl[c++];
			y1 += MikoPosTbl[c++];
			x2 += MikoPosTbl[c++];
			y2 += MikoPosTbl[c++];

			x1 -= MARGIN_XY;
			y1 -= MARGIN_XY;
			x2 += MARGIN_XY;
			y2 += MARGIN_XY;

			AddDerInfo(n == 0 ? P_MIKO_A : P_MIKO_B, x1, y1, (x2 + 1) - x1, (y2 + 1) - y1);
		}
	}

	// D_*_FIRE*
	for(int x = 0; x < 8; x++)
	for(int y = 0; y < 4; y++)
	{
#if 0 // old
		int ox = 32 * x;
		int oy = 32 * y + 112;
		int xs = 32;
		int ys = 32;

		ox += 1;
		oy += 1;
		xs -= 2;
		ys -= 2;

		AddDerInfo(P_ETAMA1, ox, oy, xs, ys);
#else
		int ox;
		int oy;
		int xs;
		int ys;

		switch(x)
		{
		case 0: oy = 7; break;
		case 1: oy = 0; break;
		case 2: oy = 6; break;
		case 3: oy = 5; break;
		case 4: oy = 4; break;
		case 5: oy = 3; break;
		case 6: oy = 1; break;
		case 7: oy = 7; break;

		default:
			error();
		}
		oy *= 64;

		switch(y)
		{
		case 0:
			ox = 100;
			oy += 30;
			xs = 30;
			ys = 30;
			break;

		case 1:
			// 剣
			ox = 237;
			oy += 0;
			xs = 20;
			ys = 30;
			/*
			// 矢印
			ox = 130;
			oy += 30;
			xs = 28;
			ys = 30;
			*/
			break;

		case 2:
			ox = 160;
			oy += 30;
			xs = 30;
			ys = 30;
			break;

		case 3:
			// 無色
			ox = 257 + 64 - 2;
			oy += 0;
			xs = 2;
			ys = 2;
			/*
			// でかい丸
			ox = 257;
			oy += 0;
			xs = 64;
			ys = 64;
			*/
			break;

		default:
			error();
		}

		AddDerInfo(P_TAMA, ox, oy, xs, ys);
#endif
	}

	AddAnimation(P_CRASH, 10, 1, 240, 240, 2);
	AddAnimation(P_PLAYER_CRASH, 5, 1, 240, 240, 2);

	errorCase(DerIndex != D_MAX);
}
int Der(int derno)
{
	if(!DerListInited) {
		DerListInited = 1;
		CreateDerList();
	}
	errorCase(derno < 0 || D_MAX <= derno);

	DerInfo_t *i = DerList + derno;

	if(i->Handle == HANDLE_CLOSED)
	{
		i->Handle = DerivationGraph(i->X, i->Y, i->W, i->H, Pic(i->ParentPic));
		if(i->Handle == HANDLE_CLOSED)
		{
			printfDx("%d", i->ParentPic);
			error();
		}
	}
	EnableTrans = S_EnableTransList()->GetElement(i->ParentPic);
	return i->Handle;
}
void ReleaseDer(int parent_pic)
{
	if(DerListInited)
	{
		for(int derno = 0; derno < D_MAX; derno++)
		{
			DerInfo_t *i = DerList + derno;

			if(i->ParentPic == parent_pic && i->Handle != HANDLE_CLOSED)
			{
				if(DeleteGraph(i->Handle) != 0) // ? エラー
				{
					error();
				}
				i->Handle = HANDLE_CLOSED;
			}
		}
	}
}
