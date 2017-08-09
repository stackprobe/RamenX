#include "Stdinc.h"

void ApLoadStage_401(void)
{
	initrnd(901);

	// ---- Wall ----

	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_104_B | DTP);
	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_104_A | DTP);
	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_103_A | DTP);
	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_103_B | DTP);
	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_101_A | DTP);
	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_102_B | DTP);
	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_101_B | DTP);
	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_102_A | DTP);

	// ---- Player ----

	GmDc.PlayerStartX = 400.0;
	GmDc.PlayerStartLeftMode = 0;

	// ---- Enemy ----

	// 0, 500

	GmDc.EnemyList->AddElement(MakeAya(100.0, 50.0, 1));
	GmDc.EnemyList->AddElement(MakeAya(700.0, 50.0, 1));

	for(int x = 50; x <= 750; x += 100)
	for(int c = 1; c <= 3; c++)
	{
		double y = -1000.0 - c * 500.0;
		GmDc.EnemyList->AddElement(MakeAlice((double)x, y, 2));
	}

	GmDc.EnemyList->AddElement(MakeCube_W(15300.0, 400.0)); // dummy

	// x-max: 15360

	GmDc.ExtraStageMode = 1;
}
void ApLoadStage_402(void)
{
	initrnd(901);

	// ---- Wall ----

	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_104_B | DTP);
	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_104_A | DTP);
	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_103_A | DTP);
	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_103_B | DTP);
	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_101_A | DTP);
	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_102_B | DTP);
	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_101_B | DTP);
	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_102_A | DTP);

	// ---- Player ----

	GmDc.PlayerStartX = 400.0;
	GmDc.PlayerStartLeftMode = 0;

	// ---- Enemy ----

	// 0, 500

	GmDc.EnemyList->AddElement(MakeAya(100.0, 50.0, 1));
	GmDc.EnemyList->AddElement(MakeAya(700.0, 50.0, 1));

	for(int x = 50; x <= 750; x += 100)
	for(int c = 1; c <= 3; c++)
	{
		double y = -1000.0 - c * 500.0;
		GmDc.EnemyList->AddElement(MakeAlice((double)x, y, 2));
	}

	GmDc.EnemyList->AddElement(MakeCube_W(15300.0, 400.0)); // dummy

	// x-max: 15360

	GmDc.ExtraStageMode = 1;
}
void ApLoadStage_403_Boss(void)
{
	initrnd(901);

	// ---- Wall ----

	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_104_B | DTP);
	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_104_A | DTP);
	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_103_A | DTP);
	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_103_B | DTP);
	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_101_A | DTP);
	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_102_B | DTP);
	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_101_B | DTP);
	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_102_A | DTP);

	// ---- Player ----

	GmDc.PlayerStartX = 400.0;
	GmDc.PlayerStartLeftMode = 0;

	// ---- Enemy ----

	// 0, 500

	GmDc.EnemyList->AddElement(MakeAya(100.0, 50.0, 1));
	GmDc.EnemyList->AddElement(MakeAya(700.0, 50.0, 1));

	for(int x = 50; x <= 750; x += 100)
	for(int c = 1; c <= 3; c++)
	{
		double y = -1000.0 - c * 500.0;
		GmDc.EnemyList->AddElement(MakeAlice((double)x, y, 2));
	}

	GmDc.EnemyList->AddElement(MakeCube_W(15300.0, 400.0)); // dummy

	// x-max: 15360

	GmDc.ExtraStageMode = 1;
}
