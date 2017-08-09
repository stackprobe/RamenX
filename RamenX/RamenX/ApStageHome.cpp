#include "Stdinc.h"

static int MusicList[STAGE_NUM] =
{
	MUSIC_FIELD_01,
	MUSIC_FIELD_02,
	MUSIC_FIELD_03,
	MUSIC_FIELD_04,
	MUSIC_BOSS,
};
static void (*StageLoaderList[STAGE_NUM])(void) =
{
	ApLoadStage_01,
	ApLoadStage_02,
	ApLoadStage_03,
	ApLoadStage_04,
	ApLoadStage_05_Boss,
};
static int StageWidthList[STAGE_NUM] =
{
	9100,
	6840,
	14380,
	10400,
	900,
};

static double OPS_WallMoveRate = 0.5;
static void OnePictureScene(int pic_resno, int music_resno, char *kari_message)
{
	CheckPicSize(Pic(pic_resno));
	double x = 800.0 - (double)Pic_W;
	double y = 600.0 - (double)Pic_H;

	GTFreezeInput();
	MusicPlay(music_resno, 0); // ! リピートしない。

	LFrmCnt = 0;

	for(; ; )
	{
		if(LFrmCnt == 30)
			GTSetCurtain(0.0, 120);

		if(60 < LFrmCnt)
		if(GTGetInput(INP_A, INP_DOWN) ||
			GTGetInput(INP_B, INP_DOWN) ||
			GTGetInput(INP_PAUSE, INP_DOWN))
			break;

		if(x < 0.0 || y < 0.0)
		{
			SimpleDrawPicFloat(x, y, Pic(pic_resno));
			x *= OPS_WallMoveRate;
			y *= OPS_WallMoveRate;
		}
		else
			SimpleDrawPic(0, 0, Pic(pic_resno));

		if(150 < LFrmCnt)
		{
			clsDx();
			printfDx("%s\n", kari_message);
		}
		GTEndFrame();
	}
	clsDx(); // 2bs
	MusicFadeout(30);
	GTSetCurtain(-1.0, 30);

	while(GTActWhiteLevel != -1.0)
	{
		SimpleDrawPic(0, 0, Pic(pic_resno));
		GTEndFrame();
	}
}
static void Ending(void)
{
	OnePictureScene(
		P_ENDINGWALL,
		MUSIC_OLDENDING,
		""
		);

#if 0 // 廃止
	OPS_WallMoveRate = 0.996;
	OnePictureScene(
		P_ENDINGWALL,
		MUSIC_ENDING,
		""
		);
#endif
}
static void GameOver(void)
{
	OnePictureScene(
		P_GAMEOVERWALL,
		MUSIC_GAMEOVER,
		""
		);

#if 0 // 廃止
	OPS_WallMoveRate = 0.9993;
	OnePictureScene(
		P_POSTGAMEOVERWALL,
		MUSIC_POSTGAMEOVER,
		""
		);
#endif
}

static void Perform(
	int *musicList,
	void (**stageLoaderList)(void),
	int stageMax,
	int zankisuu_st,
	int bombnum_st,
	int bestTimeBaseIndex,
	int stageIndex_BASE
	)
{
restart:
	int stageBeginFlag = 1;
	int stageIndex = 0;
	int zankisuu = zankisuu_st;
	int bombnum = bombnum_st;

	DAE_DirectPos = 0; // 2bs?

	for(; ; )
	{
		GmDc.MusicResno = musicList[stageIndex];

		stageLoaderList[stageIndex]();

		GmDc.Zankisuu = zankisuu;
		GmDc.BombNum = bombnum;
		GmDc.ClearAndKeepMusicMode =
			stageIndex + 1 < stageMax && musicList[stageIndex] == musicList[stageIndex + 1] ? 1 : 0;
		GmDc.DeathAndKeepMusicMode =
//			!GmDc.BossStageMode && zankisuu ? 1 : 0;
			zankisuu ? 1 : 0;

		GmDc.BestTimeFrmCnt =
			GetBestTimeFrmCnt(bestTimeBaseIndex + stageIndex);
		GmDc.BestTimeFrmCnt_NB =
			GetBestTimeFrmCnt_NB(bestTimeBaseIndex + stageIndex);
		GmDc.BestTimeFrmCnt_NBWipedOut =
			GetBestTimeFrmCnt_NBWipedOut(bestTimeBaseIndex + stageIndex);
		GmDc.BestTimeFrmCnt_NBWipedOut_NoDamage =
			GetBestTimeFrmCnt_NBWipedOut_NoDamage(bestTimeBaseIndex + stageIndex);

		GmDc.PrmStageWidth = StageWidthList[stageIndex_BASE + stageIndex];
		GmDc.PrmStageIndex = stageIndex_BASE + stageIndex;
		GmDc.PrmStageBeginFlag = stageBeginFlag;

		GmHome();

		stageBeginFlag = 0;
		zankisuu = GmRc.EndZankisuu;
		bombnum = GmRc.EndBombNum;

		SetBestTimeFrmCnt(
			bestTimeBaseIndex + stageIndex,
			GmRc.NewBestTimeFrmCnt
			);
		SetBestTimeFrmCnt_NB(
			bestTimeBaseIndex + stageIndex,
			GmRc.NewBestTimeFrmCnt_NB
			);
		SetBestTimeFrmCnt_NBWipedOut(
			bestTimeBaseIndex + stageIndex,
			GmRc.NewBestTimeFrmCnt_NBWipedOut
			);
		SetBestTimeFrmCnt_NBWipedOut_NoDamage(
			bestTimeBaseIndex + stageIndex,
			GmRc.NewBestTimeFrmCnt_NBWipedOut_NoDamage
			);

		ExportSaveData(); // フリーズ対策。落ちてもせめて記録は残るように...

		switch(GmRc.GameEndReason)
		{
		case GER_CLEAR:
			{
				stageBeginFlag = 1;
				stageIndex++;

				if(stageIndex == stageMax) // ? 全てクリア
				{
					Ending();
					goto endLoop;
				}
				/*
				if(stageIndex + stageIndex_BASE == OLD_STAGE_NUM) // エンディング跡地
				{
					SemiEnding();
				}
				*/
				*Get_P_ClearStageIndex() = m_max(*Get_P_ClearStageIndex(), stageIndex + stageIndex_BASE);
			}
			break;

		case GER_DEATH:
			{
				if(!m_countDown(zankisuu)) // ? 残機ゼロで死亡 -> Game Over
				{
					GameOver();
					/*
					if(stageIndex + stageIndex_BASE < OLD_STAGE_NUM)
						GameOver();
					else
						GameOverPost();
					*/

					goto endLoop;
				}
				bombnum = m_max(bombnum, bombnum_st); // ボムを持ち越す。<- ボム抱え死対策
			}
			break;

		case GER_USER_ABORT:
			goto endLoop;

		case GER_USER_RETRY:
			goto restart;

		default:
			error();
		}
		forscene(30) // 間隔調整。一瞬だとちょっと変だったので...。
		{
			GTEndFrame();
		}
	}
endLoop:;
}

void ApStageHome(void)
{
	Perform(
		MusicList,
		StageLoaderList,
		STAGE_NUM,
		3,
		2,
		0,
		0
		);
}
void ApStageHome_Continue(int stageIndex)
{
	errorCase(stageIndex < 0 || STAGE_NUM <= stageIndex);

	Perform(
		MusicList + stageIndex,
		StageLoaderList + stageIndex,
		STAGE_NUM - stageIndex,
		3,
		2,
		stageIndex,
		stageIndex
		);
}

#if 0 // 廃止
void ApExtraStage(void)
{
	int musicList[] =
	{
		MUSIC_EXTRA,
		MUSIC_EXTRA,
	};
	void (*stageLoaderList[])(void) =
	{
		ApLoadStage_201,
		ApLoadStage_202_Boss,
	};

	Perform(musicList, stageLoaderList, 2, 3, 1, 0, 0);
}
void ApTestPlay(void)
{
	int musicList[] =
	{
		MUSIC_FIELD_01,
	};
	void (*stageLoaderList[])(void) =
	{
		ApLoadStage_TestEnemy,
	};

	Perform(musicList, stageLoaderList, 1, 99, 99, 0, 0);
}
#endif
