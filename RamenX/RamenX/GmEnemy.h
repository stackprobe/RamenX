#define MAKE_BOSS     ((void *)MakeBossDummy)
#define MAKE_BOSSFIRE ((void *)MakeBossFireDummy)

extern char *MakeBossDummy;
extern char *MakeBossFireDummy;

extern int LastAttackedEnemyHP;
extern int LastAttackedEnemyMaxHP;

#define SCOPEALPHA 168
#define SCOPECOLOR_TAMA    RGB2I(255,   0, 255, SCOPEALPHA)
#define SCOPECOLOR_ENEMY   RGB2I(  0, 255, 255, SCOPEALPHA)
#define SCOPECOLOR_ATTACK  RGB2I(255, 255,   0, SCOPEALPHA)
#define SCOPECOLOR_PLAYER  RGB2I(255, 255, 255, SCOPEALPHA)
#define SCOPECOLOR_MAPITEM RGB2I(128, 255, 128, SCOPEALPHA)

typedef struct GmEnemy_st
{
	void *Maker; // ボスステージの場合、ボスキャラは MAKE_BOSS とする。
	double X;
	double Y;
	int Lv;
	int HP;
	int Far;
	int FrmCnt;
	int TransDZCount;
	int DamageEffectDZCount;
	int (*Actor)(struct GmEnemy_st *, void *); // ret: ? 生存
	void (*Drawer)(struct GmEnemy_st *, void *);
	int (*IsCrashed)(struct GmEnemy_st *, void *, double, double, double, double);
	void (*FuncReleaseExtraData)(void *);
	void *ExtraData;
	int DamageUketa;
	int AlwaysMove;
	int MaxHP; // 最大HP確認用

	union
	{
		struct
		{
			/*
				0: 自動
				1: 左 (LeftMode = 1)
				2: 右 (LeftMode = 0)
			*/
			int ForceDir;
		}
		Suica;
	}
	ExtraPart;
}
GmEnemy_t;

autoList<GmEnemy_t> *S_EnemyEntryQueue(void);

GmEnemy_t MakeEnemy_ved(
	void *maker,
	double x, double y, int level, int hp,
	int (*actor)(GmEnemy_t *, void *),
	void (*drawer)(GmEnemy_t *, void *),
	int (*isCrashed)(GmEnemy_t *, void *, double, double, double, double),
	void (*fred)(void *), void *ed
	);

template <class ExtraData_t>
GmEnemy_t MakeEnemy(
	void *maker,
	double x, double y, int level, int hp,
	int (*actor)(GmEnemy_t *, ExtraData_t *),
	void (*drawer)(GmEnemy_t *, ExtraData_t *),
	int (*isCrashed)(GmEnemy_t *, ExtraData_t *, double, double, double, double),
	void (*fred)(ExtraData_t *), ExtraData_t *ed = NULL
	)
{
	if(!ed)
		ed = (ExtraData_t *)memCalloc(sizeof(ExtraData_t));

	return MakeEnemy_ved(
		maker,
		x, y, level, hp,
		(int (*)(GmEnemy_t *, void *))actor,
		(void (*)(GmEnemy_t *, void *))drawer,
		(int (*)(GmEnemy_t *, void *, double, double, double, double))isCrashed,
		(void (*)(void *))fred, (void *)ed
		);
}

extern int DAE_DirectPos;
extern double DAE_X;
extern double DAE_Y;

GmEnemy_t *DinamicAddEnemy(GmEnemy_t e);

void GmActEnemy(void);
void GmDrawEnemy(void);

void GmAttackAtariHanteiFrame(void);
void GmDamageAtariHanteiFrame(void);

void GmBeforeDrawEnemy(GmEnemy_t *i);
void GmAfterDrawEnemy(GmEnemy_t *i);

GmEnemy_t *GmFindEnemy_Circle(double x, double y, double r, void *maker = NULL, GmEnemy_t *myself = NULL);
GmEnemy_t *GmFindEnemy_InCamera(double MARGIN_XY, void *maker, GmEnemy_t *myself = NULL);

void GmCommonShootYokoku(double x, double y, int pic_resno, double z_add, double r_add);
void GmCommonShootEffect(double x, double y, int pic_resno, double prm_z, double a_add);

#define GmCommonShootYokoku_S(x, y, pic_resno) GmCommonShootYokoku(x, y, pic_resno, 0.06, 0.1)
#define GmCommonShootYokoku_L(x, y, pic_resno) GmCommonShootYokoku(x, y, pic_resno, 0.13, 0.2)

#define GmCommonShootEffect_S(x, y, pic_resno) GmCommonShootEffect(x, y, pic_resno, 1.0, -0.05)
#define GmCommonShootEffect_L(x, y, pic_resno) GmCommonShootEffect(x, y, pic_resno, 2.0, -0.03)

void GmMapItemGetEffect(double x, double y);

int GmInCamera(GmEnemy_t *i, double MARGIN_XY = 0.0);
