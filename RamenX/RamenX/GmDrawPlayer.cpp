#include "Stdinc.h"

void GmDrawCharacter(double x, double y, int h, int leftmode, double zoom)
{
	DrawBegin(x - GmDc.CameraX, y, h);

	if(leftmode)
	{
		DrawXZoom(-1.0);
	}
	DrawZoom(zoom);
	DrawEnd();
}

void GmDrawStand(double x, double y, int leftmode)
{
	const int CYCLE = 8;

	const double xHosei[CYCLE] =
	{
		 0.0,
		 0.0,
		-2.0,
		-2.0,
		-1.0,
		 1.0,
		 2.0,
		 1.0,
	};
	const double xLeftHosei[CYCLE] =
	{
		 0.0,
		 0.0,
		 1.0,
		 1.0,
		 1.0,
		-2.0,
		-2.0,
		-2.0,
	};
	const double yHosei[CYCLE] =
	{
		 0.0,
		 0.0,
		 0.0,
		-1.0,
		 0.0,
		 0.0,
		 1.0,
		 1.0,
	};
	int frmidx = (LFrmCnt / 7) % CYCLE;

	x = (double)(int)x;
	y = (double)(int)y;

	GmDrawCharacter(x + (leftmode ? xLeftHosei : xHosei)[frmidx], y + yHosei[frmidx], Pic(P_STAND_00 + frmidx), leftmode);
}
void GmDrawBack(double x, double y, int leftmode)
{
	const int CYCLE = 16;
	const double yHosei = 52.0;

	int frmidx = (LFrmCnt / 5) % CYCLE;

	CheckPicSize(Pic(P_BACK_00 + frmidx));
	GmDrawCharacter(x, y - Pic_H / 2.0 + yHosei, Pic(P_BACK_00 + frmidx), leftmode);
}
void GmDrawFront(double x, double y, int leftmode)
{
	const int CYCLE = 16;
	const double xHosei = 8.0;
	const double yHosei = 52.0;

	int frmidx = (LFrmCnt / 5) % CYCLE;

	CheckPicSize(Pic(P_FRONT_00 + frmidx));
	GmDrawCharacter(x + xHosei, y - Pic_H / 2.0 + yHosei, Pic(P_FRONT_00 + frmidx), leftmode);
}

const int GmBackDogezaMax = 12; // ç≈èâÇÃäGÇÃ count

void GmDrawBackDogeza(double x, double y, int count, int leftmode, int walkmode) // count: 1-
{
	const double yHosei = 52.0;

	ec_range(count, 1, TSLD);

	int frmidx = GmBackDogezaMax - count;

	if(frmidx < 0)
	{
		frmidx = walkmode ? frmidx & 1 : 0;
	}
	CheckPicSize(Pic(P_STANDUP_00 + frmidx));
	GmDrawCharacter(x, y - Pic_H / 2.0 + yHosei, Pic(P_STANDUP_00 + frmidx), leftmode);
}
void GmDrawJump(double x, double y, int count, int leftmode) // count: 1-
{
	const int FRMMAX = 12;
//	const double xHosei = (leftmode ? -1.0 : 1.0) * 10.0;
	const double xHosei = (leftmode ? -1.0 : 1.0) * 0.0;

	ec_range(count, 1, TSLD);

	int frmidx = (count - 1) / 4;

	if(FRMMAX <= frmidx)
	{
		frmidx = FRMMAX - 2 + ((frmidx - FRMMAX) & 1);
	}
	GmDrawCharacter(x + xHosei, y, Pic(P_JUMP_00 + frmidx), leftmode);
}
void GmDrawJumpBack(double x, double y, int count, int leftmode) // count: 1-
{
	const int FRMMAX = 9;

	ec_range(count, 1, TSLD);

	int frmidx = (count - 1) / 4;

	if(FRMMAX <= frmidx)
	{
		frmidx = FRMMAX - 2 + ((frmidx - FRMMAX) & 1);
	}
	GmDrawCharacter(x, y, Pic(P_JUMPBACK_00 + frmidx), leftmode);
}
void GmDrawJumpFront(double x, double y, int count, int leftmode) // count: 1-
{
	const int FRMMAX = 10;

	ec_range(count, 1, TSLD);

	int frmidx = (count - 1) / 4;

	if(FRMMAX <= frmidx)
	{
		frmidx = FRMMAX - 2 + ((frmidx - FRMMAX) & 1);
	}
	GmDrawCharacter(x, y, Pic(P_JUMPFRONT_00 + frmidx), leftmode);
}

void GmDrawDash(double x, double y, int count, int leftmode) // count: 1-
{
	const int FRMMAX = 6;

	ec_range(count, 1, TSLD);

	int frmidx = (count - 1) / 4;

	if(FRMMAX <= frmidx)
	{
		frmidx = 1 + ((frmidx - FRMMAX) % (FRMMAX - 1));
	}
	GmDrawCharacter(x, y, Pic(P_DASH_00 + frmidx), leftmode);
}

const int GmDashEndMax = 4; // ç≈èâÇÃäGÇÃcount

void GmDrawDashEnd(double x, double y, int count, int leftmode) // count: 1 - GmDashEndMax
{
	const double xHosei[GmDashEndMax] =
	{
		15.0,
		15.0,
		6.0,
		0.0,
	};
	const double yHosei = 52.0;

	ec_range(count, 1, GmDashEndMax);

	int frmidx = GmDashEndMax - count;

	x = (double)(int)x + xHosei[frmidx] * (leftmode ? -1.0 : 1.0);

	CheckPicSize(Pic(P_DASH_06 + frmidx));
	GmDrawCharacter(x, y - Pic_H / 2.0 + yHosei, Pic(P_DASH_06 + frmidx), leftmode);
}

const int GmAirDashEndMax = 6; // ç≈èâÇÃäGÇÃcount

void GmDrawAirDashEnd(double x, double y, int count, int leftmode) // count: 1 - GmAirDashEndMax
{
	ec_range(count, 1, GmAirDashEndMax);

	int frmidx = GmAirDashEndMax - count;

	GmDrawCharacter(x, y, Pic(P_AIRDASHEND_00 + frmidx), leftmode);
}

const int GmUAttackMax = 12; // ç≈å„ÇÃäGÇÃcount
const int GmCAttackMax = 15; // ç≈å„ÇÃäGÇÃcount
const int GmDAttackMax = 16; // ç≈å„ÇÃäGÇÃcount

void GmDrawUAttack(double x, double y, int count, int leftmode) // count: 1 - GmUAttackMax
{
	static double xHosei[GmUAttackMax] =
	{
		  0.0,
		  0.0,
		  9.0,
		 32.0,
		 12.0,
		  0.0,
		-25.0,
		-10.0,
		  0.0,
		  0.0,
		  0.0,
		-10.0,
	};
	static double yHosei[GmUAttackMax] =
	{
		 0.0,
		 2.0,
		 7.0,
		10.0,
		 3.0,
		 0.0,
		 0.0,
		 0.0,
		 0.0,
		 0.0,
		 0.0,
		 0.0,
	};
	const double xHoseiHosei = 10.0;
	const double yHoseiHosei = -5.0;

	ec_range(count, 1, GmUAttackMax);

	int frmidx = count - 1;

	x = (double)(int)x;
	y = (double)(int)y;

	GmDrawCharacter(
		x + (xHosei[frmidx] + xHoseiHosei) * (leftmode ? -1.0 : 1.0),
		y + (yHosei[frmidx] + yHoseiHosei), Pic(P_UATTACK_00 + frmidx), leftmode);
}
void GmDrawCAttack(double x, double y, int count, int leftmode) // count: 1 - GmCAttackMax
{
	static double xHosei[GmCAttackMax] =
	{
		    0.0,
		   -5.0,
		   -9.0,
		  -14.0,
		  -16.0,
		    8.0,
		  -10.0,
		  -10.0,
		   -1.0,
		    3.0,
		    3.0,
		    1.0,
		    0.0,
		    0.0,
		   -7.0,
	};
	static double yHosei[GmCAttackMax] =
	{
		    0.0,
		   -5.0,
		   -7.0,
		   -5.0,
		  -10.0,
		  -17.0,
		   -4.0,
		    0.0,
		  -23.0,
		  -20.0,
		  -13.0,
		   12.0,
		   12.0,
		   14.0,
		   16.0,
	};
	const double xHoseiHosei =  20.0;
	const double yHoseiHosei = -10.0;

	ec_range(count, 1, GmCAttackMax);

	int frmidx = count - 1;

	x = (double)(int)x;
	y = (double)(int)y;

	GmDrawCharacter(
		x + (xHosei[frmidx] + xHoseiHosei) * (leftmode ? -1.0 : 1.0),
		y + (yHosei[frmidx] + yHoseiHosei), Pic(P_CATTACK_00 + frmidx), leftmode);
}
void GmDrawDAttack(double x, double y, int count, int leftmode) // count: 1 - GmDAttackMax
{
	const double xHosei[GmDAttackMax] =
	{
		  0.0,
		 -5.0,
		-20.0,
		-25.0,
		-30.0,
		-10.0,
		 15.0,
		 21.0,
		-17.0,
		-11.0,
		 -4.0,
		  6.0,
		 -2.0,
		-11.0,
		-15.0,
		-18.0,
	};
	const double yHosei[GmDAttackMax] =
	{
		 0.0,
		-5.0,
		-2.0,
		 4.0,
		 0.0,
		 0.0,
		-2.0,
		-7.0,
		-6.0,
		-5.0,
		-5.0,
		 2.0,
		 3.0,
		 2.0,
		 2.0,
		 1.0,
	};
	const double xHoseiHosei = 15.0;
	const double yHoseiHosei = -1.0;

	ec_range(count, 1, GmDAttackMax);

	int frmidx = count - 1;

	x = (double)(int)x;
	y = (double)(int)y;

	GmDrawCharacter(
		x + (xHosei[frmidx] + xHoseiHosei) * (leftmode ? -1.0 : 1.0),
		y + (yHosei[frmidx] + yHoseiHosei), Pic(P_DATTACK_00 + frmidx), leftmode);
}

const int GmHitMax = 3; // ç≈å„ÇÃäGÇÃ count

void GmDrawUHit(double x, double y, int count, int leftmode) // count: 1 - GmHitMax
{
	ec_range(count, 1, GmHitMax);

	int frmidx = count - 1;

	GmDrawCharacter(x, y, Pic(P_UHIT_00 + frmidx), leftmode);
}
void GmDrawDHit(double x, double y, int count, int leftmode) // count: 1 - GmHitMax
{
	ec_range(count, 1, GmHitMax);

	int frmidx = count - 1;

	GmDrawCharacter(x, y, Pic(P_DHIT_00 + frmidx), leftmode);
}

void GmDrawAirDash(double x, double y, double rot)
{
	int frmidx = LFrmCnt;

	frmidx /= 4;
	frmidx &= 1;

	DrawBegin(x - GmDc.CameraX, y, Pic(P_AIR_00 + frmidx));
	DrawRotate(rot + PI);
	DrawEnd();
}
