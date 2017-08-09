#include "Stdinc.h"

/*
	0: �ʏ�
	1: �ʏ�+DEBUG
	2: ��\��
	3: �Ȉ�
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
		GmDc.EnemyList->AddElements(S_EnemyEntryQueue()); // 2bs, EnemyEntryQueue �ɃG���g���[���c�����܂܏I����Ă邩������Ȃ�...�B

	while(GmDc.EnemyList->GetCount())
	{
		GmEnemy_t i = GmDc.EnemyList->UnaddElement();

		i.FuncReleaseExtraData(i.ExtraData);
	}
	delete GmDc.WallPicResnoList;
	delete GmDc.EnemyList;

	ReleaseTaskGroup(GmDc.EffectList);

	/*
		�z��ȂǗ̈�̊m�ۂƐ��n�����s���B
		��̓I�Ȓl�̐ݒ�� GmMain() �̍ŏ��ōs���B
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
