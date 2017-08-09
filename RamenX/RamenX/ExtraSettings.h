typedef struct ExtraSettings_st
{
	/*
		TransDZCountは最大69ぽい。ヒットバック前にセットされるからだろう。
		0 だと常に攻撃出来なくなる。
		... ので意味のある値域としては 1 - 70
		def: TRANS_FRAME / 2
	*/
	int AttackFukaFrame;

	/*
		def: TRUE
	*/
	int No_AttackAndTransOff;

	/*
		*_ATTACK_END の最大が C, D の 8
		... ので 0 - (8 - 1)
		0 で攻撃予約は無効になる。
		def: 3
	*/
	int AttackYoyakuMargin;

	/*
		本来ならfloatの方が滑らかなんだろうけど、intで作りこんだので今更綺麗にならなかった。
		... ので def: 0 の方が綺麗。
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
		ヒットバック時に接触した相手と反対方向に飛んでいく。(デフォは進行方向と逆)
		def: 0
	*/
	int HitBackTrueDir;

	/*
		0 -- 0.0 (標準)
		1 -- 0.0 - 0.5
		2 -- 0.5
		3 -- 0.5 - 1.0
		4 -- 1.0
		5-9 -- +アンチエイリアス
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

#define MORESNOW_MAX 10 // 最大値+1

extern ExtraSettings_t Es;

void EsInit(void);
