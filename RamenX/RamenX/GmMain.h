#define JIMEN_Y 500.0
#define FIELD_SEGMENT_X 500.0
#define FIELD_SEGMENT_MARGIN_X 100.0
#define AIRDASH_Y_MIN 450.0 // �G�A�_�b�V���\�ȍŒፂ�x
#define AIRDASH_Y_MAX -30.0 // �G�A�_�b�V���\�ȍō����x
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
//#define MAX_HP 9 // �v���C���[�̍ő�HP
#define MAX_HP (GetMaxHp()) // �v���C���[�̍ő�HP
#define FRAMESCORERATEDENOM 60
#define GAME_MIN_FRD 30 // FRDCount �� GAME_MIN_FRD �ȏ�ɂȂ����珈�������m��
#define TRANS_FRAME 90 // �_���[�W���󂯂���̖��G����

int GmGetEnemyCount(void *maker, int inv_mode = 0);
int GmGetEnemyCount_Destroyable(int in_camera = 0, int udrLv = -1);
char *GmFrameToStamp(int frmcnt);

void GmMain(void);

int GetMaxHp(void);
