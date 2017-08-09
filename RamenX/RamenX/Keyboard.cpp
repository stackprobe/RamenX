#include "Stdinc.h"

static int KeyCounterList[KEY_MAX];

void KeyFrame(void)
{
	static uchar keyList[KEY_MAX];

	if(WindowIsActive == 0) // ? ��A�N�e�B�u -> �L�[�͉���������Ă��Ȃ����ɂ���B
	{
		memset(KeyCounterList, 0x00, sizeof(KeyCounterList));
		return;
	}
	if(GetHitKeyStateAll((char *)keyList)) // ? ���s
	{
		error();
	}
	for(int keyno = 0; keyno < KEY_MAX; keyno++)
	{
		if(keyList[keyno])
		{
			KeyCounterList[keyno]++;
		}
		else
		{
			KeyCounterList[keyno] = 0;
		}
	}
}

int GetKeyPress(int keyno)
{
	KEY_INPUT_BACK; // keyno �͂��̕ӂ��Q��

	errorCase(keyno < 0 || KEY_MAX <= keyno);

	return KeyCounterList[keyno] ? 1 : 0;
}
int GetKeyDown(int keyno)
{
	errorCase(keyno < 0 || KEY_MAX <= keyno);

	return KeyCounterList[keyno] == 1;
}
int GetKeyPressCount(int keyno)
{
	errorCase(keyno < 0 || KEY_MAX <= keyno);

	return KeyCounterList[keyno];
}
int GetKeyHit(int keyno)
{
	int count = GetKeyPressCount(keyno);

#define HIT_DELAY_1ST 17
#define HIT_DELAY 4

	if(HIT_DELAY_1ST < count)
	{
		count -= HIT_DELAY_1ST;
		count %= HIT_DELAY;
	}
	return count == 1;
}

int GetKeyPressAny(void)
{
	for(int keyno = 0; keyno < KEY_MAX; keyno++)
	{
		if(KeyCounterList[keyno])
		{
			return 1;
		}
	}
	return 0;
}
int GetKeyDownAny(void)
{
	for(int keyno = 0; keyno < KEY_MAX; keyno++)
	{
		if(KeyCounterList[keyno] == 1)
		{
			return 1;
		}
	}
	return 0;
}
