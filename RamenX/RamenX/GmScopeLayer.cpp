#include "Stdinc.h"

typedef struct Box_st
{
	int X;
	int Y;
	int W;
	int H;
	int Color;
}
Box_t;

typedef struct Circle_st
{
	double X;
	double Y;
	double R;
	int Color;
}
Circle_t;

static singleton(BoxList, autoList<Box_t>, new autoList<Box_t>())
static singleton(CircleList, autoList<Circle_t>, new autoList<Circle_t>())

void GmAddBoxScope(int x, int y, int w, int h, int color)
{
	Box_t i;

	i.X = x;
	i.Y = y;
	i.W = w;
	i.H = h;
	i.Color = color;

	S_BoxList()->AddElement(i);
}
void GmAddCircleScope(double x, double y, double r, int color)
{
	Circle_t i;

	i.X = x;
	i.Y = y;
	i.R = r;
	i.Color = color;

	S_CircleList()->AddElement(i);
}

void GmScopeFrame(void)
{
	if(Es.VisibleScope)
	{
		GTDrawBox(0, 0, 800, 600, RGB2I(0, 0, 0, 128));

		for(int index = 0; index < S_BoxList()->GetCount(); index++)
		{
			Box_t *p = S_BoxList()->ElementAt(index);

			GTDrawBox(p->X - (int)GmDc.CameraX, p->Y, p->W, p->H, p->Color);
		}
		for(int index = 0; index < S_CircleList()->GetCount(); index++)
		{
			Circle_t *p = S_CircleList()->ElementAt(index);

			GTDrawCircle(p->X - GmDc.CameraX, p->Y, p->R, p->Color);
		}
	}
	S_BoxList()->Clear();
	S_CircleList()->Clear();
}
