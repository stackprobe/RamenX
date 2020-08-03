/*
	敵の初期配置をActorで行うため初期位置を適当な値にした場合、
	その位置が画面に入らないと IsFarEnemy() で弾かれ続けることに注意してね。

	発射予告は 30 フレーム前
*/

#include "Stdinc.h"

int CheckTamaShoumetsu(GmEnemy_t *i) // ret: ? 生存
{
	if(
		GmDc.ForceTamaShoumetsuMode ||
		GmDc.BombDZCount == 1 || GmDc.BombDZCount == 20 // Bomb 発動中 -> 画面上の弾消滅
		)
	{
		GTAddCommonEffect(1, D_REDRING_01 | DTP, 1, 10, i->X, i->Y);
		return 0;
	}
	return 1;
}

// ---- Tama ----

typedef struct Tama_st
{
	void *ShooterMaker;
	int PicResno;
	double X;
	double Y;
	double XAdd;
	double YAdd;
	double XAdd2;
	double YAdd2;

	struct
	{
		double R;
		double RAdd;
		double RAddRate;
		double Hankei;
		double HankeiAdd;
		double HankeiAdd2;
		double HankeiMax;
	}
	Rot;

	double R; // 表示用
	double RAdd;
	int CameraIn; // ? 一度でも画面内に入ったか。
	int CameraOutCount;
}
Tama_t;

static int TamaActor(GmEnemy_t *i, Tama_t *ed)
{
	const double SCREEN_MARGIN_XY = 20.0;

	if(!i->FrmCnt) // init
	{
//		i->FrmCnt = rnd(1);
		ed->R = drnd() * PI * 2.0;
		ed->RAdd = 0.1 * (rrnd(0, 1) ? -1.0 : 1.0);
		ed->CameraIn = 0;
	}
	if(!CheckTamaShoumetsu(i))
		return 0;

	// 移動 >

	i->X = ed->X;
	i->Y = ed->Y;

	if(ed->Rot.Hankei != 0.0)
	{
		i->X += cos(ed->Rot.R) * ed->Rot.Hankei;
		i->Y += sin(ed->Rot.R) * ed->Rot.Hankei;
	}

	ed->X += ed->XAdd;
	ed->Y += ed->YAdd;
	ed->XAdd += ed->XAdd2;
	ed->YAdd += ed->YAdd2;

	ed->Rot.R += ed->Rot.RAdd;
	ed->Rot.RAdd *= ed->Rot.RAddRate;
	ed->Rot.Hankei += ed->Rot.HankeiAdd;
	ed->Rot.HankeiAdd += ed->Rot.HankeiAdd2;

	// < 移動

	ed->R += ed->RAdd;

	if(!ed->CameraIn)
		ed->CameraIn = GmInCamera(i);

	// 2bs
	if(!GmInCamera(i))
	{
		if(60 * 60 < ed->CameraOutCount)
		{
			return 0;
		}
		ed->CameraOutCount++;
	}
	else
		ed->CameraOutCount = 0;

	if(ed->ShooterMaker == MakeMarisa &&
		i->Y < 0.0 - SCREEN_MARGIN_XY
		)
		return 0;

	if(
		ed->ShooterMaker == MakeCube ||
		ed->ShooterMaker == MakeMarisa ||
		ed->ShooterMaker == MakeAya
		)
		if(i->FrmCnt < 60 * 15)
			return 1;

	if(ed->Rot.HankeiMax < ed->Rot.Hankei)
	{
		double x = i->X - GmDc.CameraX;
		double y = i->Y;

		const double MARGIN_2 = SCREEN_MARGIN_XY;
		const double MARGIN_4 = SCREEN_MARGIN_XY;
		const double MARGIN_6 = SCREEN_MARGIN_XY;
		const double MARGIN_8 = 1000.0;

		if(x < -MARGIN_4 || 800.0 + MARGIN_6 < x)
		{
			return 0;
		}
		if(y < -MARGIN_8 || 600.0 + MARGIN_2 < y)
		{
			return 0;
		}
	}
	return 1;
}
static void TamaDrawer(GmEnemy_t *i, Tama_t *ed)
{
	if(!GmInCamera(i, 20.0))
		return;

	GmBeforeDrawEnemy(i);
	DrawBegin(i->X - GmDc.CameraX, i->Y, Pic(ed->PicResno));
	DrawRotate(ed->R);
	DrawEnd();
	GmAfterDrawEnemy(i);

	GmAddCircleScope(i->X, i->Y, 10, SCOPECOLOR_TAMA);
}
static int TamaIsCrashed(GmEnemy_t *i, Tama_t *ed, double x, double y, double w, double h)
{
	return IsCrashed_BoxCircle(x, y, w, h, i->X, i->Y, 10);
}
static void ReleaseTama(Tama_t *i)
{
	memFree(i);
}
/*
	pic_resno: (D_*_STAR | DTP) を想定
*/
GmEnemy_t MakeTama(
	void *shooter_maker,
	double x, double y, double x_add, double y_add, int pic_resno,
	double x_add2, double y_add2,
	double rot_r, double rot_r_add, double rot_r_add_rate,
	double rot_h, double rot_h_add, double rot_h_add2,
	double rot_h_max
	)
{
	Tama_t *ed = nb_(Tama_t);

	ed->ShooterMaker = shooter_maker;
	ed->PicResno = pic_resno;
	ed->X = x;
	ed->Y = y;
	ed->XAdd = x_add;
	ed->YAdd = y_add;
	ed->XAdd2 = x_add2;
	ed->YAdd2 = y_add2;

	ed->Rot.R = rot_r;
	ed->Rot.RAdd = rot_r_add;
	ed->Rot.RAddRate = rot_r_add_rate;
	ed->Rot.Hankei = rot_h;
	ed->Rot.HankeiAdd = rot_h_add;
	ed->Rot.HankeiAdd2 = rot_h_add2;
	ed->Rot.HankeiMax = rot_h_max;

	return MakeEnemy(MakeTama, x, y, 0, 0, TamaActor, TamaDrawer, TamaIsCrashed, ReleaseTama, ed); // no level
}

// ---- DOLL ----

typedef struct Doll_st
{
	double XAdd;

	struct
	{
		int FrameCount;
		int NotNeedCyclone;
		double DestX;
		double DestY;
		double NeraiX;
		double NeraiY;
		double CommonR;
	}
	Lv3;
}
Doll_t;

static singleton(DollLv3RndSet, endlessQueue<double>, makeDRndSet(100, 109));

static void AddD3Tama01(GmEnemy_t *i, Doll_t *ed)
{
//	if(i->HP <= 12)
	{
		double x = i->X;
		double y = i->Y;

		GmCommonShootEffect_L(x, y, D_C_STAR | DTP);

		P2Vector(x, y, GmDc.Player.X, GmDc.Player.Y, 12.0);

		S_EnemyEntryQueue()->AddElement(MakeD3Tama01(x, y,  Vect_Y, -Vect_X, D_C_STAR | DTP));
		S_EnemyEntryQueue()->AddElement(MakeD3Tama01(x, y, -Vect_Y,  Vect_X, D_C_STAR | DTP));
	}
}
static int DollActor(GmEnemy_t *i, Doll_t *ed)
{
	if(i->Lv == 3)
	{
		double cx = i->X - GmDc.CameraX;

		if(cx < 0.0 || 800.0 < cx)
		{
			ed->Lv3.FrameCount = 0;
			ed->Lv3.NotNeedCyclone = 0;
		}
		if(ed->Lv3.FrameCount == 0) // init, re-init
		{
			do
			{
				double r1 = S_DollLv3RndSet()->Dequeue();
				double r2 = S_DollLv3RndSet()->Dequeue();

				ed->Lv3.DestX = GmDc.CameraX + 300.0 + r1 * 200.0;
				ed->Lv3.DestY = 50.0 + r2 * 50.0;
			}
			while(m_isNear(i->X, i->Y, ed->Lv3.DestX, ed->Lv3.DestY, 100.0));
		}
		ifsceneBegin(ed->Lv3.FrameCount, 30)
		{
			// noop
		}
		ifscene(50) // Cyclone Phase
		{
			if(ed->Lv3.NotNeedCyclone)
			{
				if(fs_count % 3 == 0)
				{
					const int C_MAX = 14;

					for(int c = 0; c < C_MAX; c++)
					{
						const double nx = cos(2.0 * PI * ((double)c / C_MAX)) * 10.0;
						const double ny = sin(2.0 * PI * ((double)c / C_MAX)) *  8.0;

						S_EnemyEntryQueue()->AddElement(MakeTama(
							MakeDoll,
							i->X,
							i->Y,
							nx,
							ny,
							D_B_STAR | DTP
							));
					}
					GmCommonShootEffect_S(i->X, i->Y, D_B_STAR | DTP);
				}
			}
			else // Cyclone
			{
				const double nx = cos(1.05 * PI * fs_rate) * 10.0;
				const double ny = sin(1.05 * PI * fs_rate) *  8.0;

				S_EnemyEntryQueue()->AddElement(MakeTama(MakeDoll, i->X, i->Y,  nx,  ny, D_B_STAR | DTP));
				S_EnemyEntryQueue()->AddElement(MakeTama(MakeDoll, i->X, i->Y, -nx, -ny, D_B_STAR | DTP));

				if(fs_count % 3 == 0)
					GmCommonShootEffect_S(i->X, i->Y, D_B_STAR | DTP);
			}
		}
		ifscene(30)
		{
			if(!fs_count)
				ed->Lv3.NotNeedCyclone = 1;

			// noop
		}
		ifscene(540 - 1)
		{
			fs_count %= 180;

			if(fs_count == 0)
			{
				AddD3Tama01(i, ed);

				const double com_r = 2.0 * PI * drnd();
				const int C_MAX = 32;

				for(int c = 0; c < C_MAX; c++)
				{
					S_EnemyEntryQueue()->AddElement(MakeTama(
						MakeDoll,
						i->X,
						i->Y,
						0.0, 0.0,
						D_B_STAR | DTP,
						0.0, 0.0,
						com_r + 2.0 * PI * ((double)c / C_MAX), 0.7, 0.97,
						0.0, 1.9, 0.01,
						800.0
						));
				}
				P2Vector(i->X, i->Y, GmDc.Player.X, GmDc.Player.Y, 6.0);
				ed->Lv3.NeraiX = Vect_X;
				ed->Lv3.NeraiY = Vect_Y;
			}
			if(fs_count % 5 == 0)
			{
				S_EnemyEntryQueue()->AddElement(MakeTama(
					MakeDoll,
					i->X,
					i->Y,
					ed->Lv3.NeraiX,
					ed->Lv3.NeraiY,
					D_B_STAR | DTP
					));
				GmCommonShootEffect_S(i->X, i->Y, D_B_STAR | DTP);
			}
		}
		ifscene(60)
		{
			// noop
		}
		ifscene(330 - 1)
		{
			fs_count %= 110;

			if(fs_count == 0)
			{
				const int R_MAX = 18;

				for(int r = 0; r < R_MAX; r++)
				for(int rd = -1; rd <= 1; rd += 2)
				{
					S_EnemyEntryQueue()->AddElement(MakeTama(
						MakeDoll,
						i->X,
						i->Y,
						0.0, 0.0,
						D_B_STAR | DTP,
						0.0, 0.0,
						2.0 * PI * ((double)r / R_MAX), rd * 0.01, 0.995,
						0.0, 3.0, 0.0,
						800.0
						));
				}
				GmCommonShootYokoku_S(i->X, i->Y, D_B_STAR | DTP);
			}
			if(fs_count == 55)
			{
				AddD3Tama01(i, ed);
			}
		}
		ifscene(60)
		{
			// noop
		}
		ifscene(600 - 1)
		{
			fs_count %= 200;

			if(fs_count == 0)
			{
				ed->Lv3.CommonR = 2.0 * PI * drnd();
			}
			if(fs_count <= 60 && fs_count % 5 == 0)
			{
				const double com_r = ed->Lv3.CommonR;
				const int R_MAX = 8;

				for(int r = 0; r < R_MAX; r++)
				for(int rd = -1; rd <= 1; rd += 2)
				{
					S_EnemyEntryQueue()->AddElement(MakeTama(
						MakeDoll,
						i->X,
						i->Y,
						0.0, 0.0,
						D_B_STAR | DTP,
						0.0, 0.0,
						com_r + 2.0 * PI * ((double)r / R_MAX), rd * 0.09, 0.98,
						0.0, 3.0, 0.03,
						800.0
						));
				}
				GmCommonShootEffect_S(i->X, i->Y, D_B_STAR | DTP);
			}
			if(fs_count == 130)
			{
				AddD3Tama01(i, ed);
			}
		}
		ifscene(60)
		{
			// noop
		}
		ifsceneEnd
		{
			ed->Lv3.FrameCount = -1; // to re-init
		}
		m_countApprox(i->X, ed->Lv3.DestX, 0.97, 0.0);
		m_countApprox(i->Y, ed->Lv3.DestY, 0.99, 0.0);

		ed->Lv3.FrameCount++;
		return 1;
	}
	if(!i->FrmCnt) // init
	{
		i->FrmCnt += rnd(1000);
		ed->XAdd = 0.2 * (GmDc.Player.X < i->X ? -1.0 : 1.0);
	}
	if(i->FrmCnt % 300 == 270)
	{
		GmCommonShootYokoku_S(i->X, i->Y, D_B_STAR | DTP);
	}
	else if(i->FrmCnt % 300 == 0)
	{
		switch(i->Lv)
		{
		case 0:
			P2Vector(i->X, i->Y, GmDc.Player.X, GmDc.Player.Y, 2.0);
			S_EnemyEntryQueue()->AddElement(MakeTama(MakeDoll, i->X, i->Y, Vect_X, Vect_Y, D_B_STAR | DTP));
			break;

		case 1:
			for(int xc = -2; xc <= 2; xc++)
			{
				S_EnemyEntryQueue()->AddElement(MakeTama(MakeDoll, i->X, i->Y, 3.0 * xc, -5.0, D_B_STAR | DTP, 0.0, 0.2));
			}
			break;

		case 2:
			for(int rc = 0; rc <= 1; rc++)
			for(int rd = -1; rd <= 1; rd += 2)
			{
				S_EnemyEntryQueue()->AddElement(MakeTama(
					MakeDoll,
					i->X,
					i->Y,
					0.0, 0.0,
					D_B_STAR | DTP,
					0.0, 0.0,
					rc * PI, rd * 0.1, 0.995,
					0.0, 2.0, 0.0,
					300.0
					));
			}
			break;

		case 3:
			error(); // un-access

		default:
			error();
		}
		double speed;

		switch(i->Lv)
		{
		case 0: speed = 0.5; break;
		case 1: speed = 0.75; break;
		case 2: speed = 0.8; break;
		case 3: error(); // un-access

		default:
			error();
		}
		double xa = speed * (GmDc.Player.X < i->X ? -1.0 : 1.0);

		GmEnemy_t *near_e = GmFindEnemy_Circle(i->X, i->Y, 30.0, MakeDoll, i);

		if((uint)i < (uint)near_e) // どっちでも良いが、どちらか一方だけにしたい。(near_e == NULL なら常に偽)
		{
			xa *= -1.5;
		}
		ed->XAdd = xa;
	}
	i->X += ed->XAdd;
	return 1;
}
static void DollDrawer(GmEnemy_t *i, Doll_t *ed)
{
	int leftmode = GmDc.Player.X < i->X ? 1 : 0;
	int frmidx = i->FrmCnt;

	frmidx /= 4;
	frmidx %= 4;

	GmBeforeDrawEnemy(i);
	GmDrawCharacter(i->X, i->Y, Pic(P_DOLL_00 + frmidx), leftmode);
	GmAfterDrawEnemy(i);

	GmAddCircleScope(i->X, i->Y, 20.0, SCOPECOLOR_ENEMY);
}
static int DollIsCrashed(GmEnemy_t *i, Doll_t *ed, double x, double y, double w, double h)
{
	return IsCrashed_BoxCircle(x, y, w, h, i->X, i->Y, 20);
}
static void ReleaseDoll(Doll_t *i)
{
	memFree(i);
}
GmEnemy_t MakeDoll(double x, double y, int level)
{
	int hp;

	S_DollLv3RndSet()->Reset(); // HACK: ここで!?

	switch(level)
	{
	case 0: hp =  2; break;
	case 1: hp =  3; break;
	case 2: hp =  6; break;
	case 3: hp = 32; break;

	default:
		error();
	}
	return MakeEnemy(MakeDoll, x, y, level, hp, DollActor, DollDrawer, DollIsCrashed, ReleaseDoll);
}

// ---- Alice ----

#define ALICE_JIMEN_Y 502.0

static double MakeAliceSpeed(GmEnemy_t *i)
{
	switch(i->Lv)
	{
	case 0: return 4.0 + drnd() * 2.0;
	case 1: return 6.0 + drnd() * 1.0;
	case 2: return 7.0 + drnd() * 2.0;
	case 3: return 9.0 + drnd() * 1.0;
	}
	error();
}
typedef struct Alice_st
{
	double XAdd;
	int JumpDZCount; // 1-: frmcnt 0-
	double JumpXAdd;
	double JumpYAdd;
	double JumpXAdd2Rate;
	double JumpYAdd2;
}
Alice_t;

static int AliceActor(GmEnemy_t *i, Alice_t *ed)
{
	if(!i->FrmCnt) // init
	{
		i->FrmCnt = rnd(1000);
		ed->XAdd = MakeAliceSpeed(i) * (GmDc.Player.X < i->X ? -1.0 : 1.0);
	}
	if(ed->JumpDZCount)
	{
		ed->JumpDZCount++;
		goto endAct;
	}
	const double MARGIN_X = 30.0;

	if(i->Y < ALICE_JIMEN_Y)
	{
		double Y_ADD = 3.0;

		if(400.0 < i->Y && i->Y < 400.0 + Y_ADD * 1.1) // HACK: この高度で決めるの!?
		{
			ed->XAdd = abs(ed->XAdd) * (GmDc.Player.X < i->X ? -1.0 : 1.0);

			if(1 <= i->Lv)
			{
				if(GmDc.CameraX + 70.0 < i->X && i->X < GmDc.CameraX + 730.0) // ? カメラ内に居る。
				{
					ed->XAdd *= -1.0;
				}
			}
		}
		i->Y += Y_ADD;
	}
	else
	{
		if(1 <= i->Lv) // AliceJump 開始判定
		{
			double cx = i->X - GmDc.CameraX;

			const double cxa1 =  30.0;
			const double cxa2 =  70.0;
			const double cxb1 = 730.0;
			const double cxb2 = 770.0;

			if(
				(
					(cxa1 < cx && cx < cxa2) ||
					(cxb1 < cx && cx < cxb2)
				)
				&&
				drnd() < 0.4
				)
			{
				ed->JumpDZCount = 1;
				goto endAct;
			}
		}
		i->X += ed->XAdd;

		if(i->X < GmDc.CameraX         - MARGIN_X) ed->XAdd =  MakeAliceSpeed(i);
		if(i->X > GmDc.CameraX + 800.0 + MARGIN_X) ed->XAdd = -MakeAliceSpeed(i);

		if(3 <= i->Lv) // 画面からロストさせない。
		{
			m_range(i->X, GmDc.CameraX - MARGIN_X * 1.1, GmDc.CameraX + 800.0 + MARGIN_X * 1.1);
		}
	}
	m_range(i->Y, (double)-TSLD, ALICE_JIMEN_Y);

endAct:
	return 1;
}
static int AliceJump(GmEnemy_t *i, Alice_t *ed, int frmcnt) // frmcnt: 0-
{
	if(!frmcnt) // init
	{
		// Lv 1
		ed->JumpXAdd = drnd() * 2.0 * (GmDc.Player.X < i->X ? -1.0 : 1.0);
		ed->JumpYAdd = -8.0 - drnd() * 2.0;
		ed->JumpXAdd2Rate = 0.01 * drnd() * 0.03;
		ed->JumpYAdd2 = 0.1 + drnd() * 0.05;

		switch(i->Lv)
		{
		case 0:
			error(); // Lv 0 では飛ばない。

		case 1:
			break; // Lv 1 の定義は上

		case 2:
			ed->JumpXAdd2Rate *= 1.5;
			break;

		case 3:
			ed->JumpXAdd2Rate *= 2.0;
			ed->JumpYAdd = -10.0 - drnd() * 0.01;
			ed->JumpYAdd2 = 0.09 + drnd() * 0.01;
			break;

		default:
			error();
		}
	}
	else // 1-
	{
		i->X += ed->JumpXAdd;
		i->Y += ed->JumpYAdd;
		ed->JumpXAdd += (GmDc.Player.X - i->X) * ed->JumpXAdd2Rate;
		ed->JumpYAdd += ed->JumpYAdd2;

		if(3 <= i->Lv)
		{
			if(m_abssub(i->X, GmDc.Player.X) < 50.0) // ? 小町に接近 -> 減速してアプローチ
			{
				ed->JumpXAdd *= 0.9;
			}
		}
		if(ALICE_JIMEN_Y < i->Y) // ? 着地
		{
			i->Y = ALICE_JIMEN_Y;
			return 0;
		}
	}
	int frmidx = frmcnt / 4;

	if(8 < frmidx)
	{
		frmidx -= 8;
		frmidx = -(frmidx % 2);
		frmidx += 8;
	}
	GmBeforeDrawEnemy(i);
	GmDrawCharacter(i->X, i->Y + 15.0, Pic(P_ALICE_JUMP_00 + frmidx), GmDc.Player.X < i->X ? 1 : 0);
	GmAfterDrawEnemy(i);

	return 1;
}
static void AliceDrawer(GmEnemy_t *i, Alice_t *ed)
{
	if(ed->JumpDZCount)
	{
		if(AliceJump(i, ed, ed->JumpDZCount - 1)) // ? 継続
		{
			goto endDraw;
		}
		ed->XAdd = MakeAliceSpeed(i) * (GmDc.Player.X < i->X ? -1.0 : 1.0);
		ed->JumpDZCount = 0;
	}

	if(i->Y < ALICE_JIMEN_Y)
	{
		int frmidx = i->FrmCnt;

		frmidx /= 4;
		frmidx %= 6;

		GmBeforeDrawEnemy(i);
		GmDrawCharacter(i->X, i->Y, Pic(P_ALICE_FALL_00 + frmidx), 0);
		GmAfterDrawEnemy(i);
	}
	else
	{
		int leftmode = ed->XAdd < 0.0;
		int frmidx = i->FrmCnt % 10;

		GmBeforeDrawEnemy(i);
		GmDrawCharacter(i->X, i->Y, Pic(P_ALICE_00 + frmidx), leftmode);
		GmAfterDrawEnemy(i);
	}

endDraw:
	GmAddBoxScope((int)i->X - 20, (int)i->Y - 40, 40, 80, SCOPECOLOR_ENEMY);
}
static int AliceIsCrashed(GmEnemy_t *i, Alice_t *ed, double x, double y, double w, double h)
{
	return IsCrashed_Box(x, y, w, h, i->X - 20.0, i->Y - 40.0, 40.0, 80.0);
}
static void ReleaseAlice(Alice_t *i)
{
	memFree(i);
}
GmEnemy_t MakeAlice(double x, double y, int level)
{
	int hp;

	switch(level)
	{
	case 0: hp =  1; break;
	case 1: hp =  2; break;
	case 2: hp =  3; break;
	case 3: hp = 14; break;

	default:
		error();
	}
	return MakeEnemy(MakeAlice, x, y, level, hp, AliceActor, AliceDrawer, AliceIsCrashed, ReleaseAlice);
}

// ---- Cube ----

typedef struct Cube_st
{
	int PicResno;
	double Scale;
	double X;
	double Y;
	double RX;
	double RY;
	double RXAdd;
	double RYAdd;
	double RXScale;
	double RYScale;
}
Cube_t;

static singleton(CubeX3RndSet, endlessQueue<double>, makeDRndSet(3600, 20121110));

#define CUBE_X_HP 50

static int CubeActor(GmEnemy_t *i, Cube_t *ed)
{
	if(!i->FrmCnt) // init
	{
		i->FrmCnt = rnd(1000);
	}
	i->X = ed->X + sin(ed->RX) * ed->RXScale;
	i->Y = ed->Y + sin(ed->RY) * ed->RYScale;

	ed->RX += ed->RXAdd;
	ed->RY += ed->RYAdd;

	int frmcnt = i->FrmCnt;

	switch(ed->PicResno)
	{
	case P_G_CUBE:
		{
			if(frmcnt % 30 == 0)
			{
				frmcnt /= 30;
				frmcnt %= 10;

				if(frmcnt < 6)
				{
					if(frmcnt % 2 == 0) // 射撃予告
					{
						GmCommonShootYokoku_L(i->X, i->Y, D_G_STAR | DTP);
					}
					else if(GmInCamera(i)) // 射撃
					{
						const double r = P2Angle(i->X, i->Y, GmDc.Player.X, GmDc.Player.Y);
						const int TAMA_MAX = 12;
						const double TAMA_SPEED = 3.0;

						for(int tamacnt = 0; tamacnt < TAMA_MAX; tamacnt++)
						{
							double x = cos(r + PI * 2.0 * ((double)tamacnt / TAMA_MAX)) * TAMA_SPEED;
							double y = sin(r + PI * 2.0 * ((double)tamacnt / TAMA_MAX)) * TAMA_SPEED;
							
							S_EnemyEntryQueue()->AddElement(MakeTama(MakeCube, i->X, i->Y, x, y, D_G_STAR | DTP));
						}
					}
				}
			}
		}
		break;

	case P_B_CUBE:
		{
			if(frmcnt % 5 == 0)
			{
				frmcnt /= 5;
				frmcnt %= 30;

				if(frmcnt < 12 && GmInCamera(i))
				{
					GmCommonShootEffect_L(i->X, i->Y, D_C_STAR | DTP);

					const double r = P2Angle(i->X, i->Y, GmDc.Player.X, GmDc.Player.Y);

					double x = cos(r) * 3.0;
					double y = sin(r) * 3.0;

					S_EnemyEntryQueue()->AddElement(MakeTama(MakeCube, i->X, i->Y, x, y, D_C_STAR | DTP));
				}
			}
		}
		break;

	case P_W_CUBE:
		{
			if(frmcnt % 300 == 270) // 射撃予告
			{
				GmCommonShootYokoku_L(i->X, i->Y, D_W_STAR | DTP);
			}
			else if(frmcnt % 300 == 0 && GmInCamera(i)) // 射撃
			{
				const double r = P2Angle(i->X, i->Y, GmDc.Player.X, GmDc.Player.Y);
				const double ar = r + PI + ernd() * (PI / 8.0);
				const int TAMA_MAX = 9;
				const double TAMA_SPEED = 4.5 + drnd() * 0.5;
				const double TAMA_ADD = 0.1;

				for(int tamacnt = 0; tamacnt < TAMA_MAX; tamacnt++)
				{
					double x = cos(r + PI * 2.0 * ((double)tamacnt / TAMA_MAX)) * TAMA_SPEED;
					double y = sin(r + PI * 2.0 * ((double)tamacnt / TAMA_MAX)) * TAMA_SPEED;
					double xa = cos(ar + PI * 2.0 * ((double)tamacnt / TAMA_MAX)) * TAMA_ADD;
					double ya = sin(ar + PI * 2.0 * ((double)tamacnt / TAMA_MAX)) * TAMA_ADD;
					
					S_EnemyEntryQueue()->AddElement(MakeTama(MakeCube, i->X, i->Y, x, y, D_W_STAR | DTP, xa, ya));
				}
			}
		}
		break;

	case P_X_CUBE:
		{
			if(GmDc.CameraX < i->X && i->X < GmDc.CameraX + 800.0)
			{
#if 1
				double rainRate = (double)i->HP / CUBE_X_HP;
				rainRate = 1.0 - rainRate;
				m_range(rainRate, 0.0, 1.0);

				if(S_CubeX3RndSet()->Dequeue() < rainRate * 0.3)
				{
					double rx = GmDc.CameraX + S_CubeX3RndSet()->Dequeue() * 800.0;
					double ry = 0.0;
					int rc = rrnd(MIN_D_X_STAR, MAX_D_X_STAR) | DTP;
					GmCommonShootEffect_L(
						rx,
						ry,
						rc
						);
					S_EnemyEntryQueue()->AddElement(MakeTama(
						MakeCube,
						rx,
						ry,
						(GmDc.CameraX + 400.0 - i->X) / 200.0,
						5.0,
						rc
						));
				}
				if(i->DamageUketa)
				{
					GTAddCommonEffect(
						1,
						rrnd(MIN_D_X_STAR, MAX_D_X_STAR) | DTP,
						0, 0,
						i->X, i->Y, 3.0, ernd(), 0.5,
						0.0, -3.0, 0.0, 0.1 * ernd(), -0.005
						);
					i->DamageUketa = 0;
				}
#else // OLD
				if(GmDc.BombDZCount == 20)
				{
					GTAddCommonEffect(1, P_BOMB, 0, 0,
						i->X, i->Y, 0.1, 0.0, 1.0,
						0.0, 0.0, 0.1, 0.05, -0.03
						);
				}
//				if(GmDc.BombDZCount == 40) // old
				if(GmDc.BombDZCount == 40 || GmDc.Player.HitBackFrmCnt == 10)
				{
					i->HP = CUBE_X_HP;

					SEPlay(SE_KAIFUKU);
					GmMapItemGetEffect(i->X, i->Y);
				}
#endif
			}
		}
		break;

	default:
		1; // fall through
	}
	return 1;
}
static void CubeDrawer(GmEnemy_t *i, Cube_t *ed)
{
	int leftmode = i->X < GmDc.Player.X;
	MosaicMode = 1;

	if(ed->PicResno == P_X_CUBE)
	{
		const int COLOR_CYCLE = 60;
		int frmidx = i->FrmCnt / COLOR_CYCLE;
		int subidx = i->FrmCnt % COLOR_CYCLE;

		int pic_resno1 = P_P_CUBE + (frmidx + 0) % 6;
		int pic_resno2 = P_P_CUBE + (frmidx + 1) % 6;
		double colRate = (double)subidx / COLOR_CYCLE;

		colRate = 0.5 - cos(PI * colRate) * 0.5;

		GmBeforeDrawEnemy(i);
		GmDrawCharacter(i->X, i->Y, Pic(pic_resno1), leftmode, ed->Scale);
		if(!i->DamageEffectDZCount) {
			SetAlpha(colRate);
			GmDrawCharacter(i->X, i->Y, Pic(pic_resno2), leftmode, ed->Scale);
			ResetBlend();
		}
		GmAfterDrawEnemy(i);
	}
	else
	{
		GmBeforeDrawEnemy(i);
		GmDrawCharacter(i->X, i->Y, Pic(ed->PicResno), leftmode, ed->Scale);
		GmAfterDrawEnemy(i);
	}
	MosaicMode = 0;
	const int XY_SIZE = (int)(45 * ed->Scale);

	GmAddBoxScope(
		(int)i->X - XY_SIZE / 2,
		(int)i->Y - XY_SIZE / 2,
		XY_SIZE,
		XY_SIZE,
		SCOPECOLOR_ENEMY
		);
}
static int CubeIsCrashed(GmEnemy_t *i, Cube_t *ed, double x, double y, double w, double h)
{
	const int XY_SIZE = (int)(45 * ed->Scale);

	return IsCrashed_Box(x, y, w, h,
		i->X - XY_SIZE / 2,
		i->Y - XY_SIZE / 2,
		XY_SIZE,
		XY_SIZE
		);
}
static void ReleaseCube(Cube_t *i)
{
	memFree(i);
}
GmEnemy_t MakeCube(double x, double y, double rx, double ry, double rx_add, double ry_add, double rx_scale, double ry_scale, int pic_resno, double scale, int hp) // pic_resno: P_*_CUBE を想定
{
	Cube_t *ed = nb_(Cube_t);

	S_CubeX3RndSet()->Reset(); // ここで!? パート2

	ed->PicResno = pic_resno;
	ed->Scale = scale;
	ed->X = x;
	ed->Y = y;
	ed->RX = rx;
	ed->RY = ry;
	ed->RXAdd = rx_add;
	ed->RYAdd = ry_add;
	ed->RXScale = rx_scale;
	ed->RYScale = ry_scale;

	return MakeEnemy(MakeCube, x, 100.0, 0, hp, CubeActor, CubeDrawer, CubeIsCrashed, ReleaseCube, ed); // no level
}
GmEnemy_t MakeCube_P(double x, double y)
{
	return MakeCube(x, y,
		drnd() * PI * 2.0,
		drnd() * PI * 2.0,
		0.01 + drnd() * 0.01,
		0.01 + drnd() * 0.01,
		50.0,
		50.0,
		P_P_CUBE,
		2.0,
		10
		);
}
GmEnemy_t MakeCube_R(double x, double y)
{
	double r = drnd() * PI * 2.0;
	double a = (0.03 + drnd() * 0.01) * (rnd(2) ? 1 : -1);

	return MakeCube(x, y, r, r + PI / 2.0, a, a, 150.0, 150.0, P_R_CUBE, 1.0, 3);
}
GmEnemy_t MakeCube_G(double x, double y)
{
	return MakeCube(x, y,
		drnd() * PI * 2.0,
		drnd() * PI * 2.0,
		0.01 + drnd() * 0.015,
		0.08 + drnd() * 0.02,
		350.0,
		20.0,
		P_G_CUBE,
		1.5,
		8
		);
}
GmEnemy_t MakeCube_B(double x, double y)
{
	return MakeCube(x, y,
		drnd() * PI * 2.0,
		drnd() * PI * 2.0,
		0.08 + drnd() * 0.02,
		0.01 + drnd() * 0.015,
		35.0,
		200.0,
		P_B_CUBE,
		1.5,
		8
		);
}
GmEnemy_t MakeCube_Y(double x, double y)
{
	return MakeCube(x, y,
		drnd() * PI * 2.0,
		drnd() * PI * 2.0,
		0.04 + drnd() * 0.02,
		0.04 + drnd() * 0.02,
		200.0,
		200.0,
		P_Y_CUBE,
		2.5,
		8
		);
}
GmEnemy_t MakeCube_W(double x, double y)
{
	return MakeCube(x, y,
		drnd() * PI * 2.0,
		drnd() * PI * 2.0,
		0.07 + drnd() * 0.03,
		0.07 + drnd() * 0.03,
		10.0,
		10.0,
		P_W_CUBE,
		2.0,
		20
		);
}
GmEnemy_t MakeCube_X(double x, double y)
{
	return MakeCube(x, y,
		drnd() * PI * 2.0,
		drnd() * PI * 2.0,
		0.02 + drnd() * 0.01,
		0.02 + drnd() * 0.01,
		10.0,
		10.0,
		P_X_CUBE,
		2.0,
		CUBE_X_HP
		);
}

// ---- Suica ----

typedef struct Suica_st
{
	int LeftMode;
	int Speed; // 1 <=
	int FrmIdx;
	int AttackDZCount;
	int AttackMax;
	int AttackSleep;
}
Suica_t;

static int SuicaActor(GmEnemy_t *i, Suica_t *ed)
{
	if(!i->FrmCnt) // init
	{
		i->FrmCnt = rnd(1000);
		ed->LeftMode = rrnd(0, 1);
		ed->Speed = rrnd(3, 6);
		ed->FrmIdx = 0;

		if(i->Lv == 3)
		{
			ed->LeftMode = 1;
		}
		switch(i->ExtraPart.Suica.ForceDir)
		{
		case 1: ed->LeftMode = 1; break;
		case 2: ed->LeftMode = 0; break;

		default:
			break;
		}
	}

reAttack:
	if(ed->AttackDZCount)
	{
		int frmcnt = ed->AttackDZCount - 1; // 0-

		double sx = i->X + 17.0 * (ed->LeftMode ? -1.0 : 1.0);
		double sy = i->Y - 33.0;

		switch(i->Lv)
		{
		case 0:
			error(); // Lv 0 は攻撃しない。

		case 1:
			if(30 <= frmcnt && frmcnt % 5 == 0)
			{
				S_EnemyEntryQueue()->AddElement(MakeTama(MakeSuica, sx, sy, 0.0, -6.0, D_P_STAR | DTP));
				GmCommonShootEffect_S(sx, sy, D_P_STAR | DTP);
			}
			break;

		case 2:
			if(frmcnt == 10)
			{
				const int R_MAX = 12;
				const double x_add = (GmDc.Player.X - sx) * 0.005;
				const double y_add = -6.0 - drnd() * 0.6;

				for(int r = 0; r < R_MAX; r++)
				{
					S_EnemyEntryQueue()->AddElement(MakeTama(
						MakeSuica,
						sx,
						sy,
						x_add, y_add,
						D_P_STAR | DTP,
						0.0, 0.07,
						2.0 * PI * ((double)r / R_MAX), 0.3 * (ed->LeftMode ? 1.0 : -1.0), 0.995,
						0.0, 0.9, -0.001
						));
				}
				const int C_MAX = 8;
				const double exy_r = drnd() * 2.0 * PI;
				const double exy_speed = 4.0;

				for(int c = 0; c < C_MAX; c++) // 射撃効果
				{
					double ex = sin(exy_r + 2.0 * PI * ((double)c / C_MAX)) * exy_speed;
					double ey = cos(exy_r + 2.0 * PI * ((double)c / C_MAX)) * exy_speed;

					GTAddCommonEffect(
						1,
						D_P_STAR | DTP,
						0, 0,
						sx, sy, 1.5, 0.0, 0.5,
						ex, ey, 0.0, ernd() * 0.2, -0.02
						);
				}
			}
			break;

		case 3:
			if(frmcnt % 4 == 0)
			{
				S_EnemyEntryQueue()->AddElement(MakeTama(MakeSuica, sx - 10.0, sy, 0.0, -8.0, D_P_STAR | DTP));
				S_EnemyEntryQueue()->AddElement(MakeTama(MakeSuica, sx + 10.0, sy, 0.0, -8.0, D_P_STAR | DTP));
				GmCommonShootEffect_S(sx, sy, D_P_STAR | DTP);
			}
			break;

		default:
			error();
		}
		if(ed->AttackDZCount < ed->AttackMax)
		{
			ed->AttackDZCount++;
			goto endAct;
		}
		ed->AttackDZCount = 0;
	}
	else if(!m_countDown(ed->AttackSleep))
	{
		/*
			攻撃を開始する場合:
				ed->AttackDZCount = 1;
				ed->AttackMax     = 任意(1-);
				ed->AttackSleep   = 任意(0-);
				goto reAttack;
		*/
		switch(i->Lv)
		{
		case 0:
			break;

		case 1:
			if(m_abssub(GmDc.Player.X, i->X) < 200.0)
			{
				ed->AttackDZCount = 1;
				ed->AttackMax = 90;
				ed->AttackSleep = 90;
				goto reAttack;
			}
			break;

		case 2:
			if(m_abssub(GmDc.Player.X, i->X) < 360.0)
			{
				ed->AttackDZCount = 1;
				ed->AttackMax = 60;
				ed->AttackSleep = 240;
				goto reAttack;
			}
			break;

		case 3:
			if(((int)i->X / 10 - LFrmCnt) % 90 == 0)
			{
				ed->AttackDZCount = 1;
				ed->AttackMax = 60;
				ed->AttackSleep = 0;
				goto reAttack;
			}
			break;

		default:
			error();
		}
	}

	ed->FrmIdx = i->FrmCnt;
	errorCase(ed->Speed < 1 || TSLD < ed->Speed);
	ed->FrmIdx /= ed->Speed;
	ed->FrmIdx %= 18;

endAct:
	return 1;
}
static void SuicaDrawer(GmEnemy_t *i, Suica_t *ed)
{
	if(ed->AttackDZCount) // 攻撃中
	{
		GmBeforeDrawEnemy(i);
		GmDrawCharacter(
			i->X + 7.0 * (ed->LeftMode ? -1.0 : 1.0),
			i->Y + 7.0,
			Pic(P_SUICA_ATTACK_00 + LFrmCnt / 4 % 2), ed->LeftMode);
		GmAfterDrawEnemy(i);
	}
	else // 待機中
	{
		GmBeforeDrawEnemy(i);
		GmDrawCharacter(i->X, i->Y, Pic(P_SUICA_00 + ed->FrmIdx), ed->LeftMode);
		GmAfterDrawEnemy(i);
	}

	GmAddBoxScope(
		(int)i->X - 30,
		(int)i->Y - 30,
		60,
		60,
		SCOPECOLOR_TAMA
		);
}
static int SuicaIsCrashed(GmEnemy_t *i, Suica_t *ed, double x, double y, double w, double h)
{
	return IsCrashed_Box(x, y, w, h,
		i->X - 30.0,
		i->Y - 30.0,
		60.0,
		60.0
		);
}
static void ReleaseSuica(Suica_t *i)
{
	memFree(i);
}
GmEnemy_t MakeSuica(double x, int level)
{
	double y = 500.0; // Suica's Grand
	int hp;

	switch(level)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		hp = 0;
		break;

	default:
		error();
	}
	return MakeEnemy(MakeSuica, x, y, level, hp, SuicaActor, SuicaDrawer, SuicaIsCrashed, ReleaseSuica);
}

// ---- Bubble ----

typedef struct BubbleBack_st
{
	double X;
	double XAdd;
	double Y;
	double YAdd;
	double A;
	double AAdd;
	double R;
	double RAdd;
	double Z;
	double ZAdd;
}
BubbleBack_t;

static void InitBubbleBack(BubbleBack_t *i)
{
	double xy_r = drnd() * 2.0 * PI;

	i->X = 0.0;
	i->XAdd = sin(xy_r) * 1.5;
	i->Y = 0.0;
	i->YAdd = cos(xy_r) * 1.5;
	i->A = 1.0;
	i->AAdd = -0.01 - drnd() * 0.02;
	i->R = drnd() * 2.0 * PI;
	i->RAdd = ernd() * 0.1;
	i->Z = 1.0;
	i->ZAdd = 0.01 + drnd() * 0.02;
}
#define BUBBLEBACK_MAX 4

typedef struct Bubble_st
{
	BubbleBack_t BubbleBackList[BUBBLEBACK_MAX];
	double Scope_R;

	struct
	{
		int LastHP;
		double Speed;
		double TZoom; // Target Zoom
		double Zoom;
	}
	Lv3;
}
Bubble_t;

static int BubbleActor(GmEnemy_t *i, Bubble_t *ed)
{
	if(!i->FrmCnt) // init
	{
		i->FrmCnt = rnd(1000);

		for(int index = 0; index < BUBBLEBACK_MAX; index++)
		{
			InitBubbleBack(ed->BubbleBackList + index);
		}
		ed->Lv3.LastHP = i->HP;
		ed->Lv3.Speed = 4.0;
		ed->Lv3.TZoom = 1.0;
		ed->Lv3.Zoom = 0.1;
	}
	for(int index = 0; index < BUBBLEBACK_MAX; index++)
	{
		BubbleBack_t *bb = ed->BubbleBackList + index;

		bb->X += bb->XAdd;
		bb->Y += bb->YAdd;
		bb->A += bb->AAdd;
		bb->R += bb->RAdd;
		bb->Z += bb->ZAdd;

		if(bb->A <= 0.0)
		{
			InitBubbleBack(bb);
		}
	}
	double rate;

	switch(i->Lv)
	{
	case 0:
		rate = (sin(i->FrmCnt * ((2.0 * PI) / 60.0)) + 1.0) * 0.6;
		break;

	case 1:
		rate = (sin(i->FrmCnt * ((2.0 * PI) / 60.0)) + 0.7) * 1.8;
		break;

	case 2:
		if(!GmInCamera(i, 50.0))
		{
			rate = 0.0;
			break;
		}
		rate = (sin(i->FrmCnt * ((2.0 * PI) / 150.0)) + 0.3) * 1.2;

		// 攻撃
		{
			const int CYCLE = 120; // ! 31 <

			int frmidx = i->FrmCnt % (CYCLE * 2);
			int phase = frmidx < CYCLE;

			frmidx %= CYCLE;

			if(frmidx == CYCLE - 31)
			{
				GmCommonShootYokoku_L(i->X, i->Y, D_D_STAR | DTP);
			}
			else if(frmidx == CYCLE - 1)
			{
				const double r = 0.0 + phase * (PI / 6.0);
				const int TAMA_MAX = 6;
				const double TAMA_SPEED = 8.0;

				for(int tamacnt = 0; tamacnt < TAMA_MAX; tamacnt++)
				{
					double x = cos(r + PI * 2.0 * ((double)tamacnt / TAMA_MAX)) * TAMA_SPEED;
					double y = sin(r + PI * 2.0 * ((double)tamacnt / TAMA_MAX)) * TAMA_SPEED;

					S_EnemyEntryQueue()->AddElement(MakeTama(MakeBubble, i->X, i->Y, x, y, D_D_STAR | DTP));
				}
			}
		}
		break;

	case 3:
		while(i->HP < ed->Lv3.LastHP)
		{
			ed->Lv3.LastHP--;
			ed->Lv3.Speed += 0.06;
			ed->Lv3.TZoom += 0.6;
		}
		rate = ed->Lv3.Speed;
		m_countApprox(ed->Lv3.Zoom, ed->Lv3.TZoom, 0.97, 0.0);

		if(!GmInCamera(i))
			m_range(i->X, GmDc.CameraX, GmDc.CameraX + 800.0);

		break;

	default:
		error();
	}
	P2Vector(i->X, i->Y, GmDc.Player.X, GmDc.Player.Y, 1.0);
	i->X += Vect_X * rate;
	i->Y += Vect_Y * rate;

	GmEnemy_t *near_e = GmFindEnemy_Circle(i->X, i->Y, 250.0, MakeBubble, i);

	if((uint)i < (uint)near_e) // ! near_e == NULL -> false
	{
		P2Vector(near_e->X, near_e->Y, i->X, i->Y, 1.0);
		i->X += Vect_X;
		i->Y += Vect_Y;
	}
	return 1;
}
static void BubbleDrawer(GmEnemy_t *i, Bubble_t *ed)
{
	int frmidx = i->FrmCnt;
	double zoom;

	switch(i->Lv)
	{
	case 0:
	case 1:
	case 2:
		zoom = 1.0 + (sin(frmidx * 0.03) + 1.0) * 3.0;
		break;

	case 3:
		zoom = ed->Lv3.Zoom;
		break;

	default:
		error();
	}
	frmidx /= 3;
	frmidx %= 15;

	for(int index = 0; index < BUBBLEBACK_MAX; index++)
	{
		if(!m_countDown(GmDc.BubbleBackDrawRemain))
			break;

		BubbleBack_t *bb = ed->BubbleBackList + index;

		GmBeforeDrawEnemy(i);
		SetAlpha(bb->A);
		DrawBegin(
			i->X - GmDc.CameraX + bb->X,
			i->Y                + bb->Y,
			Pic(P_BUBBLE_BACK)
			);
		DrawRotate(bb->R);
		DrawZoom(bb->Z * m_min(6.0, zoom));
		DrawEnd();
		ResetBlend();
		GmAfterDrawEnemy(i);
	}

	{
		const double DR_ZURE_X = -1.0;
		const double DR_ZURE_Y = -1.0;

		GmBeforeDrawEnemy(i);
		DrawBegin(i->X - GmDc.CameraX + DR_ZURE_X, i->Y + DR_ZURE_Y, Pic(P_BUBBLE_00 + frmidx));
		DrawZoom(zoom * 0.7);
		DrawRotate(frmidx / 50.0);
		DrawEnd();
		GmAfterDrawEnemy(i);
	}

	ed->Scope_R = (int)(zoom * 10.0);

	GmAddCircleScope(i->X, i->Y, ed->Scope_R, SCOPECOLOR_ENEMY);
}
static int BubbleIsCrashed(GmEnemy_t *i, Bubble_t *ed, double x, double y, double w, double h)
{
	return IsCrashed_BoxCircle(x, y, w, h, i->X, i->Y, ed->Scope_R);
}
static void ReleaseBubble(Bubble_t *i)
{
	memFree(i);
}
GmEnemy_t MakeBubble(double x, double y, int level)
{
	int hp;

	switch(level)
	{
	case 0: hp =  5; break;
	case 1: hp =  7; break;
	case 2: hp =  9; break;
	case 3: hp = 24; break;

	default:
		error();
	}
	return MakeEnemy(MakeBubble, x, y, level, hp, BubbleActor, BubbleDrawer, BubbleIsCrashed, ReleaseBubble);
}

// ---- Marisa ----

static double MakeMarisaSpeed(GmEnemy_t *i)
{
	switch(i->Lv)
	{
	case 0:
	case 1: return 2.0 + drnd() * 1.0;
	case 2: return 3.0 + drnd() * 0.5;
	case 3: return 6.0 + drnd() * 2.0;
	}
	error();
}
typedef struct Marisa_st
{
	int AttackDZCount; // 1-: frmcnt 0-
	int AttackLeftMode;
	int AttackSleep;
	double XAdd;
	
	struct
	{
		double R;
		double RAdd;
	}
	Lv2;
}
Marisa_t;

static int MarisaAttack(GmEnemy_t *i, Marisa_t *ed, int frmcnt) // frmcnt: 0-
{
	int leftmode = ed->AttackLeftMode;
	double x = i->X;
	double y = i->Y;
	int justAttackFrmCnt = 60;

	if(i->Lv == 3)
	{
		justAttackFrmCnt = 300;
	}

#define XYHOSEI(x, y) \
	x -= 0.0; \
	y -= 10.0

	ifsceneBegin(frmcnt, 8 * 6 - 1)
	{
		fs_count /= 6;

		if(2 <= fs_count)
		{
			XYHOSEI(x, y);
		}
		GmBeforeDrawEnemy(i);
		GmDrawCharacter(x, y, Pic(P_MARISA_ATTACK_00 + fs_count), leftmode);
		GmAfterDrawEnemy(i);
	}
	ifscene(justAttackFrmCnt)
	{
		XYHOSEI(x, y);

		double tx = i->X - (leftmode ? 1.0 : -1.0) * 35.0;
		double ty = i->Y - 5.0;
		double cam_tx = tx - GmDc.CameraX;

		if(i->Lv == 3)
		{
			switch(fs_count)
			{
			case 0: SEPlay(SE_MASTERSPARK_00); break;
			case 60: SEPlay(SE_MASTERSPARK_01); break;
			case 120: SEPlay(SE_MASTERSPARK_02); break;
			case 180: SEPlay(SE_MASTERSPARK_03); break;

			default:
				break;
			}
			double xRate = leftmode ? -1.0 : 1.0;
			double yRate = 1.0 + m_min(1.0, fs_rate * 3.0);

			const int MSTSPK_FRMCYC = 2;
			const int MSTSPK_FRMMAX = 10;

			if(leftmode ? cam_tx < 1000.0 : -200.0 < cam_tx)
			{
				DrawBegin(
					cam_tx + xRate * 500.0,
					ty,
					Pic(P_MARISA_MASTERSPARK_00 + (i->FrmCnt / MSTSPK_FRMCYC % MSTSPK_FRMMAX))
					);
				DrawXZoom(1000.0 * xRate / 256.0);
				DrawYZoom(yRate);
				DrawEnd();
			}
			else
			{
				DrawBegin(
					400.0,
					ty,
					Pic((D_MASTERSPARK_HALF_00 + (i->FrmCnt / MSTSPK_FRMCYC % MSTSPK_FRMMAX)) | DTP)
					);
				DrawXZoom(800.0 * xRate / 128.0);
				DrawYZoom(yRate);
				DrawEnd();
			}

			/*
				発射と同時、特にレーザーを描画する前に当たって死ぬと何で死んだのか分からない。
				ので、発射から数フレームは当たらないようにする。
			*/
			if(5 < fs_count)
			{
				double msr = yRate * 70.0; // マスタースパークの太さ(半径)
				double msx = tx + msr * (leftmode ? -1 : 1);
				double msy = ty;

				S_EnemyEntryQueue()->AddElement(MakeDamageArea_Circle(msx, msy, msr, 2));

				const double dzw = 400.0;

				if(leftmode)
				{
					while(0.0 < msx)
					{
						S_EnemyEntryQueue()->AddElement(MakeDamageArea_Box(msx - dzw, msy - msr, dzw, msr * 2.0, 2));
						msx -= dzw;
					}
				}
				else
				{
					while(msx < GmDc.FieldWidth)
					{
						S_EnemyEntryQueue()->AddElement(MakeDamageArea_Box(msx, msy - msr, dzw, msr * 2.0, 2));
						msx += dzw;
					}
				}
			}
		}
		else if(fs_count % 5 == 0)
		{
			switch(i->Lv)
			{
			case 0:
				S_EnemyEntryQueue()->AddElement(MakeTama(
					MakeMarisa,
					tx,
					ty,
					(ed->AttackLeftMode ? -1.0 : 1.0) * 6.0, 0.0, D_Y_STAR | DTP
					));
				break;

			case 1:
				S_EnemyEntryQueue()->AddElement(MakeTama(
					MakeMarisa,
					tx,
					ty,
					(ed->AttackLeftMode ? -1.0 : 1.0) * 6.0,
					-7.0 - fs_rate * 12.9,
					D_Y_STAR | DTP, 0.0, 0.4
					));
				break;

			case 2:
				if(!fs_count) // init
				{
					ed->Lv2.R = 2.0 + PI * drnd();
					ed->Lv2.RAdd = (0.2 + drnd() * 0.01) * (drnd() < 0.5 ? 1.0 : -1.0);
				}
				S_EnemyEntryQueue()->AddElement(MakeTama(
					MakeMarisa,
					tx,
					ty,
					(ed->AttackLeftMode ? -1.0 : 1.0) * 1.0, -1.0,
					D_Y_STAR | DTP,
					0.0, 0.0,
					ed->Lv2.R, ed->Lv2.RAdd, 0.995,
					0.0, 1.0
					));
				break;

			case 3:
				error(); // un-access

			default:
				error();
			}
			GmCommonShootEffect_S(tx, ty, D_Y_STAR | DTP);
		}
		fs_count /= 4;
		fs_count %= 2;

		GmBeforeDrawEnemy(i);
		GmDrawCharacter(x, y, Pic(P_MARISA_ATTACK_08 + fs_count), leftmode);
		GmAfterDrawEnemy(i);

#if 0 // 廃止
		if(i->Lv == 3)
		{
			DrawBegin(
				cam_tx + 50.0 * (leftmode ? -1.0 : 1.0),
				ty,
				Pic(P_MARISA_MASTERSPARK_SUB_00 + (i->FrmCnt % 8))
				);
			DrawZoom(4.0);
			SetAlpha(0.9);
			DrawEnd();
			ResetBlend();
		}
#endif
	}
	ifscene(3 * 4 - 1)
	{
		fs_count /= 4;

		if(fs_count < 1)
		{
			XYHOSEI(x, y);
		}
		GmBeforeDrawEnemy(i);
		GmDrawCharacter(x, y, Pic(P_MARISA_ATTACK_10 + fs_count), leftmode);
		GmAfterDrawEnemy(i);
	}
	ifsceneEnd
	{
		return 0;
	}
	return 1;

#undef XYHOSEI
}
static int MarisaActor(GmEnemy_t *i, Marisa_t *ed)
{
	if(!i->FrmCnt) // init
	{
		i->FrmCnt = rnd(1000);
		ed->AttackDZCount = 0;
		ed->AttackLeftMode = -1; // dummy
		ed->AttackSleep = 0;
		ed->XAdd = MakeMarisaSpeed(i) * (GmDc.Player.X < i->X ? -1.0 : 1.0);
	}
	double ATTACKAREA_CENTER_X;
	double ATTACKAREA_MARGIN_X;

	switch(i->Lv)
	{
	case 0:
		ATTACKAREA_CENTER_X = 90.0;
		ATTACKAREA_MARGIN_X = 30.0;
		break;
	case 1:
	case 2:
		ATTACKAREA_CENTER_X = 140.0;
		ATTACKAREA_MARGIN_X = 80.0;
		break;
	case 3:
		ATTACKAREA_CENTER_X = 60.0;
		ATTACKAREA_MARGIN_X = 30.0;
		break;

	default:
		error();
	}
	const double ATTACKAREA_X1 = ATTACKAREA_CENTER_X;
	const double ATTACKAREA_X2 = 800.0 - ATTACKAREA_CENTER_X;

	double cx = i->X - GmDc.CameraX;

	if(ed->AttackDZCount)
	{
		ed->AttackDZCount++;
	}
	else if(!m_countDown(ed->AttackSleep))
	{
		if(abs(cx - ATTACKAREA_X1) < ATTACKAREA_MARGIN_X) // ? 左側発射位置
		{
			ed->AttackDZCount = 1;
			ed->AttackLeftMode = 0;
		}
		else if(abs(cx - ATTACKAREA_X2) < ATTACKAREA_MARGIN_X) // ? 右側発射位置
		{
			ed->AttackDZCount = 1;
			ed->AttackLeftMode = 1;
		}
	}

	if(ed->AttackDZCount)
	{
		// noop
	}
	else
	{
		if(i->FrmCnt % 30 == 0 && drnd() < 0.5)
		{
			if(ed->AttackSleep)
			{
				if(cx < 400.0)
					ed->XAdd = MakeMarisaSpeed(i);
				else
					ed->XAdd = -MakeMarisaSpeed(i);
			}
			else
			{
				if((ATTACKAREA_X1 < cx && cx < 400.0) || ATTACKAREA_X2 < cx)
					ed->XAdd = -MakeMarisaSpeed(i);
				else
					ed->XAdd = MakeMarisaSpeed(i);
			}
		}
		i->X += ed->XAdd;
	}
	m_range(i->X, 0.0, GmDc.FieldWidth);

	return 1;
}
static void MarisaDrawer(GmEnemy_t *i, Marisa_t *ed)
{
	if(ed->AttackDZCount)
	{
		if(MarisaAttack(i, ed, ed->AttackDZCount - 1)) // ? 継続
		{
			goto endDraw;
		}
		ed->AttackDZCount = 0;
		ed->AttackSleep = 150;
	}
	int leftmode = ed->XAdd < 0.0;
	int frmidx = i->FrmCnt;

	frmidx /= 4;
	frmidx %= 8;

	GmBeforeDrawEnemy(i);
	GmDrawCharacter(i->X, i->Y, Pic(P_MARISA_00 + frmidx), leftmode);
	GmAfterDrawEnemy(i);

endDraw:
	GmAddBoxScope((int)i->X - 20, (int)i->Y - 40, 40, 80, SCOPECOLOR_ENEMY);
}
static int MarisaIsCrashed(GmEnemy_t *i, Marisa_t *ed, double x, double y, double w, double h)
{
	return IsCrashed_Box(x, y, w, h, i->X - 20.0, i->Y - 40.0, 40.0, 80.0);
}
static void ReleaseMarisa(Marisa_t *i)
{
	memFree(i);
}
GmEnemy_t MakeMarisa(double x, int level)
{
	double y = 505.0; // Marisa's Grand
	int hp;

	switch(level)
	{
	case 0: hp =  2; break;
	case 1: hp =  8; break;
	case 2: hp = 10; break;
	case 3: hp = 40; break;

	default:
		error();
	}
	return MakeEnemy(MakeMarisa, x, y, level, hp, MarisaActor, MarisaDrawer, MarisaIsCrashed, ReleaseMarisa);
}

// ---- Aya ----

#define AYA_CYCLE 8

typedef struct Aya_st
{
	double X;
	double Y;
	double XR;
	double YR;
	double XRAdd;
	double YRAdd;
	int DashSleep;
	int DashDZCount; // 1-: frmcnt 0-
	double DashXAdd;
	double DashYAdd;
	double DashXAddRate;
	double DashYAddRate;
	int DashLeftMode;
	int RenzokuAttackRemain;
	int AttackSleep;
	int AttackDZCount; // 1-: frmcnt 0-
	double AttackLastCameraX;
	double AttackDestX;
	double AttackDestY;
	int AttackLeftMode; // ? 左方向へ攻撃、移動は逆方向(右)
	int ApproxMode;
	double ApproxXCamera;
}
Aya_t;

#define AYA_DASHAREA_X1 100.0
#define AYA_DASHAREA_X2 700.0
#define AYA_DASHAREA_MARGIN_X 50.0
#define AYA_DASHENDAREA_X1 200.0
#define AYA_DASHENDAREA_X2 600.0

#define AYA_ATTACKAREA_X 400.0
#define AYA_ATTACKAREA_MARGIN_X 300.0
#define AYA_ATTACKENDAREA_X1 100.0
#define AYA_ATTACKENDAREA_X2 700.0
#define AYA_ATTACKENDAREA_MARGIN_X 25.0

static void AyaYure(GmEnemy_t *i, Aya_t *ed)
{
	i->X = ed->X + sin(ed->XR) * 10.0;
	i->Y = ed->Y + sin(ed->YR) * 10.0;
}
static int AyaDash(GmEnemy_t *i, Aya_t *ed, int frmcnt) // frmcnt: 0-
{
	double cx = i->X - GmDc.CameraX;

	ifsceneBegin(frmcnt, 9 * 4 - 1)
	{
		fs_count /= 4;
		AyaYure(i, ed);

		GmBeforeDrawEnemy(i);
		GmDrawCharacter(i->X, i->Y, Pic(P_AYA_DASH_00 + fs_count), !ed->DashLeftMode);
		GmAfterDrawEnemy(i);
	}
	ifsceneEnd
	{
		if(!fs_count)
		{
			double kasoku_rate;

			switch(i->Lv)
			{
			case 0: kasoku_rate = 1.005; break;
			case 1: kasoku_rate = 1.02; break;
			case 2: kasoku_rate = 1.04; break;
			case 3: kasoku_rate = 1.08; break;

			default:
				error();
			}
			P2Vector(i->X, i->Y, GmDc.Player.X, GmDc.Player.Y, 5.0 + drnd() * 3.0);
			ed->DashXAdd = Vect_X;
			ed->DashYAdd = Vect_Y;
			ed->DashXAddRate = kasoku_rate;
			ed->DashYAddRate = kasoku_rate;
			ed->DashLeftMode = ed->DashXAdd < 0.0 ? 1 : 0;
		}
		if(i->Y < 0.0 || 500.0 < i->Y)
		{
			double kasoku_rate;

			switch(i->Lv)
			{
			case 0: kasoku_rate = 1.008 + drnd() * 0.002; break;
			case 1: kasoku_rate = 1.04  + drnd() * 0.002; break;
			case 2: kasoku_rate = 1.06  + drnd() * 0.002; break;
			case 3: kasoku_rate = 1.0019 + drnd() * 0.0001; break;

			default:
				error();
			}
			P2Vector(cx, i->Y, ed->DashLeftMode ? 0.0 : 800.0, 300.0, 5.0 + drnd() * 3.0);
			ed->DashXAdd = Vect_X;
			ed->DashYAdd = Vect_Y;
			ed->DashXAddRate = kasoku_rate;
			ed->DashYAddRate = 1.0;
		}
		ed->X += ed->DashXAdd;
		ed->Y += ed->DashYAdd;
		ed->DashXAdd *= ed->DashXAddRate;
		ed->DashYAdd *= ed->DashYAddRate;

		fs_count /= 3;
		fs_count %= 2;
		AyaYure(i, ed);

		GmBeforeDrawEnemy(i);
		GmDrawCharacter(i->X, i->Y, Pic(P_AYA_DASH_09 + fs_count), ed->DashLeftMode);
		GmAfterDrawEnemy(i);

		return ed->DashLeftMode ?
			AYA_DASHENDAREA_X1 < cx :
			AYA_DASHENDAREA_X2 > cx;
	}
	return 1;
}
static int AyaAttack(GmEnemy_t *i, Aya_t *ed, int frmcnt) // frmcnt: 0-
{
	if(!frmcnt)
	{
retrySetDest:
		int a_lm = GmDc.Player.X < i->X;

		ed->AttackLastCameraX = GmDc.CameraX;
		ed->AttackDestX = (a_lm ? AYA_ATTACKENDAREA_X2 : AYA_ATTACKENDAREA_X1) +
			ernd() * 0.9 * AYA_ATTACKENDAREA_MARGIN_X + GmDc.CameraX;
		ed->AttackDestY = 50.0 + drnd() * 300.0;
		ed->AttackLeftMode = a_lm;

		if(m_isNear(i->X, i->Y, ed->AttackDestX, ed->AttackDestY, 40.0))
			goto retrySetDest;
	}
	int leftmode = ed->AttackLeftMode;

	ifsceneBegin(frmcnt, 6 * 4 - 1)
	{
		fs_count /= 4;
		AyaYure(i, ed);

		GmBeforeDrawEnemy(i);
		GmDrawCharacter(i->X, i->Y, Pic(P_AYA_ATTACK_00 + fs_count), ed->AttackLeftMode);
		GmAfterDrawEnemy(i);
	}
	ifscene(8 * 4 - 1)
	{
		if(!fs_count)
		{
			GmCommonShootYokoku_L(i->X, i->Y, D_R_STAR | DTP);

			double r = P2Angle(i->X, i->Y, GmDc.Player.X, GmDc.Player.Y);
			double rr = 0.4;
			double tamaspeed;
			int tamaend;

			switch(i->Lv)
			{
			case 0:
				tamaspeed = 5.0;
				tamaend = 2;
				break;

			case 1:
				tamaspeed = 7.0;
				tamaend = 4;
				break;

			case 2:
				rr *= 0.75;
				tamaspeed = 10.0;
				tamaend = 6;
				break;

			case 3:
				rr *= 1.25;
				tamaspeed = 13.0;
				tamaend = 1;
				break;

			default:
				error();
			}
			for(int tamacnt = -tamaend; tamacnt <= tamaend; tamacnt++)
			{
				double x = cos(r + tamacnt * rr) * tamaspeed;
				double y = sin(r + tamacnt * rr) * tamaspeed;

				S_EnemyEntryQueue()->AddElement(MakeTama(MakeAya, i->X, i->Y, x, y, D_R_STAR | DTP));
			}
			if(i->Lv == 3)
			{
				const int TAMA_MAX = 16;
				const double TAMA_SPEED = 3.0;
				double x = cos(r) * TAMA_SPEED;
				double y = sin(r) * TAMA_SPEED;

				for(int tamacnt = 0; tamacnt < TAMA_MAX; tamacnt++)
				{
					S_EnemyEntryQueue()->AddElement(MakeTama(MakeAya, i->X, i->Y, x, y, D_R_STAR | DTP,
						0.0, 0.0,
						2.0 * PI * ((double)tamacnt / TAMA_MAX), 0.03, 1.0,
						40.0, 0.0, 0.0
						));
				}
			}
		}
		double dcx = GmDc.CameraX - ed->AttackLastCameraX;

		ed->AttackLastCameraX = GmDc.CameraX;
		ed->AttackDestX += dcx;

		fs_count /= 4;
		m_countApprox(ed->X, ed->AttackDestX, 0.9, 0.0);
		m_countApprox(ed->Y, ed->AttackDestY, 0.9, 0.0);
		AyaYure(i, ed);

		GmBeforeDrawEnemy(i);
		GmDrawCharacter(i->X, i->Y, Pic(P_AYA_ATTACK_06 + fs_count), ed->AttackLeftMode);
		GmAfterDrawEnemy(i);
	}
	ifsceneEnd
	{
		return 0;
	}
	return 1;
}
static int AyaActor(GmEnemy_t *i, Aya_t *ed)
{
	if(!i->FrmCnt) // init
	{
		i->FrmCnt = rnd(AYA_CYCLE);

		ed->XR = drnd() * 2.0 * PI;
		ed->YR = drnd() * 2.0 * PI;
		ed->XRAdd = ernd() * 0.1;
		ed->YRAdd = ernd() * 0.1;

		ed->DashSleep = rrnd(0, 30);
		ed->DashDZCount = 0;

		ed->AttackSleep = rrnd(0, 30);
		ed->AttackDZCount = 0;
	}
	double cx = i->X - GmDc.CameraX;

	if(ed->DashDZCount)
	{
		ed->DashDZCount++;
	}
	else if(ed->AttackDZCount)
	{
		ed->AttackDZCount++;
	}
	else
	{
		if(abs(cx - AYA_DASHAREA_X1) < AYA_DASHAREA_MARGIN_X)
		{
			ed->DashDZCount = m_countDown(ed->DashSleep) ? 0 : 1;
		}
		else if(abs(cx - AYA_DASHAREA_X2) < AYA_DASHAREA_MARGIN_X)
		{
			ed->DashDZCount = m_countDown(ed->DashSleep) ? 0 : 1;
		}
		else if(abs(cx - AYA_ATTACKAREA_X) < AYA_ATTACKAREA_MARGIN_X)
		{
			ed->AttackDZCount = m_countDown(ed->AttackSleep) ? 0 : 1;
		}
	}

	if(ed->DashDZCount)
	{
		// noop
	}
	else if(ed->AttackDZCount)
	{
		// noop
	}
	else
	{
		if(ed->ApproxMode)
		{
			double ayacx = ed->X - GmDc.CameraX;
			double rate;

			if(ed->ApproxXCamera < 400.0 ?
				ed->ApproxXCamera < ayacx :
				ed->ApproxXCamera > ayacx
				)
			{
				switch(i->Lv) // 目標より画面内側か反対側
				{
				case 0: rate = 1.0; break;
				case 1: rate = 0.999; break;
				case 2: rate = 0.98; break;
				case 3: rate = 0.9; break;

				default:
					error();
				}
			}
			else // 目標より画面外側
			{
				switch(i->Lv)
				{
				case 0: rate = 0.985; break;
				case 1: rate = 0.95; break;
				case 2: rate = 0.92; break;
				case 3: rate = 0.9; break;

				default:
					error();
				}
			}
			m_countApprox(ed->X, GmDc.CameraX + ed->ApproxXCamera, rate, 0.0);
		}
		AyaYure(i, ed);

		ed->XR += ed->XRAdd;
		ed->YR += ed->YRAdd;
	}
	return 1;
}
static void AyaDrawer(GmEnemy_t *i, Aya_t *ed)
{
	if(ed->DashDZCount)
	{
		if(AyaDash(i, ed, ed->DashDZCount - 1)) // ? 継続
		{
			goto endDraw;
		}
		ed->DashDZCount = 0;

		/*
			ed->RenzokuAttackRemain: 1 == 1回のみ, 2 == 2連続...
		*/
		switch(i->Lv)
		{
		case 0:
			ed->DashSleep = rrnd(50, 100);
			ed->RenzokuAttackRemain = 1;
			break;

		case 1:
			ed->DashSleep = rrnd(40, 50);
			ed->RenzokuAttackRemain = 2;
			break;

		case 2:
			ed->DashSleep = rrnd(10, 30);
			ed->RenzokuAttackRemain = 3;
			break;

		case 3:
			ed->DashSleep = 10;
			ed->RenzokuAttackRemain = 4;
			break;

		default:
			error();
		}
		ed->AttackDZCount = 0;
	}
reAttack:
	if(ed->AttackDZCount)
	{
		if(AyaAttack(i, ed, ed->AttackDZCount - 1)) // ? 継続
		{
			goto endDraw;
		}
		ed->AttackDZCount = 0;

		switch(i->Lv)
		{
		case 0: ed->AttackSleep = rrnd(50, 100); break;
		case 1: ed->AttackSleep = rrnd(40, 50); break;
		case 2: ed->AttackSleep = rrnd(10, 30); break;
		case 3: ed->AttackSleep = 10; break;

		default:
			error();
		}
		ed->ApproxMode = 1;
		ed->ApproxXCamera = ed->AttackDestX - GmDc.CameraX;
	}
	if(m_countDown(ed->RenzokuAttackRemain))
	{
		ed->AttackDZCount = 1;
		goto reAttack;
	}
	int leftmode = GmDc.Player.X < i->X ? 1 : 0;
	int frmidx = i->FrmCnt;

	frmidx /= 5;
	frmidx %= AYA_CYCLE;

	int x = (int)i->X;
	int y = (int)i->Y;

	int xHosei[AYA_CYCLE] =
	{
		 0,
		 1,
		 0,
		 0,
		-1,
		-1,
		-2,
		 0,
	};
	int yHosei[AYA_CYCLE] =
	{
		 0,
		 1,
		 1,
		 0,
		-1,
		-1,
		-1,
		-1,
	};
	x += xHosei[frmidx] * (leftmode ? 1 : -1);
	y += yHosei[frmidx];

	GmBeforeDrawEnemy(i);
	GmDrawCharacter(x, y, Pic(P_AYA_00 + frmidx), leftmode);
	GmAfterDrawEnemy(i);

endDraw:
	if(ed->DashDZCount)
		GmAddCircleScope(i->X, i->Y, 40.0, SCOPECOLOR_ENEMY);
	else
		GmAddBoxScope((int)i->X - 20, (int)i->Y - 40, 40, 80, SCOPECOLOR_ENEMY);
}
static int AyaIsCrashed(GmEnemy_t *i, Aya_t *ed, double x, double y, double w, double h)
{
	if(ed->DashDZCount)
		return IsCrashed_BoxCircle(x, y, w, h, i->X, i->Y, 40.0);
	else
		return IsCrashed_Box(x, y, w, h, i->X - 20.0, i->Y - 40.0, 40.0, 80.0);
}
static void ReleaseAya(Aya_t *i)
{
	memFree(i);
}
GmEnemy_t MakeAya(double x, double y, int level)
{
	Aya_t *ed = nb_(Aya_t);

	ed->X = x;
	ed->Y = y;

	DAE_DirectPos = 1;
	DAE_X = x;
	DAE_Y = y;

	int hp;

	switch(level)
	{
	case 0: hp = 12; break;
	case 1: hp = 16; break;
	case 2: hp = 18; break;
	case 3: hp = 18; break;

	default:
		error();
	}
	return MakeEnemy(MakeAya, x, 100.0, level, hp, AyaActor, AyaDrawer, AyaIsCrashed, ReleaseAya, ed);
}

// ---- Udonge ----

#define UDONGE_CYCLE 120

static double MakeUdongeSpeed(void)
{
	return 2.0 + drnd() * 0.5;
}
typedef struct Udonge_st
{
	double XAdd;
	int AttackSleep;
	int AttackDZCount; // 1-: frmcnt 0-
	int AttackLeftMode;
	int Gamennihaitta;

	struct
	{
		int MakeMUCount;
	}
	Lv3;
}
Udonge_t;

static int UdongeAttack(GmEnemy_t *i, Udonge_t *ed, int frmcnt) // frmcnt: 0-
{
	if(!frmcnt)
	{
		ed->AttackLeftMode = GmDc.Player.X < i->X;
	}
	int leftmode = ed->AttackLeftMode;

	ifsceneBegin(frmcnt, 4 * 5 - 1)
	{
		fs_count /= 5;

		GmBeforeDrawEnemy(i);
		GmDrawCharacter(i->X, i->Y, Pic(P_UDONGE_ATTACK_00 + fs_count), leftmode);
		GmAfterDrawEnemy(i);
	}
	ifscene(4 * 4 - 1)
	{
		fs_count /= 4;

		double speed;

		switch(i->Lv)
		{
		case 0: speed = 9.0; break;
		case 1: speed = 12.0; break;
		case 2: speed = 5.0; break;
		case 3: speed = 7.0; break;

		default:
			error();
		}
		i->X += speed * (leftmode ? -1.0 : 1.0);

		GmBeforeDrawEnemy(i);
		GmDrawCharacter(i->X, i->Y, Pic(P_UDONGE_ATTACK_04 + fs_count), leftmode);
		GmAfterDrawEnemy(i);
	}
	ifscene(5 * 3 - 1)
	{
		fs_count /= 3;

		double speed;

		switch(i->Lv)
		{
		case 0: speed = 4.0; break;
		case 1: speed = 6.0; break;
		case 2: speed = 13.0; break;
		case 3: speed = 7.0; break;

		default:
			error();
		}
		i->X += speed * (leftmode ? -1.0 : 1.0);

		if(1 <= i->Lv)
		{
			for(int c = 0; c < 1; c++)
//			for(int c = 0; c < 2; c++)
			{
				DrawBegin(i->X - GmDc.CameraX, i->Y, Pic(P_UDONGE_ATTACKBACK));
				DrawZoom(2.5);
				DrawRotate(2.0 * PI * fs_rate * (1.0 + c * 0.5));

				if(leftmode)
					DrawXZoom(-1.0);

				SetAlpha(0.7);
				DrawEnd();
				ResetBlend();
			}
			S_EnemyEntryQueue()->AddElement(MakeDamageArea_Circle(i->X, i->Y, 100.0, 2));
		}
		GmBeforeDrawEnemy(i);
		GmDrawCharacter(i->X, i->Y, Pic(P_UDONGE_ATTACK_08 + fs_count), leftmode, 1.2);
		GmAfterDrawEnemy(i);
	}
	ifsceneEnd
	{
		if(i->Lv == 3)
		{
			int u_lv;

			switch(++ed->Lv3.MakeMUCount)
			{
			case 1:
			case 2:
			case 3: u_lv = 0; break;
			case 4: u_lv = 2; break;

			default:
				goto endAddMawarudonge;
			}
			int d_star1 = u_lv == 0 ? D_B_STAR : D_D_STAR;
			int d_star2 = u_lv == 0 ? D_C_STAR : D_W_STAR;

			for(int c = 0; c < 24; c++) // 発射エフェクト
			{
				int pic_resno = (drnd() < 0.5 ? d_star1 : d_star2) | DTP;
				double r = 2.0 * PI * drnd();
				double speed = 4.5 + drnd() * 2.5;
				double x_add = cos(r) * speed;
				double y_add = sin(r) * speed;

				GTAddCommonEffect(
					1, pic_resno, 0, 0,
					i->X, i->Y, 1.5 + drnd() * 1.5, 2.0 * PI * drnd(), 1.0,
					x_add, y_add, 0.0, ernd() * 0.1, -0.02
					);
			}
			const int UC_MAX = 3;
			const double UC_R = P2Angle(i->X, i->Y, GmDc.Player.X, GmDc.Player.Y) + PI;
			const double UC_SPEED = 9.0 + drnd();

			for(int uc = 0; uc < UC_MAX; uc++)
			{
				double r = UC_R + 2.0 * PI * ((double)uc / UC_MAX);
				double x_add = cos(r) * UC_SPEED;
				double y_add = sin(r) * UC_SPEED;

				S_EnemyEntryQueue()->AddElement(MakeMawarudonge(i->X, i->Y, x_add, y_add, u_lv));
			}
endAddMawarudonge:;
		}
		return 0;
	}
	return 1;
}
static int UdongeActor(GmEnemy_t *i, Udonge_t *ed)
{
	if(!i->FrmCnt) // init
	{
		i->FrmCnt = rnd(UDONGE_CYCLE);
		ed->XAdd = MakeUdongeSpeed() * (GmDc.Player.X < i->X ? -1.0 : 1.0);
		ed->AttackDZCount = 0;
		ed->AttackSleep = 30;
	}

	if(ed->AttackDZCount)
	{
		ed->AttackDZCount++;
	}
	else if(!m_countDown(ed->AttackSleep))
	{
		double NEAR_R;

		switch(i->Lv)
		{
		case 0: NEAR_R = 200.0; break;
		case 1:
		case 2: NEAR_R = 300.0; break;
		case 3: NEAR_R = 250.0; break;

		default:
			error();
		}
		if(m_pow(i->X - GmDc.Player.X) + m_pow(i->Y - GmDc.Player.Y) < m_pow(NEAR_R))
		{
			ed->AttackDZCount = 1;
		}
	}

	if(ed->AttackDZCount)
	{
		// noop
	}
	else
	{
		i->X += ed->XAdd;

		if(i->FrmCnt % UDONGE_CYCLE == 0)
		{
			double vect_x = (i->X < GmDc.Player.X ? 1.0 : -1.0);
			double rate;

			switch(i->Lv)
			{
			case 0: rate = 0.2; break;
			case 1: rate = 0.1; break;
			case 2: rate = 0.003; break;
			case 3: rate = 0.0; break;

			default:
				error();
			}
			if(drnd() < rate)
			{
				vect_x *= -1.0;
			}
			ed->XAdd = vect_x * MakeUdongeSpeed();
		}
		if(i->Lv || !ed->Gamennihaitta)
		{
			double cx = i->X - GmDc.CameraX;

			if(cx < 0.0)
			{
				ed->XAdd = abs(ed->XAdd);
				ed->AttackDZCount = ed->Gamennihaitta;
			}
			else if(800.0 < cx)
			{
				ed->XAdd = -abs(ed->XAdd);
				ed->AttackDZCount = ed->Gamennihaitta;
			}
			else
			{
				ed->Gamennihaitta = 1;
			}

			if(i->Lv == 3)
			{
				m_range(i->X, GmDc.CameraX, GmDc.CameraX + 800.0);
			}
		}
	}
	return 1;
}
static void UdongeDrawer(GmEnemy_t *i, Udonge_t *ed)
{
	if(ed->AttackDZCount)
	{
		if(UdongeAttack(i, ed, ed->AttackDZCount - 1)) // ? 継続
		{
			goto endDraw;
		}
		ed->AttackDZCount = 0;

		switch(i->Lv)
		{
		case 0:
		case 1:
		case 2: ed->AttackSleep = rrnd(10, 60); break;
		case 3: ed->AttackSleep = 180; break;

		default:
			error();
		}
	}
	int leftmode = ed->XAdd < 0.0 ? 1 : 0;
	int frmidx = i->FrmCnt;

	frmidx /= 5;
	frmidx %= 3;

	int pic_leftmode = (leftmode ? 0 : 1); // 後ろ向きなので逆

	double xHosei[] =
	{
		0.0,
		2.5,
		1.0,
	};
	double x = i->X + xHosei[frmidx] * (pic_leftmode ? 1.0 : -1.0);
	double y = i->Y;

	GmBeforeDrawEnemy(i);
	GmDrawCharacter(x, y, Pic(P_UDONGE_00 + frmidx), pic_leftmode);
	GmAfterDrawEnemy(i);

endDraw:
	GmAddCircleScope(i->X, i->Y, 40.0, SCOPECOLOR_ENEMY);
}
static int UdongeIsCrashed(GmEnemy_t *i, Udonge_t *ed, double x, double y, double w, double h)
{
	return IsCrashed_BoxCircle(x, y, w, h, i->X, i->Y, 40.0);
}
static void ReleaseUdonge(Udonge_t *i)
{
	memFree(i);
}
GmEnemy_t MakeUdonge(double x, double y, int level)
{
	int hp;

	switch(level)
	{
	case 0: hp =  6; break;
	case 1: hp =  8; break;
	case 2: hp = 12; break;
	case 3: hp = 36; break;

	default:
		error();
	}
	return MakeEnemy(MakeUdonge, x, y, level, hp, UdongeActor, UdongeDrawer, UdongeIsCrashed, ReleaseUdonge);
}
