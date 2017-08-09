#define GM_VSS_MAX 4 // 最大値+1

extern int GmVisibleSubStatus;

typedef enum GmGameEndReason_et
{
	GER_NONE, // 0-default member

	GER_CLEAR,
	GER_DEATH,
	GER_USER_ABORT,
	GER_USER_RETRY,

//	GER_MAX, // num of member
}
GmGameEndReason_t;

typedef struct GmDataCenter_st // GmHome() 終了時に初期化する。(引数と広域)
{
	// 引数 >

	int MusicResno;
	// LoadStage >
	autoList<int> *WallPicResnoList;
	int WallKasaneMode;
	int SnowMode;
	double PlayerStartX;
	int PlayerStartLeftMode;
	autoList<GmEnemy_t> *EnemyList;
	int BossStageMode;
	int ExtraStageMode;
	// < LoadStage
	int Zankisuu;
	int BombNum;
	int ClearAndKeepMusicMode;
	int DeathAndKeepMusicMode;

	/*
		記録が無い場合は TSLD など
		更新させない場合は -1 など
	*/
	int BestTimeFrmCnt;
	int BestTimeFrmCnt_NB;
	int BestTimeFrmCnt_NBWipedOut;
	int BestTimeFrmCnt_NBWipedOut_NoDamage;

	int PrmStageWidth;
	int PrmStageIndex; // 0 - (STAGE_NUM - 1)
	int PrmStageBeginFlag;

	// < 引数

	double CameraX;
	double FieldWidth;

	int CubeCount;
	int NoCubesCount; // 1 <= : クリア確定
	int EnemyCount;
	int DamageKuratta;

	int BombUsed;
	int BombUsed_573Cmd;
	int BombDZCount;
	double BombX;
	double BombY;

	int BubbleBackDrawRemain;
	int ForceTamaShoumetsuMode;
	int Fire07_ForceDeathMode;

	/*
		DZCount ... disable zero count, 0 を無効とし 0 以外を有効(カウンタ値)とする。
	*/
	struct
	{
		double X;
		double Y;
		int LeftMode;
		int WalkDZCount;
		int DashKeepDZCount;
		int DashLeftMode;
		int DashDZCount;
		int DashEndDZCount;
		double YAdd;
		int JumpStartLeftMode;
		int JumpRenzokuCount;
		int JumpDZCount;
		int JumpButtonDownDZCount;
		int JumpDushDownDZCount;
		int JumpEndDZCount;
		int SecchiAndDashMode;
		int GezaDZCount;
		int AirDashRenzokuCount;
		int AirDashDZCount;
		double AirDashRotate; // 真上を 0.0 時計回り
		double AirDashLastXAdd;
		double AirDashLastYAdd;
		int AirDashRotateCount;
		int AirDashEndDZCount;
		int UAttackDZCount;
		int CAttackDZCount;
		int DAttackDZCount;
		int AttackLeftMode;
		int YoyakuReqAttack;
		int HitBackDZCount;
		int HitBackFrmCnt;
		int HitBackFromLeft;
		int HitBackFromUpper;
		double HitBackXAdd;
		int TransDZCount; // 半透明無敵期間
		int Hp;
		double KageAlpha;
	}
	Player;

	taskGroup_t *EffectList;
}
GmDataCenter_t;

typedef struct GmResultCenter_st // GmHome() 開始時に初期化する。(戻り値)
{
	GmGameEndReason_t GameEndReason;
	int EndZankisuu;
	int EndBombNum;
	int FinishFrmCnt;
	int NewBestTimeFrmCnt;
	int NewBestTimeFrmCnt_NB;
	int NewBestTimeFrmCnt_NBWipedOut;
	int NewBestTimeFrmCnt_NBWipedOut_NoDamage;
}
GmResultCenter_t;

extern GmDataCenter_t GmDc;
extern GmResultCenter_t GmRc;

void GmResetDataCenter(void);
void GmResetResultCenter(void);

void GmHome(void);
