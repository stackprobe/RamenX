void GTPrintSet(int origx, int origy, int stepx, int stepy);
void GTPrint(char *line);
void GTPrint_x(char *line);

void GTWaitFadeout(int resno_wallpic, double whiteLevel, int frmcnt);

void GTDrawBox(int x, int y, int w, int h, int color);
void GTDrawCircle(double x, double y, double r, int color);

extern double GTCameraX;
extern double GTCameraY;

// ---- Print ----

typedef struct PrintExtra_st
{
	int Color;

	// wrapped by PE_* -->

	int Border; // 0 == –³Œø
	int BorderColor;
}
PrintExtra_t;

extern PrintExtra_t PE;

void PE_Border(int color = GetColor(0, 0, 0));
void PE_Reset(void);

void SetPrint(int x = 0, int y = 0, int yStep = 16);
void PrintRet(void);
void Print(char *token);
void Print_x(char *token);

// ----
