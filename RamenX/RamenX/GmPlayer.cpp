#include "Stdinc.h"

static void CounterInc(int &count, int maxcnt, int inc_cyc)
{
	if(count)
	{
		LFrmCnt % inc_cyc == 0 && count++;

		if(maxcnt <= count)
		{
			count = 0;
		}
	}
}

static void AttackCancel(void)
{
	GmDc.Player.UAttackDZCount = 0;
	GmDc.Player.CAttackDZCount = 0;
	GmDc.Player.DAttackDZCount = 0;
}
static int GetAttackDZCount(void)
{
	int maxcnt = 0;

	maxcnt = m_max(maxcnt, GmDc.Player.UAttackDZCount);
	maxcnt = m_max(maxcnt, GmDc.Player.CAttackDZCount);
	maxcnt = m_max(maxcnt, GmDc.Player.DAttackDZCount);

	return maxcnt;
}
static int IsAttackNow(void)
{
	return
		GmDc.Player.UAttackDZCount ||
		GmDc.Player.CAttackDZCount ||
		GmDc.Player.DAttackDZCount;
}

// 攻撃モーションを中断できる最初の count
#define U_ATTACK_END 6
#define C_ATTACK_END 8
#define D_ATTACK_END 8

static int IsAttackCancellable_MARGIN(const int MARGIN)
{
	return
		U_ATTACK_END <= GmDc.Player.UAttackDZCount + MARGIN ||
		C_ATTACK_END <= GmDc.Player.CAttackDZCount + MARGIN ||
		D_ATTACK_END <= GmDc.Player.DAttackDZCount + MARGIN;
}
static int IsAttackCancellable(void)
{
	return IsAttackCancellable_MARGIN(0);
}
enum
{
	ATTACK_NONE, // 0-default member

	ATTACK_U,
	ATTACK_C,
	ATTACK_D,
};

#define PLAYER_GRAVITY_YADD2 0.7

int GmPlayerIsAttackNow(void)
{
	return IsAttackNow();
}
void GmActPlayer(int death_mode)
{
	m_countDown(GmDc.Player.TransDZCount);

	errorCase(death_mode && GmDc.Player.HitBackDZCount == 0); // 死んでるならヒットバックしてるはず。

	if(GmDc.Player.HitBackDZCount)
	{
		GmDc.Player.HitBackFrmCnt++;
//		GmDc.Player.HitBackFrmCnt % 6 == 0 && GmDc.Player.HitBackDZCount++; // 6 * 4 == 24 == 0.4 秒
		GmDc.Player.HitBackFrmCnt % 7 == 0 && GmDc.Player.HitBackDZCount++; // 7 * 4 == 28 == 0.5 秒弱
//		GmDc.Player.HitBackFrmCnt % 8 == 0 && GmDc.Player.HitBackDZCount++; // 8 * 4 == 32 == 0.5 秒強

		if(GmHitMax < GmDc.Player.HitBackDZCount)
		{
			if(!death_mode)
			{
				GmDc.Player.HitBackDZCount = 0;
				GmDc.Player.HitBackFrmCnt = 0;
				goto hit_back_end;
			}
			GmDc.Player.HitBackDZCount = GmHitMax;
		}
		GmDc.Player.X += GmDc.Player.HitBackXAdd;
		m_countApprox(GmDc.Player.HitBackXAdd, 0.0, death_mode ? 0.97 : 0.99, 0.0);

		GmDc.Player.Y += GmDc.Player.YAdd;
		GmDc.Player.YAdd += PLAYER_GRAVITY_YADD2;

		m_range(GmDc.Player.X, 0.0, (double)GmDc.FieldWidth);

		if(JIMEN_Y < GmDc.Player.Y)
		{
			GmDc.Player.Y = JIMEN_Y;
			GmDc.Player.YAdd *= -0.5;

			if(abs(GmDc.Player.YAdd) < 1.3) // 細かく跳ね続けないように...
			{
				GmDc.Player.YAdd = 0.0;
			}
		}
		return;
	}
hit_back_end:

	if(GmDc.Player.AirDashDZCount) // エアダッシュ中
	{
		if(!GTGetInput(INP_B, INP_PRESS)) // ? ダッシュボタンを離した。
		{
			goto air_dash_end;
		}
		if(GTGetInput(INP_C, INP_PRESS) || GTGetInput(INP_D, INP_PRESS)) // 攻撃ボタンを優先する。
		{
			goto air_dash_end;
		}
		const double ADC_MARGIN_X = 10.0; // Air-Dash Cancel MARGIN X

		if(GmDc.Player.X < -ADC_MARGIN_X || GmDc.FieldWidth + ADC_MARGIN_X < GmDc.Player.X)
		{
			goto air_dash_end;
		}
		if(GmDc.Player.Y < AIRDASH_Y_MAX || AIRDASH_Y_MIN < GmDc.Player.Y)
		{
			goto air_dash_end;
		}
air_dash:
		GmDc.Player.AirDashDZCount++;

		double ad_rot = GmDc.Player.AirDashRotate;
		int lrot = 0;
		int rrot = 0;

		if(ad_rot < 0.0)
		{
			ad_rot += PI * 2.0;
		}
		else if(PI * 2.0 < ad_rot)
		{
			ad_rot -= PI * 2.0;
		}
		int dir2 = GTGetInput(INP_DIR_2, INP_PRESS);
		int dir4 = GTGetInput(INP_DIR_4, INP_PRESS);
		int dir6 = GTGetInput(INP_DIR_6, INP_PRESS);
		int dir8 = GTGetInput(INP_DIR_8, INP_PRESS);

		if(ad_rot < (PI / 4.0) * 1.0 || (PI / 4.0) * 7.0 < ad_rot) // ? 上を向いている。
		{
			lrot = dir4;
			rrot = dir6;
		}
		else if(ad_rot < (PI / 4.0) * 3.0) // ? 右を向いている。
		{
			lrot = dir8;
			rrot = dir2;
		}
		else if(ad_rot < (PI / 4.0) * 5.0) // ? 下を向いている。
		{
			lrot = dir6;
			rrot = dir4;
		}
		else // ? 左を向いている。
		{
			lrot = dir2;
			rrot = dir8;
		}
		int rotcnt = GmDc.Player.AirDashRotateCount;
		m_range(rotcnt, 0, 30);

		double rotSpeed = (double)rotcnt * 0.002;

		if(Sd_PlayChara == PLAY_CHARA_03)
			rotSpeed *= 2.0;

		if(lrot || rrot)
			GmDc.Player.AirDashRotateCount++;
		else
			GmDc.Player.AirDashRotateCount = 0;

		if(lrot)
		{
			ad_rot -= rotSpeed;
		}
		if(rrot)
		{
			ad_rot += rotSpeed;
		}
		double speed = 6.0;

		if(Sd_PlayChara == PLAY_CHARA_03)
			speed *= 3.0;

		double x_add = speed * -sin(ad_rot + PI);
		double y_add = speed *  cos(ad_rot + PI);

		GmDc.Player.X += x_add;
		GmDc.Player.Y += y_add;

		GmDc.Player.AirDashRotate = ad_rot;

		GmDc.Player.AirDashLastXAdd = x_add;
		GmDc.Player.AirDashLastYAdd = y_add;
		return;
	}
air_dash_end:

	if(GmDc.Player.AirDashDZCount) // ? エアダッシュ解除
	{
		GmDc.Player.YAdd = m_min(0.0, GmDc.Player.AirDashLastYAdd * 1.5); // エアダッシュの上方向の慣性
		GmDc.Player.DashDZCount = 1; // ダッシュ有効
		GmDc.Player.JumpDZCount = TSLD / 2; // 少なくともジャンプし始めではなくする。
		GmDc.Player.JumpDushDownDZCount = 0;
		GmDc.Player.AirDashDZCount = 0;
		GmDc.Player.AirDashEndDZCount = GmAirDashEndMax;

		if(Sd_PlayChara == PLAY_CHARA_03)
			GmDc.Player.YAdd *= 0.25;
	}
	int gezamode;
	int gezacycle = 4;

	if(GmDc.Player.GezaDZCount <= GmBackDogezaMax)
	{
		gezacycle = Sd_PlayChara == PLAY_CHARA_02 || Sd_PlayChara == PLAY_CHARA_03 ? 1 : 3;
	}

	if(GTGetInput(INP_DIR_2, INP_PRESS))
	{
		gezamode = 1;
		LFrmCnt % gezacycle == 0 && GmDc.Player.GezaDZCount++;

		if(Sd_PlayChara == PLAY_CHARA_01)
			m_range(GmDc.Player.GezaDZCount, GmBackDogezaMax, TSLD);
	}
	else
	{
		gezamode = 0;
		LFrmCnt % gezacycle == 0 && GmDc.Player.GezaDZCount--;
		m_range(GmDc.Player.GezaDZCount, 0, GmBackDogezaMax);

		if(Sd_PlayChara == PLAY_CHARA_01)
			GmDc.Player.GezaDZCount = 0;
	}

	if(Sd_PlayChara == PLAY_CHARA_02 || Sd_PlayChara == PLAY_CHARA_03)
	{
		CounterInc(GmDc.Player.UAttackDZCount, GmUAttackMax, 1);
		CounterInc(GmDc.Player.CAttackDZCount, GmCAttackMax, 1);
		CounterInc(GmDc.Player.DAttackDZCount, GmDAttackMax, 1);
	}
	else
	{
		CounterInc(GmDc.Player.UAttackDZCount, GmUAttackMax, 1);
		CounterInc(GmDc.Player.CAttackDZCount, GmCAttackMax, 3);
		CounterInc(GmDc.Player.DAttackDZCount, GmDAttackMax, 2);
	}

	if(!gezamode)
	{
		int reqAttack = GmDc.Player.YoyakuReqAttack;
//		int reqAttack = ATTACK_NONE;

		GmDc.Player.YoyakuReqAttack = ATTACK_NONE;

		int kk_input_mode = INP_DOWN;

		// 連打er
		if(
			Sd_PlayChara == PLAY_CHARA_02 ||
			Sd_PlayChara == PLAY_CHARA_03
			)
			kk_input_mode = INP_PRESS;

		if(GTGetInput(INP_C, kk_input_mode)) // ? 弱攻撃
		{
			if(GTGetInput(INP_DIR_8, INP_PRESS))
			{
				reqAttack = ATTACK_U;
			}
			else
			{
				reqAttack = ATTACK_D;
			}
		}
		else if(GTGetInput(INP_D, kk_input_mode)) // ? 強攻撃
		{
			reqAttack = ATTACK_C;
		}

		if(reqAttack) // ゴリ押し防止 [GB]
		{
			if(GmDc.Player.TransDZCount < Es.AttackFukaFrame)
//			if(GmDc.Player.TransDZCount < TRANS_FRAME / 2)
			{
				if(!Es.No_AttackAndTransOff)
					GmDc.Player.TransDZCount = 0; // 半透明強制終了

//				GmDc.Player.TransDZCount = 0; // 半透明強制終了 -> 攻撃判定時に行う。(OLD)
			}
			else
			{
				reqAttack = ATTACK_NONE; // 攻撃禁止
			}
		}

		if(reqAttack) // ? 攻撃開始要求
		{
			if(!IsAttackNow() || IsAttackCancellable()) // 攻撃開始
			{
				AttackCancel();

				switch(reqAttack)
				{
				case ATTACK_U:
					GmDc.Player.UAttackDZCount = 1;
					SEPlay(SE_ATTACK_02);
					break;

				case ATTACK_C:
					GmDc.Player.CAttackDZCount = 1;
					SEPlay(SE_ATTACK_HEAVY);
					break;

				case ATTACK_D:
					GmDc.Player.DAttackDZCount = 1;
					SEPlay(SE_ATTACK_01);
					break;

				default:
					error();
				}
				GmDc.Player.AttackLeftMode = GmDc.Player.LeftMode;
			}
			else // 攻撃予約
			{
				if(IsAttackCancellable_MARGIN(Es.AttackYoyakuMargin))
//				if(IsAttackCancellable_MARGIN(3))
				{
					GmDc.Player.YoyakuReqAttack = reqAttack;
				}
			}
		}
	}
	GmDc.Player.YAdd += PLAYER_GRAVITY_YADD2;
	GmDc.Player.Y += GmDc.Player.YAdd;

	if(JIMEN_Y < GmDc.Player.Y) // ? 着地 || 地上
	{
		if(GmDc.Player.JumpDZCount) // ? 着地
		{
			GmDc.Player.JumpEndDZCount = GmDashEndMax;
		}
		GmDc.Player.Y = JIMEN_Y;
		GmDc.Player.YAdd = 0.0;
		GmDc.Player.JumpRenzokuCount = 0;
		GmDc.Player.JumpDZCount = 0;
		GmDc.Player.AirDashRenzokuCount = 0;
	}
	else
		GmDc.Player.JumpDZCount++;

	int reqWalk = 0; // 歩きというか、あらゆる横移動

	if(GTGetInput(INP_DIR_4, INP_PRESS))
	{
		reqWalk = 1;
		GmDc.Player.LeftMode = 1;
	}
	if(GTGetInput(INP_DIR_6, INP_PRESS))
	{
		reqWalk = 1;
		GmDc.Player.LeftMode = 0;
	}
	if(reqWalk)
		GmDc.Player.WalkDZCount++;
	else
		GmDc.Player.WalkDZCount = 0;

	if(GTGetInput(INP_A, INP_DOWN)) // ジャンプを開始する。
	{
		// ジャンプ開始要件
		if(
			!gezamode &&
			GmDc.Player.JumpRenzokuCount < RENZOKUJUMP_MAX &&
//			(!IsAttackNow() || IsAttackCancellable()) &&
			1
			)
		{
			GmDc.Player.YAdd = -17.0;
			GmDc.Player.JumpStartLeftMode = GmDc.Player.LeftMode;
			GmDc.Player.JumpRenzokuCount++;
			GmDc.Player.JumpDZCount = 1;
			GmDc.Player.JumpButtonDownDZCount = 1;
			GmDc.Player.JumpDushDownDZCount = 0;

			if(Sd_PlayChara == PLAY_CHARA_02)
//				if(!GmDc.Player.DashDZCount && !GTGetInput(INP_R, INP_PRESS)) // 低速移動廃止
				if(!GmDc.Player.DashDZCount)
					GmDc.Player.DashDZCount = 1;

			if(Es.ForceDashMode)
				GmDc.Player.DashDZCount = 1;
		}
	}

	if(GTGetInput(INP_A, INP_PRESS))
	{
		if(GmDc.Player.JumpButtonDownDZCount)
			GmDc.Player.JumpButtonDownDZCount++;
	}
	else
	{
		// ? ジャンプボタンちょい押し -> 小ジャンプ化
		if(
			GmDc.Player.JumpButtonDownDZCount &&
			GmDc.Player.JumpButtonDownDZCount < 9
			)
		{
			GmDc.Player.YAdd *= 0.55;
		}
		GmDc.Player.JumpButtonDownDZCount = 0;
	}

	if(GmDc.Player.DashDZCount)
		GmDc.Player.DashDZCount++;

	int reqDash = 0;
	m_countDown(GmDc.Player.DashKeepDZCount);

	if(GTGetInput(INP_DIR_4, INP_DOWN) || GTGetInput(INP_DIR_6, INP_DOWN))
	{
		if(1 <= GmDc.Player.DashKeepDZCount)
		{
			if(m_compCond(GmDc.Player.DashLeftMode, GmDc.Player.LeftMode))
			{
				reqDash = 1;
			}
		}
		GmDc.Player.DashKeepDZCount = 13;
		GmDc.Player.DashLeftMode = GmDc.Player.LeftMode;
	}
	if(GmDc.Player.JumpDZCount) // ? ジャンプ中
	{
		if(reqDash)
		{
			GmDc.Player.SecchiAndDashMode = 1;
		}
		if(GTGetInput(INP_B, INP_DOWN))
		{
			GmDc.Player.JumpDushDownDZCount++; // エアダッシュ終了時に 0 に戻される。
		}
		if(GmDc.Player.JumpDushDownDZCount && GTGetInput(INP_B, INP_PRESS))
		{
			// エアダッシュ開始要件
			if(
				GmDc.Player.AirDashRenzokuCount < RENZOKUAIRDASH_MAX &&
				(!IsAttackNow() || IsAttackCancellable()) &&
				1
				)
			{
				AttackCancel();

				int dir2 = GTGetInput(INP_DIR_2, INP_PRESS);
				int dir4 = GTGetInput(INP_DIR_4, INP_PRESS);
				int dir6 = GTGetInput(INP_DIR_6, INP_PRESS);
				int dir8 = GTGetInput(INP_DIR_8, INP_PRESS);

				if(dir2 || dir4 || dir6 || dir8) // ? エアダッシュ開始
				{
					double ad_rot;

					     if(dir6 && dir8) ad_rot = (PI / 4.0) * 1.0;
					else if(dir6 && dir2) ad_rot = (PI / 4.0) * 3.0;
					else if(dir4 && dir2) ad_rot = (PI / 4.0) * 5.0;
					else if(dir4 && dir8) ad_rot = (PI / 4.0) * 7.0;
					else if(dir8)         ad_rot = (PI / 4.0) * 0.0;
					else if(dir6)         ad_rot = (PI / 4.0) * 2.0;
					else if(dir2)         ad_rot = (PI / 4.0) * 4.0;
					else if(dir4)         ad_rot = (PI / 4.0) * 6.0;
					else                  error();

					GmDc.Player.AirDashRenzokuCount++;
					GmDc.Player.AirDashRotate = ad_rot;
					GmDc.Player.AirDashEndDZCount = 0;
					goto air_dash;
				}
			}
		}
		LFrmCnt % 4 == 0 && m_countDown(GmDc.Player.AirDashEndDZCount);
	}
	else // ? 接地している。
	{
		if(GmDc.Player.SecchiAndDashMode)
		{
			GmDc.Player.SecchiAndDashMode = 0;
			reqDash = 1;
		}
		if(GmDc.Player.DashEndDZCount || Es.ForceDashMode)
		{
			reqDash = GTGetInput(INP_DIR_4, INP_PRESS) || GTGetInput(INP_DIR_6, INP_PRESS) ? 1 : 0;
		}
		if(GmDc.Player.WalkDZCount && !reqDash)
		{
			int b_press = GTGetInput(INP_B, INP_PRESS);

			if(Sd_PlayChara == PLAY_CHARA_02)
//				b_press = GTGetInput(INP_R, INP_PRESS) ? 0 : 1; // 低速移動廃止
				b_press = 1;

			reqDash = b_press;
		}
		if(reqDash)
		{
			if(!GmDc.Player.DashDZCount)
				GmDc.Player.DashDZCount = 1;
		}
		GmDc.Player.AirDashEndDZCount = 0;
	}
	if(GmDc.Player.WalkDZCount) // ? y-移動中
	{
		int walkcnt = GmDc.Player.WalkDZCount;
		m_range(walkcnt, 0, 5);

		double speed = 1.0 + (double)walkcnt * 0.45; // 歩く速さ

		if(GmDc.Player.DashDZCount)
		{
			speed = 5.0;
		}
		if(GmDc.Player.JumpDZCount == 0) // ? 接地している。
		{
			if(GmDc.Player.GezaDZCount)
			{
				speed = (double)walkcnt * 0.2;

				if(Sd_PlayChara == PLAY_CHARA_03)
					speed *= 13.0;

				if(gezamode ?
					GmDc.Player.GezaDZCount < GmBackDogezaMax / 2 :
					GmDc.Player.GezaDZCount > GmBackDogezaMax / 2)
				{
					speed = 0.0;
				}
			}
		}
		if(IsAttackCancellable())
		{
			AttackCancel();
		}
		if(IsAttackNow() && GmDc.Player.JumpDZCount == 0)
		{
			if(GmDc.Player.DashDZCount)
			{
				speed = GmDc.Player.CAttackDZCount ? 1.3 : 1.7;
			}
			else
			{
				speed = 0.0;
			}
		}

		if(Sd_PlayChara == PLAY_CHARA_03)
			speed *= 2.0;

		GmDc.Player.X += speed * (GmDc.Player.LeftMode ? -1.0 : 1.0);
		GmDc.Player.DashEndDZCount = 0;
		GmDc.Player.JumpEndDZCount = 0;
	}
	else // ? y-停止
	{
		if(LFrmCnt % 4 == 0)
		{
			m_countDown(GmDc.Player.DashEndDZCount);
			m_countDown(GmDc.Player.JumpEndDZCount);
		}
		if(GmDc.Player.DashEndDZCount)
		{
			GmDc.Player.JumpEndDZCount = 0;
		}
		if(GmDc.Player.DashDZCount)
		{
			GmDc.Player.DashEndDZCount = GmDashEndMax;
		}
		if(GmDc.Player.JumpDZCount == 0) // ? 接地している。
		{
			GmDc.Player.X = (double)(int)GmDc.Player.X;
			GmDc.Player.DashDZCount = 0; // 接地してy-停止なら、ダッシュ解除
		}
	}
	if(!gezamode && GmDc.Player.JumpDZCount) // ダッシュ終了中ならジャンプ終了モーションは不用
	{
		GmDc.Player.GezaDZCount = 0;
	}
	m_range(GmDc.Player.X, 0.0, (double)GmDc.FieldWidth);
}
void GmSetPlayerHitBack(int fromleft, int fromupper) // fromleft: ? 左からヒット、右の方へ移動(ヒットバック)する。
{
	if(!Es.HitBackTrueDir)
		fromleft = GmDc.Player.LeftMode; // 当たった方向に関係無く、進行方向とは逆の方へヒットバックさせる。

	GmDc.Player.HitBackDZCount = 1;
	GmDc.Player.HitBackFromLeft = fromleft ? 1 : 0;
	GmDc.Player.HitBackFromUpper = fromupper ? 1 : 0;
	GmDc.Player.HitBackXAdd = 9.0 * (fromleft ? 1.0 : -1.0);
	GmDc.Player.TransDZCount = TRANS_FRAME;
	GmDc.Player.YoyakuReqAttack = ATTACK_NONE; // 2bs
	m_range(GmDc.Player.YAdd, -10.0, -2.0);

	// 着地するまで飛翔させない。
	{
		GmDc.Player.AirDashDZCount = 0;
		GmDc.Player.AirDashRenzokuCount = TSLD;
	}
	AttackCancel();
}
void GmDrawPlayer(int death_mode)
{
	double x = GmDc.Player.X;
	double y = GmDc.Player.Y;
	int leftmode = GmDc.Player.LeftMode;

	if(GmDc.Player.TransDZCount && !death_mode)
	{
		SetAlpha(0.5);
	}

	switch(Sd_PlayChara)
	{
	case PLAY_CHARA_00: SetPictureGroup(S_Kuma00PicGrp()); break;
	case PLAY_CHARA_01: SetPictureGroup(S_Kuma01PicGrp()); break;
	case PLAY_CHARA_02: SetPictureGroup(S_Kuma02PicGrp()); break;
	case PLAY_CHARA_03: SetPictureGroup(S_Kuma03PicGrp()); break;

	default:
		error();
	}

	if(GmDc.Player.HitBackDZCount)
	{
		int frmcnt = GmDc.Player.HitBackDZCount;
		int hb_leftmode = GmDc.Player.HitBackFromLeft;

		if(GmDc.Player.HitBackFromUpper)
		{
			GmDrawUHit(x, y, frmcnt, hb_leftmode);
		}
		else
		{
			GmDrawDHit(x, y, frmcnt, hb_leftmode);
		}
	}
	else if(GmDc.Player.UAttackDZCount)
	{
		GmDrawUAttack(x, y, GmDc.Player.UAttackDZCount, GmDc.Player.AttackLeftMode);
	}
	else if(GmDc.Player.CAttackDZCount)
	{
		GmDrawCAttack(x, y, GmDc.Player.CAttackDZCount, GmDc.Player.AttackLeftMode);
	}
	else if(GmDc.Player.DAttackDZCount)
	{
		GmDrawDAttack(x, y, GmDc.Player.DAttackDZCount, GmDc.Player.AttackLeftMode);
	}
	else if(GmDc.Player.AirDashEndDZCount)
	{
		GmDrawAirDashEnd(x, y, GmDc.Player.AirDashEndDZCount, leftmode);
	}
	else if(GmDc.Player.AirDashDZCount)
	{
		GmDrawAirDash(x, y, GmDc.Player.AirDashRotate);
	}
	else if(GmDc.Player.GezaDZCount)
	{
		GmDrawBackDogeza(x, y, GmDc.Player.GezaDZCount, leftmode, GmDc.Player.WalkDZCount);
	}
	else if(GmDc.Player.JumpDZCount)
	{
		if(GmDc.Player.WalkDZCount)
		{
			if(m_compCond(GmDc.Player.JumpStartLeftMode, leftmode))
			{
				GmDrawJumpFront(x, y, GmDc.Player.JumpDZCount, leftmode);
			}
			else
			{
				GmDrawJumpBack(x, y, GmDc.Player.JumpDZCount, !leftmode);
			}
		}
		else
		{
			GmDrawJump(x, y, GmDc.Player.JumpDZCount, leftmode);
		}
	}
	else if(GmDc.Player.WalkDZCount)
	{
		if(GmDc.Player.DashDZCount)
		{
			GmDrawDash(x, y, GmDc.Player.DashDZCount, leftmode);
		}
		else
		{
			if(leftmode)
			{
				GmDrawBack(x, y, 0);
			}
			else
			{
				GmDrawFront(x, y, 0);
			}
		}
	}
	else
	{
		int endcnt = m_max(GmDc.Player.DashEndDZCount, GmDc.Player.JumpEndDZCount);

		if(endcnt)
		{
			GmDrawDashEnd(x, y, endcnt, leftmode);
		}
		else
		{
			GmDrawStand(x, y, leftmode);
		}
	}

	SetPictureGroup(NULL);

	if(GmDc.Player.TransDZCount && !death_mode)
	{
		ResetBlend();
	}
}
int GmPlayerIsDogezaNow(void)
{
	return GmBackDogezaMax <= GmDc.Player.GezaDZCount;
}
void GmDrawPlayerKage(void)
{
	m_countApprox(
		GmDc.Player.KageAlpha,
		GmDc.Player.JumpDZCount ? 0.3 : 0.0,
		GmDc.Player.JumpDZCount ? 0.99 : 0.9,
		0.0
		);

	DrawBegin(GmDc.Player.X - GmDc.CameraX, 550.0, Pic(P_PLAYER_KAGE));
	DrawRotate(LFrmCnt / 60.0);
	DrawXZoom(0.5);
	DrawYZoom(0.2);
	SetBlendAdd(GmDc.Player.KageAlpha);
	DrawEnd();
	ResetBlend();
}
