#include "Stdinc.h"

#define FIRE_R 10.0
#define BOSS_HP 50

#define MIKOBACK_BASEZOOM 0.5
#define MIKOBACK_BASE_A 0.2

/*
	ORIG_X: 0
	ORIG_Y: 20
	SPAN_X: 220
	SPAN_Y: 210

	Color_A != 0 であるピクセルの min_x, min_y, max_x, max_y
*/
const int MikoPosTbl[128] =
{
	54, 28, 198, 198, // 111
	44, 28, 188, 197, // 112
	43, 16, 187, 185, // 113
	48, 24, 192, 193, // 114
	24, 32, 168, 201, // 121
	18, 23, 162, 192, // 122
	15, 15, 159, 184, // 123
	13, 16, 157, 185, // 124
	45, 26, 189, 195, // 131
	40, 20, 184, 190, // 132
	43, 16, 187, 185, // 133
	44, 24, 188, 193, // 134
	40, 25, 184, 194, // 141
	42, 21, 185, 190, // 142
	41, 11, 185, 180, // 143
	34, 27, 178, 196, // 144
	45, 22, 189, 192, // 211
	45, 12, 189, 181, // 212
	45,  9, 188, 178, // 213
	47, 14, 191, 183, // 214
	37, 20, 181, 189, // 221
	36,  7, 180, 177, // 222
	33, 10, 177, 179, // 223
	33, 11, 177, 181, // 224
	51, 20, 195, 189, // 231
	51,  8, 195, 178, // 232
	48,  7, 192, 176, // 233
	44, 16, 188, 185, // 234
	26, 24, 170, 193, // 241
	35,  9, 179, 178, // 242
	38,  9, 181, 180, // 243
	25, 18, 169, 187, // 244
};

// ---- Miko* ----

typedef struct Miko_st
{
	double X;
	double Y;

	double ZureX;
	double ZureY;

	union
	{
		struct
		{
			GmEnemy_t (*NextMaker)(double, double);
		}
		Wound;

		struct
		{
			double R;
		}
		Miko02;

		struct
		{
			double VectR;
			double R;
			double BackA;
			double BackA_R;
			double BackR;
		}
		Miko03;

		struct
		{
			double R;
		}
		Miko04;

		struct
		{
			double BackA;
		}
		Miko05;

		struct
		{
			double GrandR;
			double GrandRAdd;
			double R;
		}
		Miko06;

		struct
		{
			int LastHP;
			int DamageDZCount;
			double KomachiBackA;
		}
		Miko07;

		struct
		{
			double A;
			int StealthMode;
		}
		Death;
	}
	u;
}
Miko_t;

static int MikoPostActor_Slow(GmEnemy_t *i, Miko_t *ed, int slow_mode)
{
	static double mSubX;
	static double mSubY;

	if(!LFrmCnt) // init
	{
		mSubX = ed->X;
		mSubY = ed->Y;
	}

	P2Vector(mSubX, mSubY, ed->X, ed->Y, 2.0);

	mSubX += Vect_X;
	mSubY += Vect_Y;

	double rate = 0.97;
	if(slow_mode)
		rate = 0.99;

	m_countApprox(i->X, mSubX, rate, 0.0);
	m_countApprox(i->Y, mSubY, rate, 0.0);

#define SCOPECOLOR_MIKOPOS RGB2I(255, 128, 0, SCOPEALPHA)

	GmAddCircleScope(ed->X, ed->Y, 10.0, SCOPECOLOR_MIKOPOS);
	GmAddCircleScope(mSubX, mSubY, 10.0, SCOPECOLOR_MIKOPOS);
	GmAddCircleScope(i->X, i->Y, 10.0, SCOPECOLOR_MIKOPOS);

	return 1;
}
static int MikoPostActor(GmEnemy_t *i, Miko_t *ed)
{
	return MikoPostActor_Slow(i, ed, 0);
}
static void MikoPostDrawer(GmEnemy_t *i, Miko_t *ed, int pIndex, int muteki_mode)
{
	int leftmode = GmDc.Player.X < i->X ? 1 : 0;

	errorCase(pIndex < 0 || 32 <= pIndex);

	GmBeforeDrawEnemy(i);
	GmDrawCharacter(
		i->X + ed->ZureX,
		i->Y + ed->ZureY,
		Pic((D_MIKO_111 + pIndex) | DTP),
		leftmode ? 0 : 1
		);
	GmAfterDrawEnemy(i);

	GmAddCircleScope(i->X, i->Y + 20.0, 70.0, muteki_mode ? SCOPECOLOR_TAMA : SCOPECOLOR_ENEMY);
}
static int MikoCommonIsCrashed(GmEnemy_t *i, Miko_t *ed, double x, double y, double w, double h)
{
	return IsCrashed_BoxCircle(x, y, w, h, i->X, i->Y + 20.0, 70.0);
}
static void ReleaseMikoCommon(Miko_t *i)
{
	memFree(i);
}

// ---- MikoWound ----

static int MikoWoundActor(GmEnemy_t *i, Miko_t *ed)
{
	ed->X = 450.0;
	ed->Y = 300.0;

	ed->ZureX = ernd() * 10.0;
	ed->ZureY = ernd() * 10.0;

	GmDc.ForceTamaShoumetsuMode = i->FrmCnt <= 10 ? 1 : 0;

	if(i->FrmCnt == 0)
	{
		SEPlay(SE_SEMIDEATH_BOSS);

		GTACE_BlendAddMode = 1;
		GTAddCommonEffect(1, D_CRASH_00 | DTP, 1, 20, i->X, i->Y, 2.0);
		GTACE_BlendAddMode = 0;

//		GTAddCommonEffect(1, P_CRASH_00, 1, 18, i->X, i->Y, 5.0); // old

		i->DamageEffectDZCount = 30;
	}
	if(i->FrmCnt == 90)
	{
		S_EnemyEntryQueue()->AddElement(ed->u.Wound.NextMaker(i->X, i->Y));
		return 0;
	}
	return MikoPostActor(i, ed);
}
static void MikoWoundDrawer(GmEnemy_t *i, Miko_t *ed)
{
	MikoPostDrawer(i, ed, 23, 1);
}
static int MikoWoundIsCrashed(GmEnemy_t *i, Miko_t *ed, double x, double y, double w, double h)
{
	return MikoCommonIsCrashed(i, ed, x, y, w, h);
}
static void ReleaseMikoWound(Miko_t *i)
{
	ReleaseMikoCommon(i);
}
GmEnemy_t MakeMikoWound(double x, double y, GmEnemy_t (*next_maker)(double, double))
{
	Miko_t *ed = nb_(Miko_t);

	ed->u.Wound.NextMaker = next_maker;

	return MakeEnemy(MAKE_BOSS, x, y, 0, 0,
		MikoWoundActor,
		MikoWoundDrawer,
		MikoWoundIsCrashed, ReleaseMikoWound, ed);
}

// ---- Miko01 ----

static int Miko01Actor(GmEnemy_t *i, Miko_t *ed)
{
	ifsceneBegin(i->FrmCnt, 60)
	{
		ed->X = 450.0;
		ed->Y = 400.0;
	}
	ifscene(60)
	{
		ed->Y = 100.0;
	}
	ifsceneEnd
	{
		S_EnemyEntryQueue()->AddElement(MakeMiko02(i->X, i->Y));
//		S_EnemyEntryQueue()->AddElement(MakeMiko03(i->X, i->Y)); // test
//		S_EnemyEntryQueue()->AddElement(MakeMiko04(i->X, i->Y)); // test
//		S_EnemyEntryQueue()->AddElement(MakeMiko05(i->X, i->Y)); // test
//		S_EnemyEntryQueue()->AddElement(MakeMiko06(i->X, i->Y)); // test
//		S_EnemyEntryQueue()->AddElement(MakeMiko07(i->X, i->Y)); // test
//		S_EnemyEntryQueue()->AddElement(MakeMikoDeath(i->X, i->Y)); // test
		return 0;
	}
	return MikoPostActor(i, ed);
}
static void Miko01Drawer(GmEnemy_t *i, Miko_t *ed)
{
	MikoPostDrawer(i, ed, 2, 1);
}
static int Miko01IsCrashed(GmEnemy_t *i, Miko_t *ed, double x, double y, double w, double h)
{
	return MikoCommonIsCrashed(i, ed, x, y, w, h);
}
static void ReleaseMiko01(Miko_t *i)
{
	ReleaseMikoCommon(i);
}
GmEnemy_t MakeMiko01(double x, double y)
{
	x = 450.0;
	y = -100.0;

	return MakeEnemy(MAKE_BOSS, x, y, 0, 0,
		Miko01Actor,
		Miko01Drawer,
		Miko01IsCrashed, ReleaseMiko01);
}

// ---- Miko02 ----

static void AddFire02(double x, double y, double r)
{
	double x_vec = cos(r);
	double y_vec = sin(r);
	double x_shot = x + x_vec * 100.0;
	double y_shot = y + y_vec * 100.0;

	GmCommonShootEffect_L(x_shot, y_shot, D_W_STAR | DTP);
	S_EnemyEntryQueue()->AddElement(MakeFire02(x_shot, y_shot, x_vec, y_vec));

	/*
		隙間対策 for 02
	*/
	for(int c = 0; c < 5; c++)
	{
		const double SKM_R = 60.0 + c * 10.0;
		const double SKM_H = 20.0;

		S_EnemyEntryQueue()->AddElement(MakeDamageArea_Circle(
			x + x_vec * SKM_R,
			y + y_vec * SKM_R,
			SKM_H,
			3 // 2フレーム
			));
	}
}

static int Miko02Actor(GmEnemy_t *i, Miko_t *ed)
{
	if(!i->HP) // ? Semi-Death
	{
		S_EnemyEntryQueue()->AddElement(MakeMikoWound(i->X, i->Y, MakeMiko03));
		return 0;
	}
	ed->X = 450.0 + sin((double)i->FrmCnt / 40.0) * 100.0;
	ed->Y = 300.0 + cos((double)i->FrmCnt / 40.0) * 100.0;

	if(i->FrmCnt % 10 == 0)
	{
		AddFire02(i->X, i->Y, ed->u.Miko02.R);
		AddFire02(i->X, i->Y, ed->u.Miko02.R + (PI * 2.0) / 3.0);
		AddFire02(i->X, i->Y, ed->u.Miko02.R + (PI * 4.0) / 3.0);

		ed->u.Miko02.R += 0.2;
	}
	return MikoPostActor(i, ed);
}
static void Miko02Drawer(GmEnemy_t *i, Miko_t *ed)
{
	MikoPostDrawer(i, ed, 0, 0);
}
static int Miko02IsCrashed(GmEnemy_t *i, Miko_t *ed, double x, double y, double w, double h)
{
	return MikoCommonIsCrashed(i, ed, x, y, w, h);
}
static void ReleaseMiko02(Miko_t *i)
{
	ReleaseMikoCommon(i);
}
GmEnemy_t MakeMiko02(double x, double y)
{
	return MakeEnemy(MAKE_BOSS, x, y, 0, BOSS_HP,
		Miko02Actor,
		Miko02Drawer,
		Miko02IsCrashed, ReleaseMiko02);
}

// ---- Fire02 ----

typedef struct Fire02_st
{
	double VectX;
	double VectY;
}
Fire02_t;

static int Fire02Actor(GmEnemy_t *i, Fire02_t *ed)
{
	if(!CheckTamaShoumetsu(i))
		return 0;

	ifsceneBegin(i->FrmCnt, 60)
	{
		const double SPEED = 9.0;

		i->X += ed->VectX * (1.0 - fs_rate) * SPEED;
		i->Y += ed->VectY * (1.0 - fs_rate) * SPEED;
	}
	ifsceneEnd
	{
		GmCommonShootEffect_L(i->X, i->Y, D_W_STAR | DTP);

		const double SPEED = 3.0;
		const double SPEED_ADD = 1.5;

		for(int c = 0; c < 5; c++)
		for(int d = 0; d < 2; d++)
		{
			S_EnemyEntryQueue()->AddElement(MakeTama(
				MAKE_BOSSFIRE, i->X, i->Y,
				ed->VectY * (SPEED + c * SPEED_ADD) * (d ?  1.0 : -1.0),
				ed->VectX * (SPEED + c * SPEED_ADD) * (d ? -1.0 :  1.0),
				D_Y_STAR | DTP
				));
		}
		return 0;
	}
	return 1;
}
static void Fire02Drawer(GmEnemy_t *i, Fire02_t *ed)
{
	GmBeforeDrawEnemy(i);

	DrawBegin(i->X - GmDc.CameraX, i->Y, Pic(D_Y_FIRE1 | DTP));
	DrawZoom(2.0);
	DrawRotate(LFrmCnt / 10.0);
	DrawEnd();

	DrawBegin(i->X - GmDc.CameraX, i->Y, Pic(D_Y_FIRE4 | DTP));
	DrawZoom(3.0);
	DrawRotate(LFrmCnt / 13.0);
	DrawEnd();

	GmAfterDrawEnemy(i);

	GmAddCircleScope(i->X, i->Y, FIRE_R * 2.0, SCOPECOLOR_TAMA);
}
static int Fire02IsCrashed(GmEnemy_t *i, Fire02_t *ed, double x, double y, double w, double h)
{
	return IsCrashed_BoxCircle(x, y, w, h, i->X, i->Y, FIRE_R * 2.0);
}
static void ReleaseFire02(Fire02_t *i)
{
	memFree(i);
}
GmEnemy_t MakeFire02(double x, double y, double x_vec, double y_vec)
{
	Fire02_t *ed = nb_(Fire02_t);

	ed->VectX = x_vec;
	ed->VectY = y_vec;

	return MakeEnemy(MAKE_BOSSFIRE, x, y, 0, 0,
		Fire02Actor,
		Fire02Drawer,
		Fire02IsCrashed, ReleaseFire02, ed);
}

// ---- Miko03 ----

static singleton(Miko03Fire03RndSet, endlessQueue<double>, makeDRndSet(10000, 35031));
static singleton(Miko03PosRndSet,    endlessQueue<double>, makeDRndSet(10000, 35032));

void Miko03Touch(void)
{
	S_Miko03Fire03RndSet();
	S_Miko03PosRndSet();
}

static int Miko03Actor(GmEnemy_t *i, Miko_t *ed)
{
	if(!i->FrmCnt) // init
	{
		ed->X += 450.0;
		ed->Y += 200.0;

		S_Miko03Fire03RndSet()->Reset();
	}
	else // 2nd...
	{
		if(i->FrmCnt % 100 == 0)
		{
			double dr1 = S_Miko03PosRndSet()->Dequeue();
			double dr2 = S_Miko03PosRndSet()->Dequeue();

			ed->X = 400.0 + dr1 * 100.0;
			ed->Y = 100.0 + dr2 * 100.0;
		}
	}

	if(!i->HP) // ? Semi-Death
	{
		S_EnemyEntryQueue()->AddElement(MakeMikoWound(i->X, i->Y, MakeMiko04));
		return 0;
	}

	if(i->FrmCnt % (20 + (i->HP * 10) / BOSS_HP) == 0)
	{
		double dr1 = S_Miko03Fire03RndSet()->Dequeue();
		double dr2 = S_Miko03Fire03RndSet()->Dequeue();
		double dr3 = S_Miko03Fire03RndSet()->Dequeue();

		S_EnemyEntryQueue()->AddElement(MakeFire03(
			ed->X + (150.0 + dr1 * 250.0) * (dr2 < 0.5 ? 1.0 : -1.0),
			-100.0,
			5.0 + dr3 * 2.0
			));
	}
	ed->u.Miko03.VectR += 0.004;
	ed->u.Miko03.R += sin(ed->u.Miko03.VectR) * 0.04;
	ed->u.Miko03.BackA = m_min(1.0, ed->u.Miko03.BackA + 0.003);
	ed->u.Miko03.BackA_R += 0.01;
	ed->u.Miko03.BackR += sin(ed->u.Miko03.VectR - 0.3) * 0.02;

	return MikoPostActor(i, ed);
}
static void Miko03Drawer(GmEnemy_t *i, Miko_t *ed)
{
	DrawBegin(i->X - GmDc.CameraX, i->Y, Pic(P_MIKO_BACK));
	DrawZoom(2.0 * MIKOBACK_BASEZOOM);
	DrawRotate(ed->u.Miko03.BackR);
	SetBlendAdd(ed->u.Miko03.BackA * (sin(ed->u.Miko03.BackA_R) * 0.3 + 0.7) * MIKOBACK_BASE_A);
	DrawEnd();
	ResetBlend();

	double tamaX = i->X + cos(ed->u.Miko03.R) * 130.0;
	double tamaY = i->Y + sin(ed->u.Miko03.R) * 130.0 + 10.0;

	if(!i->FrmCnt)
	{
		const int C_MAX = 12;

		for(int c = 0; c < C_MAX; c++)
		{
			double vecR = 2.0 * PI * ((double)c / C_MAX);

			GTAddCommonEffect(1, D_C_STAR | DTP, 0, 0, tamaX, tamaY,
				4.0, 0.0, 1.0, cos(vecR) * 10.0, sin(vecR) * 5.0,
				0.0, 1.0, -0.03, cos(vecR) * 0.2, sin(vecR) * 0.1);
		}
	}

	DrawBegin(tamaX - GmDc.CameraX, tamaY, Pic(D_C_FIRE1 | DTP));
	DrawZoom(3.5);
	DrawRotate(LFrmCnt / 3.0);
	DrawEnd();

	DrawBegin(tamaX - GmDc.CameraX, tamaY, Pic(D_C_FIRE4 | DTP));
	DrawZoom(5.0);
	DrawRotate(LFrmCnt / 5.0);
	DrawEnd();

	if(drnd() < 0.3)
		GTAddCommonEffect(1, D_C_STAR | DTP, 0, 0,
			tamaX + ernd() * 5.0,
			tamaY + ernd() * 5.0,
			4.0, 2.0 * PI * drnd(), 0.5,
			0.0, 0.0, -0.1, ernd() * 0.1, -0.02);

	S_EnemyEntryQueue()->AddElement(MakeDamageArea_Circle(tamaX, tamaY, 45.0, 2));

	MikoPostDrawer(i, ed, GmDc.Player.HitBackDZCount ? 14 : 13, 0);
}
static int Miko03IsCrashed(GmEnemy_t *i, Miko_t *ed, double x, double y, double w, double h)
{
	return MikoCommonIsCrashed(i, ed, x, y, w, h);
}
static void ReleaseMiko03(Miko_t *i)
{
	ReleaseMikoCommon(i);
}
GmEnemy_t MakeMiko03(double x, double y)
{
	return MakeEnemy(MAKE_BOSS, x, y, 0, BOSS_HP,
		Miko03Actor,
		Miko03Drawer,
		Miko03IsCrashed, ReleaseMiko03);
}

// ---- Fire03 ----

#define F03_SPEED 1.0
#define F03_R_SPEED 0.003

#define F03_CENTER_X 450.0
#define F03_CENTER_Y 200.0

typedef struct Fire03_st
{
	double SpeedRate;
	double YAdd;
	int CurveMode;
	double CurveR;
	double CurveRAdd;
	double CurveHankei;
}
Fire03_t;

static int Fire03Actor(GmEnemy_t *i, Fire03_t *ed)
{
	if(!CheckTamaShoumetsu(i))
		return 0;

	if(drnd() < 0.3)
		GTAddCommonEffect(1, D_C_STAR | DTP, 0, 0,
			i->X + ernd() * 5.0,
			i->Y + ernd() * 5.0,
			2.0, 2.0 * PI * drnd(), 0.5,
			0.0, 0.0, -0.05, ernd() * 0.1, -0.02);

	if(ed->CurveMode)
	{
		ed->CurveR += ed->CurveRAdd;

		i->X = F03_CENTER_X + cos(ed->CurveR) * ed->CurveHankei;
		i->Y = F03_CENTER_Y + sin(ed->CurveR) * ed->CurveHankei;

		if(i->Y < F03_CENTER_Y)
		{
			ed->CurveMode = 0;
			ed->YAdd = -F03_SPEED * ed->SpeedRate;
		}
		return 1;
	}
	i->Y += ed->YAdd;

	if(ed->YAdd < 0.0) // ? 上昇
	{
		if(i->Y < -100.0) // ? 画面外
		{
			return 0;
		}
	}
	else // ? 下降
	{
		if(F03_CENTER_Y < i->Y)
		{
			ed->CurveMode = 1;

			if(F03_CENTER_X < i->X) // ? R++
			{
				ed->CurveR = 0.0;
				ed->CurveRAdd = F03_R_SPEED * ed->SpeedRate;
				ed->CurveHankei = i->X - F03_CENTER_X;
			}
			else // ? R--
			{
				ed->CurveR = PI;
				ed->CurveRAdd = -F03_R_SPEED * ed->SpeedRate;
				ed->CurveHankei = F03_CENTER_X - i->X;
			}
		}
	}
	return 1;
}
static void Fire03Drawer(GmEnemy_t *i, Fire03_t *ed)
{
	GmBeforeDrawEnemy(i);

	DrawBegin(i->X - GmDc.CameraX, i->Y, Pic(D_C_FIRE1 | DTP));
	DrawZoom(2.0);
	DrawRotate(LFrmCnt / 10.0);
	DrawEnd();

	DrawBegin(i->X - GmDc.CameraX, i->Y, Pic(D_C_FIRE4 | DTP));
	DrawZoom(3.0);
	DrawRotate(LFrmCnt / 13.0);
	DrawEnd();

	GmAfterDrawEnemy(i);

	GmAddCircleScope(i->X, i->Y, FIRE_R * 2.0, SCOPECOLOR_TAMA);
}
static int Fire03IsCrashed(GmEnemy_t *i, Fire03_t *ed, double x, double y, double w, double h)
{
	return IsCrashed_BoxCircle(x, y, w, h, i->X, i->Y, FIRE_R * 2.0);
}
static void ReleaseFire03(Fire03_t *i)
{
	memFree(i);
}
GmEnemy_t MakeFire03(double x, double y, double speed_rate)
{
	Fire03_t *ed = nb_(Fire03_t);

	ed->SpeedRate = speed_rate;
	ed->YAdd = F03_SPEED * speed_rate;

	return MakeEnemy(MAKE_BOSSFIRE, x, y, 0, 0,
		Fire03Actor,
		Fire03Drawer,
		Fire03IsCrashed, ReleaseFire03, ed);
}

// ---- Miko04 ----

static void AddFire04(double x, double y, double r, int color_green)
{
	double x_vec = cos(r);
	double y_vec = sin(r);
	double x_shot = x + x_vec * 100.0;
	double y_shot = y + y_vec * 100.0;

	GmCommonShootEffect_L(x_shot, y_shot, D_W_STAR | DTP);
	S_EnemyEntryQueue()->AddElement(MakeFire04(x_shot, y_shot, x_vec, y_vec, color_green));

	/*
		隙間対策 for 04
	*/
	for(int c = 0; c < 5; c++)
	{
		const double SKM_R = 60.0 + c * 10.0;
		const double SKM_H = 20.0;

		S_EnemyEntryQueue()->AddElement(MakeDamageArea_Circle(
			x + x_vec * SKM_R,
			y + y_vec * SKM_R,
			SKM_H,
			3 // 2フレーム
			));
	}
}

static int Miko04Actor(GmEnemy_t *i, Miko_t *ed)
{
	if(!i->HP) // ? Semi-Death
	{
		S_EnemyEntryQueue()->AddElement(MakeMikoWound(i->X, i->Y, MakeMiko05));
		return 0;
	}
	ed->X = 450.0 + sin((double)i->FrmCnt / 40.0) * 100.0;
	ed->Y = 300.0 + cos((double)i->FrmCnt / 40.0) * 100.0;

	if(30 <= i->FrmCnt && i->FrmCnt % 10 == 0)
	{
		AddFire04(i->X, i->Y, ed->u.Miko04.R, 0);
		AddFire04(i->X, i->Y, ed->u.Miko04.R + PI, 1);

		ed->u.Miko04.R -= 0.2;
	}
	return MikoPostActor(i, ed);
}
static void Miko04Drawer(GmEnemy_t *i, Miko_t *ed)
{
	MikoPostDrawer(i, ed, 0, 0);
}
static int Miko04IsCrashed(GmEnemy_t *i, Miko_t *ed, double x, double y, double w, double h)
{
	return MikoCommonIsCrashed(i, ed, x, y, w, h);
}
static void ReleaseMiko04(Miko_t *i)
{
	ReleaseMikoCommon(i);
}
GmEnemy_t MakeMiko04(double x, double y)
{
	return MakeEnemy(MAKE_BOSS, x, y, 0, BOSS_HP,
		Miko04Actor,
		Miko04Drawer,
		Miko04IsCrashed, ReleaseMiko04);
}

// ---- Fire04 ----

typedef struct Fire04_st
{
	int ColorGreen;
	double VectX;
	double VectY;
}
Fire04_t;

static int Fire04Actor(GmEnemy_t *i, Fire04_t *ed)
{
	if(!CheckTamaShoumetsu(i))
		return 0;

	ifsceneBegin(i->FrmCnt, 60)
	{
		const double SPEED = 10.0;

		i->X += ed->VectX * (1.0 - fs_rate) * SPEED;
		i->Y += ed->VectY * (1.0 - fs_rate) * SPEED;
	}
	ifscene(20)
	{
		// noop
	}
	ifscene(180)
	{
		if(!fs_count)
		{
			P2Vector(i->X, i->Y, GmDc.Player.X, GmDc.Player.Y, 1.0);
			ed->VectX = Vect_X;
			ed->VectY = Vect_Y;

			int ts_pr = D_D_STAR | DTP;

			if(ed->ColorGreen)
				ts_pr = D_G_STAR | DTP;

			for(int c = 1; c <= 5; c++)
			{
				const double TS_SPEED = c * 0.3;
				const double TS_KASOKU = 0.01;

				S_EnemyEntryQueue()->AddElement(MakeTama(
					MAKE_BOSSFIRE, i->X, i->Y,
					-ed->VectX * TS_SPEED,
					-ed->VectY * TS_SPEED, ts_pr,
					-ed->VectX * TS_KASOKU,
					-ed->VectY * TS_KASOKU));
			}
		}
		const double SPEED = 7.0;

		i->X += ed->VectX * fs_rate * SPEED;
		i->Y += ed->VectY * fs_rate * SPEED;
	}
	ifsceneEnd
	{
		GmCommonShootEffect_L(i->X, i->Y, D_W_STAR | DTP);

		/*
		const double SPEED = 3.0;
//		P2Vector(i->X, i->Y, GmDc.Player.X, GmDc.Player.Y, SPEED);
		S_EnemyEntryQueue()->AddElement(MakeTama(
			MAKE_BOSSFIRE, i->X, i->Y,
			ed->VectX * SPEED,
			ed->VectY * SPEED,
			D_G_STAR | DTP));
			*/

		for(int x = -1; x <= 1; x += 2)
		for(int y = -1; y <= 1; y += 2)
		{
			/*
			S_EnemyEntryQueue()->AddElement(MakeTama(
				MAKE_BOSSFIRE, i->X, i->Y,
				x * SPEED,
				y * SPEED,
				D_G_STAR | DTP));
			*/
		}
		return 0;
	}
	return 1;
}
static void Fire04Drawer(GmEnemy_t *i, Fire04_t *ed)
{
	int pr1 = D_D_FIRE1 | DTP;
	int pr4 = D_D_FIRE4 | DTP;

	if(ed->ColorGreen)
	{
		pr1 = D_G_FIRE1 | DTP;
		pr4 = D_G_FIRE4 | DTP;
	}
	GmBeforeDrawEnemy(i);

	DrawBegin(i->X - GmDc.CameraX, i->Y, Pic(pr1));
	DrawZoom(2.0);
	DrawRotate(LFrmCnt / 10.0);
	DrawEnd();

	DrawBegin(i->X - GmDc.CameraX, i->Y, Pic(pr4));
	DrawZoom(3.0);
	DrawRotate(LFrmCnt / 13.0);
	DrawEnd();

	GmAfterDrawEnemy(i);

	GmAddCircleScope(i->X, i->Y, FIRE_R * 2.0, SCOPECOLOR_TAMA);
}
static int Fire04IsCrashed(GmEnemy_t *i, Fire04_t *ed, double x, double y, double w, double h)
{
	return IsCrashed_BoxCircle(x, y, w, h, i->X, i->Y, FIRE_R * 2.0);
}
static void ReleaseFire04(Fire04_t *i)
{
	memFree(i);
}
GmEnemy_t MakeFire04(double x, double y, double x_vec, double y_vec, int color_green)
{
	Fire04_t *ed = nb_(Fire04_t);

	ed->ColorGreen = color_green;
	ed->VectX = x_vec;
	ed->VectY = y_vec;

	return MakeEnemy(MAKE_BOSSFIRE, x, y, 0, 0,
		Fire04Actor,
		Fire04Drawer,
		Fire04IsCrashed, ReleaseFire04, ed);
}

// ---- Miko05 ----

static int F5ICount;

static void AddFire05(double x, double y, double x_dest)
{
	GmCommonShootYokoku_L(x, y, D_W_STAR | DTP);
	S_EnemyEntryQueue()->AddElement(MakeFire05(x, y, x_dest, F5ICount));
	F5ICount += 1111;
}
static void AddFire05Ex(double x, double y, double r, double r_add, int chr_type)
{
	S_EnemyEntryQueue()->AddElement(MakeFire05Ex(x, y, r, r_add, chr_type));
}

static int Miko05Actor(GmEnemy_t *i, Miko_t *ed)
{
	if(!i->HP) // ? Semi-Death
	{
		S_EnemyEntryQueue()->AddElement(MakeMikoWound(i->X, i->Y, MakeMiko06));
		return 0;
	}
	ed->X = 200.0 + (i->HP * 500.0) / BOSS_HP;
	ed->Y = 200.0 + sin(LFrmCnt / 30.0) * 30.0;

	const int SUBCYC = 300;
	int subcnt = i->FrmCnt / SUBCYC;
	int subidx = i->FrmCnt % SUBCYC;

	if(subcnt % 3 == 2)
	{
		const int SUBCYC2 = 10;
		int subcnt2 = subidx / SUBCYC2;
		int subidx2 = subidx % SUBCYC2;

		if(!subidx2)
		{
			double r = PI * ((double)subcnt2 / 15.0);

			AddFire05Ex(i->X, i->Y, r, 0.1, 'A');
			AddFire05Ex(i->X, i->Y, r + PI * 0.666, 0.1, 'B');
			AddFire05Ex(i->X, i->Y, r + PI * 1.333, 0.1, 'C');
		}
	}
	else
	{
		const int SUBCYC2 = 30;
		int subcnt2 = subidx / SUBCYC2;
		int subidx2 = subidx % SUBCYC2;

		if(!subidx2 && subcnt2 < 5)
		{
			AddFire05(i->X, i->Y - 100.0, 50.0 + subcnt2 * 200.0);
		}
	}
	return MikoPostActor(i, ed);
}
static void Miko05Drawer(GmEnemy_t *i, Miko_t *ed)
{
	for(int c = 0; c < 3; c++)
	{
		double r1 = LFrmCnt / 60.0 + c * 0.2;
		double r2 = LFrmCnt / 70.0 + c * 0.2;

		if(c == 1) {
			r1 *= 1.03;
			r2 *= 1.05;
			r2 = -r2;
		}
		if(c == 2) {
			r1 *= 1.07;
			r2 *= 1.13;
		}

		double yz = sin(r1);
		if(abs(yz) < 0.1)
			continue;

		double a = m_min(0.6, i->FrmCnt / 300.0);

		DrawBegin(i->X - GmDc.CameraX, i->Y, Pic(P_MIKO_BACK));
		DrawZoom(1.75 * MIKOBACK_BASEZOOM);
		DrawRotate(2.0 * PI * ((double)c / 3.0));
		DrawYZoom(yz);
		DrawRotate(r2);
		SetBlendAdd(abs(yz) * a * MIKOBACK_BASE_A);
		DrawEnd();
		ResetBlend();
	}
	MikoPostDrawer(i, ed, GmDc.Player.HitBackDZCount ? 8 : 12, 0);
}
static int Miko05IsCrashed(GmEnemy_t *i, Miko_t *ed, double x, double y, double w, double h)
{
	return MikoCommonIsCrashed(i, ed, x, y, w, h);
}
static void ReleaseMiko05(Miko_t *i)
{
	ReleaseMikoCommon(i);
}

static singleton(Fire05RRndList,    autoList<double>, makeDRndList(10000, 35051));
static singleton(Fire05StarRndList, autoList<int>,    makeRRndList(10000, 35052, MIN_D_X_STAR, MAX_D_X_STAR));

void Miko05Touch(void)
{
	S_Fire05RRndList();
	S_Fire05StarRndList();
}
GmEnemy_t MakeMiko05(double x, double y)
{
	F5ICount = 0;

	return MakeEnemy(MAKE_BOSS, x, y, 0, BOSS_HP,
		Miko05Actor,
		Miko05Drawer,
		Miko05IsCrashed, ReleaseMiko05);
}

// ---- Fire05 ----

typedef struct Fire05_st
{
	double DestX;
	int RndIndex;
}
Fire05_t;

static int Fire05Actor(GmEnemy_t *i, Fire05_t *ed)
{
	if(!CheckTamaShoumetsu(i))
		return 0;

	ifsceneBegin(i->FrmCnt, 90)
	{
		double x_dest = ed->DestX;
		double y_dest = 500.0;

		m_countApprox(i->X, x_dest, 0.94, 0.0);
		m_countApprox(i->Y, y_dest, 0.96, 0.0);
	}
	ifscene(30)
	{
		// noop
	}
	ifsceneEnd
	{
		i->Y -= 10.0;

		if(i->Y < 100.0)
		{
			GmCommonShootEffect_L(i->X, i->Y, D_W_STAR | DTP);
			return 0;
		}

#if 0 // old
		double r = 2.0 * PI * drnd();
		int pic_resno = rrnd(MIN_D_X_STAR, MAX_D_X_STAR) | DTP;
#else
		double r = 2.0 * PI * S_Fire05RRndList()->GetElement_Endless(ed->RndIndex);
		int pic_resno = S_Fire05StarRndList()->GetElement_Endless(ed->RndIndex) | DTP;

		ed->RndIndex++;
#endif

		S_EnemyEntryQueue()->AddElement(MakeTama(
			MAKE_BOSSFIRE,
			i->X, i->Y,
			cos(r), sin(r), pic_resno,
			0.0, 0.1
			));
		GmCommonShootEffect_S(i->X, i->Y, pic_resno);
	}
	return 1;
}
static void Fire05Drawer(GmEnemy_t *i, Fire05_t *ed)
{
	if(drnd() < 0.3)
		GTAddCommonEffect(1, D_W_STAR | DTP, 0, 0, i->X, i->Y,
			2.0, 2.0 * PI * drnd(), 0.5,
			0.0, 0.0, -0.05, ernd() * 0.1, -0.02);

	GmBeforeDrawEnemy(i);

	DrawBegin(i->X - GmDc.CameraX, i->Y, Pic(D_W_FIRE1 | DTP));
	DrawZoom(2.0);
	DrawRotate(LFrmCnt / 10.0);
	DrawEnd();

	DrawBegin(i->X - GmDc.CameraX, i->Y, Pic(D_W_FIRE4 | DTP));
	DrawZoom(3.0);
	DrawRotate(LFrmCnt / 13.0);
	DrawEnd();

	GmAfterDrawEnemy(i);

	GmAddCircleScope(i->X, i->Y, FIRE_R * 2.0, SCOPECOLOR_TAMA);
}
static int Fire05IsCrashed(GmEnemy_t *i, Fire05_t *ed, double x, double y, double w, double h)
{
	return IsCrashed_BoxCircle(x, y, w, h, i->X, i->Y, FIRE_R * 2.0);
}
static void ReleaseFire05(Fire05_t *i)
{
	memFree(i);
}
GmEnemy_t MakeFire05(double x, double y, double x_dest, int rndIndex)
{
	Fire05_t *ed = nb_(Fire05_t);

	ed->DestX = x_dest;
	ed->RndIndex = rndIndex;

	return MakeEnemy(MAKE_BOSSFIRE, x, y, 0, 0,
		Fire05Actor,
		Fire05Drawer,
		Fire05IsCrashed, ReleaseFire05, ed);
}

// ---- Fire05Ex ----

typedef struct Fire05Ex_st
{
	int ChrType; // "ABC"
	double CentX;
	double CentY;
	double H;
	double R;
	double RAdd;

	double LastX;
	double LastY;
}
Fire05Ex_t;

static int Fire05ExActor(GmEnemy_t *i, Fire05Ex_t *ed)
{
	if(!CheckTamaShoumetsu(i))
		return 0;

	if(1000.0 < ed->H)
		return 0;

	ed->H += 1.5;
	ed->R += ed->RAdd;
	ed->RAdd *= 0.99;

	ed->LastX = i->X;
	ed->LastY = i->Y;

	i->X = ed->CentX + cos(ed->R) * ed->H;
	i->Y = ed->CentY + sin(ed->R) * ed->H;

	if(!i->FrmCnt) // 1st
	{
		GmCommonShootEffect_L(i->X, i->Y, D_W_STAR | DTP);
	}
	return 1;
}
static void Fire05ExDrawer(GmEnemy_t *i, Fire05Ex_t *ed)
{
	/*
		SetBlendAdd(); がかなり重いようだ。引数 0.5 のせい??
		Fire_5 を撃って次の Fire_4 を撃ち始めたころに GmDrawEnemy(); が 0.5 ms を越えるようになる。
		全ステージ通しでプレイしても(コンスタントに) 0.5 ms を越えてくるのはここくらいしかない。
		画面外は描画しないよう(直下のとおり)にして、まずは 0.5 ms を切ってくれた。
	*/
	if(!GmInCamera(i, 20.0))
		return;

	double r_vec = P2Angle(ed->LastX, ed->LastY, i->X, i->Y);

	GmBeforeDrawEnemy(i);

	DrawBegin(i->X - GmDc.CameraX, i->Y, Pic(D_W_FIRE1 | DTP));
	DrawZoom(3.0);
	DrawRotate(LFrmCnt / 3.0);
	SetAlpha(0.5); // ba2a
//	SetBlendAdd(0.5); // 重い!?
	DrawEnd();
	ResetBlend();

	int pr;

	switch(ed->ChrType)
	{
	case 'A': pr = D_P_FIRE2 | DTP; break;
	case 'B': pr = D_R_FIRE2 | DTP; break;
	case 'C': pr = D_Y_FIRE2 | DTP; break;

	default:
		error();
	}
	DrawBegin(i->X - GmDc.CameraX, i->Y, Pic(pr));
	DrawZoom(2.0);
	DrawRotate(r_vec + PI * 0.5);
	DrawEnd();

	GmAfterDrawEnemy(i);

	GmAddCircleScope(i->X, i->Y, FIRE_R * 2.0, SCOPECOLOR_TAMA);
}
static int Fire05ExIsCrashed(GmEnemy_t *i, Fire05Ex_t *ed, double x, double y, double w, double h)
{
	return IsCrashed_BoxCircle(x, y, w, h, i->X, i->Y, FIRE_R * 2.0);
}
static void ReleaseFire05Ex(Fire05Ex_t *i)
{
	memFree(i);
}
GmEnemy_t MakeFire05Ex(double x, double y, double r, double r_add, int chr_type)
{
	Fire05Ex_t *ed = nb_(Fire05Ex_t);

	ed->ChrType = chr_type;
	ed->CentX = x;
	ed->CentY = y;
	ed->H = 100.0;
	ed->R = r;
	ed->RAdd = r_add;

	return MakeEnemy(MAKE_BOSSFIRE, x, y, 0, 0,
		Fire05ExActor,
		Fire05ExDrawer,
		Fire05ExIsCrashed, ReleaseFire05Ex, ed);
}

// ---- Miko06 ----

static void AddFire06(double x, double y, double r, int chr_color) // chr_color: "DRPBCGYW"
{
	int pre;
	int pr1;
	int pr2;

	switch(chr_color)
	{
	case 'D': pre = D_D_STAR | DTP; pr1 = D_D_FIRE1 | DTP; pr2 = D_D_FIRE4 | DTP; break;
	case 'R': pre = D_R_STAR | DTP; pr1 = D_R_FIRE1 | DTP; pr2 = D_R_FIRE4 | DTP; break;
	case 'P': pre = D_P_STAR | DTP; pr1 = D_P_FIRE1 | DTP; pr2 = D_P_FIRE4 | DTP; break;
	case 'B': pre = D_B_STAR | DTP; pr1 = D_B_FIRE1 | DTP; pr2 = D_B_FIRE4 | DTP; break;
	case 'C': pre = D_C_STAR | DTP; pr1 = D_C_FIRE1 | DTP; pr2 = D_C_FIRE4 | DTP; break;
	case 'G': pre = D_G_STAR | DTP; pr1 = D_G_FIRE1 | DTP; pr2 = D_G_FIRE4 | DTP; break;
	case 'Y': pre = D_Y_STAR | DTP; pr1 = D_Y_FIRE1 | DTP; pr2 = D_Y_FIRE4 | DTP; break;
	case 'W': pre = D_W_STAR | DTP; pr1 = D_W_FIRE1 | DTP; pr2 = D_W_FIRE4 | DTP; break;

	default:
		error();
	}
	const double R = 100.0;
	double x_shot = x + cos(r) * R;
	double y_shot = y + sin(r) * R;

	GmCommonShootEffect_S(x_shot, y_shot, pre);
	S_EnemyEntryQueue()->AddElement(MakeFire06(x_shot, y_shot, r, pr1, pr2));

	/*
		隙間対策 for 06

		耳子の当り半径 70.0, 中心は下に 20.0
		弾出現位置 100.0
		弾当り半径 15.0, 速度 8.0 (移動してから描画する)
		なので、最大 35.0 + 8.0 の隙間が空く、これを埋める。
	*/
	for(int c = 0; c < 5; c++)
	{
		const double SKM_R = 60.0 + c * 10.0;
		const double SKM_H = 15.0;

		S_EnemyEntryQueue()->AddElement(MakeDamageArea_Circle(
			x + cos(r) * SKM_R,
			y + sin(r) * SKM_R,
			SKM_H,
			3 // 2フレーム
			));
	}

}
static int F6NextColor(void)
{
//	const char *clst = "DRPBCGYW";
	const char *clst = "RPBCGYW";
//	const int cnum = 8;
	const int cnum = 7;
	static int index;

	index %= cnum;
	return clst[index++];
}

static int Miko06Actor(GmEnemy_t *i, Miko_t *ed)
{
	if(!i->HP) // ? Semi-Death
	{
		S_EnemyEntryQueue()->AddElement(MakeMikoWound(i->X, i->Y, MakeMiko07));
		return 0;
	}
	int subcnt = i->FrmCnt / 180;

	int dx = 60;
	int dy = 30;

	if((subcnt / 2) & 1) dx *= -1;
	if((subcnt / 3) & 1) dy *= -1;

	ed->X = 450.0 + dx;
	ed->Y = 250.0 - dy;

	if(i->FrmCnt % 20 == 0)
	{
		double gr = ed->u.Miko06.GrandR;
		double r = ed->u.Miko06.R;

		AddFire06(i->X, i->Y,              r + gr, F6NextColor());
		AddFire06(i->X, i->Y, PI * 0.666 + r + gr, F6NextColor());
		AddFire06(i->X, i->Y, PI * 1.333 + r + gr, F6NextColor());
		AddFire06(i->X, i->Y,             -r + gr, F6NextColor());
		AddFire06(i->X, i->Y, PI * 0.666 - r + gr, F6NextColor());
		AddFire06(i->X, i->Y, PI * 1.333 - r + gr, F6NextColor());

		ed->u.Miko06.GrandR += ed->u.Miko06.GrandRAdd;
		ed->u.Miko06.GrandRAdd += 0.003;
		ed->u.Miko06.R += 0.1;

		PIRanger(ed->u.Miko06.GrandR);
		PIRanger(ed->u.Miko06.GrandRAdd);
		PIRanger(ed->u.Miko06.R);
	}
	return MikoPostActor(i, ed);
}
static void Miko06Drawer(GmEnemy_t *i, Miko_t *ed)
{
	MikoPostDrawer(i, ed, 0, 0);
}
static int Miko06IsCrashed(GmEnemy_t *i, Miko_t *ed, double x, double y, double w, double h)
{
	return MikoCommonIsCrashed(i, ed, x, y, w, h);
}
static void ReleaseMiko06(Miko_t *i)
{
	ReleaseMikoCommon(i);
}
GmEnemy_t MakeMiko06(double x, double y)
{
	return MakeEnemy(MAKE_BOSS, x, y, 0, BOSS_HP,
		Miko06Actor,
		Miko06Drawer,
		Miko06IsCrashed, ReleaseMiko06);
}

// ---- Fire06 ----

typedef struct Fire06_st
{
	int PicResno1;
	int PicResno2;
	double R;
}
Fire06_t;

static int Fire06Actor(GmEnemy_t *i, Fire06_t *ed)
{
	if(!CheckTamaShoumetsu(i))
		return 0;

	if(!GmInCamera(i, 100.0))
		return 0;

	const double SPEED = 8.0;
	double xa = cos(ed->R) * SPEED;
	double ya = sin(ed->R) * SPEED;

	i->X += xa;
	i->Y += ya;

	return 1;
}
static void Fire06Drawer(GmEnemy_t *i, Fire06_t *ed)
{
	if(i->FrmCnt % 3 == 0)
		GTAddCommonEffect(1, D_W_STAR | DTP, 0, 0, i->X, i->Y,
			2.0, 2.0 * PI * drnd(), 0.5,
			0.0, 0.0, -0.05, ernd() * 0.1, -0.02);

	GmBeforeDrawEnemy(i);
//	SetBlendAdd(1.0);

	DrawBegin(i->X - GmDc.CameraX, i->Y, Pic(ed->PicResno1));
	DrawZoom(1.5);
	DrawRotate(LFrmCnt / 10.0);
	DrawEnd();

	DrawBegin(i->X - GmDc.CameraX, i->Y, Pic(ed->PicResno1));
	DrawZoom(2.0);
	DrawRotate(LFrmCnt / 13.0);
	DrawEnd();

//	ResetBlend();
	GmAfterDrawEnemy(i);

	GmAddCircleScope(i->X, i->Y, FIRE_R * 1.5, SCOPECOLOR_TAMA);
}
static int Fire06IsCrashed(GmEnemy_t *i, Fire06_t *ed, double x, double y, double w, double h)
{
	return IsCrashed_BoxCircle(x, y, w, h, i->X, i->Y, FIRE_R * 1.5);
}
static void ReleaseFire06(Fire06_t *i)
{
	memFree(i);
}
GmEnemy_t MakeFire06(double x, double y, double r, int pr1, int pr2)
{
	Fire06_t *ed = nb_(Fire06_t);

	ed->PicResno1 = pr1; // maybe (D_*_FIRE1 | DTP)
	ed->PicResno2 = pr2; // maybe (D_*_FIRE4 | DTP)
	ed->R = r;

	return MakeEnemy(MAKE_BOSSFIRE, x, y, 0, 0,
		Fire06Actor,
		Fire06Drawer,
		Fire06IsCrashed, ReleaseFire06, ed);
}

// ---- Miko07 ----

static void AddFire07(double x, double y, double speed)
{
	GmCommonShootEffect_L(x, y, D_P_STAR | DTP);
	P2Vector(x, y, GmDc.Player.X, GmDc.Player.Y, speed);
	S_EnemyEntryQueue()->AddElement(MakeFire07(x, y, Vect_X, Vect_Y));
}
static void M7DamageAction(GmEnemy_t *i, Miko_t *ed, int hp)
{
	const int SHOT_HP_40 = (BOSS_HP * 4) / 5;
	const int SHOT_HP_30 = (BOSS_HP * 3) / 5;

	if(hp == SHOT_HP_40)
	{
		AddFire07(i->X, i->Y, 5.0);
	}
	else if(hp == SHOT_HP_30)
	{
		AddFire07(i->X, i->Y, 7.0);
	}
	ed->u.Miko07.DamageDZCount = 30;
}

static int Miko07Actor(GmEnemy_t *i, Miko_t *ed)
{
	if(!i->HP) // ? Semi-Death
	{
		S_EnemyEntryQueue()->AddElement(MakeMikoDeath(i->X, i->Y));
		return 0;
	}
	if(!i->FrmCnt) // init
	{
		ed->u.Miko07.LastHP = i->HP;
		AddFire07(i->X, i->Y, 3.0);
	}
	if(i->FrmCnt % 60 == 0)
	{
		const double MARGIN_XY = 170.0;
		const double FIELD_W = GmDc.FieldWidth;
		const double FIELD_H = 600.0;
		const double BORDER_X = 450.0;
		const double BORDER_Y = 300.0;

		if(GmDc.Player.X < BORDER_X) // 左
		{
			if(GmDc.Player.Y < BORDER_Y) // 左上
			{
				ed->X = FIELD_W - MARGIN_XY;
				ed->Y = FIELD_H - MARGIN_XY;
			}
			else // 左下
			{
				ed->X = FIELD_W - MARGIN_XY;
				ed->Y = MARGIN_XY;
			}
		}
		else // 右
		{
			if(GmDc.Player.Y < BORDER_Y) // 右上
			{
				ed->X = MARGIN_XY;
				ed->Y = FIELD_H - MARGIN_XY;
			}
			else // 右下
			{
				ed->X = MARGIN_XY;
				ed->Y = MARGIN_XY;
			}
		}
	}
	m_countDown(ed->u.Miko07.DamageDZCount);
	ed->u.Miko07.KomachiBackA = m_min(1.0, ed->u.Miko07.KomachiBackA + 0.01);

	while(i->HP < ed->u.Miko07.LastHP)
	{
		ed->u.Miko07.LastHP--;
		M7DamageAction(i, ed, ed->u.Miko07.LastHP); // (MAX - 1) ... 1
	}
	return MikoPostActor_Slow(i, ed, ed->u.Miko07.DamageDZCount ? 1 : 0);
}
static void Miko07Drawer(GmEnemy_t *i, Miko_t *ed)
{
	// Draw KOMACHI_BACK
	{
		DrawBegin(GmDc.Player.X - GmDc.CameraX, GmDc.Player.Y, Pic(P_MIKO_KOMACHI_BACK));
		DrawZoom(1.5);
		DrawRotate(LFrmCnt / 20.0);
		SetBlendAdd(ed->u.Miko07.KomachiBackA); // a2ba
		DrawEnd();
		ResetBlend();
	}

	for(int c = 0; c < 3; c++)
	{
		double r1 = LFrmCnt / 101.0 + c * PI * 0.666;
		double r2 = LFrmCnt / 181.0;

		double yz = sin(r1);
		if(abs(yz) < 0.1)
			continue;

		double a = m_min(0.9, i->FrmCnt / 300.0);

		DrawBegin(i->X - GmDc.CameraX, i->Y, Pic(P_MIKO_BACK));
		DrawZoom(1.5 * MIKOBACK_BASEZOOM);
		DrawRotate(LFrmCnt / 2011.0);
		DrawYZoom(yz);
		DrawRotate(r2);
		SetBlendAdd(abs(yz) * a * MIKOBACK_BASE_A);
		DrawEnd();
		ResetBlend();
	}
	int face_pi;

	if(ed->u.Miko07.DamageDZCount)
	{
		face_pi = 23;

		ed->ZureX = ernd() * 3.0;
		ed->ZureY = ernd() * 3.0;
	}
	else
	{
		if(i->HP <= (BOSS_HP * 1) / 5)
		{
			face_pi = GmDc.Player.HitBackDZCount ? 11 : 15;
		}
		else if(i->HP <= (BOSS_HP * 2) / 5)
		{
			face_pi = GmDc.Player.HitBackDZCount ? 30 : 19;
		}
		else
		{
			face_pi = GmDc.Player.HitBackDZCount ? 29 : 18;
		}
		ed->ZureX = 0.0;
		ed->ZureY = 0.0;
	}
	MikoPostDrawer(i, ed, face_pi, 0);
}
static int Miko07IsCrashed(GmEnemy_t *i, Miko_t *ed, double x, double y, double w, double h)
{
	return MikoCommonIsCrashed(i, ed, x, y, w, h);
}
static void ReleaseMiko07(Miko_t *i)
{
	ReleaseMikoCommon(i);
}
GmEnemy_t MakeMiko07(double x, double y)
{
	return MakeEnemy(MAKE_BOSS, x, y, 0, BOSS_HP,
		Miko07Actor,
		Miko07Drawer,
		Miko07IsCrashed, ReleaseMiko07);
}

// ---- Fire07 ----

typedef struct Fire07_st
{
	double XAdd;
	double YAdd;
	int NearPlayerMode;
}
Fire07_t;

static int Fire07Actor(GmEnemy_t *i, Fire07_t *ed)
{
	// ボムとかで消滅しない。
	/*
	if(!CheckTamaShoumetsu(i))
		return 0;
	*/

	if(GmDc.Fire07_ForceDeathMode)
	{
		GTAddCommonEffect(1, D_REDRING_01 | DTP, 1, 10, i->X, i->Y, 2.0);
		return 0;
	}
	const double HANE_MARGIN_XY = 40.0;
	const double FIELD_W = GmDc.FieldWidth;
	const double FIELD_H = 600.0;

	if(i->X < HANE_MARGIN_XY)
	{
		ed->XAdd = abs(ed->XAdd);
	}
	if(i->Y < HANE_MARGIN_XY)
	{
		ed->YAdd = abs(ed->YAdd);
	}
	if(FIELD_W - HANE_MARGIN_XY < i->X)
	{
		ed->XAdd = -abs(ed->XAdd);
	}
	if(FIELD_H - HANE_MARGIN_XY < i->Y)
	{
		ed->YAdd = -abs(ed->YAdd);
	}
	double x_add2 = 0.01;
	double y_add2 = 0.01;

	if(GmDc.Player.X < i->X) x_add2 *= -1.0;
	if(GmDc.Player.Y < i->Y) y_add2 *= -1.0;

	ed->XAdd += x_add2;
	ed->YAdd += y_add2;

	i->X += ed->XAdd;
	i->Y += ed->YAdd;

	ed->NearPlayerMode = m_isNear(i->X, i->Y, GmDc.Player.X, GmDc.Player.Y, 200.0) ? 1 : 0;

	if(ed->NearPlayerMode)
	{
		if(60 <= i->FrmCnt && i->FrmCnt % 2 == 0)
		{
			P2Vector(i->X, i->Y, GmDc.Player.X, GmDc.Player.Y, 1.0);

			double x_vec = Vect_X;
			double y_vec = Vect_Y;

			S_EnemyEntryQueue()->AddElement(MakeTama(
				MAKE_BOSSFIRE, i->X, i->Y,
				x_vec * 3.0,
				y_vec * 3.0,
				D_P_STAR | DTP,
				x_vec * 0.3,
				y_vec * 0.3
				));
		}
		if(i->FrmCnt % 4 == 0)
			SEPlay(SE_FIRE07_GREZE);
	}
	return 1;
}
static void Fire07Drawer(GmEnemy_t *i, Fire07_t *ed)
{
	double x = i->X - GmDc.CameraX;
	double y = i->Y;

	if(ed->NearPlayerMode)
	{
		x += ernd() * 3.0;
		y += ernd() * 3.0;
	}

//	GmBeforeDrawEnemy(i);
	SetAlpha(1.0); // ba2a

	DrawBegin(x, y, Pic(D_P_FIRE1 | DTP));
	DrawZoom(4.0);
	DrawRotate(LFrmCnt / 13.0);
	DrawEnd();

	DrawBegin(x, y, Pic(D_P_FIRE4 | DTP));
	DrawZoom(5.0);
	DrawRotate(LFrmCnt / 11.0);
	DrawEnd();

	ResetBlend();
//	GmAfterDrawEnemy(i);

	GmAddCircleScope(i->X, i->Y, FIRE_R * 4.0, SCOPECOLOR_TAMA);
}
static int Fire07IsCrashed(GmEnemy_t *i, Fire07_t *ed, double x, double y, double w, double h)
{
	return IsCrashed_BoxCircle(x, y, w, h, i->X, i->Y, FIRE_R * 2.0);
}
static void ReleaseFire07(Fire07_t *i)
{
	memFree(i);
}
GmEnemy_t MakeFire07(double x, double y, double x_add, double y_add)
{
	Fire07_t *ed = nb_(Fire07_t);

	ed->XAdd = x_add;
	ed->YAdd = y_add;

	return MakeEnemy(MAKE_BOSSFIRE, x, y, 0, 0,
		Fire07Actor,
		Fire07Drawer,
		Fire07IsCrashed, ReleaseFire07, ed);
}

// ---- MikoDeath ----

static int MikoDeathActor(GmEnemy_t *i, Miko_t *ed)
{
	int tamakeshi = i->FrmCnt < 10 ? 1 : 0;

	GmDc.ForceTamaShoumetsuMode = tamakeshi;
	GmDc.Fire07_ForceDeathMode = tamakeshi;

	ifsceneBegin(i->FrmCnt, 180)
	{
		if(!fs_count) // 1st
		{
			SEPlay(SE_SEMIDEATH_BOSS);

			GTACE_BlendAddMode = 1;
			GTAddCommonEffect(1, D_CRASH_00 | DTP, 1, 20, i->X, i->Y, 2.0);
			GTACE_BlendAddMode = 0;

//			GTAddCommonEffect(1, P_CRASH_00, 1, 18, i->X, i->Y, 5.0); // old

			i->DamageEffectDZCount = 30;

			ed->X = 450.0;
			ed->Y = 300.0;
		}
		ed->u.Death.A = fs_rate;
	}
	ifscene(120)
	{
		if(!fs_count) // 1st
		{
			SEPlay(SE_DEATH_BOSS);
			ed->u.Death.StealthMode = 1;

			for(int c = 0; c < 256; c++)
			{
				double speed = 5.0 + drnd() * 10.0;
				double r = 2.0 * PI * drnd();

//				GTACE_BlendAddMode = 1; ba2a
				GTAddCommonEffect(
					1,
//					rrnd(MIN_D_X_STAR, MAX_D_X_STAR) | DTP,
					rrnd(MIN_D_X_STAR_COLOR, MAX_D_X_STAR_COLOR) | DTP,
					0,
					0,
					i->X,
					i->Y,
					2.0 + drnd() * 3.0,
					2.0 * PI * drnd(),
					1.0,
					cos(r) * speed,
					sin(r) * speed,
					drnd() * 0.03,
					ernd() * 0.5
					);
//				GTACE_BlendAddMode = 0;
			}
		}
		double r = 20.0 * (1.0 - fs_rate);

		ScreenSlip_X = m_d2i(ernd() * r);
		ScreenSlip_Y = m_d2i(ernd() * r);
	}
	ifsceneEnd
	{
		return 0;
	}
	return MikoPostActor(i, ed);
}
static void MikoDeathDrawer(GmEnemy_t *i, Miko_t *ed)
{
	if(ed->u.Death.StealthMode)
		return;

	int leftmode = (LFrmCnt / 30) & 1;

	GmDrawCharacter(
		i->X + ernd() * 10.0,
		i->Y + ernd() * 10.0,
		Pic(D_MIKO_211 | DTP),
		leftmode
		);

	for(int c = 0; c < 5; c++)
	{
		SetAlpha(ed->u.Death.A);
		GmDrawCharacter(
			i->X + ernd() * 20.0,
			i->Y + ernd() * 20.0,
			Pic(D_MIKO_211 | DTP),
			leftmode
			);
		ResetBlend();
	}
}
static int MikoDeathIsCrashed(GmEnemy_t *i, Miko_t *ed, double x, double y, double w, double h)
{
	return 0;
}
static void ReleaseMikoDeath(Miko_t *i)
{
	ReleaseMikoCommon(i);
}
GmEnemy_t MakeMikoDeath(double x, double y)
{
	return MakeEnemy(MAKE_BOSS, x, y, 0, 0,
		MikoDeathActor,
		MikoDeathDrawer,
		MikoDeathIsCrashed, ReleaseMikoDeath);
}
