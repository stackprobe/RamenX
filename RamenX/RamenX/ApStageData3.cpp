#include "Stdinc.h"

void ApLoadStage_301(void)
{
	initrnd(1);

	// ---- Wall ----

	GmDc.WallPicResnoList->AddElement(D_FIELDWALL_101_A | DTP); // dummy

	// ---- Player ----

	GmDc.PlayerStartX = 400.0;
	GmDc.PlayerStartLeftMode = 0;

	// ---- Enemy ----

	// 0, 500

	GmDc.EnemyList->AddElement(MakeCube_R(200.0, 200.0)); // dummy

	// x-max: ?
}
void ApLoadStage_302(void)
{
	initrnd(1);

	// ---- Wall ----

	GmDc.WallPicResnoList->AddElement(D_FIELDWALL_101_A | DTP); // dummy

	// ---- Player ----

	GmDc.PlayerStartX = 400.0;
	GmDc.PlayerStartLeftMode = 0;

	// ---- Enemy ----

	// 0, 500

	GmDc.EnemyList->AddElement(MakeCube_R(200.0, 200.0)); // dummy

	// x-max: ?
}
void ApLoadStage_303_Boss(void)
{
	initrnd(1);

	// ---- Wall ----

	GmDc.WallPicResnoList->AddElement(D_FIELDWALL_101_A | DTP); // dummy

	// ---- Player ----

	GmDc.PlayerStartX = 400.0;
	GmDc.PlayerStartLeftMode = 0;

	// ---- Enemy ----

	// 0, 500

	GmDc.EnemyList->AddElement(MakeCube_R(200.0, 200.0)); // dummy

	// x-max: ?
}
