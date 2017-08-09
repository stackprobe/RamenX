#include "Stdinc.h"

int GTCommonEffect(GTCommonEffect_t *i)
{
	if(0 < i->FrmCnt && !--i->FrmCnt)
	{
		return 0;
	}
	CheckPicSize(Pic(i->PicResno));

	double MARGIN_XY = 30.0 + (double)m_max(Pic_W, Pic_H) / 2.0;

	double x = i->X;
	double y = i->Y;

	if(i->ByCamera)
	{
		x -= GTCameraX;
		y -= GTCameraY;
	}
	if(x < -MARGIN_XY || 800.0 + MARGIN_XY < x)
	{
		return 0;
	}
	if(y < -MARGIN_XY || 600.0 + MARGIN_XY < y)
	{
		return 0;
	}
	if(i->Z <= 0.0)
	{
		return 0;
	}
	if(i->A <= 0.0)
	{
		return 0;
	}

	DrawBegin(x, y, Pic(i->PicResno));
	DrawZoom(i->Z);
	DrawRotate(i->R);

	if(i->BlendAddMode)
		SetBlendAdd(i->A);
	else
		SetAlpha(i->A);

	DrawEnd();
	ResetBlend();

	i->PicResno += i->PicResnoInc;

	i->X += i->XAdd;
	i->Y += i->YAdd;
	i->Z += i->ZAdd;
	i->R += i->RAdd;
	i->A += i->AAdd;

	i->XAdd += i->XAdd2;
	i->YAdd += i->YAdd2;
	i->ZAdd += i->ZAdd2;
	i->RAdd += i->RAdd2;
	i->AAdd += i->AAdd2;

	return 1;
}
int GTACE_BlendAddMode;
taskGroup_t *GTACE_ExtraTaskGroup;

GTCommonEffect_t *GTAddCommonEffect(
	int by_camera,
	int pic_resno,
	int pic_resno_inc,
	int frmnum,
	double x, double y, double z, double r, double a,
	double x_add, double y_add, double z_add, double r_add, double a_add,
	double x_add2, double y_add2, double z_add2, double r_add2, double a_add2
	)
{
	GTCommonEffect_t *i = (GTCommonEffect_t *)memAlloc(sizeof(GTCommonEffect_t));

	i->ByCamera = by_camera;
	i->PicResno = pic_resno;
	i->PicResnoInc = pic_resno_inc;
	i->FrmCnt = frmnum; // frmnum ‰ñ GTCommonEffect() ‚ðŽÀs‚·‚éB0 == –³§ŒÀ

	i->X = x;
	i->Y = y;
	i->Z = z;
	i->R = r;
	i->A = a;

	i->XAdd = x_add;
	i->YAdd = y_add;
	i->ZAdd = z_add;
	i->RAdd = r_add;
	i->AAdd = a_add;

	i->XAdd2 = x_add2;
	i->YAdd2 = y_add2;
	i->ZAdd2 = z_add2;
	i->RAdd2 = r_add2;
	i->AAdd2 = a_add2;

	i->BlendAddMode = GTACE_BlendAddMode;

	taskGroup_t *el = GTGetEffectList();

	if(GTACE_ExtraTaskGroup)
		el = GTACE_ExtraTaskGroup;

	return AddTask(el, GTCommonEffect, i, (void (*)(GTCommonEffect_t *))memFree);
}
