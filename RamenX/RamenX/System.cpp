#include "Stdinc.h"

#define LOGFILE "C:\\tmp\\RamenX_Log.txt"

void error2(char *source, int lineno, char *function)
{
	static int callcnt;

	if(callcnt)
	{
		exit(2);
	}
	callcnt++;

	if(!DxLibInited)
	{
		l_x(xcout("%s (%d) %s", source, lineno, function));
		exit(3);
	}
	DrawBox(0, 0, 800, 600, GetColor(0, 0, 64), 1);

	printfDx("----\n");
	printfDx("致命的なエラーが発生しました。\n");
//	printfDx("下記(及び上記)メッセージと併せてご連絡いただけると幸いです。\n");
//	printfDx("エスケープキーを押して、このアプリケーションを終了して下さい。\n");
	printfDx("----\n");
	printfDx("場所: %s (%d) %s\n", source, lineno, function);

	ScreenFlip();

	while(CheckHitKey(KEY_INPUT_ESCAPE) == 0 && ProcessMessage() != -1)
	{
		ScreenFlip();
	}
	termination(1);
}
void l2_x(char *source, int lineno, char *function, char *message)
{
#if 1
	error();
#else // 廃止 2015.1.2
	static int outed;
	FILE *fp = fileOpen(LOGFILE, outed ? "at" : "wt");

	fprintf(fp, "%s (%d) %s: %s\n", source, lineno, function, message);
	fileClose(fp);
	outed = 1;
	memFree(message);
#endif
}

int callCount_memAlloc;
int callCount_memRealloc;
int callCount_memFree;

static void *REAL_memAlloc(int size)
{
	void *block;

	callCount_memAlloc++;

	errorCase(size < 0 || TSLD < size);
SW_Lap();
	block = malloc(m_max(size, 1));
SW_Lap();
	errorCase(!block);

	return block;
}
static void *REAL_memRealloc(void *block, int size)
{
	callCount_memRealloc++;

	errorCase(size < 0 || TSLD < size);
SW_Lap();
	block = realloc(block, m_max(size, 1));
SW_Lap();
	errorCase(!block);

	return block;
}
static void REAL_memFree(void *block)
{
	callCount_memFree++;
SW_Lap();
	free(block);
SW_Lap();
}

int reuseCount_memAlloc;
int reuseCount_memRealloc;
int reuseCount_memFree;

#define MSTOCK_MSIZE 1024
#define MSTOCK_MAX 4096

static uchar *MemStock;
static void **MemPStock;
static int MemPNum;
static int MemPNumMin;

int ms_GetMemPNum(void)
{
	return MemPNum;
}
int ms_GetMemPNumMin(void)
{
	return MemPNumMin;
}

static __inline void MS_INIT(void)
{
	static int inited;

	if(!inited) {
		inited = 1;

		MemStock = (uchar *)REAL_memAlloc(MSTOCK_MSIZE * MSTOCK_MAX);
		MemPStock = (void **)REAL_memAlloc(MSTOCK_MAX * sizeof(void *));
		MemPNum = MSTOCK_MAX;
		MemPNumMin = MemPNum;

		for(int index = 0; index < MSTOCK_MAX; index++)
		{
			MemPStock[index] = MemStock + index * MSTOCK_MSIZE;
		}
	}
}
static __inline int IsMemP(void *block)
{
	if(MemStock <= block && block < MemStock + MSTOCK_MSIZE * MSTOCK_MAX)
	{
		errorCase(((uint)block - (uint)MemStock) % MSTOCK_MSIZE != 0);
		return 1;
	}
	return 0;
}
void *memAlloc(int size)
{
	MS_INIT();

	if(size <= MSTOCK_MSIZE && 1 <= MemPNum)
	{
		reuseCount_memAlloc++;

		MemPNum--;
		MemPNumMin = m_min(MemPNumMin, MemPNum);
		return MemPStock[MemPNum];
	}
	return REAL_memAlloc(size);
}
void *memRealloc(void *block, int size)
{
	MS_INIT();

	if(IsMemP(block))
	{
		if(size <= MSTOCK_MSIZE)
		{
			reuseCount_memRealloc++;
			return block;
		}
		MemPStock[MemPNum] = block;
		MemPNum++;

		block = memcpy(REAL_memAlloc(MSTOCK_MSIZE), block, MSTOCK_MSIZE);
	}
	return REAL_memRealloc(block, size);
}
void memFree(void *block)
{
	MS_INIT();

	if(IsMemP(block))
	{
		reuseCount_memFree++;

		MemPStock[MemPNum] = block;
		MemPNum++;
		return;
	}
	REAL_memFree(block);
}

void *memCalloc(int size)
{
	void *block = memAlloc(size);

	memset(block, 0x00, size);
	return block;
}
void *memClone(void *block, int size)
{
	void *newBlock = memAlloc(size);

	memcpy(newBlock, block, size);
	return newBlock;
}

int fs_count;
int fs_max;
double fs_rate;

int IsWindowActive(void) // ret: ? ウィンドウがアクティブ状態
{
	return GetActiveFlag() ? 1 : 0;
}
