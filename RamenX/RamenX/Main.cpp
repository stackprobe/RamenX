#include "Stdinc.h"

static char *GetVersionString(void)
{
	const char *CONCERT_PTN = "{a9a54906-791d-4e1a-8a71-a4c69359cf68}:0.00"; // shared_uuid@g
	return (char *)strchr(CONCERT_PTN, ':') + 1;
}

int DxLibInited;

void termination(int errorLevel)
{
	if(DxLibInited)
	{
		DxLib_End();
	}
	exit(errorLevel);
}
void EndProc(void)
{
	// PROC_FNLZ
	{
		ExportSaveData();
		ReleaseResources();
	}
	termination(0);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if(__argc == 2 && !strcmp(__argv[1], "--extra-mode-01")) Sd_PlayChara = PLAY_CHARA_01;
	if(__argc == 2 && !strcmp(__argv[1], "--extra-mode-02")) Sd_PlayChara = PLAY_CHARA_02;
	if(__argc == 2 && !strcmp(__argv[1], "--extra-mode-03")) Sd_PlayChara = PLAY_CHARA_03;

//Sd_PlayChara = PLAY_CHARA_01; // test
//Sd_PlayChara = PLAY_CHARA_02; // test
//Sd_PlayChara = PLAY_CHARA_03; // test

	initrnd((int)time(NULL));
	ImportSaveData();

	if(__argc == 2 && !strcmp(__argv[1], "--easy-mode-off")) Sd_PlayChara = PLAY_CHARA_00;
	if(__argc == 2 && !strcmp(__argv[1], "--easy-mode-on"))  Sd_PlayChara = PLAY_CHARA_02;

	// Core >

	SetOutApplicationLogValidFlag(false);
	SetAlwaysRunFlag(true); // フォーカス外したら、true: 動く, false: 止まる。

	/*
		fixme:
		このソースをコンパイルしないと日付が変わらない。
		リリース前は必ずリビルドしてね!!
	*/
	SetMainWindowText(xcout("しろくまACT %s", GetVersionString()));

	SetGraphMode(800, 600, 32);
	ChangeWindowMode(Sd.FullScreenMode ? 0 : 1); // 0: 全画面, 1: 窓

	SetWindowIconID(333); // ウィンドウ左上のアイコン

	if(DxLib_Init())
	{
		return 1;
	}
	DxLibInited = 1;

	SetMouseDispFlag(Sd.DispMouseCursorMode ? 1 : 0); // 0: マウス消し, 1: マウス表示
	SetVSyncFlag(1);

	/*
		ウィンドウの右下をドラッグで、ウィンドウ伸縮 true: 可, false: 不可
		可にすると SetWindowSizeExtendRate() の最大サイズに影響する。
	*/
//	SetWindowSizeChangeEnableFlag(true);

	// < Core

	// PROC_INIT
	{
//		EsInit();
		GmResetDataCenter();
		GmResetResultCenter();

		// *_Reset
		{
			PE_Reset();
		}
	}
	ApHome();

	EndProc();
}

void ApplyDispMouseCursorMode(void)
{
	SetMouseDispFlag(Sd.DispMouseCursorMode ? 1 : 0);
}
void ApplyWindowSize(void)
{
	SetWindowSizeExtendRate(Sd.ScreenZoom);
	ApplyDispMouseCursorMode();
}
void ApplyScreenMode(void)
{
	ReleasePictures();

	/*
		ウィンドウが非アクティブな時にスクリーンモードを変更するとフリーズする。
		これを呼び出す前に IsWindowActive() でアクティブであることを確認してね。
		とりあえず IsWindowActive() == 0 の時はキー・パッド入力を抑止して、
		非アクティブ時にこれを呼び出す状況を作らないようにする。
		シビアなタイミングでアクティブ状態が変化したら駄目だろうけど、そこまで考えなくていいや...。
	*/
	ChangeWindowMode(Sd.FullScreenMode ? 0 : 1);

	ApplyDispMouseCursorMode();
	SetVSyncFlag(1);
}
void ReleaseResources(void)
{
	ReleasePictures();
	ReleaseMusics();
	ReleaseSoundEffects();
	ReleasePads();
}
void TouchResources(void)
{
	for(int r = 0; r < P_MAX; r++)
	{
		Pic(r);
		/*
		SetPictureGroup(S_EnemyLv1PictureGroup());
		Pic(r);
		SetPictureGroup(S_EnemyLv2PictureGroup());
		Pic(r);
		SetPictureGroup(S_EnemyLv3PictureGroup());
		Pic(r);
		SetPictureGroup(NULL);
		*/
	}
	for(int r = 0; r < D_MAX; r++)
	{
		Der(r);
	}
	for(int r = 0; r < SE_MAX; r++)
	{
		SETouch(r);
	}
}
