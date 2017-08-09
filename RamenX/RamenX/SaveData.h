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
		���\�[�X�ŗL���ʂ̃f�t�H�� 0.5 �Ȃ̂ŁA
	*/
	double MusicMasterVolume; // 0.0 - 2.0 == ���� - (�ő� * 2)
	double SEMasterVolume;    // 0.0 - 2.0 == ���� - (�ő� * 2)

//	int &ClearStageIndex;
	/*
		0: �N���A����
		1: 1�X�e�[�W�N���A�ς�
		2: 2�X�e�[�W�N���A�ς�
		...
		STAGE_NUM: �I�[���N���A�ς�
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
	PLAY_CHARA_00, // ��ՓxNormal�����Ҍ���, �W��
	PLAY_CHARA_01, // ��ՓxNormal�����Ҍ���, ���Ō���(�W���̕s������������ver)
	PLAY_CHARA_02, // ��ՓxEasy���S�Ҍ���, �����U��
	PLAY_CHARA_03, // ��ՓxHard�㋉�Ҍ���, ����(�Z�����l����)

	PLAY_CHARA_NUM, // num of member
}
PlayChara_t;
extern PlayChara_t Sd_PlayChara;
extern autoList<int> *S_Sd_BestTimeTable(void);

void ImportSaveData(void);
void ExportSaveData(void);

int *Get_P_ClearStageIndex(int playChara = Sd_PlayChara);
