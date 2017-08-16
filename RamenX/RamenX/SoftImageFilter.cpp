#include "Stdinc.h"

void SIF_DarkToTrans(int h, int x_size, int y_size)
{
	int x;
	int y;

	for(x = 0; x < x_size; x++)
	for(y = 0; y < y_size; y++)
	{
		SILoadPixel(h, x, y);

		Color_A = m_max(Color_R, m_max(Color_G, Color_B));
		Color_A *= Color_A;
		Color_A /= 255;

		SISavePixel(h, x, y);
	}
}
static int IsHadairo(void)
{
	if(Color_R == 255 && Color_G == 230 && Color_B == 222) return 1;
	if(Color_R == 255 && Color_G == 222 && Color_B == 205) return 1;
	if(Color_R == 255 && Color_G == 172 && Color_B == 139) return 1;
	if(Color_R == 205 && Color_G ==  98 && Color_B ==  74) return 1;
	if(Color_R == 131 && Color_G ==  49 && Color_B ==   0) return 1;

	return 0;
}
void SIF_EnemyLv1(int h, int x_size, int y_size)
{
	int x;
	int y;
	int swapColor;

	for(x = 0; x < x_size; x++)
	for(y = 0; y < y_size; y++)
	{
		SILoadPixel(h, x, y);

		if(!IsHadairo())
		{
			if(Color_B == 0 && Color_G == 0 && Color_R == 0)
			{
				Color_R = 128;
			}
			else
			{
				m_rot3(Color_B, Color_G, Color_R, swapColor);
			}

			SISavePixel(h, x, y);
		}
	}
}
void SIF_EnemyLv2(int h, int x_size, int y_size)
{
	int x;
	int y;

	for(x = 0; x < x_size; x++)
	for(y = 0; y < y_size; y++)
	{
		SILoadPixel(h, x, y);

		if(!IsHadairo())
		{
			int gs = (Color_R + Color_G + Color_B) / 3;

			Color_R = gs;
			Color_G = gs;
			Color_B = gs;

			SISavePixel(h, x, y);
		}
	}
}
void SIF_EnemyLv3(int h, int x_size, int y_size)
{
	int x;
	int y;
	int swapColor;

	for(x = 0; x < x_size; x++)
	for(y = 0; y < y_size; y++)
	{
		SILoadPixel(h, x, y);

		if(!IsHadairo())
		{
			m_sort3(Color_B, Color_G, Color_R, swapColor);

			Color_G = 128 + Color_G / 2;
			Color_R = 128 + Color_R / 2;

			SISavePixel(h, x, y);
		}
	}
}

static int KumaDotCommon(void) // ret: ? ŒF‚Ì–Ñ(•\–Ê)‚Ì•”•ª
{
	int swapColor;

	m_swap(Color_R, Color_B, swapColor);

	if(Color_R == 255 && Color_G == 0 && Color_B == 0) // ? –ÑŽ…‹…‚Ì•”•ª
		return 0;

	if(Color_R == 0 && Color_G == 0 && Color_B == 0) // ? —ÖŠs‚Ì•”•ª
		return 0;

	return 1;
}
void SIF_Kuma00(int h, int x_size, int y_size)
{
	int x;
	int y;

	for(x = 0; x < x_size; x++)
	for(y = 0; y < y_size; y++)
	{
		SILoadPixel(h, x, y);
		KumaDotCommon();
		SISavePixel(h, x, y);
	}
}
void SIF_Kuma01(int h, int x_size, int y_size)
{
	int x;
	int y;

	for(x = 0; x < x_size; x++)
	for(y = 0; y < y_size; y++)
	{
		SILoadPixel(h, x, y);
		
		if(KumaDotCommon())
		{
			Color_R *= 3; Color_R /= 4;
		}
		SISavePixel(h, x, y);
	}
}
void SIF_Kuma02(int h, int x_size, int y_size)
{
	int x;
	int y;

	for(x = 0; x < x_size; x++)
	for(y = 0; y < y_size; y++)
	{
		SILoadPixel(h, x, y);
		
		if(KumaDotCommon())
		{
			Color_B /= 2;
		}
		SISavePixel(h, x, y);
	}
}
void SIF_Kuma03(int h, int x_size, int y_size)
{
	int x;
	int y;

	for(x = 0; x < x_size; x++)
	for(y = 0; y < y_size; y++)
	{
		SILoadPixel(h, x, y);
		
		if(KumaDotCommon())
		{
			Color_G /= 2;
			Color_B /= 2;
		}
		SISavePixel(h, x, y);
	}
}

// dev >

static void CheckMikoSize(int c_type, int h, int x_size, int y_size)
{
	const int ORIG_X = 0;
	const int ORIG_Y = 20;
	const int SPAN_X = 220;
	const int SPAN_Y = 210;

	for(int bx = 0; bx < 4; bx++)
	for(int by = 0; by < 4; by++)
	{
		int minx = TSLD;
		int miny = TSLD;
		int maxx = -TSLD;
		int maxy = -TSLD;

		for(int x = 0; x < SPAN_X; x++)
		for(int y = 0; y < SPAN_Y; y++)
		{
			SILoadPixel(
				h,
				ORIG_X + bx * SPAN_X + x,
				ORIG_Y + by * SPAN_Y + y
				);

			if(Color_A)
			{
				minx = m_min(minx, x);
				miny = m_min(miny, y);
				maxx = m_max(maxx, x);
				maxy = m_max(maxy, y);
			}
		}
		l_x(xcout("%c %d %d %d %d // %d %d", c_type, minx, miny, maxx, maxy, bx + 1, by + 1));
	}
}
void SIF_CheckMikoSize_A(int h, int x_size, int y_size)
{
	CheckMikoSize('A', h, x_size, y_size);
}
void SIF_CheckMikoSize_B(int h, int x_size, int y_size)
{
	CheckMikoSize('B', h, x_size, y_size);
}

// < dev
