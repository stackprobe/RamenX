enum
{
	INP_DIR_2,
	INP_DIR_4,
	INP_DIR_6,
	INP_DIR_8,
	INP_A,
	INP_B,
	INP_C,
	INP_D,
	INP_E,
//	INP_F, // not used
//	INP_L, // not used
//	INP_R, // not used (’á‘¬ˆÚ“®‚Í”pŽ~)
	INP_PAUSE,

	INP_MAX, // num of 'input' member

	INP_PRESS,
	INP_DOWN,
	INP_PRESSCOUNT,
	INP_HIT,
};

extern int GTPrimaryPadno;

int GTGetInput(int input, int mode);
void GTFreezeInput(void);
