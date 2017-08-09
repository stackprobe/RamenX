typedef struct Sd_st
{
	int FullScreenMode;
	double ScreenZoom; // 0.1 - 10.0
	int DispMouseCursorMode;

	struct
	{
		int Dir2;
		int Dir4;
		int Dir6;
		int Dir8;
		int A;
		int B;
		int C;
		int D;
		int E;
		int F; // not used
		int L; // not used
		int R; // not used without e2
		int Pause;
	}
	PadBtn;

	/*
		リソース固有音量のデフォが 0.5 なので、
	*/
	double MusicMasterVolume; // 0.0 - 2.0 == 無音 - (最大 * 2)
	double SEMasterVolume;    // 0.0 - 2.0 == 無音 - (最大 * 2)

//	int &ClearStageIndex;
	/*
		0: クリア無し
		1: 1ステージクリア済み
		2: 2ステージクリア済み
		...
		STAGE_NUM: オールクリア済み
	*/
	int CSI_PC00;
	int CSI_PC01;
	int CSI_PC02;
	int CSI_PC03;

	int SysTuneEnabled;
	int FirstBootEnded;
}
Sd_t;

extern Sd_t Sd;

typedef enum PlayChara_et
{
	PLAY_CHARA_00, // 難易度Normal中級者向け, 標準
	PLAY_CHARA_01, // 難易度Normal中級者向け, 強打撃力(標準の不満を解消したver)
	PLAY_CHARA_02, // 難易度Easy初心者向け, 高速攻撃
	PLAY_CHARA_03, // 難易度Hard上級者向け, 高速(忙しい人向け)

	PLAY_CHARA_NUM, // num of member
}
PlayChara_t;
extern PlayChara_t Sd_PlayChara;
extern autoList<int> *S_Sd_BestTimeTable(void);

void ImportSaveData(void);
void ExportSaveData(void);

int *Get_P_ClearStageIndex(int playChara = Sd_PlayChara);
