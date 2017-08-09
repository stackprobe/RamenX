#include "Stdinc.h"

#define SAVEDATAFILE "SaveData.dat"
#define CHECKDIGIT 4690

Sd_t Sd;

PlayChara_t Sd_PlayChara;
singleton(Sd_BestTimeTable, autoList<int>, new autoList<int>());

static autoQueue<uchar> *FileImage;

static char *ReadLine(void)
{
	return q_readLine(FileImage);
}
static char *c_ReadLine(void)
{
	static char *stock;
	memFree(stock);
	return stock = ReadLine();
}
static void WriteLine(char *line)
{
	q_writeLine(FileImage->GetList(), line);
}
static void WriteLine_x(char *line)
{
	q_writeLine_x(FileImage->GetList(), line);
}
static int ReadInt(int minval, int maxval)
{
	int value = atoi_x(ReadLine());

	m_range(value, minval, maxval);
	return value;
}
static void WriteInt(int value)
{
	WriteLine_x(xcout("%d", value));
}
static double ReadDouble(double minval, double maxval, int scale)
{
	double value = (double)atoi_x(ReadLine()) / scale;

	m_range(value, minval, maxval);
	return value;
}
static void WriteDouble(double value, int scale)
{
	WriteInt(m_d2i(value * scale));
}

static void LoadDefaultSaveData(void)
{
	Sd.ScreenZoom = 1.0;

	Sd.PadBtn.Dir2 = SWPB_DIR_2;
	Sd.PadBtn.Dir4 = SWPB_DIR_4;
	Sd.PadBtn.Dir6 = SWPB_DIR_6;
	Sd.PadBtn.Dir8 = SWPB_DIR_8;
	Sd.PadBtn.A = SWPB_A1;
	Sd.PadBtn.B = SWPB_B1;
	Sd.PadBtn.C = SWPB_A2;
	Sd.PadBtn.D = SWPB_B2;
	Sd.PadBtn.E = SWPB_A3;
	Sd.PadBtn.F = SWPB_B3;
	Sd.PadBtn.L = SWPB_L;
	Sd.PadBtn.R = SWPB_R;
	Sd.PadBtn.Pause = SWPB_DSTART;

	Sd.MusicMasterVolume = 0.95; // 0.0 - 2.0
	Sd.SEMasterVolume    = 0.95; // 0.0 - 2.0

	S_Sd_BestTimeTable()->Clear();
}
void ImportSaveData(void)
{
	memset(&Sd, 0x00, sizeof(Sd_t));
	EsInit();

	if(!accessible(SAVEDATAFILE))
	{
		LoadDefaultSaveData();
		return;
	}
	FileImage = new autoQueue<uchar>(readBinary(SAVEDATAFILE), TSLD);
	Jammer(FileImage->GetList());

	if(
//		strcmp(__DATE__, c_ReadLine()) != 0 ||
//		strcmp(__TIME__, c_ReadLine()) != 0 ||
		ReadInt(CHECKDIGIT - 1, CHECKDIGIT + 1) != CHECKDIGIT ||
		0
		)
	{
		delete FileImage;
		LoadDefaultSaveData();
		return;
	}
	Sd.FullScreenMode      = ReadInt(-1, 1);
	Sd.ScreenZoom          = ReadDouble(0.1, 10.0, 100);
	Sd.DispMouseCursorMode = ReadInt(-1, 1);

	Sd.PadBtn.Dir2 = ReadInt(0, 31);
	Sd.PadBtn.Dir4 = ReadInt(0, 31);
	Sd.PadBtn.Dir6 = ReadInt(0, 31);
	Sd.PadBtn.Dir8 = ReadInt(0, 31);
	Sd.PadBtn.A = ReadInt(0, 31);
	Sd.PadBtn.B = ReadInt(0, 31);
	Sd.PadBtn.C = ReadInt(0, 31);
	Sd.PadBtn.D = ReadInt(0, 31);
	Sd.PadBtn.E = ReadInt(0, 31);
//	Sd.PadBtn.F = ReadInt(0, 31);
//	Sd.PadBtn.L = ReadInt(0, 31);
//	Sd.PadBtn.R = ReadInt(0, 31);
	Sd.PadBtn.Pause = ReadInt(0, 31);

	Sd.MusicMasterVolume = ReadDouble(0.0, 2.0, TSLD / 2);
	Sd.SEMasterVolume    = ReadDouble(0.0, 2.0, TSLD / 2);

	Sd_PlayChara = (PlayChara_t)ReadInt(0, PLAY_CHARA_NUM - 1);

	// BestTimeTable
	{
		S_Sd_BestTimeTable()->Clear();

		int btt_max = ReadInt(0, STAGE_NUM * 4 * 4);
//		int btt_max = ReadInt(0, TSLD);

		for(int index = 0; index < btt_max; index++)
		{
			S_Sd_BestTimeTable()->AddElement(ReadInt(0, TSLD));
		}
	}
	Sd.CSI_PC00 = ReadInt(0, STAGE_NUM);
	Sd.CSI_PC01 = ReadInt(0, STAGE_NUM);
	Sd.CSI_PC02 = ReadInt(0, STAGE_NUM);
	Sd.CSI_PC03 = ReadInt(0, STAGE_NUM);

	Sd.SysTuneEnabled = ReadInt(-1, 1);
	Sd.FirstBootEnded = ReadInt(-1, 1);

	Es.VisibleScope = ReadInt(-1, 1);
	Es.AttackFukaFrame = ReadInt(1, 70);
	Es.No_AttackAndTransOff = ReadInt(-1, 1);
	Es.AttackYoyakuMargin = ReadInt(0, 7);
	Es.DrawRectFloat = ReadInt(-1, 1);
	Es.ForceDashMode = ReadInt(-1, 1);
	Es.ShowEnemyHPMode = ReadInt(-1, 1);
	Es.HitBackTrueDir = ReadInt(-1, 1);
	Es.MoreSnow = ReadInt(-1, MORESNOW_MAX - 1);
	GmVisibleSubStatus = ReadInt(0, GM_VSS_MAX - 1);

	delete FileImage;
}
void ExportSaveData(void)
{
	FileImage = new autoQueue<uchar>(TSLD);

//	WriteLine(__DATE__);
//	WriteLine(__TIME__);
	WriteInt(CHECKDIGIT);
	WriteInt(Sd.FullScreenMode);
	WriteDouble(Sd.ScreenZoom, 100);
	WriteInt(Sd.DispMouseCursorMode);

	WriteInt(Sd.PadBtn.Dir2);
	WriteInt(Sd.PadBtn.Dir4);
	WriteInt(Sd.PadBtn.Dir6);
	WriteInt(Sd.PadBtn.Dir8);
	WriteInt(Sd.PadBtn.A);
	WriteInt(Sd.PadBtn.B);
	WriteInt(Sd.PadBtn.C);
	WriteInt(Sd.PadBtn.D);
	WriteInt(Sd.PadBtn.E);
//	WriteInt(Sd.PadBtn.F);
//	WriteInt(Sd.PadBtn.L);
//	WriteInt(Sd.PadBtn.R);
	WriteInt(Sd.PadBtn.Pause);

	WriteDouble(Sd.MusicMasterVolume, TSLD / 2);
	WriteDouble(Sd.SEMasterVolume,    TSLD / 2);

	WriteInt(Sd_PlayChara);

	// BestTimeTable
	{
		int btt_max = S_Sd_BestTimeTable()->GetCount();

		WriteInt(btt_max);

		for(int index = 0; index < btt_max; index++)
		{
			WriteInt(S_Sd_BestTimeTable()->GetElement(index));
		}
	}
	WriteInt(Sd.CSI_PC00);
	WriteInt(Sd.CSI_PC01);
	WriteInt(Sd.CSI_PC02);
	WriteInt(Sd.CSI_PC03);

	WriteInt(Sd.SysTuneEnabled);
	WriteInt(Sd.FirstBootEnded);

	WriteInt(Es.VisibleScope);
	WriteInt(Es.AttackFukaFrame);
	WriteInt(Es.No_AttackAndTransOff);
	WriteInt(Es.AttackYoyakuMargin);
	WriteInt(Es.DrawRectFloat);
	WriteInt(Es.ForceDashMode);
	WriteInt(Es.ShowEnemyHPMode);
	WriteInt(Es.HitBackTrueDir);
	WriteInt(Es.MoreSnow);
	WriteInt(GmVisibleSubStatus);

	Jammer(FileImage->GetList());
	writeBinary(SAVEDATAFILE, FileImage->GetList());

	delete FileImage;
}

int *Get_P_ClearStageIndex(int playChara)
{
	switch(playChara)
	{
	case 0: return &Sd.CSI_PC00;
	case 1: return &Sd.CSI_PC01;
	case 2: return &Sd.CSI_PC02;
	case 3: return &Sd.CSI_PC03;

	default:
		error();
	}
	error();
	return NULL; // dummy
}
