typedef struct ExtraSettings_st
{
	/*
		TransDZCount�͍ő�69�ۂ��B�q�b�g�o�b�N�O�ɃZ�b�g����邩�炾�낤�B
		0 ���Ə�ɍU���o���Ȃ��Ȃ�B
		... �̂ňӖ��̂���l��Ƃ��Ă� 1 - 70
		def: TRANS_FRAME / 2
	*/
	int AttackFukaFrame;

	/*
		def: TRUE
	*/
	int No_AttackAndTransOff;

	/*
		*_ATTACK_END �̍ő傪 C, D �� 8
		... �̂� 0 - (8 - 1)
		0 �ōU���\��͖����ɂȂ�B
		def: 3
	*/
	int AttackYoyakuMargin;

	/*
		�{���Ȃ�float�̕������炩�Ȃ񂾂낤���ǁAint�ō�肱�񂾂̂ō��X�Y��ɂȂ�Ȃ������B
		... �̂� def: 0 �̕����Y��B
	*/
	int DrawRectFloat;

	/*
		def: 0
	*/
	int ForceDashMode;

	/*
		def: 0
	*/
	int ShowEnemyHPMode;

	/*
		�q�b�g�o�b�N���ɐڐG��������Ɣ��Ε����ɔ��ł����B(�f�t�H�͐i�s�����Ƌt)
		def: 0
	*/
	int HitBackTrueDir;

	/*
		0 -- 0.0 (�W��)
		1 -- 0.0 - 0.5
		2 -- 0.5
		3 -- 0.5 - 1.0
		4 -- 1.0
		5-9 -- +�A���`�G�C���A�X
		def: 0
	*/
	int MoreSnow;

	/*
		def: FALSE
	*/
	int VisibleScope;
}
ExtraSettings_t;

#define ES_AFF_MIN 1
#define ES_AFF_MAX 70

#define ES_AYM_MIN 0
#define ES_AYM_MAX 7

#define MORESNOW_MAX 10 // �ő�l+1

extern ExtraSettings_t Es;

void EsInit(void);
