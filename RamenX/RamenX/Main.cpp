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
	SetAlwaysRunFlag(true); // �t�H�[�J�X�O������Atrue: ����, false: �~�܂�B

	/*
		fixme:
		���̃\�[�X���R���p�C�����Ȃ��Ɠ��t���ς��Ȃ��B
		�����[�X�O�͕K�����r���h���Ă�!!
	*/
	SetMainWindowText(xcout("���낭��ACT %s", GetVersionString()));

	SetGraphMode(800, 600, 32);
	ChangeWindowMode(Sd.FullScreenMode ? 0 : 1); // 0: �S���, 1: ��

	SetWindowIconID(333); // �E�B���h�E����̃A�C�R��

	if(DxLib_Init())
	{
		return 1;
	}
	DxLibInited = 1;

	SetMouseDispFlag(Sd.DispMouseCursorMode ? 1 : 0); // 0: �}�E�X����, 1: �}�E�X�\��
	SetVSyncFlag(1);

	/*
		�E�B���h�E�̉E�����h���b�O�ŁA�E�B���h�E�L�k true: ��, false: �s��
		�ɂ���� SetWindowSizeExtendRate() �̍ő�T�C�Y�ɉe������B
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
		�E�B���h�E����A�N�e�B�u�Ȏ��ɃX�N���[�����[�h��ύX����ƃt���[�Y����B
		������Ăяo���O�� IsWindowActive() �ŃA�N�e�B�u�ł��邱�Ƃ��m�F���ĂˁB
		�Ƃ肠���� IsWindowActive() == 0 �̎��̓L�[�E�p�b�h���͂�}�~���āA
		��A�N�e�B�u���ɂ�����Ăяo���󋵂����Ȃ��悤�ɂ���B
		�V�r�A�ȃ^�C�~���O�ŃA�N�e�B�u��Ԃ��ω�������ʖڂ��낤���ǁA�����܂ōl���Ȃ��Ă�����...�B
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
