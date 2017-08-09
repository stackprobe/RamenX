#include "Stdinc.h"

/*
	0: 通常
	1: 通常+DEBUG
	2: 非表示
	3: 簡易
*/
int GmVisibleSubStatus;

GmDataCenter_t GmDc;
GmResultCenter_t GmRc;

void GmResetDataCenter(void)
{
	static int inited;

	if(!inited)
		goto init;

	if(S_EnemyEntryQueue()->GetCount())
		GmDc.EnemyList->AddElements(S_EnemyEntryQueue()); // 2bs, EnemyEntryQueue にエントリーを残したまま終わってるかもしれない...。

	while(GmDc.EnemyList->GetCount())
	{
		GmEnemy_t i = GmDc.EnemyList->UnaddElement();

		i.FuncReleaseExtraData(i.ExtraData);
	}
	delete GmDc.WallPicResnoList;
	delete GmDc.EnemyList;

	ReleaseTaskGroup(GmDc.EffectList);

	/*
		配列など領域の確保と整地だけ行う。
		具体的な値の設定は GmMain() の最初で行う。
	*/
init:
	memset(&GmDc, 0x00, sizeof(GmDc));

	GmDc.WallPicResnoList = new autoList<int>();
	GmDc.EnemyList = new autoList<GmEnemy_t>(4096);

	GmDc.EffectList = CreateTaskGroup();

	inited = 1;
}
void GmResetResultCenter(void)
{
	static int inited;

	if(!inited)
		goto init;

init:
	memset(&GmRc, 0x00, sizeof(GmRc));

	inited = 1;
}

void GmHome(void)
{
	GmResetResultCenter();
	GmMain();
	GmResetDataCenter();
}
