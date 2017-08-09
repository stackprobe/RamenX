#include "Stdinc.h"

ExtraSettings_t Es;

void EsInit(void)
{
	Es.AttackFukaFrame = TRANS_FRAME / 2;
	Es.No_AttackAndTransOff = 1;
	Es.AttackYoyakuMargin = 3;
	Es.DrawRectFloat = 0;
	Es.ForceDashMode = 0;
	Es.ShowEnemyHPMode = 0;
	Es.HitBackTrueDir = 0;
	Es.MoreSnow = 0;
	Es.VisibleScope = 0;
}
