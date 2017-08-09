#define GM_VSS_MAX 4 // �ő�l+1

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

typedef struct GmDataCenter_st // GmHome() �I�����ɏ���������B(�����ƍL��)
{
	// ���� >

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
		�L�^�������ꍇ�� TSLD �Ȃ�
		�X�V�����Ȃ��ꍇ�� -1 �Ȃ�
	*/
	int BestTimeFrmCnt;
	int BestTimeFrmCnt_NB;
	int BestTimeFrmCnt_NBWipedOut;
	int BestTimeFrmCnt_NBWipedOut_NoDamage;

	int PrmStageWidth;
	int PrmStageIndex; // 0 - (STAGE_NUM - 1)
	int PrmStageBeginFlag;

	// < ����

	double CameraX;
	double FieldWidth;

	int CubeCount;
	int NoCubesCount; // 1 <= : �N���A�m��
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
		DZCount ... disable zero count, 0 �𖳌��Ƃ� 0 �ȊO��L��(�J�E���^�l)�Ƃ���B
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
		double AirDashRotate; // �^��� 0.0 ���v���
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
		int TransDZCount; // ���������G����
		int Hp;
		double KageAlpha;
	}
	Player;

	taskGroup_t *EffectList;
}
GmDataCenter_t;

typedef struct GmResultCenter_st // GmHome() �J�n���ɏ���������B(�߂�l)
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
