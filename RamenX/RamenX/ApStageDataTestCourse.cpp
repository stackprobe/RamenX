#include "Stdinc.h"

void ApLoadStage_TestEnemy(void)
{
	initrnd(123);

	// ---- Wall ----

	for(int c = 0; c < 30; c++)
	{
		GmDc.WallPicResnoList->AddElement(D_FIELDWALL_101_A | DTP);
		GmDc.WallPicResnoList->AddElement(D_FIELDWALL_101_B | DTP);
	}

	// ---- Player ----

	GmDc.PlayerStartX = 80.0;
	GmDc.PlayerStartLeftMode = 0;

	// ---- Enemy ----

	double x = 500.0;

	x += 500.0;
	GmDc.EnemyList->AddElement(MakeAya(x, 250.0, 2));
	GmDc.EnemyList->AddElement(MakeAya(x, 350.0, 2));

//	GmDc.EnemyList->AddElement(MakeBubble(x += 500.0, 300.0, 3));
//	GmDc.EnemyList->AddElement(MakeDoll(x += 500.0, 300.0, 3));

	GmDc.EnemyList->AddElement(MakeSuica(x += 500.0, 0));
	GmDc.EnemyList->AddElement(MakeSuica(x += 500.0, 0));

	GmDc.EnemyList->AddElement(MakeUdonge(x += 500.0, 300.0, 3));

	GmDc.EnemyList->AddElement(MakeSuica(x += 500.0, 0));
	GmDc.EnemyList->AddElement(MakeMapItem(x, 400.0, MIE_KAIFUKU));
	GmDc.EnemyList->AddElement(MakeSuica(x += 500.0, 0));
	GmDc.EnemyList->AddElement(MakeMapItem(x, 400.0, MIE_ADD_BOMB));
	GmDc.EnemyList->AddElement(MakeSuica(x += 500.0, 0));
	GmDc.EnemyList->AddElement(MakeMapItem(x, 400.0, MIE_ADD_ZANKI));
	GmDc.EnemyList->AddElement(MakeSuica(x += 500.0, 0));

	GmDc.EnemyList->AddElement(MakeCube_X(x += 500.0, 300.0));

	x += 500.0;
	GmDc.EnemyList->AddElement(MakeSuica(x +   0.0, 3));
	GmDc.EnemyList->AddElement(MakeSuica(x +  80.0, 3));
	GmDc.EnemyList->AddElement(MakeSuica(x + 160.0, 3));
	GmDc.EnemyList->AddElement(MakeSuica(x + 240.0, 3));

	GmDc.EnemyList->AddElement(MakeSuica(x += 500.0, 0));
	GmDc.EnemyList->AddElement(MakeSuica(x += 500.0, 0));

	GmDc.EnemyList->AddElement(MakeAlice(x += 500.0, -100.0, 3));
	GmDc.EnemyList->AddElement(MakeAlice(x, -200.0, 3));
	GmDc.EnemyList->AddElement(MakeAlice(x, -300.0, 3));
	GmDc.EnemyList->AddElement(MakeAlice(x, -400.0, 3));
	GmDc.EnemyList->AddElement(MakeAlice(x, -500.0, 3));
	GmDc.EnemyList->AddElement(MakeAlice(x, -600.0, 3));

	GmDc.EnemyList->AddElement(MakeSuica(x += 500.0, 0));
	GmDc.EnemyList->AddElement(MakeSuica(x += 500.0, 0));

	GmDc.EnemyList->AddElement(MakeBubble(x += 500.0, 300.0, 3));
	GmDc.EnemyList->AddElement(MakeDoll(x += 500.0, 300.0, 3));

	GmDc.EnemyList->AddElement(MakeMarisa(x += 500.0, 2));

	GmDc.EnemyList->AddElement(MakeSuica(x += 500.0, 2));
	GmDc.EnemyList->AddElement(MakeBubble(x += 500.0, 100.0, 2));

	GmDc.EnemyList->AddElement(MakeCube_W(x += 500.0, 300.0));

	GmDc.EnemyList->AddElement(MakeDoll(x += 500.0, 100.0, 2));
	GmDc.EnemyList->AddElement(MakeDoll(x += 500.0, 300.0, 2));
	GmDc.EnemyList->AddElement(MakeDoll(x += 500.0, 500.0, 2));

	GmDc.EnemyList->AddElement(MakeDoll(x += 500.0, 300.0, 0));
	GmDc.EnemyList->AddElement(MakeDoll(x += 500.0, 300.0, 1));
	GmDc.EnemyList->AddElement(MakeDoll(x += 500.0, 300.0, 2));

	GmDc.EnemyList->AddElement(MakeAlice(x += 500.0, 0.0, 0));
	GmDc.EnemyList->AddElement(MakeAlice(x += 500.0, 0.0, 1));
	GmDc.EnemyList->AddElement(MakeAlice(x += 500.0, 0.0, 2));

	GmDc.EnemyList->AddElement(MakeCube_P(x += 500.0, 300.0));
	GmDc.EnemyList->AddElement(MakeCube_R(x += 500.0, 300.0));
	GmDc.EnemyList->AddElement(MakeCube_G(x += 500.0, 300.0));
	GmDc.EnemyList->AddElement(MakeCube_B(x += 500.0, 300.0));
	GmDc.EnemyList->AddElement(MakeCube_Y(x += 500.0, 300.0));
	GmDc.EnemyList->AddElement(MakeCube_W(x += 500.0, 300.0));

	GmDc.EnemyList->AddElement(MakeSuica(x += 500.0, 0));
	GmDc.EnemyList->AddElement(MakeSuica(x += 500.0, 1));
	GmDc.EnemyList->AddElement(MakeSuica(x += 500.0, 2));

	GmDc.EnemyList->AddElement(MakeBubble(x += 500.0, 100.0, 0));
	GmDc.EnemyList->AddElement(MakeBubble(x += 500.0, 100.0, 1));
	GmDc.EnemyList->AddElement(MakeBubble(x += 500.0, 100.0, 2));

	GmDc.EnemyList->AddElement(MakeMarisa(x += 500.0, 0));
	GmDc.EnemyList->AddElement(MakeMarisa(x += 500.0, 1));
	GmDc.EnemyList->AddElement(MakeMarisa(x += 500.0, 2));

	GmDc.EnemyList->AddElement(MakeAya(x += 500.0, 200.0, 0));
	GmDc.EnemyList->AddElement(MakeAya(x += 500.0, 200.0, 1));
	GmDc.EnemyList->AddElement(MakeAya(x += 500.0, 200.0, 2));

	GmDc.EnemyList->AddElement(MakeUdonge(x += 500.0, 400.0, 0));
	GmDc.EnemyList->AddElement(MakeUdonge(x += 500.0, 400.0, 1));
	GmDc.EnemyList->AddElement(MakeUdonge(x += 500.0, 400.0, 2));
	GmDc.EnemyList->AddElement(MakeUdonge(x += 500.0, 400.0, 3));
}

void ApLoadStage_TestCourse1(void)
{
	initrnd(1);

	// ---- Wall ----

	GmDc.WallPicResnoList->AddElement(D_FIELDWALL_101_A | DTP);

	// ---- Player ----

	GmDc.PlayerStartX = 400.0;
	GmDc.PlayerStartLeftMode = 0;

	// ---- Enemy ----

	GmDc.EnemyList->AddElement(MakeCube_R(400.0, 200.0));
}
void ApLoadStage_TestCourse2(void)
{
	initrnd(1);

	// ---- Wall ----

	GmDc.WallPicResnoList->AddElement(D_FIELDWALL_201_A | DTP);

	// ---- Player ----

	GmDc.PlayerStartX = 400.0;
	GmDc.PlayerStartLeftMode = 0;

	// ---- Enemy ----

	GmDc.EnemyList->AddElement(MakeCube_R(400.0, 200.0));
}
void ApLoadStage_TestCourse3(void)
{
	initrnd(1);

	// ---- Wall ----

	GmDc.WallPicResnoList->AddElement(P_DUMMY);

	// ---- Player ----

	GmDc.PlayerStartX = 400.0;
	GmDc.PlayerStartLeftMode = 0;

	// ---- Enemy ----

	GmDc.EnemyList->AddElement(MakeCube_R(400.0, 200.0));
}
