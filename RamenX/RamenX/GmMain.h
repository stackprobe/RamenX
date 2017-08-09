#define JIMEN_Y 500.0
#define FIELD_SEGMENT_X 500.0
#define FIELD_SEGMENT_MARGIN_X 100.0
#define AIRDASH_Y_MIN 450.0 // エアダッシュ可能な最低高度
#define AIRDASH_Y_MAX -30.0 // エアダッシュ可能な最高高度
#define RENZOKUJUMP_MAX ( \
	Sd_PlayChara == PLAY_CHARA_01 ? 2 : \
	Sd_PlayChara == PLAY_CHARA_03 ? 9 : \
	3 \
	)
#define RENZOKUAIRDASH_MAX ( \
	Sd_PlayChara == PLAY_CHARA_01 ? 2 : \
	Sd_PlayChara == PLAY_CHARA_03 ? 9 : \
	3 \
	)
//#define MAX_HP 9 // プレイヤーの最大HP
#define MAX_HP (GetMaxHp()) // プレイヤーの最大HP
#define FRAMESCORERATEDENOM 60
#define GAME_MIN_FRD 30 // FRDCount が GAME_MIN_FRD 以上になったら処理落ち確定
#define TRANS_FRAME 90 // ダメージを受けた後の無敵期間

int GmGetEnemyCount(void *maker, int inv_mode = 0);
int GmGetEnemyCount_Destroyable(int in_camera = 0, int udrLv = -1);
char *GmFrameToStamp(int frmcnt);

void GmMain(void);

int GetMaxHp(void);
