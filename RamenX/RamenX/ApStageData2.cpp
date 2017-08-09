#include "Stdinc.h"

static void LoadBlock_001(double xPos)
{
	for(int c = 0; c < 50; c++)
	{
		DinamicAddEnemy(MakeAlice(xPos + ( 800.0 + c * 30), 600.0, 1));
		DinamicAddEnemy(MakeAlice(xPos - (2800.0 + c * 30), 600.0, 1));
	}
	for(int c = 0; c < 5; c++)
	{
		DinamicAddEnemy(MakeAlice(xPos - 300.0, 50.0 + c * 100, 0));
		DinamicAddEnemy(MakeAlice(xPos + 300.0, 50.0 + c * 100, 0));
	}
}
static void LoadBlock_002(double xPos)
{
	DinamicAddEnemy(MakeAya(xPos - 300.0, 100.0, 0));
	DinamicAddEnemy(MakeAya(xPos + 300.0, 200.0, 0));
	DinamicAddEnemy(MakeAya(xPos - 300.0, 300.0, 0));
	DinamicAddEnemy(MakeAya(xPos + 300.0, 400.0, 0));

	for(int c = 0; c < 5; c++)
	{
		DinamicAddEnemy(MakeAlice(xPos - 3000.0, 600.0, 2));
		DinamicAddEnemy(MakeAlice(xPos + 3000.0, 600.0, 2));
	} 
}
static void LoadBlock_003(double xPos)
{
	for(int c = 0; c < 4; c++)
	{
		DinamicAddEnemy(MakeUdonge(xPos - 200.0, 100.0 + c * 100, m_min(c, 2)));
		DinamicAddEnemy(MakeUdonge(xPos + 200.0, 150.0 + c * 100, m_min(c, 2)));
	}
	DinamicAddEnemy(MakeDoll(xPos - 300.0, 175.0, 2));
	DinamicAddEnemy(MakeDoll(xPos + 300.0, 175.0, 2));
	DinamicAddEnemy(MakeDoll(xPos, 75.0, 1));
}
static void LoadBlock_004(double xPos)
{
	for(int c = 0; c < 18; c++)
	{
		GmEnemy_t *e1 = DinamicAddEnemy(MakeSuica(xPos - (150.0 + c * 100), c < 10 ? 1 : c % 2 + 1));
		GmEnemy_t *e2 = DinamicAddEnemy(MakeSuica(xPos + (150.0 + c * 100), c < 10 ? 1 : c % 2 + 1));

		e1->ExtraPart.Suica.ForceDir = 2;
		e2->ExtraPart.Suica.ForceDir = 1;
	}

	// todo
}
void ApLoadStage_201(void)
{
	initrnd(201);

	// ---- Wall ----

	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_103_A | DTP);
	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_103_B | DTP);
	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_104_B | DTP);
	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_104_A | DTP);
	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_101_A | DTP);
	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_102_B | DTP);
	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_101_B | DTP);
	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_102_A | DTP);
//	GmDc.ExtraStageMode = 1;

	// ---- Player ----

	GmDc.PlayerStartX = 100.0;
	GmDc.PlayerStartLeftMode = 0;

	// ---- Enemy ----

	double xPos = 0.0;

	xPos += 500.0;
	GmDc.EnemyList->AddElement(MakeTrigger(xPos, LoadBlock_001));

	xPos += 500.0;
	GmDc.EnemyList->AddElement(MakeTrigger(xPos, LoadBlock_002));

	xPos += 500.0;
	GmDc.EnemyList->AddElement(MakeTrigger(xPos, LoadBlock_003));

	xPos += 500.0;
	GmDc.EnemyList->AddElement(MakeTrigger(xPos, LoadBlock_004));

	xPos += 500.0;
	xPos += 500.0;
	xPos += 500.0;
	GmDc.EnemyList->AddElement(MakeCube_R(xPos, 200.0));

	// x-max: ?
}
void ApLoadStage_202_Boss(void)
{
	initrnd(1);

	// ---- Wall ----

	GmDc.WallPicResnoList->AddElement(D_EXTRAWALL_103_A | DTP); // dummy

	// ---- Player ----

	GmDc.PlayerStartX = 450.0;
	GmDc.PlayerStartLeftMode = 0;

	// ---- Enemy ----

	// 0, 500

	GmDc.EnemyList->AddElement(MakeCube_R(450.0, 200.0)); // dummy

	// x-max: ?
}
