#include "Stdinc.h"

char *MakeBossDummy     = "MAKE_BOSS";
char *MakeBossFireDummy = "MAKE_BOSSFIRE";

int LastAttackedEnemyHP;
int LastAttackedEnemyMaxHP;

/*
	GmActEnemy() �� i->Actor() ���ŁAGmDc.EnemyList �ɒ��ڒǉ�����Ɨ����邱�Ƃ�����B
	GmDc.EnemyList ���g�����ꂽ�Ƃ��Ƀo�b�t�@���Ĕz�u������ i �������ɂȂ��Ă��܂����߁B
*/
singleton(EnemyEntryQueue, autoList<GmEnemy_t>, new autoList<GmEnemy_t>())

GmEnemy_t MakeEnemy_ved(
	void *maker,
	double x, double y, int level, int hp,
	int (*actor)(GmEnemy_t *, void *),
	void (*drawer)(GmEnemy_t *, void *),
	int (*isCrashed)(GmEnemy_t *, void *, double, double, double, double),
	void (*fred)(void *), void *ed
	)
{
	// hp == 0: ���G

	GmEnemy_t i;

	i.Maker = maker;
	i.X = x;
	i.Y = y;
	i.Lv = level;
	i.HP = hp;
	i.Far = 1;
	i.FrmCnt = -1; // actor() �O�� ++ ����̂� 0-
	i.TransDZCount = 0;
	i.DamageEffectDZCount = 0;
	i.Actor = actor;
	i.Drawer = drawer;
	i.IsCrashed = isCrashed;
	i.FuncReleaseExtraData = fred;
	i.ExtraData = ed;
	i.DamageUketa = 0;
	i.AlwaysMove = 0;
	i.MaxHP = hp; // �ő�HP�m�F�p

	memset(&i.ExtraPart, 0x00, sizeof(i.ExtraPart));

	return i;
}

int DAE_DirectPos;
double DAE_X;
double DAE_Y;

GmEnemy_t *DinamicAddEnemy(GmEnemy_t e)
{
	double ex = e.X;
	double ey = e.Y;

	if(DAE_DirectPos)
	{
		ex = DAE_X;
		ey = DAE_Y;

		DAE_DirectPos = 0;
	}
	double cx = ex - GmDc.CameraX;
	double cy = ey;

	if(
		0.0 <= cx && cx <= 800.0 &&
		0.0 <= cy && cy <= 600.0
		)
	{
		GmCommonShootEffect_L(ex, ey, D_W_STAR | DTP);
	}
	e.AlwaysMove = 1;
	S_EnemyEntryQueue()->AddElement(e);
	return S_EnemyEntryQueue()->ElementAt(S_EnemyEntryQueue()->GetCount() - 1); // ret: ���ǉ������v�f�ւ̎Q��
}

static int IsFarEnemy(GmEnemy_t *i)
{
	const double SEGMENT_X = FIELD_SEGMENT_X;
	const double MARGIN_X = FIELD_SEGMENT_MARGIN_X;
	double x1 = GmDc.CameraX - MARGIN_X;
	double x2 = GmDc.CameraX + 800.0 + MARGIN_X;

	if(0.0 < x1)
	{
		x1 /= SEGMENT_X;
		x1 = (double)(int)x1;
		x1 *= SEGMENT_X;
	}
	else
		x1 = -SEGMENT_X;

	x2 /= SEGMENT_X;
	x2 = (double)((int)x2 + 1);
	x2 *= SEGMENT_X;

	return i->X < x1 || x2 < i->X;
}
static int IsFarEnemy_EncounteredStronger(GmEnemy_t *i) // �G���J�E���g�������Ȃǂ̓G������ IsFarEnemy()
{
	const double MARGIN_X = 2000.0;

	double x1 = GmDc.CameraX - MARGIN_X;
	double x2 = GmDc.CameraX + 800.0 + MARGIN_X;

	return i->X < x1 || x2 < i->X;
}
static void KillEnemy(int index)
{
	GmEnemy_t i = GmDc.EnemyList->FastDesertElement(index);

	i.FuncReleaseExtraData(i.ExtraData);
}
void GmActEnemy(void)
{
	int start_index = 0;

restart:
	GmDc.EnemyList->ListLock = 1;

	for(int index = start_index; index < GmDc.EnemyList->GetCount(); index++)
	{
		GmEnemy_t *i = GmDc.EnemyList->ElementAt(index);

		// Set i->Far >

		i->Far = IsFarEnemy(i);

		if(10 <= i->FrmCnt) // ? Encountered
		{
			// ? Stronger
			if(
				i->Maker == MakeCube ||
				i->Maker == MakeMarisa ||
				i->Maker == MakeAya ||
				i->Maker == MakeUdonge ||
				i->Lv == 3
				)
				i->Far = IsFarEnemy_EncounteredStronger(i);
		}
		if(i->Maker == MakeTama ||
			i->Maker == MAKE_BOSSFIRE ||
			i->Maker == MakeDamageArea)
			i->Far = 0;

		// < Set i->Far

		if(i->Far && i->AlwaysMove == 0)
			continue;

		i->FrmCnt++;
		m_countDown(i->TransDZCount);
		m_countDown(i->DamageEffectDZCount);

		if(!i->Actor(i, i->ExtraData)) // ? ����
		{
			KillEnemy(index);
			index--;
		}
	}
	GmDc.EnemyList->ListLock = 0;

	/*
		�G���g���[��f���o������ Actor() �����s����B

		�G��ǉ������t���[���� Actor(), Drawer() �����s����B
		��ԑJ�ڂ�ړI�Ƃ������łƓG(���`��)�̐����Ɋԍ��������Ȃ����߁B
		DamageArea �̔�����x�������Ȃ����߁B

		Drawer() ����� Actor() �����s����B
		Far �̏����l�� 1 �Ȃ̂Ō���ł͊֌W�Ȃ��B
	*/
	if(S_EnemyEntryQueue()->GetCount())
	{
		start_index = GmDc.EnemyList->GetCount();

		GmDc.EnemyList->AddElements(S_EnemyEntryQueue());
		S_EnemyEntryQueue()->Clear();

		goto restart;
	}
}
void GmDrawEnemy(void)
{
	for(int index = 0; index < GmDc.EnemyList->GetCount(); index++)
	{
		GmEnemy_t *i = GmDc.EnemyList->ElementAt(index);

		if(i->Far)
			continue;

		i->Drawer(i, i->ExtraData);
	}
}

static void CheckAttackAtariHantei(double x, double y, double w, double h, int attackPower, int bomb_flag)
{
	if(!bomb_flag) // �S�������h�~ [GB]
	{
		GmDc.Player.TransDZCount = 0; // �����������I��
	}
	GmAddBoxScope((int)x, (int)y, (int)w, (int)h, SCOPECOLOR_ATTACK);

	for(int index = 0; index < GmDc.EnemyList->GetCount(); index++)
	{
		GmEnemy_t *e = GmDc.EnemyList->ElementAt(index);

		if(e->IsCrashed(e, e->ExtraData, x, y, w, h))
		{
			if(e->TransDZCount) // ? �ꎞ�I���G
			{
				// noop
			}
			else if(e->HP) // ? �񖳓G
			{
				for(int ac = 0; ac < attackPower; ac++) // �_���[�W
				{
					m_countDown(e->HP);
					e->DamageUketa = 1;
				}
				LastAttackedEnemyHP = e->HP;
				LastAttackedEnemyMaxHP = e->MaxHP;

				if(e->HP) // ? ����
				{
					SEPlay(SE_HIT_ENEMY);

					/*
						�v���C���[�̈��̍U���� 2, 3 �t���[���Ȃ̂ŁA���̊Ԃɕ�����H���Ȃ��悤�ɁB
					*/
					e->TransDZCount = 4;
					e->DamageEffectDZCount = 5 * attackPower;
				}
				else // ? ���S
				{
					/*
						�{�X�L�����̏ꍇ Actor() �ɔC����B
						���� Actor() �� HP == 0 �𔻒肵�A���S�葱�����s�� 0 ��Ԃ��ď��ł��Ă��炤�B
						HP == 0 �ɂȂ�̂ŁA�Ō�̈ꌂ�ȍ~���G�����ɂȂ邱�Ƃɒ��ӁB
					*/
					if(e->Maker == MAKE_BOSS)
						goto endDeath;

					SEPlay(SE_DEATH_ENEMY);
					GTAddCommonEffect(1, D_EXPLOSION_01 | DTP, 1, 10, e->X, e->Y);

					if(e->Maker == MakeCube && GmGetEnemyCount(MakeCube, 0) == 1) // ? �Ō�̔�
					{
						const double MI_MARGIN_XY = 100.0;
						double mi_x = e->X;
						double mi_y = e->Y;

						m_range(mi_x, GmDc.CameraX + MI_MARGIN_XY, GmDc.CameraX + 800.0 - MI_MARGIN_XY);
						m_range(mi_y, MI_MARGIN_XY, 600.0 - MI_MARGIN_XY);

						S_EnemyEntryQueue()->AddElement(MakeMapItem(mi_x, mi_y, MIE_ADD_ZANKI));
					}
					KillEnemy(index);
					index--;
endDeath:;
				}
			}
			else // ? ���G
			{
				// noop
			}
		}
	}
}
void GmAttackAtariHanteiFrame(void)
{
	if(GmDc.BombDZCount == 20) // Bomb �ɂ��U������
	{
		CheckAttackAtariHantei(GmDc.CameraX, 0, 800, 600, 10, 1);
	}
	double x = GmDc.Player.X;
	double y = GmDc.Player.Y;

	int POWER_K = 2;
	int POWER_J = 1;
	int POWER_JU = 1;
	if(Sd_PlayChara == PLAY_CHARA_01) {
		POWER_K = 3;
		POWER_J = 2;
		POWER_JU = 1;
	}
	if(Sd_PlayChara == PLAY_CHARA_02) {
		POWER_K = 1;
		POWER_J = 2;
		POWER_JU = 2;
	}
	if(Sd_PlayChara == PLAY_CHARA_03) {
		POWER_K = 1;
		POWER_J = 1;
		POWER_JU = 1;
	}

	if(GmDc.Player.UAttackDZCount == 5) // ���U�������蔻�蔭��
	{
		const double KUMASEBAME = 5.0;

		if(GmDc.Player.AttackLeftMode) // ? ������
		{
			CheckAttackAtariHantei(
				x - 130.0 + KUMASEBAME,
				y - 50.0,
				180.0 - KUMASEBAME * 2.0,
				100.0,
				POWER_JU,
				0
				);
		}
		else // ? �E����
		{
			CheckAttackAtariHantei(
				x - 50.0 + KUMASEBAME,
				y - 50.0,
				180.0 - KUMASEBAME * 2.0,
				100.0,
				POWER_JU,
				0
				);
		}
	}
	if(GmDc.Player.CAttackDZCount == 7) // ���U�������蔻�蔭��
	{
		double xHosei = 0.0;

		if(!GmDc.Player.LeftMode)
			xHosei = 20.0;

		CheckAttackAtariHantei(
			x - 130.0 + xHosei + 30.0,
			y - 145.0          +  0.0,
			240.0 - 60.0,
			260.0 -  0.0,
			POWER_K,
			0
			);
		CheckAttackAtariHantei(
			x - 130.0 + xHosei +  0.0,
			y - 145.0          + 40.0,
			240.0 -  0.0,
			260.0 - 80.0,
			POWER_K,
			0
			);
	}
	if(GmDc.Player.DAttackDZCount == 7) // �㉺�U�������蔻�蔭��
	{
		const double KUMAZURE = 10.0;

		if(GmDc.Player.AttackLeftMode) // ? ������
		{
			CheckAttackAtariHantei(x - 150.0 + KUMAZURE, y - 50.0, 200.0, 100.0, POWER_J, 0);
		}
		else // ? �E����
		{
			CheckAttackAtariHantei(x - 50.0 - KUMAZURE, y - 50.0, 200.0, 100.0, POWER_J, 0);
		}
	}
}
static GmEnemy_t *CheckDamageAtariHantei(double x, double y, double w, double h)
{
	GmAddBoxScope((int)x, (int)y, (int)w, (int)h, SCOPECOLOR_PLAYER);

	for(int index = 0; index < GmDc.EnemyList->GetCount(); index++)
	{
		GmEnemy_t *e = GmDc.EnemyList->ElementAt(index);

		if(e->IsCrashed(e, e->ExtraData, x, y, w, h))
		{
			return e;
		}
	}
	return NULL;
}
void GmDamageAtariHanteiFrame(void)
{
	if(GmDc.Player.TransDZCount) // ? �v���C���[���G
	{
		return;
	}
	GmEnemy_t *e = NULL;
	double xHosei = (GmDc.Player.LeftMode ? -1.0 : 1.0) * 7.0;
	const int FRAME_MARGIN = 10;

	if(GmDc.Player.JumpDZCount)
	{
#if 1
		e = CheckDamageAtariHantei(
				GmDc.Player.X - 8.0 + 4.0,
				GmDc.Player.Y - 8.0 + 0.0,
				16.0 - 8.0,
				16.0 - 0.0
				);

		if(!e)
			e = CheckDamageAtariHantei(
				GmDc.Player.X - 8.0 + 0.0,
				GmDc.Player.Y - 8.0 + 4.0,
				16.0 - 0.0,
				16.0 - 8.0
				);
#else
		if(GmDc.Player.WalkDZCount <= FRAME_MARGIN && GmDc.Player.AirDashDZCount <= FRAME_MARGIN)
		{
			e = CheckDamageAtariHantei(
				GmDc.Player.X - 15.0 + xHosei,
				GmDc.Player.Y - 20.0,
				30.0,
				40.0
				);
		}
		else
		{
			e = CheckDamageAtariHantei(
				GmDc.Player.X - 20.0 + 10.0,
				GmDc.Player.Y - 20.0 +  0.0,
				40.0 - 20.0,
				40.0 -  0.0
				);

			if(!e)
				e = CheckDamageAtariHantei(
					GmDc.Player.X - 20.0 +  0.0,
					GmDc.Player.Y - 20.0 + 10.0,
					40.0 -  0.0,
					40.0 - 20.0
					);
		}
#endif
	}
	else if(GmBackDogezaMax / 2 <= GmDc.Player.GezaDZCount && !GmPlayerIsAttackNow())
	{
		e = CheckDamageAtariHantei(
			GmDc.Player.X - 15.0 + xHosei,
			GmDc.Player.Y + 20.0,
			30.0,
			25.0
			);
	}
	else
	{
		if(GmDc.Player.DashDZCount <= FRAME_MARGIN)
		{
			e = CheckDamageAtariHantei(GmDc.Player.X - 15.0 + xHosei, GmDc.Player.Y - 40.0, 30.0, 85.0);
		}
		else
		{
			e = CheckDamageAtariHantei(GmDc.Player.X - 15.0 + xHosei, GmDc.Player.Y - 40.0, 30.0, 40.0);

			if(!e)
				e = CheckDamageAtariHantei(
					GmDc.Player.X - 15.0 + xHosei + (GmDc.Player.LeftMode ? 1.0 : -1.0) * 25.0,
					GmDc.Player.Y,
					30.0,
					45.0
					);
		}
	}

	if(e) // ? ���������B
	{
		GTAddCommonEffect(1, D_PLAYER_CRASH_00 | DTP, 1, 10, GmDc.Player.X, GmDc.Player.Y, 0.75);
//		GTAddCommonEffect(1, P_PLAYER_CRASH_00, 1, 12, GmDc.Player.X, GmDc.Player.Y, 1.5); // old
		GmSetPlayerHitBack(e->X < GmDc.Player.X, e->Y < GmDc.Player.Y);
		GmDc.DamageKuratta = 1;

		if(m_countDown(GmDc.Player.Hp)) // ? ����
		{
			SEPlay(SE_HIT_PLAYER);
		}
		else // ? ���S
		{
//			SEPlay(SE_DEATH_PLAYER);
			SEPlayEcho(SE_HIT_PLAYER);
			GmRc.GameEndReason = GER_DEATH;
		}
	}
}

void GmBeforeDrawEnemy(GmEnemy_t *i)
{
	if(i->DamageEffectDZCount && (LFrmCnt & 2))
	{
		SetBlendInv();
	}

	PictureGroup_t *epglist[] =
	{
		NULL, // Lv 0
		S_EnemyLv1PictureGroup(),
		S_EnemyLv2PictureGroup(),
		S_EnemyLv3PictureGroup(),
	};
	ec_range(i->Lv, 0, lengthof(epglist) - 1);

	if(i->Lv)
	{
		SetPictureGroup(epglist[i->Lv]);
	}
}
void GmAfterDrawEnemy(GmEnemy_t *i)
{
	if(i->DamageEffectDZCount && (LFrmCnt & 2))
	{
		ResetBlend();
	}
	if(i->Lv)
	{
		SetPictureGroup(NULL);
	}
}

/*
	���a r �������ōŊ�̓G��T���B

	maker: NULL == �S��
	ret:   NULL == ������Ȃ��B
*/
GmEnemy_t *GmFindEnemy_Circle(double x, double y, double r, void *maker, GmEnemy_t *myself)
{
	GmEnemy_t *nearest_i = NULL;
	double nearest_d = -1.0;

	for(int index = 0; index < GmDc.EnemyList->GetCount(); index++)
	{
		GmEnemy_t *i = GmDc.EnemyList->ElementAt(index);

		double dx = x - i->X;
		double dy = y - i->Y;

		double d = m_pow(dx) + m_pow(dy);

		if((maker == NULL || maker == i->Maker) && i != myself && d < m_pow(r))
		{
			if(!nearest_i || d < nearest_d)
			{
				nearest_i = i;
				nearest_d = d;
			}
		}
	}
	return nearest_i;
}
GmEnemy_t *GmFindEnemy_InCamera(double MARGIN_XY, void *maker, GmEnemy_t *myself)
{
	for(int index = 0; index < GmDc.EnemyList->GetCount(); index++)
	{
		GmEnemy_t *i = GmDc.EnemyList->ElementAt(index);

		if((maker == NULL || maker == i->Maker) && i != myself && GmInCamera(i, MARGIN_XY))
		{
			return i;
		}
	}
	return NULL;
}

void GmCommonShootYokoku(double x, double y, int pic_resno, double z_add, double r_add) // pic_resno: maybe (D_*_STAR | DTP)
{
	/*
		param   S     L
		-------------------
		z_add   0.07  0.13
		r_add   0.1   0.2
	*/
	GTAddCommonEffect(
		1,
		pic_resno,
		0, 0,
		x, y, 1.0, drnd() * PI * 2.0, 0.9,
		0.0, 0.0, z_add, r_add, -0.02
		);
}
void GmCommonShootEffect(double x, double y, int pic_resno, double prm_z, double a_add) // pic_resno: maybe (D_*_STAR | DTP)
{
	for(int c = 0; c < 5; c++)
	{
		double exy_r = drnd() * 2.0 * PI;
		double ex = sin(exy_r) * 5.0;
		double ey = cos(exy_r) * 5.0;

		/*
			param   S     L
			-----------------
			prm_z   1.0   2.0
			a_add  -0.05 -0.03
		*/
		GTAddCommonEffect(
			1,
			pic_resno,
			0, 0,
			x, y, 0.5 + drnd() * prm_z, 0.0, 0.5,
			ex, ey, 0.0, ernd() * 0.2, a_add
			);
	}
}
void GmMapItemGetEffect(double x, double y)
{
	for(int c = 0; c < 20; c++)
	{
		GTAddCommonEffect(
			1, P_MAPITEMTOTTA_00 + rnd(3), 0, 0,
			x + ernd() * 80.0,
			y + ernd() * 80.0,
			0.5, 0.0, 1.0,
			0.0, -3.0 - drnd() * 3.0, 0.0, drnd() * 2.0, -0.02 - drnd() * 0.02
			);
	}
}

int GmInCamera(GmEnemy_t *i, double MARGIN_XY)
{
	const double cx = i->X - GmDc.CameraX;
	const double cy = i->Y;

	return
		0.0 - MARGIN_XY <= cx && cx <= 800.0 + MARGIN_XY &&
		0.0 - MARGIN_XY <= cy && cy <= 600.0 + MARGIN_XY;
}
