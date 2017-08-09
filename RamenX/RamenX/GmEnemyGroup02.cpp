#include "Stdinc.h"

// ---- DamageArea ----

typedef struct DamageArea_st
{
	int LifeCount; // 0: –³§ŒÀ, 1: ‘¦Ž€–S, 2: 1ƒtƒŒ[ƒ€...

	// Box
	double W;
	double H;

	// Circle
	double R;
}
DamageArea_t;

static int DamageAreaActor(GmEnemy_t *i, DamageArea_t *ed)
{
	if(m_countDown(ed->LifeCount) && !ed->LifeCount)
	{
		return 0;
	}
	return 1;
}
static void DamageAreaDrawer(GmEnemy_t *i, DamageArea_t *ed)
{
	double mxy = 0.0;
	
	mxy = m_max(mxy, ed->W);
	mxy = m_max(mxy, ed->H);
	mxy = m_max(mxy, ed->R);

	if(!GmInCamera(i, mxy))
		return;

	if(1.0 <= ed->W && 1.0 <= ed->H)
	{
		GmAddBoxScope((int)i->X, (int)i->Y, (int)ed->W, (int)ed->H, SCOPECOLOR_TAMA);
	}
	if(1.0 <= ed->R)
	{
		GmAddCircleScope(i->X, i->Y, ed->R, SCOPECOLOR_TAMA);
	}
}
static int DamageAreaIsCrashed(GmEnemy_t *i, DamageArea_t *ed, double x, double y, double w, double h)
{
	if(1.0 <= ed->W && 1.0 <= ed->H)
	{
		return IsCrashed_Box(x, y, w, h, i->X, i->Y, ed->W, ed->H);
	}
	if(1.0 <= ed->R)
	{
		return IsCrashed_BoxCircle(x, y, w, h, i->X, i->Y, ed->R);
	}
	return 0;
}
static void ReleaseDamageArea(DamageArea_t *i)
{
	memFree(i);
}
GmEnemy_t MakeDamageArea(double x, double y, double w, double h, double r, int lifeCount)
{
	DamageArea_t *ed = nb(DamageArea_t);

	ed->LifeCount = lifeCount;
	ed->W = w;
	ed->H = h;
	ed->R = r;

	return MakeEnemy(MakeDamageArea, x, y, 0, 0,
		DamageAreaActor,
		DamageAreaDrawer,
		DamageAreaIsCrashed, ReleaseDamageArea, ed);
}

// ---- Doll3Tama01 ----

typedef struct D3Tama01_st
{
	int PicResno;
	double XAdd;
	double YAdd;
}
D3Tama01_t;

static int D3Tama01Actor(GmEnemy_t *i, D3Tama01_t *ed)
{
	i->X += ed->XAdd;
	i->Y += ed->YAdd;

	ed->XAdd *= 0.95;
	ed->YAdd *= 0.95;

	ifsceneBegin(i->FrmCnt, 20)
	{
		// noop
	}
	ifscene(30)
	{
		if(fs_count % 3 == 0)
		{
			GmCommonShootEffect_S(i->X, i->Y, ed->PicResno);
			P2Vector(i->X, i->Y, GmDc.Player.X, GmDc.Player.Y, 10.0);
			S_EnemyEntryQueue()->AddElement(MakeTama(MakeD3Tama01, i->X, i->Y, Vect_X, Vect_Y, ed->PicResno));
		}
	}
	ifsceneEnd
	{
		GmCommonShootYokoku_L(i->X, i->Y, ed->PicResno);
		return 0;
	}
	return 1;
}
static void D3Tama01Drawer(GmEnemy_t *i, D3Tama01_t *ed)
{
	int frmidx = i->FrmCnt;

	frmidx /= 4;
	frmidx %= 5;

//	GmBeforeDrawEnemy(i);
	DrawBegin(i->X - GmDc.CameraX, i->Y, Pic(ed->PicResno));
	DrawZoom(2.0);
	DrawRotate(i->FrmCnt / 10.0);
	DrawEnd();
//	GmAfterDrawEnemy(i);

	GmAddCircleScope(i->X, i->Y, 20.0, SCOPECOLOR_TAMA);
}
static int D3Tama01IsCrashed(GmEnemy_t *i, D3Tama01_t *ed, double x, double y, double w, double h)
{
	return IsCrashed_BoxCircle(x, y, w, h, i->X, i->Y, 20.0);
}
static void ReleaseD3Tama01(D3Tama01_t *i)
{
	memFree(i);
}
GmEnemy_t MakeD3Tama01(double x, double y, double x_add, double y_add, int pic_resno) // pic_resno ... maybe (D_*_STAR | DTP)
{
	D3Tama01_t *ed = nb(D3Tama01_t);

	ed->PicResno = pic_resno;
	ed->XAdd = x_add;
	ed->YAdd = y_add;

	return MakeEnemy(MakeD3Tama01, x, y, 0, 0,
		D3Tama01Actor,
		D3Tama01Drawer,
		D3Tama01IsCrashed, ReleaseD3Tama01, ed);
}

// ---- Mawarudonge ----

typedef struct Mawarudonge_st
{
	int FrameCount;
	double XAdd;
	double YAdd;
}
Mawarudonge_t;

static int MawarudongeActor(GmEnemy_t *i, Mawarudonge_t *ed)
{
	if(!i->FrmCnt) // init
	{
		i->FrmCnt = rnd(1000);
		ed->FrameCount = 0;
	}
	else
		ed->FrameCount++;

	if(abs(ed->XAdd) < 0.75 && abs(ed->YAdd) < 0.75)
	{
		S_EnemyEntryQueue()->AddElement(MakeUdonge(i->X, i->Y, i->Lv));
		return 0;
	}
	i->X += ed->XAdd;
	i->Y += ed->YAdd;

	if(70 < ed->FrameCount && !GmFindEnemy_Circle(i->X, i->Y, 100.0, MakeUdonge, i))
	{
		ed->XAdd *= 0.982;
		ed->YAdd *= 0.982;
	}
	double MARGIN_Y = 60.0;

	if(i->X < GmDc.CameraX)
	{
		ed->XAdd = abs(ed->XAdd);
	}
	else if(GmDc.CameraX + 800.0 < i->X)
	{
		ed->XAdd = -abs(ed->XAdd);
	}
	if(i->Y < MARGIN_Y)
	{
		ed->YAdd = abs(ed->YAdd);
	}
	else if(600.0 - MARGIN_Y < i->Y)
	{
		ed->YAdd = -abs(ed->YAdd);
	}
	m_range(i->X, GmDc.CameraX, GmDc.CameraX + 800.0);

	return 1;
}
static void MawarudongeDrawer(GmEnemy_t *i, Mawarudonge_t *ed)
{
	int leftmode = ed->XAdd < 0.0 ? 1 : 0;
	int frmidx = i->FrmCnt;

	frmidx /= 4;
	frmidx %= 5;

	GmBeforeDrawEnemy(i);
	GmDrawCharacter(i->X, i->Y, Pic(P_UDONGE_ATTACK_08 + frmidx), leftmode, 1.2);
	GmAfterDrawEnemy(i);

	GmAddCircleScope(i->X, i->Y, 40.0, SCOPECOLOR_ENEMY);
}
static int MawarudongeIsCrashed(GmEnemy_t *i, Mawarudonge_t *ed, double x, double y, double w, double h)
{
	return IsCrashed_BoxCircle(x, y, w, h, i->X, i->Y, 40.0);
}
static void ReleaseMawarudonge(Mawarudonge_t *i)
{
	memFree(i);
}
GmEnemy_t MakeMawarudonge(double x, double y, double x_add, double y_add, int level)
{
	Mawarudonge_t *ed = nb(Mawarudonge_t);

	ed->XAdd = x_add;
	ed->YAdd = y_add;

	return MakeEnemy(MakeMawarudonge, x, y, level, 8,
		MawarudongeActor,
		MawarudongeDrawer,
		MawarudongeIsCrashed, ReleaseMawarudonge, ed);
}

// ---- MapItem ----

typedef struct MapItem_st
{
	MapItemEffect_t Effect;
	double X;
	double Y;
}
MapItem_t;

#define MI_FRMCNT_BORN 50
#define MI_R 50.0

static int MapItemActor(GmEnemy_t *i, MapItem_t *ed)
{
	if(!i->FrmCnt) // init
	{
		// noop
	}

	i->X = ed->X + sin((double)i->FrmCnt / 13.0) * 5.0;
	i->Y = ed->Y + sin((double)i->FrmCnt / 11.0) * 5.0;

	if(MI_FRMCNT_BORN < i->FrmCnt &&
		m_pow(i->X - GmDc.Player.X) + m_pow(i->Y - GmDc.Player.Y) < m_pow(MI_R)
		)
	{
		switch(ed->Effect)
		{
		case MIE_KAIFUKU:
			GmDc.Player.Hp = MAX_HP;
			SEPlay(SE_KAIFUKU);
			break;

		case MIE_ADD_BOMB:
			GmDc.BombNum++;
			SEPlay(SE_ADD_BOMB);
			break;

		case MIE_ADD_ZANKI:
			GmDc.Zankisuu++;
			SEPlay(SE_ADD_ZANKI);
			break;

		default:
			error();
		}
		GmMapItemGetEffect(GmDc.Player.X, GmDc.Player.Y);
		return 0;
	}
	return 1;
}
static void MapItemDrawer(GmEnemy_t *i, MapItem_t *ed)
{
	const int SIZE_XY = 32;
	int pic_resno;

	switch(ed->Effect)
	{
	case MIE_KAIFUKU:   pic_resno = D_MAPITEM_KAIFUKU   | DTP; break;
	case MIE_ADD_BOMB:  pic_resno = D_MAPITEM_ADD_BOMB  | DTP; break;
	case MIE_ADD_ZANKI: pic_resno = D_MAPITEM_ADD_ZANKI | DTP; break;

	default:
		error();
	}
	if(i->FrmCnt < MI_FRMCNT_BORN)
	{
		SetAlpha(0.5);
		DrawBegin(i->X - GmDc.CameraX, i->Y, Pic(pic_resno));
		DrawZoom(2.0);
		DrawRotate((double)i->FrmCnt / 5);
		DrawEnd();
		ResetBlend();
	}
	else
	{
		DrawPicRect(
			(int)i->X - SIZE_XY / 2 - (int)GmDc.CameraX,
			(int)i->Y - SIZE_XY / 2,
			SIZE_XY,
			SIZE_XY,
			Pic(pic_resno)
			);
		GmAddCircleScope(i->X, i->Y, MI_R, SCOPECOLOR_MAPITEM);
	}
}
static int MapItemIsCrashed(GmEnemy_t *i, MapItem_t *ed, double x, double y, double w, double h)
{
	return 0;
}
static void ReleaseMapItem(MapItem_t *i)
{
	memFree(i);
}
GmEnemy_t MakeMapItem(double x, double y, MapItemEffect_t effect)
{
	MapItem_t *ed = nb(MapItem_t);

	ed->Effect = effect;
	ed->X = x;
	ed->Y = y;

	return MakeEnemy(MakeMapItem, x, y, 0, 0,
		MapItemActor,
		MapItemDrawer,
		MapItemIsCrashed, ReleaseMapItem, ed);
}

// ---- Providence ----

typedef struct Providence_st
{
	int HellModeDZCount;
}
Providence_t;

static int ProvidenceActor(GmEnemy_t *i, Providence_t *ed)
{
	i->X += 1.0;
	return 1;
}
static void ProvidenceDrawer(GmEnemy_t *i, Providence_t *ed)
{
	error(); // todo
}
static int ProvidenceIsCrashed(GmEnemy_t *i, Providence_t *ed, double x, double y, double w, double h)
{
	return 0;
}
static void ReleaseProvidence(Providence_t *i)
{
	memFree(i);
}
GmEnemy_t MakeProvidence(double x)
{
	return MakeEnemy(MakeProvidence, x, 0.0, 0, 0,
		ProvidenceActor,
		ProvidenceDrawer,
		ProvidenceIsCrashed, ReleaseProvidence);
}
