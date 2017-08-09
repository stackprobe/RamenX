#include "Stdinc.h"

#define OUTFILE "C:\\tmp\\RamenX_Delay.txt"

int SW_DelayCount;

static int LastTime;

void SW_Start2(void)
{
	LastTime = GetNowCount();
}
void SW_Lap2(char *source, int lineno, char *function)
{
	int laptm = GetNowCount() - LastTime;

	if(2 <= laptm)
	{
		static int outed;
		FILE *fp = fileOpen(OUTFILE, outed ? "at" : "wt");

		fprintf(fp,
			"%08d (%08d) %s %d %s: %d\n"
			,FrameCount
			,LFrmCnt
			,source
			,lineno
			,function
			,laptm
			);
		fileClose(fp);
		outed = 1;

		SW_DelayCount++;
	}
	LastTime = GetNowCount();
}
