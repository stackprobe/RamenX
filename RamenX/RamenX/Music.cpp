#include "Stdinc.h"

singleton(MusicVolumeList, autoList<double>, makeList(MUSIC_MAX, (double)0.5))

int Mus_Load(int resno, autoList<uchar> *image)
{
	return LoadSound(image);
}
void Mus_Unload(int resno, int h)
{
	UnloadSound(h);
}
Resource_t *Mus_Res(void)
{
	static Resource_t *res;

	if(!res)
		res = CreateResource("Data1.dat", "..\\..\\Music.txt", MUSIC_MAX, Mus_Load, Mus_Unload);

	return res;
}

enum
{
	REQCMD_LOAD,
	REQCMD_PLAY,
	REQCMD_SETVOLUME,
	REQCMD_STOP,
	REQCMD_NOOP,
};
typedef struct Request_st
{
	int Command;
	int Resno;
	int EndlessMode;
	int FromTop;
	double FadeVolume; // 0.0 - 1.0
	int SleepFrame;
}
Request_t;

static int ActResno = HANDLE_CLOSED; // HANDLE_CLOSED == ñ¢çƒê∂
static int SleepFrame;

static singleton(RequestQueue, autoQueue<Request_t>, new autoQueue<Request_t>(256))

void ReleaseMusics(void)
{
	S_RequestQueue()->Clear();
	ActResno = HANDLE_CLOSED;
	SleepFrame = 1;

	UnloadAllHandle(Mus_Res()); // çƒê∂íÜÇÃâπÇÕïÅí Ç…é~Ç‹ÇÈÇÊÇ§ÇæÅB
}
int GetMusicRequestCount(void)
{
	return S_RequestQueue()->GetCount();
}

void MusicFrame(void)
{
	Request_t req;

	if(m_countDown(SleepFrame))
	{
		return;
	}
	req.Command = REQCMD_NOOP;
	req.Resno = HANDLE_CLOSED;
	req.EndlessMode = -1;
	req.FromTop     = -1;
	req.FadeVolume  = -1.0;
	req.SleepFrame  = 0;

	req = S_RequestQueue()->Dequeue(req);

	switch(req.Command)
	{
	case REQCMD_LOAD:
		GetHandle(Mus_Res(), req.Resno);
		break;

	case REQCMD_PLAY:
		SoundPlay(GetHandle(Mus_Res(), req.Resno), req.EndlessMode, req.FromTop);
		break;

	case REQCMD_SETVOLUME:
		{
			double v = Sd.MusicMasterVolume * S_MusicVolumeList()->GetElement(req.Resno) * req.FadeVolume;
			
			m_range(v, 0.0, 1.0);
			
			SetVolume(GetHandle(Mus_Res(), req.Resno), v);
		}
		break;

	case REQCMD_STOP:
		SoundStop(GetHandle(Mus_Res(), req.Resno));
		break;

	case REQCMD_NOOP:
		break;

	default:
		error();
	}
	SleepFrame = req.SleepFrame;
}

void MusicPlay(int resno, int endless_mode, int from_top)
{
	Request_t req;

	errorCase(resno < 0 || MUSIC_MAX <= resno);

	if(resno == ActResno)
	{
		return;
	}
	MusicStop();

	ActResno = resno;


	req.Command = REQCMD_LOAD;
	req.Resno = resno;
	req.EndlessMode = -1;
	req.FromTop     = -1;
	req.FadeVolume  = -1.0;
	req.SleepFrame  = 1;

	S_RequestQueue()->GetList()->AddElement(req);


	UpdateMusicVolume();


	req.Command = REQCMD_PLAY;
	req.Resno = resno;
	req.EndlessMode = endless_mode;
	req.FromTop     = from_top;
	req.FadeVolume  = -1.0;
	req.SleepFrame  = 1;

	S_RequestQueue()->GetList()->AddElement(req);
}
void MusicFadeout(int frmcnt)
{
	Request_t req;

	errorCase(frmcnt < 1 || TSLD < frmcnt);

	if(ActResno == HANDLE_CLOSED)
	{
		return;
	}
	for(int count = frmcnt - 1; count; count--)
	{
		req.Command = REQCMD_SETVOLUME;
		req.Resno = ActResno;
		req.EndlessMode = -1;
		req.FromTop     = -1;
		req.FadeVolume  = (double)count / frmcnt;
		req.SleepFrame  = 0;

		S_RequestQueue()->GetList()->AddElement(req);
	}
	MusicStop();
}
void MusicStop(void)
{
	Request_t req;

	if(ActResno == HANDLE_CLOSED)
	{
		return;
	}


	req.Command = REQCMD_SETVOLUME;
	req.Resno = ActResno;
	req.EndlessMode = -1;
	req.FromTop     = -1;
	req.FadeVolume  = 0.0;
	req.SleepFrame  = 1;

	S_RequestQueue()->GetList()->AddElement(req);


	req.Command = REQCMD_STOP;
	req.Resno = ActResno;
	req.EndlessMode = -1;
	req.FromTop     = -1;
	req.FadeVolume  = -1.0;
	req.SleepFrame  = 1;

	S_RequestQueue()->GetList()->AddElement(req);


	ActResno = HANDLE_CLOSED;
}
void UpdateMusicVolume(void)
{
	Request_t req;

	if(ActResno == HANDLE_CLOSED)
	{
		return;
	}
	req.Command = REQCMD_SETVOLUME;
	req.Resno = ActResno;
	req.EndlessMode = -1;
	req.FromTop     = -1;
	req.FadeVolume  = 1.0;
	req.SleepFrame  = 1;

	S_RequestQueue()->GetList()->AddElement(req);
}
