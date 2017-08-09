#include "Stdinc.h"

void SILoadPixel(int h, int x, int y)
{
	if(GetPixelSoftImage(h, x, y, &Color_R, &Color_G, &Color_B, &Color_A) != 0) // ? 失敗
	{
		error();
	}
	ec_range(Color_R, 0, 255);
	ec_range(Color_G, 0, 255);
	ec_range(Color_B, 0, 255);
	ec_range(Color_A, 0, 255);
}
void SISavePixel(int h, int x, int y)
{
	m_range(Color_R, 0, 255);
	m_range(Color_G, 0, 255);
	m_range(Color_B, 0, 255);
	m_range(Color_A, 0, 255);

	if(DrawPixelSoftImage(h, x, y, Color_R, Color_G, Color_B, Color_A) != 0) // ? 失敗
	{
		error();
	}
}

int Pic_Load(int resno, autoList<uchar> *image)
{
	void (*si_filter1)(int, int, int) = S_SoftImageFilterList()->GetElement(resno);
	void (*si_filter2)(int, int, int) = GetPictureGroup()->SIFilter;
	void (*gh_filter1)(int) = S_GraphicHandleFilterList()->GetElement(resno);
	void (*gh_filter2)(int) = GetPictureGroup()->GHFilter;
	int h;

	if(si_filter1 || si_filter2)
	{
		h = LoadSoftImageToMem(image->ElementAt(0), image->GetCount());
		errorCase(h == -1); // ? 失敗

		int x_size;
		int y_size;

		if(GetSoftImageSize(h, &x_size, &y_size) != 0) // ? 失敗
		{
			error();
		}
		errorCase(x_size < 1 || TSLD < x_size);
		errorCase(y_size < 1 || TSLD < y_size);

		int stock_color_r = Color_R;
		int stock_color_g = Color_G;
		int stock_color_b = Color_B;
		int stock_color_a = Color_A;

		if(si_filter1) si_filter1(h, x_size, y_size);
		if(si_filter2) si_filter2(h, x_size, y_size);

		Color_R = stock_color_r;
		Color_G = stock_color_g;
		Color_B = stock_color_b;
		Color_A = stock_color_a;

		int grph_h = CreateGraphFromSoftImage(h);
		errorCase(grph_h == -1); // ? 失敗

		if(DeleteSoftImage(h) == -1) // ? 失敗
		{
			error();
		}
		h = grph_h;
	}
	else
	{
		h = CreateGraphFromMem(image->ElementAt(0), image->GetCount());
		if(h == -1)
		{
			printfDx("%d\n", resno);
			error();
		}
//		errorCase(h == -1); // ? 失敗
	}

	if(gh_filter1) gh_filter1(h);
	if(gh_filter2) gh_filter2(h);

	return h;
}
void Pic_Unload(int resno, int h)
{
	ReleaseDer(resno);

	if(DeleteGraph(h) != 0)
	{
		error();
	}
}
static Resource_t *Pic_Res_pg(PictureGroup_t *pg)
{
	if(!pg->Res)
		pg->Res = CreateResource("Data2.dat", "..\\..\\Picture.txt", P_MAX, Pic_Load, Pic_Unload);

	return pg->Res;
}
Resource_t *Pic_Res(void)
{
	return Pic_Res_pg(GetPictureGroup());
}

void ReleasePictures(void)
{
	autoList<PictureGroup_t *> *pgl = S_PictureGroupList();

	for(int index = 0; index < pgl->GetCount(); index++)
	{
		UnloadAllHandle(Pic_Res_pg(pgl->GetElement(index)));
	}
}

singleton(SoftImageFilterList,     autoList<void (*)(int, int, int)>, makeList(P_MAX, (void (*)(int, int, int))NULL))
singleton(GraphicHandleFilterList, autoList<void (*)(int)>,           makeList(P_MAX, (void (*)(int))NULL))
singleton(EnableTransList,         autoList<int>,                     makeList(P_MAX, 1))

int EnableTrans;

int Pic(int resno)
{
	if(resno & DTP)
	{
		return Der(resno & ~DTP);
	}
	int h = GetHandle(Pic_Res(), resno);

	/*
		ここで設定された値を次の描画時に参照する。
		ので DrawPic(x, y, Pic(P_*)) のように、描画系関数の引数に直接 Pic() を記述して、
		Pic() の呼び出しと描画の間に別の Pic() が入る余地の無い記述を心掛けること。
	*/
	EnableTrans = S_EnableTransList()->GetElement(resno);
	return h;
}

static void SetBlend(int mode, double a)
{
	m_range(a, 0.0, 1.0);

	int pal = m_d2i(a * 255.0);

	errorCase(pal < 0 || 255 < pal);

	if(SetDrawBlendMode(mode, pal) != 0)
	{
		error();
	}
}
void SetAlpha(double a) // a: 0.0 - 1.0 == 透明 - 不透明
{
	SetBlend(DX_BLENDMODE_ALPHA, a);
}
void SetBlendAdd(double a) // a: 0.0 - 1.0 == 描画しない - 真っ白?
{
	SetBlend(DX_BLENDMODE_ADD, a); // けっこう重い!?
}
void SetBlendInv(double a) // a: 不使用?
{
	SetBlend(DX_BLENDMODE_INVSRC, a);
}
void ResetBlend(void)
{
	if(SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0) != 0)
	{
		error();
	}
}
void SetBright(double r, double g, double b) // (r, g, b): 0.0 - 1.0 == 黒 - 色彩あり
{
	m_range(r, 0.0, 1.0);
	m_range(g, 0.0, 1.0);
	m_range(b, 0.0, 1.0);

	int pal_r = m_d2i(r * 255.0);
	int pal_g = m_d2i(g * 255.0);
	int pal_b = m_d2i(b * 255.0);

	errorCase(pal_r < 0 || 255 < pal_r);
	errorCase(pal_g < 0 || 255 < pal_g);
	errorCase(pal_b < 0 || 255 < pal_b);

	if(SetDrawBright(pal_r, pal_g, pal_b) != 0)
	{
		error();
	}
}
void ResetBright(void)
{
	if(SetDrawBright(255, 255, 255) != 0)
	{
		error();
	}
}

int Pic_W;
int Pic_H;

void CheckPicSize(int h)
{
	Pic_W = -1;
	Pic_H = -1;

	GetGraphSize(h, &Pic_W, &Pic_H);

	errorCase(Pic_W < 1 || TSLD < Pic_W);
	errorCase(Pic_H < 1 || TSLD < Pic_H);
}

int ScreenSlip_X;
int ScreenSlip_Y;

void SimpleDrawPic(int x, int y, int h)
{
	DrawGraph(x + ScreenSlip_X, y + ScreenSlip_Y, h, EnableTrans);
}
void SimpleDrawPic(double x, double y, int h)
{
	DrawGraph((int)x, (int)y, h, EnableTrans);
}
void SimpleDrawPicFloat(double x, double y, int h)
{
	SetDrawMode(DX_DRAWMODE_BILINEAR); // ギャザ＆カクカク対策
#if 0
	CheckPicSize(h);
	DrawModiGraphF(
		(float)x,
		(float)y,
		(float)(x + Pic_W),
		(float)y,
		(float)(x + Pic_W),
		(float)(y + Pic_H),
		(float)x,
		(float)(y + Pic_H),
		h,
		EnableTrans
		);
#else
	DrawGraphF((float)x, (float)y, h, EnableTrans);
#endif
	SetDrawMode(DX_DRAWMODE_NEAREST); // デフォに戻す。
}
void DrawPic(double x, double y, int h)
{
	CheckPicSize(h);
	SimpleDrawPic(x - Pic_W / 2.0, y - Pic_H / 2.0, h);
}
void DrawPicRect(int x, int y, int w, int h, int hdl)
{
	x += ScreenSlip_X;
	y += ScreenSlip_Y;

	errorCase(x < -TSLD || TSLD / 2 < x);
	errorCase(y < -TSLD || TSLD / 2 < y);
	errorCase(w <     1 || TSLD / 2 < w);
	errorCase(h <     1 || TSLD / 2 < h);
	
	if(DrawModiGraph(x, y, x + w, y, x + w, y + h, x, y + h, hdl, EnableTrans) != 0)
	{
		error();
	}
}

static double Draw_X;
static double Draw_Y;
static double Draw_LT_X;
static double Draw_LT_Y;
static double Draw_RT_X;
static double Draw_RT_Y;
static double Draw_RB_X;
static double Draw_RB_Y;
static double Draw_LB_X;
static double Draw_LB_Y;
static int Draw_Handle;
static int Draw_EnableTrans;

void DrawBegin(double x, double y, int h)
{
	if(ScreenSlip_X)
		x += (double)ScreenSlip_X;

	if(ScreenSlip_Y)
		y += (double)ScreenSlip_Y;

	Draw_X = x;
	Draw_Y = y;

	CheckPicSize(h);

	double hw = Pic_W / 2.0;
	double hh = Pic_H / 2.0;

	Draw_LT_X = -hw;
	Draw_LT_Y = -hh;
	Draw_RT_X =  hw;
	Draw_RT_Y = -hh;
	Draw_RB_X =  hw;
	Draw_RB_Y =  hh;
	Draw_LB_X = -hw;
	Draw_LB_Y =  hh;

	Draw_Handle = h;
	Draw_EnableTrans = EnableTrans;
}
void DrawRotate(double rot) // rot: 時計回り, 0.0 == 0度, PI == 180度
{
	double w;

#define ROTATE(x, y) \
	w = x * cos(rot) - y * sin(rot); \
	y = x * sin(rot) + y * cos(rot); \
	x = w

	ROTATE(Draw_LT_X, Draw_LT_Y);
	ROTATE(Draw_RT_X, Draw_RT_Y);
	ROTATE(Draw_RB_X, Draw_RB_Y);
	ROTATE(Draw_LB_X, Draw_LB_Y);
}
void DrawRotate(int numer, int denom)
{
	errorCase(numer < -TSLD || TSLD < numer);
	errorCase(denom <     1 || TSLD < denom);

	numer %= denom;

	DrawRotate((PI * 2.0) * ((double)numer / denom));
}
void DrawXZoom(double zoom)
{
	Draw_LT_X *= zoom;
	Draw_RT_X *= zoom;
	Draw_RB_X *= zoom;
	Draw_LB_X *= zoom;
}
void DrawYZoom(double zoom)
{
	Draw_LT_Y *= zoom;
	Draw_RT_Y *= zoom;
	Draw_RB_Y *= zoom;
	Draw_LB_Y *= zoom;
}
void DrawZoom(double zoom)
{
	DrawXZoom(zoom);
	DrawYZoom(zoom);
}
int MosaicMode;
void DrawEnd(void)
{
	if(!MosaicMode)
		SetDrawMode(DX_DRAWMODE_BILINEAR); // ジャギー対策

	if(Es.DrawRectFloat ?
		(DrawModiGraphF(
			(float)(Draw_X + Draw_LT_X),
			(float)(Draw_Y + Draw_LT_Y),
			(float)(Draw_X + Draw_RT_X),
			(float)(Draw_Y + Draw_RT_Y),
			(float)(Draw_X + Draw_RB_X),
			(float)(Draw_Y + Draw_RB_Y),
			(float)(Draw_X + Draw_LB_X),
			(float)(Draw_Y + Draw_LB_Y),
			Draw_Handle,
			Draw_EnableTrans) != 0) :
		(DrawModiGraph(
			(int)(Draw_X + Draw_LT_X),
			(int)(Draw_Y + Draw_LT_Y),
			(int)(Draw_X + Draw_RT_X),
			(int)(Draw_Y + Draw_RT_Y),
			(int)(Draw_X + Draw_RB_X),
			(int)(Draw_Y + Draw_RB_Y),
			(int)(Draw_X + Draw_LB_X),
			(int)(Draw_Y + Draw_LB_Y),
			Draw_Handle,
			Draw_EnableTrans) != 0)
		)
	{
		printfDx("Draw_X:%f\n", Draw_X);
		printfDx("Draw_Y:%f\n", Draw_Y);
		printfDx("Draw_LT_X:%f\n", Draw_LT_X);
		printfDx("Draw_LT_Y:%f\n", Draw_LT_Y);
		printfDx("Draw_RT_X:%f\n", Draw_RT_X);
		printfDx("Draw_RT_Y:%f\n", Draw_RT_Y);
		printfDx("Draw_RB_X:%f\n", Draw_RB_X);
		printfDx("Draw_RB_Y:%f\n", Draw_RB_Y);
		printfDx("Draw_LB_X:%f\n", Draw_LB_X);
		printfDx("Draw_LB_Y:%f\n", Draw_LB_Y);
		printfDx("Draw_Handle:%d\n", Draw_Handle);
		printfDx("Draw_EnableTrans:%d\n", Draw_EnableTrans);

		error();
	}

	if(!MosaicMode)
		SetDrawMode(DX_DRAWMODE_NEAREST); // デフォに戻す。
}
