#include "Stdinc.h"

static singleton(WallEffectList, taskGroup_t, CreateTaskGroup());

static double LastMoreSnowAlpha = 0.0;

static void DrawSnow(int wallMode)
{
	int ms_ksr = Es.MoreSnow % 5; // �~���
	int ms_aa = Es.MoreSnow / 5; // �A���`�G�C���A�X

	if(!ms_ksr && !wallMode)
		return;

	const int SCREEN_W = 800;
	const int SCREEN_H = 600;
	const int SNOW_W = 1000;

	int lfc = LFrmCnt;

	if(!wallMode)
		lfc += 390 + 15;

	int xlst[] =
	{
		lfc % SNOW_W,
		((lfc * 3) / 2) % SNOW_W,
		(lfc * 2) % SNOW_W,
	};
	int frmidxlst[] =
	{
		lfc % 30,
		(lfc + 10) % 30,
		(lfc + 20) % 30,
	};
	int cx = (int)GmDc.CameraX;

	if(!wallMode)
	{
		double mina = 0.0;
		double maxa = 1.0;

		switch(ms_ksr)
		{
		case 1:
			mina = 0.0;
			maxa = 0.5;
			break;

		case 2:
			mina = 0.5;
			maxa = 0.5;
			break;

		case 3:
			mina = 0.5;
			maxa = 1.0;
			break;

		case 4:
			mina = 1.0;
			maxa = 1.0;
			break;

		default:
			error();
		}
		int d = 3600;
		double a = (LFrmCnt % d) / (double)d;

		if(0.5 < a)
			a = 1.0 - a;

		a *= 2.0;
		a = mina + (maxa - mina) * a;
		SetBlendAdd(a);
		LastMoreSnowAlpha = a;
	}
	else
		SetAlpha(0.2);

	if(ms_aa)
		SetDrawMode(DX_DRAWMODE_BILINEAR);

	for(int index = 0; index < 3; index++)
	for(int r_x = -SNOW_W; r_x < GmDc.FieldWidth; r_x += SNOW_W)
	{
		int x = r_x + xlst[index] - cx;

		if(-10.0 - SNOW_W < x && x < SCREEN_W)
		{
			DrawPicRect(x, 0, SNOW_W, SCREEN_H, Pic(P_DUMMY)); // �p�~
		}
	}
	if(ms_aa)
		SetDrawMode(DX_DRAWMODE_NEAREST);

	ResetBlend();
}
static void DrawWall(void)
{
#if 0 // �p�~
	{
		const int MARGIN_Y = 6;
		const int YORUMADE_SEC = 90;

		int y = (int)((GmDc.Player.Y / 600.0) * MARGIN_Y);
		m_range(y, -MARGIN_Y + 1, MARGIN_Y - 1);
		y += MARGIN_Y;
		y = 0; // HACK

		double a = LFrmCnt / (60.0 * YORUMADE_SEC);
		m_range(a, 0.0, 1.0);

		SimpleDrawPic(-(int)GmDc.CameraX, y, Pic(P_FIELDWALL_HIRU));
		SetAlpha(a);
		SimpleDrawPic(-(int)GmDc.CameraX, y, Pic(P_FIELDWALL_YORU));
		ResetBlend();
	}
#endif
	if(GmDc.WallKasaneMode)
	{
		/*
			1�w: [0] [0] [0]
			2�w: [1] [1] [1] [1] [1]
			3�w: [2] [2] [2] [2] [2] ... [2] <- GmDc.FieldWidth

			[0-2] �� 800 x 600 �ł��邱�ƁB
		*/
		double scrollRate = GmDc.CameraX / (GmDc.FieldWidth - 800.0); // 0.0 - 1.0

		int x1 = (int)(scrollRate * 800.0 * 3.0) % 800;
		int x2 = (int)(scrollRate * 800.0 * 5.0) % 800;
		int x3 = (int)GmDc.CameraX % 800;

		SimpleDrawPic(  0 - x1, 0, Pic(GmDc.WallPicResnoList->GetElement(0)));
		SimpleDrawPic(800 - x1, 0, Pic(GmDc.WallPicResnoList->GetElement(0)));
		SetAlpha(0.5);
		SimpleDrawPic(  0 - x2, 0, Pic(GmDc.WallPicResnoList->GetElement(1)));
		SimpleDrawPic(800 - x2, 0, Pic(GmDc.WallPicResnoList->GetElement(1)));
		SimpleDrawPic(  0 - x3, 0, Pic(GmDc.WallPicResnoList->GetElement(2)));
		SimpleDrawPic(800 - x3, 0, Pic(GmDc.WallPicResnoList->GetElement(2)));
		ResetBlend();

		/*
			�n�ʂ̂悤�Ȃ��̂�`���Ă݂�Bfixme -> �摜�͌Œ�
			���X�N���[����������ɂ��������̂�...�B
		*/
		{
			const int w = 800;
			const int h = 30;
			int x = (int)GmDc.CameraX % w;

			SimpleDrawPic(0 - x,       0, Pic(D_FIELDWALL_401_C | DTP));
			SimpleDrawPic(w - x,       0, Pic(D_FIELDWALL_401_C | DTP));
			SimpleDrawPic(0 - x, 600 - h, Pic(D_FIELDWALL_401_C | DTP));
			SimpleDrawPic(w - x, 600 - h, Pic(D_FIELDWALL_401_C | DTP));
		}
	}
	else
	{
		int x = -(int)GmDc.CameraX;

		for(int index = 0; index < GmDc.WallPicResnoList->GetCount(); index++)
		{
			int pic_resno = GmDc.WallPicResnoList->GetElement(index);

			CheckPicSize(Pic(pic_resno));

			int xr = x + Pic_W;
			const int MARGIN_X = 10;

			if(x <= 800 + MARGIN_X && -MARGIN_X <= xr)
			{
				for(int y = 0; y < 600; y += Pic_H)
				{
					SimpleDrawPic(x, y, Pic(pic_resno));
				}
			}
			x = xr;
		}
	}
	if(GmDc.SnowMode)
	{
		DrawSnow(1);
	}
	double rpx = GmDc.Player.X / GmDc.FieldWidth;
	m_range(rpx, 0.0, 1.0); // 2bs

	if(GmDc.ExtraStageMode && drnd() < rpx)
	{
		double k = drnd();
		k *= k;
		k = 1.0 - k;
		double z = 1.0 + k * 2.0;
		double a = 0.07 * m_min(1.0, 1.5 * rpx);
		double x = 880.0;
		double xa = (-10.0 - k * 20.0) * (1.0 + 0.5 * rpx);
		double y = k * 300.0;
		double ya = ernd() * 1.5;

		if(drnd() < 0.5)
			y *= -1.0;

		y = 300.0 + y;

		GTACE_BlendAddMode = 1;
		GTACE_ExtraTaskGroup = S_WallEffectList();

		GTAddCommonEffect(0, P_DUMMY, 0, 0, x, y, z, PI, a, xa, ya); // �p�~

		GTACE_BlendAddMode = 0;
		GTACE_ExtraTaskGroup = NULL;
	}
}
static void DrawBomb(void)
{
	if(!GmDc.BombDZCount || 30 < GmDc.BombDZCount)
		return;

	double r = 0.1 + GmDc.BombDZCount * GmDc.BombDZCount * (4.0 / 400.0);

	DrawBegin(GmDc.BombX - GmDc.CameraX, GmDc.BombY, Pic(P_BOMB));
	DrawRotate(GmDc.BombDZCount * 0.1);
	DrawZoom(r);
	DrawEnd();
}

static void DrawStatus(void)
{
#if 0 // �p�~
	if(GmVisibleSubStatus == 2) // ��\��
		return;

	if(GmVisibleSubStatus == 3) // �Ȉ�
	{
		SetAlpha(0.3);
		SetBright(0, 0, 0);
		DrawPicRect(0, 0, 300, 16, Pic(P_WHITEBOX));
		ResetBlend();
		ResetBright();

		SetPrint(0, 0);
		for(int c = 0; c < 9; c++)
		{
			Print(c < GmDc.Player.Hp ? "��" : "��");
		}
		Print(" ");
		for(int c = 0; c < 6; c++)
		{
			Print(c < GmDc.BombNum ? "��" : "��");
		}
		{
			int z = GmDc.Zankisuu;
			if(0 <= z && z <= 9)
			{
				char znum[] = "�O";
				znum[1] += z;
				Print_x(xcout(" %s", znum));
			}
			else
				Print_x(xcout(" %d", GmDc.Zankisuu));
		}
		return;
	}

	// �ʏ�

	SetAlpha(0.3);
	SetBright(0, 0, 0);
	DrawPicRect(0, 0, 800, 16 * (GmVisibleSubStatus == 1 ? 3 : 1), Pic(P_WHITEBOX));
	ResetBlend();
	ResetBright();

	SetPrint(0, 0);
	Print("�ð��(��) ");
	Print_x(xcout("HP:%d ", GmDc.Player.Hp));
	Print_x(xcout("(Z:%d B:%d) ", GmDc.Zankisuu, GmDc.BombNum));
	Print_x(xcout("÷:%d(%d) ", GmDc.CubeCount + GmGetEnemyCount_Destroyable(0, 3), GmDc.EnemyCount));
	Print_x(xcout("[%02d] ", LastAttackedEnemyHP));
	Print_x(xcout("%s ", GmFrameToStamp(GmDc.NoCubesCount ? GmRc.FinishFrmCnt : LFrmCnt)));
	Print_x(xcout("Hz:%d%s", Hz, GAME_MIN_FRD <= FRDCount ? "<��>" : ""));
	PrintRet();

	if(GmVisibleSubStatus == 1) // �ʏ�+DEBUG
	{
		Print_x(xcout("X:%7.2f ", GmDc.Player.X));
		Print_x(xcout("Y:%7.2f ", GmDc.Player.Y));
		Print_x(xcout("G:%d ", GmPlayerIsDogezaNow() ? 1 : 0));
		Print_x(xcout("F:%s ", GmFrameToStamp(GmRc.FinishFrmCnt)));
		Print_x(xcout("FRD:%d(%d/%d)V:%d ", FRDCount, FRDLevel, MaxFRDLevel, VSyncFlag));
		Print_x(xcout("D:%d ", SW_DelayCount));
		Print_x(xcout("EnmL:%d EffL:%d", GmDc.EnemyList->GetCount(), GTGetEffectList()->List->GetCount()));
		PrintRet();

		Print_x(xcout("BU:%d ", GmDc.BombUsed));
		Print_x(xcout("DK:%d ", GmDc.DamageKuratta));
//		Print_x(xcout("CSI:%d ", Sd.ClearStageIndex));
		Print_x(xcout("M:%d,%d,%d(%d,%d,%d)%d(%d) ",
			callCount_memAlloc, callCount_memRealloc, callCount_memFree,
			reuseCount_memAlloc, reuseCount_memRealloc, reuseCount_memFree,
			ms_GetMemPNumMin(), ms_GetMemPNum()
			));
		Print_x(xcout("J:%d,%d ", GmDc.Player.JumpRenzokuCount, GmDc.Player.AirDashRenzokuCount));
		Print_x(xcout("DEC:%d ", GmDc.Player.DashEndDZCount));
		Print_x(xcout("MSA:%0.2f", LastMoreSnowAlpha));
		PrintRet();

		// reset
		LastMoreSnowAlpha = 0.0;
	}
#else
	PE_Border(GetColor(32, 64, 96));
	SetPrint(3, 2);

	{
		int no = GmDc.PrmStageIndex + 1;

		switch(no)
		{
		case 5:
			Print("�{�X");
			break;

		default:
			Print_x(xcout(
				"%s��"
				,c_toString(no)
				));
			break;
		}
	}

	PE_Border(GetColor(0, 0, 192));
	Print("�@�̗�");

#if 1
	{
		const int l = 90;
		const int t = 2;
		const int w = 152;
		const int h = 15;

		SetBright(0, 0, 1);
		DrawPicRect(l, t, w, h, Pic(P_WHITEBOX));
		SetBright(0, 0, 0.75);
		DrawPicRect(l + 1, t + 1, w - 2, h - 2, Pic(P_WHITEBOX));
		ResetBright();

		double hp_rate = (double)GmDc.Player.Hp / MAX_HP;
		int hp_w = (int)(hp_rate * (w - 2) + 0.5);

		if(1 <= hp_w)
			DrawPicRect(l + 1, t + 1, hp_w, h - 2, Pic(P_WHITEBOX));

		for(int c = 1; c < 10; c++)
			Print("�@");
	}
#else // old
	for(int c = 1; c < 10; c++)
	{
		if(c <= GmDc.Player.Hp)
			Print("��");
		else
			Print("�E");
	}
#endif

	{
		int z = GmDc.Zankisuu;
		int b = GmDc.BombNum;

//		m_range(z, 0, 9);
//		m_range(b, 0, 9);

		Print_x(xcout(
			"�@�c�F%s�@�{��%s"
			,c_toString(z)
			,c_toString(b)
			));
		PE_Border(GetColor(128, 0, 128));
		Print_x(xcout(
			"�@�G%s(%s)�@[%s/%s]"
			,c_toString(GmDc.CubeCount + GmGetEnemyCount_Destroyable(0, 3))
			,c_toString(GmDc.EnemyCount)
			,c_toString(LastAttackedEnemyHP)
			,c_toString(LastAttackedEnemyMaxHP)
			));
		PE_Border(GetColor(0, 192, 0));
		Print_x(xcout(
			"�@%s�@Hz:%s"
			,GmFrameToStamp(GmDc.NoCubesCount ? GmRc.FinishFrmCnt : LFrmCnt)
			,c_toString(Hz)
			));
		PE_Border(GetColor(0, 128, 128));

		{
			int jChr = RENZOKUJUMP_MAX - GmDc.Player.JumpRenzokuCount;
			int adCh = RENZOKUAIRDASH_MAX - GmDc.Player.AirDashRenzokuCount;

			if(0 <= jChr && jChr <= 9) jChr += '0'; else jChr = 'X';
			if(0 <= adCh && adCh <= 9) adCh += '0'; else adCh = 'X';

			Print_x(xcout(
				"�@<%c,%c>"
				,jChr
				,adCh
				));
		}
	}

	PE_Reset();
#endif
}
static void DrawEnemyHP(void)
{
	if(!Es.ShowEnemyHPMode)
		return;

	for(int index = 0; index < GmDc.EnemyList->GetCount(); index++)
	{
		GmEnemy_t *e = GmDc.EnemyList->ElementAt(index);

		if(GmInCamera(e) && e->HP)
		{
			char strhp[12];
			sprintf(strhp, "%d", e->HP);

			int hplen = strlen(strhp);

			const double KANKAKU = 32.0;
			double lx = e->X;
			double ly = e->Y;

			lx -= (double)(hplen - 1) * KANKAKU * 0.5;
			lx -= GmDc.CameraX;

			for(int n = 0; strhp[n]; n++)
			{
				int num = strhp[n] - '0';
				m_range(num, 0, 9);

				double x = lx + n * KANKAKU;
				double y = ly;

				m_countApprox(x, 400.0, 0.99, 0.0);
				m_countApprox(y, 300.0, 0.99, 0.0);

				int resno = (0/*D_0*/ + num) | DTP; // �p�~

				for(int c = 0; c <= 2; c++)
				{
					DrawBegin(x, y, Pic(resno));
					DrawZoom(3.5 + c * 0.5);
					SetAlpha(0.03 + 0.02 * sin(LFrmCnt / (1.7 + c * 0.3)));
					SetBright(0.6, 0.9, 0.0);
					DrawEnd();
					ResetBlend();
					ResetBright();
				}
				for(int c = 0; c <= 2; c++)
				{
					DrawBegin(x, y, Pic(resno));
					DrawZoom(2.9 + c * 0.1);
					SetBlendAdd(0.7 - c * 0.2);
					DrawEnd();
					ResetBlend();
				}
			}
		}
	}
}
static void ApproxCamera(void)
{
	m_countApprox(GmDc.CameraX, GmDc.Player.X - 400.0, 0.9, 0.5);
	m_range(GmDc.CameraX, 0.0, GmDc.FieldWidth - 800.0);
}
static void DropDownSkyHiEnemy(void)
{
	const double BORDER_Y = -100.0;
	double x1 = GmDc.CameraX;
	double x2 = GmDc.CameraX + 800.0;
	double ymax = (double)-TSLD;

#define SHE_COND(e) \
	(x1 < (e)->X && (e)->X < x2 && (e)->HP)

	for(int index = 0; index < GmDc.EnemyList->GetCount(); index++)
	{
		GmEnemy_t *e = GmDc.EnemyList->ElementAt(index);

		if(SHE_COND(e))
		{
			ymax = m_max(ymax, e->Y);
		}
	}
	double yadd = BORDER_Y - ymax;

	if(yadd < 0.0)
		return;

	int dropedDown = 0;

	for(int index = 0; index < GmDc.EnemyList->GetCount(); index++)
	{
		GmEnemy_t *e = GmDc.EnemyList->ElementAt(index);

		if(SHE_COND(e))
		{
			e->Y += yadd;
			dropedDown = 1;
		}
	}
	if(dropedDown)
		SEPlay(SE_DROPDOWNSKYHIENEMY);
#undef SHE_COND
}

int GmGetEnemyCount(void *maker, int inv_mode)
{
	int count = 0;

	for(int index = 0; index < GmDc.EnemyList->GetCount(); index++)
	{
		GmEnemy_t *i = GmDc.EnemyList->ElementAt(index);

		if(inv_mode ?
			i->Maker != maker :
			i->Maker == maker
			)
		{
			count++;
		}
	}
	return count;
}
int GmGetEnemyCount_Destroyable(int in_camera, int udrLv)
{
	int count = 0;

	for(int index = 0; index < GmDc.EnemyList->GetCount(); index++)
	{
		GmEnemy_t *i = GmDc.EnemyList->ElementAt(index);

		if(i->HP != 0) // ? ���G����Ȃ��B
		{
			if(!in_camera || GmInCamera(i))
			{
				if(udrLv == -1 || udrLv <= i->Lv)
				{
					count++;
				}
			}
		}
	}
	return count;
}
char *GmFrameToStamp(int frmcnt)
{
	errorCase(frmcnt < 0);

	static char stamp[9];

	int tm_m = frmcnt / 3600;
	int tm_s = (frmcnt / 60) % 60;
	int tm_cs = ((frmcnt % 60) * 100) / 60;

	if(tm_m < 100)
		sprintf(stamp, "%02d:%02d.%02d", tm_m, tm_s, tm_cs);
	else
		strcpy(stamp, "**:**.**");

	return stamp;
}

static double Nekomachi_A;
static void DrawPauseWall(void)
{
	m_countApprox(Nekomachi_A, 0.7, 0.9, 0.0);

	DrawWall();

	SetAlpha(Nekomachi_A);
	SimpleDrawPic(0, 0, Pic(P_PAUSEWALL));
	ResetBlend();

	GTEarlyCurtain();
}
static void ExtraSettings(void)
{
	GTFreezeInput();

	const int selectMax = 7;
	int selectIndex = 0;

	for(; ; )
	{
		if(GTGetInput(INP_DIR_8, INP_HIT))
		{
			selectIndex && selectIndex--;
		}
		if(GTGetInput(INP_DIR_2, INP_HIT))
		{
			selectIndex < selectMax - 1 && selectIndex++;
		}

		switch(selectIndex)
		{
		case 0:
			{
				if(GTGetInput(INP_A, INP_DOWN)) Es.VisibleScope = Es.VisibleScope ? 0 : 1;
			}
			break;

		case 1:
			{
				if(GTGetInput(INP_DIR_4, INP_HIT)) Es.AttackFukaFrame--;
				if(GTGetInput(INP_DIR_6, INP_HIT)) Es.AttackFukaFrame++;
			}
			break;

		case 2:
			{
				if(GTGetInput(INP_A, INP_DOWN)) Es.No_AttackAndTransOff = Es.No_AttackAndTransOff ? 0 : 1;
			}
			break;

		case 3:
			{
				if(GTGetInput(INP_DIR_4, INP_HIT)) Es.AttackYoyakuMargin--;
				if(GTGetInput(INP_DIR_6, INP_HIT)) Es.AttackYoyakuMargin++;
			}
			break;

		case 4:
			{
				if(GTGetInput(INP_A, INP_DOWN)) Es.HitBackTrueDir = Es.HitBackTrueDir ? 0 : 1;
			}
			break;

		case 5:
			{
				if(GTGetInput(INP_A, INP_DOWN)) EsInit();
			}
			break;

		case 6:
			{
				if(GTGetInput(INP_A, INP_DOWN)) goto endMenu;
			}
			break;

		default:
			error();
		}

		m_range(Es.AttackFukaFrame, ES_AFF_MIN, ES_AFF_MAX);
		m_range(Es.AttackYoyakuMargin, ES_AYM_MIN, ES_AYM_MAX);

		if(GTGetInput(INP_B, INP_DOWN))
		{
			if(selectIndex == selectMax - 1)
			{
				break;
			}
			selectIndex = selectMax - 1;
		}
		if(GTGetInput(INP_PAUSE, INP_DOWN))
		{
			break;
		}
		DrawPauseWall();

		int sicnt = 0;

		PE_Border(GetColor(64, 64, 64));
		SetPrint(50, 50, 40);
		Print("---- �V�X�e���̒��� ----");
		PrintRet();
		PrintRet();
		Print_x(xcout("[%c] �����蔻��̕\�� = %s", selectIndex == sicnt++ ? '>' : ' ', Es.VisibleScope ? "�\������" : "�\�����Ȃ�"));
		PrintRet();
		Print_x(xcout("[%c] �U���s�t���[���� = %d (%d-%d)", selectIndex == sicnt++ ? '>' : ' ', Es.AttackFukaFrame, ES_AFF_MIN, ES_AFF_MAX));
		PrintRet();
		Print_x(xcout("[%c] ���G�������� = %s", selectIndex == sicnt++ ? '>' : ' ', Es.No_AttackAndTransOff ? "�U������̔���" : "�U���̊J�n"));
		PrintRet();
		Print_x(xcout("[%c] �U���\��}�[�W�� = %d (%d-%d)", selectIndex == sicnt++ ? '>' : ' ', Es.AttackYoyakuMargin, ES_AYM_MIN, ES_AYM_MAX));
		PrintRet();
		Print_x(xcout("[%c] �q�b�g�o�b�N���� = %s", selectIndex == sicnt++ ? '>' : ' ', Es.HitBackTrueDir ? "�ڐG�ΏۂƔ��΂̕���" : "�i�s�����̋t"));
		PrintRet();
		Print_x(xcout("[%c] �f�t�H���g�ݒ�𕜌�����", selectIndex == sicnt++ ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c] �߂�", selectIndex == sicnt++ ? '>' : ' '));
		PE_Reset();

		GTEndFrame();
	}
endMenu:
	GTFreezeInput();
}
#if 0 // �p�~
static void ResourceControl(void)
{
	GTFreezeInput();

	const int selectMax = 3;
	int selectIndex = 0;

	for(; ; )
	{
		if(GTGetInput(INP_DIR_8, INP_HIT))
		{
			selectIndex && selectIndex--;
		}
		if(GTGetInput(INP_DIR_2, INP_HIT))
		{
			selectIndex < selectMax - 1 && selectIndex++;
		}
		if(GTGetInput(INP_A, INP_DOWN))
		{
			switch(selectIndex)
			{
			case 0:
				ReleaseResources();
				break;

			case 1:
				TouchResources();
				break;

			case 2:
				goto endMenu;

			default:
				error();
			}
		}
		if(GTGetInput(INP_B, INP_DOWN))
		{
			if(selectIndex == selectMax - 1)
			{
				break;
			}
			selectIndex = selectMax - 1;
		}
		if(GTGetInput(INP_PAUSE, INP_DOWN))
		{
			break;
		}
		DrawPauseWall();

		int sicnt = 0;

		clsDx();
		printfDx("\n");
		printfDx("�@���\�[�X�̑���(��)\n");
		printfDx("\n");
		printfDx("\n");
		printfDx("�@%c ReleaseResources();\n", selectIndex == sicnt++ ? '>' : ' ');
		printfDx("\n");
		printfDx("�@%c TouchResources();\n", selectIndex == sicnt++ ? '>' : ' ');
		printfDx("\n");
		printfDx("�@%c �߂�\n", selectIndex == sicnt++ ? '>' : ' ');
		printfDx("\n");
		printfDx("\n");
		printfDx("\n");

		switch(selectIndex)
		{
		case 0:
			printfDx("�@** ��������̃��\�[�X��S�ĊJ�����܂��B\n");
			printfDx("\n");
			printfDx("�@** �󂫃��������N�����Ă���ꍇ�A�s�v�ȃ��\�[�X��ǂ��o�����ʂ����҂ł��܂��B\n");
			printfDx("\n");
			printfDx("�@** �A���A�K�v�ȃ��\�[�X�͍ēǂݍ��݂��邱�ƂɂȂ�܂��B\n");
			break;

		case 1:
			printfDx("�@** ���\�[�X���قڑS�ēǂݍ��݂܂��B\n");
			printfDx("\n");
			printfDx("�@** ��C�Ƀ������������̂Ń������ɗ]�T������ꍇ(1GB���x)�������s���ĉ������B\n");
			printfDx("\n");
			printfDx("�@** ����ȍ~���\�[�X�̓ǂݍ��݂͌������܂��B\n");
			break;

		default:
			break;
		}
		GTEndFrame();
	}
endMenu:
	GTFreezeInput();
}
#endif
void PauseGame(void)
{
	int bk_FRDLevel = FRDLevel;
	int bk_FRDCount = FRDCount;

	Nekomachi_A = 0.0;

	GTFreezeInput();
	GTSetCurtain(-0.7, 30);

	int selectMax = 5;
	int selectIndex = 0;

	if(!Sd.SysTuneEnabled) selectMax--;

	for(; ; )
	{
		// �R�i�~�R�}���h (��㉺�����E���EDCDCE)
		{
			int keysq[] =
			{
				INP_DIR_8,
				INP_DIR_8,
				INP_DIR_2,
				INP_DIR_2,
				INP_DIR_4,
				INP_DIR_6,
				INP_DIR_4,
				INP_DIR_6,
				INP_D,
				INP_C,
				INP_D,
				INP_C,
				INP_E,
			};
			static int keyidx;

			if(GTGetInput(keysq[keyidx], INP_DOWN))
			{
				keyidx++;

				if(lengthof(keysq) <= keyidx) // ? �R�}���h���͊���
				{
					SEPlay(SE_DEATH_BOSS_0001);

//					GmDc.Player.Hp = 99;
					GmDc.Zankisuu = 99;
					GmDc.BombNum = 99;

//					*Get_P_ClearStageIndex() = STAGE_NUM; // �S�X�e�[�W�N���A

					GmDc.BombUsed = 1; // �{�����g�p�������ɂ���B-> �^�C���X�V�j�~
					GmDc.BombUsed_573Cmd = 1;

					keyidx = 0;
				}
			}
			else
			{
				for(int index = 0; index < lengthof(keysq); index++)
				{
					if(GTGetInput(keysq[index], INP_DOWN))
					{
						keyidx = 0;
					}
				}
			}
		}

		if(GTGetInput(INP_DIR_8, INP_HIT))
		{
			selectIndex && selectIndex--;
		}
		if(GTGetInput(INP_DIR_2, INP_HIT))
		{
			selectIndex < selectMax - 1 && selectIndex++;
		}
		if(GTGetInput(INP_A, INP_DOWN))
		{
			switch(selectIndex)
			{
			case 0:
				goto endMenu;

			case 1:
				GmRc.GameEndReason = GER_USER_RETRY;
				goto endMenu;

			case 2:
				GmRc.GameEndReason = GER_USER_ABORT;
				goto endMenu;

			case 3:
				EndProc();

			case 4:
				ExtraSettings();
				break;

			default:
				error();
			}
		}
		if(GTGetInput(INP_B, INP_DOWN))
		{
			/*
			if(selectIndex == selectMax - 1)
			{
				break;
			}
			selectIndex = selectMax - 1;
			*/
			break;
		}
		if(GTGetInput(INP_PAUSE, INP_DOWN))
		{
			break;
		}
		DrawPauseWall();

		int sicnt = 0;

		PE_Border(GetColor(64, 64, 64));
		SetPrint(50, 50, 40);
		Print("---- PAUSE ----");
		PrintRet();
		PrintRet();
		Print_x(xcout("[%c] �Q�[���ɖ߂�", selectIndex == sicnt++ ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c] �J�n�X�e�[�W�����蒼��", selectIndex == sicnt++ ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c] �^�C�g���ɖ߂�", selectIndex == sicnt++ ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c] �Q�[�����I������", selectIndex == sicnt++ ? '>' : ' '));
		PrintRet();

		if(Sd.SysTuneEnabled)
		{
			Print_x(xcout("[%c] �V�X�e���̒���", selectIndex == sicnt++ ? '>' : ' '));
			PE_Reset();
		}
		GTEndFrame();
	}
endMenu:
	clsDx();
	GTSetCurtain(0.0, 30);

	FRDLevel = bk_FRDLevel;
	FRDCount = bk_FRDCount;
}
/*
	ret: ? �Q�[�����p������B(�Q�[���𒆒f����(�ߒl0)�̏ꍇ MusicStop(); �����܂�)
*/
static int PauseFrame(void)
{
	if(GTGetInput(INP_PAUSE, INP_DOWN))
	{
		MusicStop();
		SEStopEcho(); // ���S���Ƀ|�[�Y�����Ƃ��΍�
		SEPlay(SE_PAUSE);

		PauseGame();

		if(GmRc.GameEndReason != GER_NONE)
			return 0;

		MusicPlay(GmDc.MusicResno, 1, 0);
	}
	return 1;
}

// ---- StageBegin ----

static int SB_No;

static void SB_DrawWall(void)
{
	SetBright(0, 0, 0);
	DrawPicRect(0, 0, 800, 600, Pic(P_DUMMY));
	ResetBright();
}
static void SB_DrawText(void)
{
	PE_Reset(); // �ǂ�����PE_Reset�Y��Ă�B

//	PE_Border(GetColor(128, 128, 128));
	SetPrint(400 - 80, 300 - 8);

	switch(SB_No)
	{
	case 5:
		Print("�{�@�X");
		break;

	default:
		Print_x(xcout("%s�@��", c_toString(SB_No)));
		break;
	}
//	PE_Reset();
}
static void StageBegin(int no)
{
	const int FRMCNT = 15;

	SB_No = no;

	const int KUMA_X = 550;
	const int KUMA_Y = 335;

	forscene(FRMCNT * 3)
	{
		if(fs_count == 10) // �d�߂̃��[�h����..
		{
			TouchResources();

			switch(Sd_PlayChara)
			{
			case PLAY_CHARA_00: SetPictureGroup(S_Kuma00PicGrp()); break;
			case PLAY_CHARA_01: SetPictureGroup(S_Kuma01PicGrp()); break;
			case PLAY_CHARA_02: SetPictureGroup(S_Kuma02PicGrp()); break;
			case PLAY_CHARA_03: SetPictureGroup(S_Kuma03PicGrp()); break;

			default:
				error();
			}
		}

		SB_DrawWall();
		SB_DrawText();
		GTEndFrame();
	}
	for(int c = 0; c < 7; c++)
	{
		forscene(FRMCNT)
		{
			SB_DrawWall();
			SB_DrawText();

			if(c % 2 == 0 || c == 1)
			{
				DrawBegin(KUMA_X, KUMA_Y, Pic(P_STAND_00));
			}
			else
			{
				DrawBegin(KUMA_X + 10, KUMA_Y, Pic(P_STAND_00));
				DrawXZoom(-1.0);
			}
			DrawEnd();
			GTEndFrame();
		}
	}
	forscene(FRMCNT * 3)
	{
		SB_DrawWall();
		SB_DrawText();

		SetPrint(KUMA_X - 8, KUMA_Y - 80);

		switch(SB_No)
		{
		case 5:
			Print("�I");
			break;

		default:
			Print("�H");
			break;
		}

		DrawBegin(KUMA_X + 10, KUMA_Y, Pic(P_STAND_00));
		DrawXZoom(-1.0);
		DrawEnd();
		GTEndFrame();
	}
	SetPictureGroup(NULL);
}

// ----

void GmMain(void)
{
	if(GmDc.PrmStageBeginFlag)
	{
//		TouchResources(); // -> StageBegin()

		GTSetCurtain(0.0);
		StageBegin(GmDc.PrmStageIndex + 1);

		MusicPlay(GmDc.MusicResno);
		GTSetCurtain(-1.0, 0.0, 30);
	}
	else // �]���ʂ�
	{
		TouchResources();

		MusicPlay(GmDc.MusicResno);
		GTSetCurtain(0.0, 30);
	}
	LFrmCnt = 0;
	FRDCount = 0;

	// �p�~
	/*
	GmDc.FieldWidth = 0.0;
	for(int index = 0; index < GmDc.WallPicResnoList->GetCount(); index++)
	{
		CheckPicSize(Pic(GmDc.WallPicResnoList->GetElement(index)));
		GmDc.FieldWidth += Pic_W;
	}
	*/
//	errorCase(GmDc.FieldWidth != GmDc.PrmStageWidth); // test
	GmDc.FieldWidth = GmDc.PrmStageWidth;

	for(; ; )
	{
		int wprcnt = GmDc.WallPicResnoList->GetCount();
		int wSum = 0;

		for(int index = 0; index < wprcnt; index++)
		{
			CheckPicSize(Pic(GmDc.WallPicResnoList->GetElement(index)));
			wSum += Pic_W;
		}
		if(GmDc.FieldWidth <= wSum)
			break;

		for(int index = 0; index < wprcnt; index++)
		{
			GmDc.WallPicResnoList->AddElement(GmDc.WallPicResnoList->GetElement(index));
		}
	}

	GmDc.Player.X = GmDc.PlayerStartX;
	GmDc.Player.Y = JIMEN_Y;
	GmDc.Player.Hp = MAX_HP;

	GmDc.CameraX = GmDc.Player.X - 400.0;

	for(; ; )
	{
		SW_Start();

		// ---- ���� ----

		GmDc.CubeCount = GmGetEnemyCount(MakeCube, 0);
		GmDc.EnemyCount = GmGetEnemyCount_Destroyable();

		if(GmGetEnemyCount_Destroyable(1) == 0 && GmPlayerIsDogezaNow()) // ���̓G�������o��
		{
			DropDownSkyHiEnemy();
		}

		// ? ����S�j�󂵁ABOSS�����Ȃ��BLv4�����Ȃ��B
		if(
			GmDc.CubeCount == 0 &&
			GmGetEnemyCount(MAKE_BOSS) == 0 &&
			GmGetEnemyCount_Destroyable(0, 3) == 0
			)
		{
			if(!GmDc.NoCubesCount)
			{
				GmRc.FinishFrmCnt = LFrmCnt;
			}
			GmDc.NoCubesCount += GmFindEnemy_InCamera(0.0, MakeMapItem) ? 1 : 10;

			if(600 < GmDc.NoCubesCount)
			{
				GmRc.GameEndReason = GER_CLEAR;
				break;
			}
		}

		if(GmDc.BombDZCount == 60) // ? Bomb �I��
		{
			GmDc.BombDZCount = 0;
		}
		else if(GmDc.BombDZCount) // ? Bomb ��
		{
			GmDc.BombDZCount++;
		}
		else if(GTGetInput(INP_E, INP_DOWN) && m_countDown(GmDc.BombNum)) // ? Bomb �J�n
		{
			GmDc.BombUsed = 1;
			GmDc.BombDZCount = 1;
			GmDc.BombX = GmDc.Player.X;
			GmDc.BombY = GmDc.Player.Y;
			SEPlay(SE_BOMB);
		}

		// �f�o�b�O�p!?
		{
			if(GetKeyDown(KEY_INPUT_D))
			{
				GmVisibleSubStatus = (GmVisibleSubStatus + 1) % GM_VSS_MAX;
			}
		}

		SW_Lap();
		GmActPlayer();
		SW_Lap();
		GmActEnemy();
		SW_Lap();

		GmAttackAtariHanteiFrame();
		SW_Lap();
		GmDamageAtariHanteiFrame();
		SW_Lap();

		if(GmRc.GameEndReason == GER_DEATH)
			break;

		ApproxCamera();

		GTCameraX = GmDc.CameraX;
		GTCameraY = 0.0;

		if(!PauseFrame())
			break;

		/*
			������ Lv 3 ���Ō�Ɏ����Ă���B
			�}�X�^�[�X�p�[�N�̏�ɑ��̓G��e���㏑������Ă���ƊԔ����Ȃ̂ŁA�Ō�ɕ`�悳�������B
		*/
#if 1
		if(2 <= GmDc.EnemyList->GetCount())
		{
			int index = LFrmCnt % (GmDc.EnemyList->GetCount() - 1);
			GmEnemy_t *i = GmDc.EnemyList->ElementAt(index);

			if(i->Maker == MakeMarisa && i->Lv == 3)
			{
				GmDc.EnemyList->Swap(index, GmDc.EnemyList->GetCount() - 1); // ! 2�C�ȏ�͑z�肵�Ȃ��B
			}
		}
#else
		for(int index = 0; index < GmDc.EnemyList->GetCount() - 1; index++)
		{
			GmEnemy_t *i = GmDc.EnemyList->ElementAt(index);

			if(i->Maker == MakeMarisa && i->Lv == 3)
			{
				GmDc.EnemyList->Swap(index, GmDc.EnemyList->GetCount() - 1); // ! 2�C�ȏ�͑z�肵�Ȃ��B
				break;
			}
		}
#endif

#if 0
		if(GetKeyDown(KEY_INPUT_T)) // test
		{
			SW_Export();
		}
#endif
		
		// ---- �`�� ----

		GmDc.BubbleBackDrawRemain = 12;

		SW_Lap();
		DrawWall();
		SW_Lap();
		ExecuteTaskGroup(S_WallEffectList());
		SW_Lap();
		GmDrawPlayerKage();
		SW_Lap();
		GmDrawPlayer();
		SW_Lap();
		GmDrawEnemy();
		SW_Lap();
		DrawBomb();
		SW_Lap();
		DrawStatus();
		SW_Lap();
		DrawEnemyHP();
		SW_Lap();
		GmScopeFrame();
		SW_Lap();

		if(GmDc.SnowMode && Es.MoreSnow)
		{
			DrawSnow(0);
			SW_Lap();
		}
		GTEndFrame();
	}

endGame:
	GTFreezeInput();

	// ���r���[�ŏI�������Ƃ��΍�
	{
		ScreenSlip_X = 0;
		ScreenSlip_Y = 0;
	}
	clsDx(); // 2bs?

	switch(GmRc.GameEndReason)
	{
	case GER_CLEAR:
		{
			char *message = "";

			// �x�X�g�^�C���X�V
			{
				int nobomb = GmDc.BombUsed ? 0 : 1;
				int wipedout = GmDc.EnemyCount == 0 ? 1 : 0;
				int nodamage = GmDc.DamageKuratta ? 0 : 1;

				// BFC <= BFC_WO <= BFC_WO_ND

				int bfc = GmDc.BestTimeFrmCnt;

				if(nobomb)
				{
					bfc = GmDc.BestTimeFrmCnt_NB;

					if(wipedout)
					{
						bfc = GmDc.BestTimeFrmCnt_NBWipedOut;

						if(nodamage)
						{
							bfc = GmDc.BestTimeFrmCnt_NBWipedOut_NoDamage;
						}
					}
				}

				// ? �x�X�g�^�C���X�V
				if(GmRc.FinishFrmCnt < bfc)
				{
					SEPlay(SE_NEWRECORD);

					message = "<<<< �N���A�^�C���X�V >>>>";

					if(nobomb)
					{
						message = "<<<< �N���A�^�C���X�V(�m�[�{��) >>>>";

						if(wipedout)
						{
							message = "<<<< �N���A�^�C���X�V(�m�[�{���{�r��) >>>>";

							if(nodamage)
							{
								message = "<<<< �N���A�^�C���X�V(�m�[�{���{�r�Ł{�m�[�_���[�W) >>>>";

								GmDc.BestTimeFrmCnt_NBWipedOut_NoDamage = GmRc.FinishFrmCnt;
								GmDc.BestTimeFrmCnt_NBWipedOut = m_min(GmDc.BestTimeFrmCnt_NBWipedOut, GmRc.FinishFrmCnt);
								GmDc.BestTimeFrmCnt_NB = m_min(GmDc.BestTimeFrmCnt_NB, GmRc.FinishFrmCnt);
								GmDc.BestTimeFrmCnt = m_min(GmDc.BestTimeFrmCnt, GmRc.FinishFrmCnt);
							}
							else
							{
								GmDc.BestTimeFrmCnt_NBWipedOut = GmRc.FinishFrmCnt;
								GmDc.BestTimeFrmCnt_NB = m_min(GmDc.BestTimeFrmCnt_NB, GmRc.FinishFrmCnt);
								GmDc.BestTimeFrmCnt = m_min(GmDc.BestTimeFrmCnt, GmRc.FinishFrmCnt);
							}
						}
						else
						{
							GmDc.BestTimeFrmCnt_NB = GmRc.FinishFrmCnt;
							GmDc.BestTimeFrmCnt = m_min(GmDc.BestTimeFrmCnt, GmRc.FinishFrmCnt);
						}
					}
					else
					{
						GmDc.BestTimeFrmCnt = GmRc.FinishFrmCnt;
					}
				}
			}
			forscene(120)
			{
				GmActPlayer();
				ApproxCamera();

				DrawWall();
				GTDrawCurtain(-fs_rate);
				GmDrawPlayer();

				PE_Border(GetColor(128, 128, 0));
				SetPrint(3, 2);
				Print_x(xcout("%s", message));
				PE_Reset();

				GTEndFrame();
			}
			if(!GmDc.ClearAndKeepMusicMode)
			{
				MusicFadeout(30);
			}
			GTSetCurtain(-1.0, 30);

			while(GTActWhiteLevel != -1.0)
			{
				GTFreezeInput();
				GmActPlayer();
				ApproxCamera();

				GTDrawBlackWall();
				GmDrawPlayer(1);

				GTEndFrame();
			}
		}
		break;

	case GER_DEATH:
		{
			forscene(90)
			{
				GmActPlayer(1);
				ApproxCamera();

				if(!PauseFrame())
					goto endGame;

				DrawWall();
				GTDrawCurtain(-fs_rate);
				GmDrawPlayer(1);

				GTEndFrame();
			}
			if(!GmDc.DeathAndKeepMusicMode)
			{
				MusicFadeout(30);
			}
			GTSetCurtain(-1.0, 30);

			while(GTActWhiteLevel != -1.0)
			{
				GmActPlayer(1);
				ApproxCamera();

				/*
				if(!PauseFrame())
					goto endGame;
				*/

				GTDrawBlackWall();
				GmDrawPlayer(1);

				GTEndFrame();
			}
		}
		break;

	case GER_USER_ABORT:
	case GER_USER_RETRY:
		{
			MusicFadeout(30); // �ꎞ��~��ʂ��痈��̂Ŏ~�܂��Ă�͂��B2bs
			GTSetCurtain(-1.0, 30);

			while(GTActWhiteLevel != -1.0)
			{
				DrawWall();
				GTEndFrame();
			}
		}
		break;

	default:
		error();
	}
	GmRc.EndZankisuu = GmDc.Zankisuu;
	GmRc.EndBombNum = GmDc.BombNum;

	GmRc.NewBestTimeFrmCnt = GmDc.BestTimeFrmCnt;
	GmRc.NewBestTimeFrmCnt_NB = GmDc.BestTimeFrmCnt_NB;
	GmRc.NewBestTimeFrmCnt_NBWipedOut = GmDc.BestTimeFrmCnt_NBWipedOut;
	GmRc.NewBestTimeFrmCnt_NBWipedOut_NoDamage = GmDc.BestTimeFrmCnt_NBWipedOut_NoDamage;

	clsDx(); // 2bs?

	ClearTaskGroup(S_WallEffectList());
}

int GetMaxHp(void)
{
	switch(Sd_PlayChara)
	{
	case PLAY_CHARA_00: return 10;
	case PLAY_CHARA_01: return 10;
	case PLAY_CHARA_02: return 20;
	case PLAY_CHARA_03: return 5;

	default:
		error();
	}
	error();
	return -1; // dummy
}
