enum // SANWA Pad Button
{
	SWPB_DIR_2,
	SWPB_DIR_4,
	SWPB_DIR_6,
	SWPB_DIR_8,
	SWPB_A1,
	SWPB_A2,
	SWPB_A3,
	SWPB_B1,
	SWPB_B2,
	SWPB_B3,
	SWPB_L,
	SWPB_R,
	SWPB_USTART,
	SWPB_DSTART,
};

#define PAD_MAX 16

void PadFrame(void);
void ReleasePads(void);

int GetPadPress(int padno);
int GetPadDown(int padno);
int GetPadPressCount(int padno, int btnbit);
int GetPadHit(int padno, int btnbit);

int GetPadPress_bb(int padno, int btnbit);
int GetPadDown_bb(int padno, int btnbit);
