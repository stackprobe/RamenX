#include "Stdinc.h"

static int InputTypes[PAD_MAX] =
{
	DX_INPUT_PAD1,
	DX_INPUT_PAD2,
	DX_INPUT_PAD3,
	DX_INPUT_PAD4,
	DX_INPUT_PAD5,
	DX_INPUT_PAD6,
	DX_INPUT_PAD7,
	DX_INPUT_PAD8,
	DX_INPUT_PAD9,
	DX_INPUT_PAD10,
	DX_INPUT_PAD11,
	DX_INPUT_PAD12,
	DX_INPUT_PAD13,
	DX_INPUT_PAD14,
	DX_INPUT_PAD15,
	DX_INPUT_PAD16,
};

typedef struct Button_st
{
	int PressCount;
}
Button_t;

typedef struct Pad_st
{
	int Status[2];
	Button_t *Buttons[32];
}
Pad_t;

static Pad_t *PadList[PAD_MAX];
static int PadPrimary;

void PadFrame(void)
{
	PadPrimary ^= 1;

	for(int padno = 0; padno < PAD_MAX; padno++)
	{
		if(PadList[padno])
		{
			/*
				非アクティブ -> ボタンは何も押されてない事にする。
			*/
			PadList[padno]->Status[PadPrimary] = WindowIsActive ? GetJoypadInputState(InputTypes[padno]) : 0;

			for(int btnno = 0; btnno < 32; btnno++)
			{
				if(PadList[padno]->Buttons[btnno])
				{
					if((uint)PadList[padno]->Status[PadPrimary] & 1u << btnno)
					{
						PadList[padno]->Buttons[btnno]->PressCount++;
					}
					else
					{
						PadList[padno]->Buttons[btnno]->PressCount = 0;
					}
				}
			}
		}
	}
}
void ReleasePads(void)
{
	for(int padno = 0; padno < PAD_MAX; padno++)
	{
		if(PadList[padno])
		{
			for(int btnbit = 0; btnbit < 32; btnbit++)
			{
				memFree(PadList[padno]->Buttons[btnbit]);
			}
			memFree(PadList[padno]);

			PadList[padno] = NULL;
		}
	}
}

static Pad_t *GetPad(int padno)
{
	errorCase(padno < 0 || PAD_MAX <= padno);

	if(!PadList[padno])
		PadList[padno] = (Pad_t *)memCalloc(sizeof(Pad_t));

	return PadList[padno];
}
static Button_t *GetButton(int padno, int btnbit)
{
	Pad_t *pad = GetPad(padno);

	errorCase(btnbit < 0 || 31 < btnbit);

	if(!pad->Buttons[btnbit])
		pad->Buttons[btnbit] = (Button_t *)memCalloc(sizeof(Button_t));

	return pad->Buttons[btnbit];
}

int GetPadPress(int padno)
{
	return GetPad(padno)->Status[PadPrimary];
}
static int GetLastPress(int padno)
{
	return GetPad(padno)->Status[PadPrimary ^ 1];
}
int GetPadDown(int padno)
{
	return GetPadPress(padno) & ~GetLastPress(padno);
}
int GetPadPressCount(int padno, int btnbit)
{
	return GetButton(padno, btnbit)->PressCount;
}
int GetPadHit(int padno, int btnbit)
{
	int count = GetPadPressCount(padno, btnbit);

#define HIT_DELAY_1ST 17
#define HIT_DELAY 4

	if(HIT_DELAY_1ST < count)
	{
		count -= HIT_DELAY_1ST;
		count %= HIT_DELAY;
	}
	return count == 1;
}

int GetPadPress_bb(int padno, int btnbit)
{
	errorCase(btnbit < 0 || 31 < btnbit);

	return (uint)GetPadPress(padno) & (1u << btnbit);
}
int GetPadDown_bb(int padno, int btnbit)
{
	errorCase(btnbit < 0 || 31 < btnbit);

	return (uint)GetPadDown(padno) & (1u << btnbit);
}
